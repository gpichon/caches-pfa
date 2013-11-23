#include "architecture.h"
#include "list.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

#define GET_NUMBER(inte,name) 	a_value = xmlGetNoNsProp(n, (xmlChar *) name); \
                                sprintf(buf, "%s", a_value);	\
                                inte = atoi(buf); \
				free(a_value)

void prefix_search(xmlNodePtr node, struct architecture * archi, struct cache ** cstack, int stack_head) {
  xmlNodePtr n = node;
  xmlAttr * attr;

  int i;
  char buf[30];
  int depth = 1;
  struct cache * c;
  int size, linesize, nb_ways, nb_blocks;
  xmlChar * a_name, * a_value;

  while(n != NULL){
    //printf("%s | ", n->name);
    attr = n->properties;
    while(attr != NULL){
      //printf(" (%s : %s)", attr->name, xmlGetNoNsProp(n, attr->name));
      //Some architecture information
      a_name = xmlGetNoNsProp(n, attr->name);
      if(strcmp((char *) attr->name, "name") == 0){
	a_value = xmlGetNoNsProp(n, (xmlChar *) "value");
	if(strcmp((char *) a_name, "Architecture") == 0){
	  sprintf(archi->name, "%s", a_value);
	}
	else if(strcmp((char *) a_name, "CPUModel") == 0){
	  sprintf(archi->CPU_name, "%s", a_value);
	}
	if(a_value)
	  xmlFree(a_value);
      }
      //Add a cache
      else if(strcmp((char *) attr->name, "type") == 0 && strcmp((char *) a_name, "Cache") == 0){
	GET_NUMBER(depth,"depth");
	GET_NUMBER(size, "cache_size");
	GET_NUMBER(linesize, "cache_linesize");
	GET_NUMBER(nb_ways, "cache_associativity");
	nb_blocks = size / (linesize * nb_ways);
	c = init_cache(size, linesize, nb_ways, nb_blocks, depth);
	//Add the cache to the levels table
	if(archi->levels[depth-1] == NULL){
	  archi->levels[depth-1] = init_list(c);
	}
	else{
	  add_list(archi->levels[depth-1], c);
	}
	//And to the threads table
	//First pop levels below in the stack
	while(stack_head > 0 && cstack[stack_head]->depth <= depth){
	  stack_head--;
	}
	//Then we push
	stack_head++;
	cstack[stack_head] = c;

	if(depth == 1){
	  archi->threads[archi->number_threads] = init_list(c);
	  for(i = stack_head-1; i >= 0 ; i--){
	    add_list(archi->threads[archi->number_threads], cstack[i]);
	  }
	  archi->number_threads++;
	}
      }
      if(a_name)
	xmlFree(a_name);
      attr = attr->next;
    }
    //printf("\n");
    prefix_search(n->children, archi, cstack, stack_head);
    n=n->next;
  }
  
  if(depth > archi->number_levels)
    archi->number_levels = depth;
}

struct architecture parse_archi_file(const char * filename){
  
  int i;
  struct architecture archi;
  //Default values
  archi.nb_bits = 64;
  strcpy(archi.name, "unknown");
  strcpy(archi.CPU_name, "unknown");
  archi.number_threads = 0;
  archi.number_levels = 0;
  for(i=0;i<MAX_THREADS;i++){
    archi.threads[i] = NULL;
  }
  for(i=0;i<MAX_LEVELS;i++){
    archi.levels[i] = NULL;
  }

  xmlKeepBlanksDefault(0);
  xmlDocPtr xmlfile = xmlParseFile(filename);
  if(xmlfile == NULL){
    fprintf(stderr, "Could not load %s\n", filename);
  }

  xmlNode * root = xmlDocGetRootElement(xmlfile);
  if (root == NULL) {
    fprintf(stderr, "Empty XML file\n");
  }

  struct cache * cstack[MAX_THREADS];
  for(i=0;i<MAX_THREADS;i++){
    cstack[i] = NULL;
  }
  int stack_head = -1;

  prefix_search(root, &archi, cstack, stack_head);

  xmlFreeDoc(xmlfile);
  xmlCleanupParser();

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
    printf("Pour level %d\n", i+1);
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
    l = archi->levels[i];
    printf("Pour level %d\n", i+1);
    while(l != NULL){
      printf("\tL%d (taille : %d, ligne : %d, associativite %d, nb_blocks : %d, misses: %d, hits: %d, writes_back: %d)\n", l->cache->depth, l->cache->size, l->cache->linesize, l->cache->nb_ways,  l->cache->nb_blocks, l->cache->misses, l->cache->hits, l->cache->writes_back);
      l = l->next;
    }
  }
}
