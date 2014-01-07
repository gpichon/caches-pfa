/**
 * \file architecture.c
 * \brief Initiate the XML configuration's file.
 * \author ~gmarait
 * \version 1.0
 * \date 3rd january 2014
 * 
 * 
 *
 */

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <string.h>
#include "architecture.h"

/**
 * \def CHECK_XPATH(result)
 * \brief Secure XPath result.
 */
#define CHECK_XPATH(result) do { if (result == NULL) { fprintf(stderr, "Error XPath request\n"); \
      return EXIT_FAILURE;						\
    } } while(0)

/**
 * \def DEFAULT_REPLACEMENT_FCT 
 * \param policy Name of the policy wanted to be used in default.
 * \brief Define the string used in XML confifuration file, to describe the replacement policy.
 * \note This policy is used by default.
 * \warning Never used in code !
 */
#define DEFAULT_REPLACEMENT_FCT "LRU"

/**
 * \def DEFAULT_COHERENCE_FCT
 * \param policy Name of the policy wanted to be used in default.
 * \brief Define the string used in XML confifuration file, to describe the coherence policy.
 * \note This policy is used by default.
 * \warning Never used in code !
 */
#define DEFAULT_COHERENCE_FCT "MESI"

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
 * \bug This functions seems to be called with a replacement policy name instead of a coherence one.
 */
void (*get_coherence_function(char * name)) (struct cache *){
  if(strcmp(name, "MSI") == 0)
    return coherence_MSI;
  return coherence_MESI;
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
  char replacement_prot[10];
  struct cache * c;
  struct cache ** cstack;
  //Begin parsing
  res = xmlXPathEvalExpression(BAD_CAST "/Architecture", context);
  CHECK_XPATH(res);
  GET_ATTRIBUT_TXT("name", res->nodesetval->nodeTab[0], archi->name);
  GET_ATTRIBUT_TXT("CPU_name", res->nodesetval->nodeTab[0], archi->CPU_name);
  GET_ATTRIBUT_INT("number_levels", res->nodesetval->nodeTab[0], archi->number_levels);
  archi->levels = calloc(archi->number_levels, sizeof(struct list*));
  cstack = (struct cache **) malloc(archi->number_levels * sizeof(struct cache *));
  xmlXPathFreeObject(res);

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
