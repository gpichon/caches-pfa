#include"scriptclua.h"

#define NB_INSTRUCTIONS 20

int script_c_lua()
{
  lua_State *L = luaL_newstate();   /* opens Lua  state*/
  int current = 0;
  int nb_thread = 4;
  int i;

  luaL_openlibs(L);   
  luaL_loadfile(L, "entrelacement.lua");
     
  lua_call(L, 0, 0);

  for(i = 0; i <= NB_INSTRUCTIONS; i++){ 
    lua_getglobal(L, "foutre");                  
    lua_pushnumber(L, current);                     
    lua_pushnumber(L, nb_thread);                    
 
    
  lua_call(L, 2, 1);  

  current = lua_tonumber(L, -1);
  printf("%d current\n", current);
  lua_pop(L, 1);  
  }
  lua_close(L);
return 0;
}

