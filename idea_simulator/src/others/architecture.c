/**
 * \file architecture.c
 * \brief Initiate the XML configuration's file.
 * \author ~gmarait
 * \version 1.0
 * \date 3rd january 2014
 */

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <string.h>
#include <unistd.h>
#include "architecture.h"
#include "node.h"

/**
 * \def CHECK_XPATH(result)
 * \brief Secure XPath result.
 */
#define CHECK_XPATH(result) do { if (result == NULL) { fprintf(stderr, "Error XPath request\n"); \
      return EXIT_FAILURE;						\
    } } while(0)

/**
 * \def CHECK_ALLOC(ptr)
 * \brief Check if the allocation on ptr worked.
 */
#define CHECK_ALLOC(ptr) do { if(ptr == NULL) { fprintf(stderr, "Allocation error\n"); _exit(1); } } while(0)

/**
 * \def XSL_DOC
 * \param path 
 * \brief Path (absolute or relative) to the xsl result.
 * \todo Precise if the result is overwritten.
 */
#define XSL_DOC "res/archi_maker.xsl"

/**
 * \def GET_ATTRIBUT_TXT(name,node,target)
 * \brief Copy the node's text (identified by node name) in the target.
 */
#define GET_ATTRIBUT_TXT(name,node,target)   tmp = xmlGetProp(node, BAD_CAST name); \
  sprintf(buf, "%s", tmp);						\
  strcpy(target, buf);							\
  xmlFree(tmp);								\
  buf[0] = 0

/**
 * \def GET_ATTRIBUT_INT(name,node,target)
 * \brief Copy the node's number (identified by node name) in the target.
 */
#define GET_ATTRIBUT_INT(name,node,target)   tmp = xmlGetProp(node, BAD_CAST name); \
  sprintf(buf, "%s", tmp);						\
  target = atoi(buf);							\
  xmlFree(tmp);								\
  buf[0] = 0

struct level{
  int type;
  void (*coherence_protocol) (struct cache *);
  bool snooping;
  bool directory_manager;
};

/**
 * \brief Compare the replacement policy's name in order to choose the associated replacement function.
 * \param name Name to compare with implemented policies.
 */
void (*get_replacement_function(char * name)) (struct cache *){
  if(strcmp(name, "FIFO") == 0)
    return replacement_FIFO;
  if(strcmp(name, "LFU") == 0)
    return replacement_LFU;
  return replacement_LRU;
}

/**
 * \brief Compare the coherence policy's name in order to choose the associated replacement function.
 * \param name Name to compare with implemented policies.
 */
void (*get_coherence_function(char * name)) (struct cache *){
  if(strcmp(name, "MSI") == 0)
    return coherence_MSI;
  return coherence_MESI;
}

/**
 * \brief Gives the value of the cache corresponding to the xml text
 * \param txt The xml text in the attribute type
 * \return The corresponding number
 */
int get_cache_type(char * txt){
  if(strcmp(txt, "exclusive") == 0)
    return Exclusive;
  if(strcmp(txt, "niio") == 0)
    return NIIO;
  if(strcmp(txt, "nieo") == 0)
    return NIEO;
  return Inclusive;
}

/**
 * \brief Turn the text of a boolean (true/false) into a boolean
 * \param txt The text
 * \return The corresponding bool
 */
bool get_bool(char * txt){
  if(strcmp(txt, "true") == 0)
    return true;
  return false;
}

/**
 * \brief Modify the name of the XML output file.
 * \param name Previous name.
 * \param out New name.
 * \todo Precise the call context. Previous file is overwritten ?
 */
void change_filename(const char * name, char * out){
  int l = strlen(name);
  char extention[30];
  strcpy(extention, &name[l-4]);
  strcpy(out, name);
  if(strcmp(extention, ".xml") == 0){
    out[l-4] = 0;
  }
  strcat(out, ".cassis.xml");
}

int parse_archi_file(const char * filename, struct architecture * archi){
  int i,j;
  char file_in[50];
  char buf[100];
  char buf2[100];
  buf[0] = 0;
  buf2[0] = 0;
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
  char replacement_prot[10];
  struct cache * c;
  struct node * n;
  struct node ** cstack;

  //Begin parsing
  //Global values
  res = xmlXPathEvalExpression(BAD_CAST "/Architecture", context);
  CHECK_XPATH(res);
  GET_ATTRIBUT_TXT("name", res->nodesetval->nodeTab[0], archi->name);
  GET_ATTRIBUT_TXT("CPU_name", res->nodesetval->nodeTab[0], archi->CPU_name);
  GET_ATTRIBUT_INT("number_levels", res->nodesetval->nodeTab[0], archi->number_levels);
  cstack = (struct node **) malloc(archi->number_levels * sizeof(struct node *));
  xmlXPathFreeObject(res);

  //Level parsing
  struct level * L = (struct level *) malloc(archi->number_levels * sizeof(struct level));
  CHECK_ALLOC(L);
  res = xmlXPathEvalExpression(BAD_CAST "//Level", context);
  CHECK_XPATH(res);
  if(archi->number_levels != res->nodesetval->nodeNr){
    fprintf(stderr, "Parsing error : wrong number of \"Level\"");
    _exit(1);
  }
  for(i=0; i<res->nodesetval->nodeNr; i++){
    cur = res->nodesetval->nodeTab[i];
    j = archi->number_levels - i - 1;
    GET_ATTRIBUT_TXT("type", cur, buf2);
    L[j].type = get_cache_type(buf2);
    GET_ATTRIBUT_TXT("coherence_protocol", cur, buf2);
    L[j].coherence_protocol = get_coherence_function(buf2);
    GET_ATTRIBUT_TXT("snooping", cur, buf2);
    L[j].snooping = get_bool(buf2);
    GET_ATTRIBUT_TXT("directory_manager", cur, buf2);
    L[j].directory_manager = get_bool(buf2);
  }
  xmlXPathFreeObject(res);

  //Cache parsing
  struct node ** first_sibling = (struct node **) calloc(archi->number_levels, sizeof(struct node *));
  struct node ** last_sibling = (struct node **) calloc(archi->number_levels, sizeof(struct node *));
  res = xmlXPathEvalExpression(BAD_CAST "//Cache", context);
  CHECK_XPATH(res);
  for(i=0; i<res->nodesetval->nodeNr; i++){
    cur = res->nodesetval->nodeTab[i];
    GET_ATTRIBUT_INT("depth", cur, depth);
    GET_ATTRIBUT_INT("cache_size", cur, size);
    GET_ATTRIBUT_INT("cache_linesize", cur, linesize);
    GET_ATTRIBUT_INT("cache_associativity", cur, nb_ways);
    nb_blocks = size / (linesize * nb_ways);
    GET_ATTRIBUT_TXT("replacement_protocol", cur, replacement_prot);
    c = init_cache(size, linesize, nb_ways, nb_blocks, depth, get_replacement_function(replacement_prot), L[depth-1].coherence_protocol, L[depth-1].type, L[depth-1].snooping, L[depth-1].directory_manager);
    n = init_node();
    set_data(n, c);

    //Add to the threads table
    //First pop levels below in the stack
    while(stack_head > 0 && cstack[stack_head]->data->depth <= depth){
      stack_head--;
    }
    //Then we push
    stack_head++;
    cstack[stack_head] = n;

    if(depth == 1){
      archi->threads = realloc(archi->threads, (archi->number_threads + 1) * sizeof(struct node *));
      CHECK_ALLOC(archi->threads);
      archi->threads[archi->number_threads] = n;
      for(j = stack_head-1; j >= 0 ; j--){
	if(!get_parent(cstack[j+1])){
	  set_parent(cstack[j+1], cstack[j]);
	  add_child(cstack[j], cstack[j+1]);
	}
      }
      archi->number_threads++;
    }
    
    //Add the siblings
    if(first_sibling[depth-1] == 0){
      first_sibling[depth-1] = n;
      last_sibling[depth-1] = n;
    }
    set_next_sibling(last_sibling[depth-1], n);
    set_next_sibling(n, first_sibling[depth-1]); //Link last to the first
    last_sibling[depth-1] = n;
  }

  /* Set directory managers */
  n = get_root(n);
  init_directories(n);

  xmlXPathFreeObject(res);
  free(L);
  free(cstack);
  free(first_sibling);
  free(last_sibling);

  xmlXPathFreeContext(context);
  xmlFreeDoc(doc);
  xmlCleanupParser();
  
  return EXIT_SUCCESS;
}

void print_archi_rec(struct node * n){
  unsigned int i;
  printf("\tL%d (taille : %d, ligne : %d, associativité : %d, nb_blocks : %d)\n", n->data->depth, n->data->size, n->data->linesize, n->data->nb_ways,  n->data->nb_blocks);
  printf("\tSnooping : %d, directory_manager %d\n", n->data->snooping, n->data->directory);
  printf("\tInfo du noeud : id %d, nombre de fils %d\n\n", n->id, n->nb_children);
  for(i=0;i<n->nb_children;i++){
    print_archi_rec(get_child(n,i));
  }
}

void print_archi(struct architecture * archi){
  printf("Architecture %d bits : %s\n", archi->nb_bits, archi->name);
  printf("CPU model : %s\n", archi->CPU_name);
  printf("Arbre des caches : \n");
  struct node * root = get_root(archi->threads[0]);
  print_archi_rec(root);
}

int convert_archi_xml(const char * file_in, const char * file_out){
  FILE * out = fopen(file_out,"w");
  if(out == NULL){
    fprintf(stderr, "Cannot open %s\n", file_out);
    return EXIT_FAILURE;
  }

  xmlSubstituteEntitiesDefault(1);
  xmlLoadExtDtdDefaultValue = 1;

  xmlDocPtr doc = xmlParseFile(file_in);
  xsltStylesheetPtr xsl = xsltParseStylesheetFile(BAD_CAST XSL_DOC);
  int nb_params = 0;
  const char * params[nb_params + 1];
  params[nb_params] = NULL;
  xmlDocPtr fin_doc = xsltApplyStylesheet(xsl, doc, params);
  xsltSaveResultToFile(out, fin_doc, xsl);

  xsltFreeStylesheet(xsl);
  xmlFreeDoc(doc);
  xmlFreeDoc(fin_doc);
  fclose(out);

  xsltCleanupGlobals();
  xmlCleanupParser();
  
  printf("Architecture xml written in file : %s\n", file_out);

  return EXIT_SUCCESS;
}

void print_caches(struct architecture * archi){
  struct node * n = get_root(archi->threads[0]);
  int cond = 1;
  unsigned int id_beg;
  printf("Liste des caches\n");
  while(cond){
    id_beg = n->id;
    do{
      printf("\tL%d (misses: %10d, hits: %10d, writes_back: %10d, broadcasts: %10d)\n", n->data->depth, n->data->misses, n->data->hits, n->data->writes_back, n->data->broadcasts);
      n=get_sibling(n);
    } while(n->id != id_beg);
    if(n->data->depth == 1)
      cond = 0;
    else
      n=get_child(n,0);
  }
}

void delete_archi_rec(struct node * n){
  unsigned int i;
  for(i = 0; i<n->nb_children; i++){
    if(get_child(n,i)){
      delete_archi_rec(get_child(n, i));
    }
  }
  delete_cache(n->data);
  free_node(n);
}

void delete_archi(struct architecture * archi){
  struct node * root = get_root(archi->threads[0]);
  delete_directories(root);
  delete_archi_rec(root);
  free(archi->threads);
}
