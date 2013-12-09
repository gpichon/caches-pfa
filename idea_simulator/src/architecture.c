#include "architecture.h"
#include "list.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <string.h>

#define CHECK_XPATH(result) do { if (result == NULL) { fprintf(stderr, "Error XPath request\n"); \
      return EXIT_FAILURE;						\
    } } while(0)

#define DEFAULT_REPLACEMENT_FCT "LRU"
#define DEFAULT_COHERENCE_FCT "MESI"

#define GET_ATTRIBUT_TXT(name,node,target)   tmp = xmlGetProp(node, BAD_CAST name); \
  sprintf(buf, "%s", tmp);						\
  strcpy(target, buf);							\
  xmlFree(tmp);								\
  buf[0] = 0
#define GET_ATTRIBUT_INT(name,node,target)   tmp = xmlGetProp(node, BAD_CAST name); \
  sprintf(buf, "%s", tmp);						\
  target = atoi(buf);							\
  xmlFree(tmp);								\
  buf[0] = 0
/*
TODO :
options : 
- parse HWLOC -> archi file and execute DONE
- parse HWLOC -> archi file only
- use archi file and execute DONE
*/

void (*get_replacement_function(char * name)) (struct cache *){
  if(strcmp(name, "FIFO") == 0)
    return replacement_FIFO;
  if(strcmp(name, "LFU") == 0)
    return replacement_LFU;
  return replacement_LRU;
}

void (*get_coherence_function(char * name)) (struct cache *){
  if(strcmp(name, "MSI") == 0)
    return coherence_MSI;
  return coherence_MESI;
}

void change_filename(const char * name, char * out){
  int l = strlen(name);
  char extention[30];
  strcpy(extention, &name[l-4]);
  strcpy(out, name);
  if(strcmp(extention, ".xml") == 0){
    out[l-4] = 0;
  }
  strcat(out, "_archi.xml");
}

int parse_archi_file(const char * filename, struct architecture * archi){
  int i,j;
  char file_in[50];
  char buf[100];
  buf[0] = 0;
  strcpy(file_in, filename);

  xmlDocPtr doc = xmlParseFile(file_in);
  if(doc == NULL){
    fprintf(stderr, "Could not load %s\n", filename);
  }

  //Check if we need to create a _archi.xml
  xmlNodePtr root_test = xmlDocGetRootElement(doc);
  if(strcmp((char *) root_test->name, "Architecture") != 0){
    change_filename(filename, file_in);
    convert_archi_xml(filename, file_in);
    xmlFreeDoc(doc);
    doc = xmlParseFile(file_in);
  }

  //Default values
  archi->nb_bits = 64;
  strcpy(archi->name, "unknown");
  strcpy(archi->CPU_name, "unknown");
  archi->number_threads = 0;
  archi->number_levels = 0;
  archi->threads = NULL;
  archi->levels = NULL;

  xmlXPathInit();
  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  if (context == NULL) {
    fprintf(stderr, "Error XPath context\n");
  }
  xmlXPathObjectPtr res;
  xmlNodePtr cur;
  xmlChar * tmp;
  int depth, size, linesize, nb_ways, nb_blocks;
  int stack_head = -1;
  char coherence_prot[10], replacement_prot[10];
  struct cache * c;
  struct cache ** cstack;
  //Begin parsing
  res = xmlXPathEvalExpression(BAD_CAST "/Architecture", context);
  CHECK_XPATH(res);
  GET_ATTRIBUT_TXT("name", res->nodesetval->nodeTab[0], archi->name);
  GET_ATTRIBUT_TXT("CPU_name", res->nodesetval->nodeTab[0], archi->CPU_name);
  xmlXPathFreeObject(res);

  res = xmlXPathEvalExpression(BAD_CAST "//Cache", context);
  CHECK_XPATH(res);
  GET_ATTRIBUT_INT("depth", res->nodesetval->nodeTab[0], archi->number_levels);
  archi->levels = calloc(archi->number_levels, sizeof(struct list*));
  cstack = (struct cache **) malloc(archi->number_levels * sizeof(struct cache *));
  for(i=0; i<res->nodesetval->nodeNr; i++){
    cur = res->nodesetval->nodeTab[i];
    GET_ATTRIBUT_INT("depth", cur, depth);
    GET_ATTRIBUT_INT("cache_size", cur, size);
    GET_ATTRIBUT_INT("cache_linesize", cur, linesize);
    GET_ATTRIBUT_INT("cache_associativity", cur, nb_ways);
    nb_blocks = size / (linesize * nb_ways);
    GET_ATTRIBUT_TXT("replacement_protocol", cur, replacement_prot);
    GET_ATTRIBUT_TXT("coherence_protocol", cur, coherence_prot);
    c = init_cache(size, linesize, nb_ways, nb_blocks, depth, get_replacement_function(replacement_prot), get_coherence_function(replacement_prot));

    //Add the cache to the levels table
    if(archi->levels[depth-1] == 0){
      archi->levels[depth-1] = init_list(c);
    }
    else{
      add_list(archi->levels[depth-1], c);
    }
    //Add to the threads table
    //First pop levels below in the stack
    while(stack_head > 0 && cstack[stack_head]->depth <= depth){
      stack_head--;
    }
    //Then we push
    stack_head++;
    cstack[stack_head] = c;

    if(depth == 1){
      archi->threads = realloc(archi->threads, (archi->number_threads + 1) * sizeof(struct list *));
      archi->threads[archi->number_threads] = init_list(c);
      for(j = stack_head-1; j >= 0 ; j--){
	add_list(archi->threads[archi->number_threads], cstack[j]);
      }
      archi->number_threads++;
    }
  }
  xmlXPathFreeObject(res);
  free(cstack);

  xmlXPathFreeContext(context);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  
  return EXIT_SUCCESS;
}

void print_archi(struct architecture * archi){
  int i;
  struct list * l;
  printf("Architecture %d bits : %s\n", archi->nb_bits, archi->name);
  printf("CPU model : %s\n", archi->CPU_name);
  printf("Liste des threads : \n");
  for(i=0;i<archi->number_threads;i++){
    l = archi->threads[i];
    printf("Pour thread %d\n", i+1);
    while(l != NULL){
      printf("\tL%d (taille : %d, ligne : %d, associativité : %d, nb_blocks : %d)\n", l->cache->depth, l->cache->size, l->cache->linesize, l->cache->nb_ways,  l->cache->nb_blocks);
      l = l->next;
    }
  }
  printf("\n\nListe des niveaux\n");
  for(i=0;i<archi->number_levels;i++){
    l = archi->levels[i];
    printf("Pour niveau %d\n", i+1);
    while(l != NULL){
      printf("\tL%d (taille : %d, ligne : %d, associativité : %d, nb_blocks : %d)\n", l->cache->depth, l->cache->size, l->cache->linesize, l->cache->nb_ways,  l->cache->nb_blocks);
      l = l->next;
    }
  }
}

int convert_archi_xml(const char * file_in, const char * file_out){
  int i = 0;
  FILE * out = fopen(file_out,"w");
  if(out == NULL){
    fprintf(stderr, "Cannot open %s\n", file_out);
    return EXIT_FAILURE;
  }

  xmlXPathInit();
  xmlChar * tmp;
  xmlNodePtr root_cache;
  xmlNodePtr cur;
  xmlDocPtr doc = xmlParseFile(file_in);
  xmlDocPtr fin_doc = xmlNewDoc(BAD_CAST "1.0");
  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  if (context == NULL) {
    fprintf(stderr, "Error XPath context\n");
    return EXIT_FAILURE;
  }

  xmlXPathObjectPtr res = xmlXPathEvalExpression(BAD_CAST "//object[@type=\"Cache\"]", context);
  CHECK_XPATH(res);
  if (res->type == XPATH_NODESET) {
    for(i=0; i<res->nodesetval->nodeNr; i++){ //Modifiying names and attributes of the interesting nodes
      cur = res->nodesetval->nodeTab[i];
      //Remove useless attributes
      xmlUnsetProp(cur, BAD_CAST "type");
      xmlUnsetProp(cur, BAD_CAST "cpuset");
      xmlUnsetProp(cur, BAD_CAST "complete_cpuset");
      xmlUnsetProp(cur, BAD_CAST "online_cpuset");
      xmlUnsetProp(cur, BAD_CAST "allowed_cpuset");
      xmlUnsetProp(cur, BAD_CAST "cache_type");
      //Changing node name
      xmlNodeSetName(cur, BAD_CAST "Cache");
      //Adding attributes
      xmlSetProp(cur, BAD_CAST "replacement_protocol", BAD_CAST DEFAULT_REPLACEMENT_FCT);
      xmlSetProp(cur, BAD_CAST "coherence_protocol", BAD_CAST DEFAULT_COHERENCE_FCT);
    }
    root_cache = res->nodesetval->nodeTab[0]; //Setting the right root
  }
  xmlXPathFreeObject(res);


  //Removing useless node
  /* this will be automatically deleted when their parents ("core") will be deleted */
  /*
  res = xmlXPathEvalExpression(BAD_CAST "//object[@type=\"PU\"]", context);
  CHECK_XPATH(res);
  for(i=0; i<res->nodesetval->nodeNr; i++){
    cur = res->nodesetval->nodeTab[i];
    //xmlUnlinkNode(cur);
    //xmlFreeNode(cur);
  }
  xmlXPathFreeObject(res);
  */

  //We have to put the nodes to delete, in another temp tabular, because of libxml operation
  res = xmlXPathEvalExpression(BAD_CAST "//object[@type=\"Core\"]", context);
  CHECK_XPATH(res);
  int n = res->nodesetval->nodeNr;
  void **tabTemp = malloc(sizeof(void *)*n);
  for(i=0; i<res->nodesetval->nodeNr; i++){
    tabTemp[i] = (void *) res->nodesetval->nodeTab[i];
  }
  xmlXPathFreeObject(res);
  //We delete the nodes, then the tabular
  for(i=0; i<n; i++){
    xmlUnlinkNode(tabTemp[i]);
    xmlFreeNode(tabTemp[i]);
  }
  free(tabTemp);


  xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "Architecture");
  res = xmlXPathEvalExpression(BAD_CAST "//info[@name=\"Architecture\"]", context);
  CHECK_XPATH(res);
  tmp = xmlGetProp(res->nodesetval->nodeTab[0], BAD_CAST "value");
  xmlSetProp(root, BAD_CAST "name", tmp);
  free(tmp);
  xmlXPathFreeObject(res);
  res = xmlXPathEvalExpression(BAD_CAST "//info[@name=\"CPUModel\"]", context);
  CHECK_XPATH(res);
  tmp = xmlGetProp(res->nodesetval->nodeTab[0], BAD_CAST "value");
  xmlSetProp(root, BAD_CAST "CPU_name", tmp);
  free(tmp);
  xmlXPathFreeObject(res); 

  //Change root
  xmlAddChild(root, xmlDocCopyNodeList(doc, root_cache));
  xmlDocSetRootElement(fin_doc, root);
  xmlDocFormatDump(out, fin_doc, 1);

  xmlCleanupParser();
  xmlXPathFreeContext(context);
  xmlFreeDoc(fin_doc);
  xmlFreeDoc(doc);
  fclose(out);
  
  printf("Architecture xml written in file : %s\n", file_out);

  return EXIT_SUCCESS;
}

void print_caches(struct architecture * archi){
  int i;
 struct list * l;
  printf("Liste des caches\n");
  for(i=0;i<archi->number_levels;i++){
    l = archi->levels[archi->number_levels-i-1];
    while(l != NULL){      
      printf("\tL%d (misses: %10d, hits: %10d, writes_back: %10d, broadcasts: %10d, invalid_back:%10d)\n", l->cache->depth, l->cache->misses, l->cache->hits, l->cache->writes_back, l->cache->broadcasts, l->cache->invalid_back);
      l = l->next;
    }
  }
}

void delete_archi(struct architecture * archi){
  int i;
  for(i=0;i<archi->number_threads;i++){
    delete_list(archi->threads[i]);
  }
  for(i=0;i<archi->number_levels;i++){
    delete_list_def(archi->levels[i]);
  }
  free(archi->threads);
  free(archi->levels);
}
