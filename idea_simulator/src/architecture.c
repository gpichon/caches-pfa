#include "architecture.h"
#include "list.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

#define GET_NUMBER(inte,name) 	getAttribute(n, name, a_value);	\
                                inte = atoi(a_value)

//Get the value of the attribute "name" in node n
void getAttribute(xmlNode * n, char * name, char * dest){
  xmlChar * a = xmlGetNoNsProp(n, (xmlChar *) name);
  dest[0] = 0;
  if(a == NULL){
    dest = NULL;
  }
  else{
    sprintf(dest, "%s", a);
    xmlFree(a);
  }
}

void prefix_search(xmlNodePtr node, struct architecture * archi, struct cache ** cstack, int stack_head) {
  xmlNodePtr n = node;
  xmlAttr * attr;
  int i;
  int depth = 1;
  struct cache * c;
  int size, linesize, nb_ways, nb_blocks;
  static char a_name[50], a_value[50];

  while(n != NULL){
    attr = n->properties;
    while(attr != NULL){
      //Some architecture information
      getAttribute(n, (char *) attr->name, a_name);
      if(strcmp((char *) attr->name, "name") == 0){
	getAttribute(n, "value", a_value);
	if(strcmp(a_name, "Architecture") == 0){
	  sprintf(archi->name, "%s", a_value);
	}
	else if(strcmp(a_name, "CPUModel") == 0){
	  sprintf(archi->CPU_name, "%s", a_value);
	}
      }
      //Add a cache
      else if(strcmp((char *) attr->name, "type") == 0 && strcmp((char *) a_name, "Cache") == 0){
	
	getAttribute(n, "cache_type", a_value);
	if(a_value != NULL && atoi(a_value) <= 1){ //verify that the cache is not an L1i
	  GET_NUMBER(depth,"depth");
	  if(depth > archi->number_levels){
	    archi->number_levels = depth;
	    archi->levels = calloc(archi->number_levels, sizeof(struct list *));
	    cstack = calloc(archi->number_levels, sizeof(struct cache *));
	  }
	  GET_NUMBER(size, "cache_size");
	  GET_NUMBER(linesize, "cache_linesize");
	  GET_NUMBER(nb_ways, "cache_associativity");
	  nb_blocks = size / (linesize * nb_ways);
	  /* Default policy: LFU */
	  c = init_cache(size, linesize, nb_ways, nb_blocks, depth, &id_line_to_replace_LFU, update_LFU);
	  //Add the cache to the levels table
	  if(archi->levels[depth-1] == NULL){
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
	    for(i = stack_head-1; i >= 0 ; i--){
	      add_list(archi->threads[archi->number_threads], cstack[i]);
	    }
	    archi->number_threads++;
	  }
	}
      }
      attr = attr->next;
    }
    //printf("\n");
    prefix_search(n->children, archi, cstack, stack_head);
    n=n->next;
  }
}

struct architecture parse_archi_file(const char * filename){
  
  struct architecture archi;
  //Default values
  archi.nb_bits = 64;
  strcpy(archi.name, "unknown");
  strcpy(archi.CPU_name, "unknown");
  archi.number_threads = 0;
  archi.number_levels = 0;
  archi.threads = NULL;
  archi.levels = NULL;

  xmlKeepBlanksDefault(0);
  xmlDocPtr xmlfile = xmlParseFile(filename);
  if(xmlfile == NULL){
    fprintf(stderr, "Could not load %s\n", filename);
  }

  xmlNode * root = xmlDocGetRootElement(xmlfile);
  if (root == NULL) {
    fprintf(stderr, "Empty XML file\n");
  }

  struct cache ** cstack = NULL;
  int stack_head = -1;

  prefix_search(root, &archi, cstack, stack_head);

  xmlFreeDoc(xmlfile);
  xmlCleanupParser();
  if(cstack != NULL){
    free(cstack);
  }

  return archi;
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

void print_caches(struct architecture * archi){
  int i;
  struct list * l;
  printf("Liste des caches\n");
  for(i=0;i<archi->number_levels;i++){
    l = archi->levels[archi->number_levels-i-1];
    while(l != NULL){      
      printf("\tL%d (misses: %d, hits: %d, writes_back: %d, broadcasts: %d)\n", l->cache->depth, l->cache->misses, l->cache->hits, l->cache->writes_back, l->cache->broadcasts);
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
