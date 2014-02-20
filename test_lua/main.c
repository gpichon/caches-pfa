#include <stdio.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
    
int l_calltest(lua_State *L){
  
  lua_pushstring(L, "pfffffffffffffff");

  return 1;
}

int l_calltest2(lua_State *L){
  const char * buf = lua_tostring(L, -1);
  int len;
  
  len = strlen(buf);
  
  len = len * 2;

  lua_pushstring(L, buf);
  lua_pushnumber(L, len);
  return 1;
}


int main (void) {

  lua_State *L = luaL_newstate();   /* opens Lua */
 
  luaL_openlibs(L);
   
  luaL_dofile(L, "brouillon.lua");
  
  
  /*--- Lecture dans un script lua ---*/

  //on rempli la pile avec les variables
  lua_getglobal(L, "toto");
  lua_getglobal(L, "titi");
  lua_getglobal(L, "tata");
  
  //On affiche les valeurs succecives du sommet de pile

  int i = lua_tonumber(L, -1);
  
  printf("%d valeur de  L -1\n", i);
  lua_pop(L, 1);
  
  i = lua_tonumber(L, -1);
  printf("%d valeur de  L -1\n", i);
  lua_pop(L, 1);

  i = lua_tonumber(L, -1);
  printf("%d valeur de  L -1\n", i);
  lua_pop(L, 1);
  /*--- ---*/


  /*--- Appel d'un fonction lua dans un script lua ---*/
  lua_getglobal(L, "foo");
  lua_pushstring(L, "trololo");
  // Lua State nb_args, nb_retour, err
  lua_pcall(L, 1, 0, 0);
  /*---  ---*/
  
  /*--- Appel de fonction C à travers un script lua ---*/
  
  //A débugger

  lua_pushcfunction(L, l_calltest);
  lua_setglobal(L, "patern_c");

  int res;
 

  //Saisie du premier patern
  lua_getglobal(L, "bar");
  lua_pushstring(L, "12345");
  lua_pcall(L, 1, 1, 0);
 
  res = lua_tonumber(L, -1);
  printf("%d res bar\n", res);
 
  /*
    
  lua_pushcfunction(L, l_calltest2);
  lua_setglobal(L,"partern_c2");

  //Saisie du second patern
  lua_getglobal(L, "bar2");
  lua_pushstring(L, "1234567");

  s = lua_tostring(L, -1);
  lua_pop(L, 1);
  res = lua_tonumber(L, -1);
  printf("%d res %s s bar2\n", res, s);
  */
  return 0;
}
