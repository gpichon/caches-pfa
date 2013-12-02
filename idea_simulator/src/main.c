#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "add_line_hierarchy.h"
#include "architecture.h"

#define FILENAME "data/architest.xml"

int main(int argc, char *argv[]) {
  (void) argc;
  (void) argv;

  struct architecture A = parse_archi_file(FILENAME, 1);
  struct architecture *archi = &A;

  /* Classics loads */
  load_line_hierarchy(archi->levels, archi->threads[0], 163+2048); /* Miss L1_0, L2_0, L3_0 */
  load_line_hierarchy(archi->levels, archi->threads[1], 163+2048); /* Miss L1_1 Hit L2_0 */
  load_line_hierarchy(archi->levels, archi->threads[2], 163+2048); /* Miss L1_2, L2_1 Hit L3_0 */

  /* Store value in cache -> Hit */
  store_line_hierarchy(archi->levels, archi->threads[0], 163+2048);   /* Hit L1_0 */

  /* Value in L1_1 was invalidated last store, but value is still in L2_0 */
  load_line_hierarchy(archi->levels, archi->threads[1], 163+2048); /* WB L1_0 Miss L1_1 Hit L2_0*/

  /* Invalidated archi->threads */
  store_line_hierarchy(archi->levels, archi->threads[2], 163+2048); /* Miss L1_2, L2_1 WB L2_0 Hit L3_0 */


  /* Informations about caches */
  print_caches(archi);
  delete_archi(archi);

  return EXIT_SUCCESS;
}
