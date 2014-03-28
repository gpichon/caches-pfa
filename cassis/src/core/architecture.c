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
#include "option.h"

// True when the user has entered a value and must be warned if the input is not correct
bool display_warning = true;
bool fatal = false;

#define QUOTE(name) #name
#define STR(macro) QUOTE(macro)


/**
 * \def CHECK_XPATH(result)
 * \brief Check XPath result
 */
#define CHECK_XPATH(result) do { if (result == NULL) { fprintf(stderr, "Error XPath request\n"); \
      return EXIT_FAILURE;						\
    } } while(0)

/**
 * \def CHECK_ALLOC(ptr)
 * \brief Check if the allocation on ptr worked
 */
#define CHECK_ALLOC(ptr) do { if(ptr == NULL) { fprintf(stderr, "Allocation error\n"); _exit(1); } } while(0)


#ifndef XSL_DOC_FILE
#define XSL_DOC_FILE architecture/archi_maker.xsl
#endif


/**
 * \def XSL_DOC
 * \param path 
 * \brief Relative path of the xsl sheet describing the transfomation hwloc -> cassis
 */
#define XSL_DOC STR(XSL_DOC_FILE)

/**
 * \def GET_ATTRIBUT_TXT(name,node,target)
 * \brief Copy the node's text (identified by node name) in the target.
 */
#define GET_ATTRIBUT_TXT(name,node,target)   tmp = xmlGetProp(node, BAD_CAST name); \
  display_warning = false;						\
  if(tmp != NULL){							\
    display_warning = true;						\
    sprintf(buf, "%s", tmp);						\
    strcpy(target, buf); }						\
  xmlFree(tmp);								\
  buf[0] = 0

/**
 * \def GET_ATTRIBUT_INT(name,node,target)
 * \brief Copy the node's number (identified by node name) in the target.
 */
#define GET_ATTRIBUT_INT(name,node,target)   tmp = xmlGetProp(node, BAD_CAST name); \
  sprintf(buf, "%s", tmp);						\
  target = strtol(buf, NULL, 0);					\
  xmlFree(tmp);								\
  buf[0] = 0

/**
 * \def FATAL_WARNING
 * \brief Warning that can crash the program execution
 */
#define FATAL_WARNING   fprintf(stderr, "\033[31mWARNING\033[0m: architecture not valid\n"); \
  fatal = true;
  
struct level{
  int type;
  enum cache_coherence coherence;
  bool snooping;
  bool directory_manager;
};

/**
 * \brief Compare the replacement policy's name in order to choose the associated replacement function.
 * \param name Name to compare with implemented policies.
 */
void (*get_replacement_function(char *name)) (struct cache *){
  if(strcmp(name, "FIFO") == 0)
    return replacement_FIFO;
  if(strcmp(name, "LFU") == 0)
    return replacement_LFU;
  if(strcmp(name, "LRU") == 0)
    return replacement_LRU;
  if(display_warning)
    fprintf(stderr, "\033[31mWARNING\033[0m: Replacement protocol not recognized: %s , set to LRU\n", name);
  return replacement_LRU;
}

/**
 * \brief Compare the coherence policy's name in order to choose the associated replacement function.
 * \param name Name to compare with implemented policies.
 */
enum cache_coherence coherence (char *name){
  if(strcmp(name, "MSI") == 0)
    return MSI;
  if(strcmp(name, "MESI") == 0)
    return MESI;
  if(strcmp(name, "MOSI") == 0){
    fprintf(stderr, "\033[31mWARNING\033[0m: Coherence protocol %s is experimental\n", name);
    return MOSI;
  }
  if(strcmp(name, "MOESI") == 0){
    fprintf(stderr, "\033[31mWARNING\033[0m: Coherence protocol %s is experimental\n", name);
    return MOESI;
  }
  if(strcmp(name, "MESIF") == 0){
    fprintf(stderr, "\033[31mWARNING\033[0m: Coherence protocol %s is experimental\n", name);
    return MESIF;
  }
  if(display_warning)
    fprintf(stderr, "\033[31mWARNING\033[0m: Coherence protocol not recognized: %s , set to MESI\n", name);
  return MESI;
}

/**
 * \brief Gives the value of the cache corresponding to the xml text
 * \param txt The xml text in the attribute type
 * \return The corresponding number
 */
int get_cache_type(char *txt){
  if(strcmp(txt, "exclusive") == 0)
    return Exclusive;
  if(strcmp(txt, "inclusive") == 0)
    return Inclusive;
  if(strcmp(txt, "niio") == 0)
    return NIIO;
  if(strcmp(txt, "nieo") == 0)
    return NIEO;
  if(display_warning)
    fprintf(stderr, "\033[31mWARNING\033[0m: Cache type not recognized: %s , set to Inclusive\n", txt);
  return Inclusive;
}

/**
 * \brief Turn the text of a boolean (true/false) into a boolean
 * \param txt The text
 * \return The corresponding bool
 */
bool get_bool(char *txt){
  if(strcmp(txt, "true") == 0)
    return true;
  if(strcmp(txt, "false") == 0)
    return false;
  if(display_warning)
    fprintf(stderr, "\033[31mWARNING\033[0m: Boolean value not recognized: %s , set to false\n", txt);
  return false;
}

/**
 * \brief Generates the name of the XML output file from the previous xml file
 * \param name Previous name.
 * \param out New name.
 */
void change_filename(const char *name, char *out){
  int l = strlen(name);
  char extention[30];
  strcpy(extention, &name[l-4]);
  strcpy(out, name);
  if(strcmp(extention, ".xml") == 0){
    out[l-4] = 0;
  }
  strcat(out, ".cassis.xml");
}

int parse_archi_file(const char *filename, struct architecture *archi){
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

  //Check if we need to create a .cassis.xml file
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
  archi->warning = false;

  xmlXPathInit();
  xmlXPathContextPtr context = xmlXPathNewContext(doc);
  if (context == NULL) {
    fprintf(stderr, "Error XPath context\n");
  }
  xmlXPathObjectPtr res;
  xmlNodePtr cur;
  xmlChar *tmp;
  int depth, size, linesize, nb_ways, nb_blocks;
  int stack_head = -1;
  char replacement_prot[10];
  struct cache *c;
  struct node *n;
  struct node **cstack;

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
  struct level *L = (struct level *) malloc(archi->number_levels * sizeof(struct level));
  CHECK_ALLOC(L);
  res = xmlXPathEvalExpression(BAD_CAST "//Level", context);
  CHECK_XPATH(res);
  if(archi->number_levels != (unsigned int) res->nodesetval->nodeNr){
    fprintf(stderr, "Parsing error : wrong number of \"Level\"");
    _exit(1);
  }
  for(i=0; i<res->nodesetval->nodeNr; i++){
    cur = res->nodesetval->nodeTab[i];
    j = archi->number_levels - i - 1;
    GET_ATTRIBUT_TXT("type", cur, buf2);
    L[j].type = get_cache_type(buf2);
    GET_ATTRIBUT_TXT("coherence_protocol", cur, buf2);
    L[j].coherence = coherence(buf2);
    GET_ATTRIBUT_TXT("snooping", cur, buf2);
    L[j].snooping = get_bool(buf2);
    GET_ATTRIBUT_TXT("directory_manager", cur, buf2);
    L[j].directory_manager = get_bool(buf2);
  }
  xmlXPathFreeObject(res);

  //Cache parsing
  struct node **first_sibling = (struct node **) calloc(archi->number_levels, sizeof(struct node *));
  struct node **last_sibling = (struct node **) calloc(archi->number_levels, sizeof(struct node *));
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
    c = init_cache(size, linesize, nb_ways, nb_blocks, depth, get_replacement_function(replacement_prot), L[depth-1].coherence, L[depth-1].type, L[depth-1].snooping, L[depth-1].directory_manager);
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

  /* Check the architecture */
  check_archi(archi);
  if(fatal == true)
    archi->warning = true;

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

void print_archi_rec(struct node *n, int nb_levels){
  unsigned int i;
  printf("L%d (size: %d) ", n->data->depth, n->data->size);
  if(n->data->depth > 1){
    switch(n->data->type){
    case Inclusive:
      printf("inclusive");
      break;
    case Exclusive:
      printf("exclusive");
      break;
    case NIIO:
      printf("NIIO");
      break;
    case NIEO:
      printf("NIEO");
      break;
    }
  }
  if(n->data->snooping && n->data->depth != nb_levels)
    printf(", with snooping");
  if(n->data->directory && n->data->depth > 1)
    printf(", with directory manager");
  
  printf("\n");

  for(i=0;i<n->nb_children;i++){
    print_archi_rec(get_child(n,i), nb_levels);
  }
}

void print_archi(struct architecture *archi){
  printf("Architecture %d bits : %s\n", archi->nb_bits, archi->name);
  printf("CPU model : %s\n", archi->CPU_name);
  struct node *root = get_root(archi->threads[0]);
  print_archi_rec(root, archi->number_levels);
}

int convert_archi_xml(const char *file_in, const char *file_out){
  FILE *out = fopen(file_out,"w");
  if(out == NULL){
    fprintf(stderr, "Cannot open %s\n", file_out);
    return EXIT_FAILURE;
  }

  xmlSubstituteEntitiesDefault(1);
  xmlLoadExtDtdDefaultValue = 1;

  xmlDocPtr doc = xmlParseFile(file_in);
  xsltStylesheetPtr xsl = xsltParseStylesheetFile(BAD_CAST XSL_DOC);
  int nb_params = 0;
  const char *params[nb_params + 1];
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

void print_caches_rec(struct node *n, int nb_levels, unsigned int j){
  unsigned int i;
  printf("L%d  basics     (misses:    %10d, hits:     %10d, writes_back: %10d)\n", n->data->depth, n->data->misses[j], n->data->hits[j], n->data->writes_back[j]);
  if (verbose_mode > 1){
    printf("    evinctions (coherence: %10d, capacity: %10d, cache_types: %10d)\n", n->data->evincted_coherence[j], n->data->evincted_capacity[j], n->data->evincted_caches_types[j]);
    if (verbose_mode > 2){
      printf("    misses     (snooping:  %10d, above:    %10d, below:       %10d)\n", n->data->misses_snooping[j], n->data->misses_above[j], n->data->misses_below[j]);
      if (verbose_mode > 3){
	printf("    broadcasts (coherence: %10d, snooping: %10d)\n\n", n->data->broadcast_coherence[j], n->data->broadcast_snooping[j]);
      }
    }
  }
  for(i=0;i<n->nb_children;i++){
    print_caches_rec(get_child(n,i), nb_levels, j);
  }
}

void print_caches(struct architecture *archi, int arch){
  if (arch){
    print_archi(archi);
    printf("\n\n");
  }
  struct node *root = get_root(archi->threads[0]);
  unsigned int j;
  for (j=print_mode; j<tracking_count; j++){
    if (j==0){
      printf("Statistics to all instructions\n");
    }
    else{
      printf("Statistics related to tracked instructions\n");
    }
    print_caches_rec(root, archi->number_levels, j);
    printf("\n\n");
  }
}

int get_size_below_rec(struct node *n){
  int sum = n->data->size;
  unsigned int i;
  for(i=0;i<n->nb_children;i++){
    sum += get_size_below_rec(get_child(n, i));
  }
  return sum;
}

/**
 * \brief Get the sum of the sizes of the caches below a cache
 * \param n The cache to check
 */
int get_size_below(struct node *n){
  unsigned int i;
  int sum = 0;
  for(i=0;i<n->nb_children;i++){
    sum += get_size_below_rec(get_child(n, i));
  }
  return sum;
}

/**
 * \brief Checks the architecture consistency cache by cache
 * \param n The node to check (call with the root for all caches)
 * \return true if no error has occured
 */
bool check_cache_rec(struct node *n){
  unsigned int i;
  for(i=0;i<n->nb_children;i++){
    check_cache_rec(get_child(n, i));
  }
  
  //Check size for inclusive cache
  if(n->data->type == Inclusive){
    if(get_size_below(n) > n->data->size){
      FATAL_WARNING;
      fprintf(stderr, "Size error: an inclusive cache (L%d) is too small for the lower levels\n", n->data->depth);
    }
  }

  return true;
}

void check_archi(struct architecture *archi){
  struct node *root = get_root(archi->threads[0]);
  struct node *n = archi->threads[0];
  //If last level is not inclusive and has no directory manager
  if(root->data->type != Inclusive){
    //Is there a directory manager
    if(root->data->directory == false){
      //Is there snooping in all the levels below
      bool all_snooping = true;
      while(n != root){
	if(n->data->snooping != true)
	  all_snooping = false;
	n = get_parent(n);
      }
      if(all_snooping == false){
	FATAL_WARNING;
	fprintf(stderr, "Last level (L%d) is not inclusive and has no directory manager nor snooping in all levels below\n", archi->number_levels);
      }
    }
  }

  check_cache_rec(root);
}

void delete_archi_rec(struct node *n){
  unsigned int i;
  for(i = 0; i<n->nb_children; i++){
    if(get_child(n,i)){
      delete_archi_rec(get_child(n, i));
    }
  }
  delete_cache(n->data);
  free_node(n);
}

void delete_archi(struct architecture *archi){
  struct node *root = get_root(archi->threads[0]);
  delete_directories(root);
  delete_archi_rec(root);
  free(archi->threads);
}
