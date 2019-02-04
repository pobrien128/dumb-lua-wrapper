#ifndef LUA_HELPERS_HEADER_HH
#define LUA_HELPERS_HEADER_HH

//LUA
#include <lua.hpp>

//STD C++
#include <functional>
#include <sstream>

namespace lua_wrapper{

typedef std::function<int(lua_State*)> LuaCppFunction;
char const * cpp_function = "CPP_Function";

static bool lua_iscppfunction(lua_State* L, int index)
{
    void* funcPtr = luaL_testudata(L, index, cpp_function);

    return funcPtr;
}

static int functionDispatcher ( lua_State* L )
{
    void* funcPtr = luaL_testudata(L, 1, cpp_function);
    if(funcPtr)
    {
            LuaCppFunction f = *static_cast<LuaCppFunction *> (funcPtr);
            lua_remove(L,1);
            int result = (f)(L);
            return result;
        }
        return 0;
    }

    static int functionCleanup(lua_State* L)
    {
        LuaCppFunction* d = static_cast< LuaCppFunction *>(luaL_testudata(L, 1, cpp_function));
        if ( d == NULL ){
            std::cerr << "ERROR: " << __func__ << " called on data of type: " << lua_typename(L, lua_type(L, 1 ) ) << std::endl;
            lua_pushstring(L, "C++ function object garbage collection failure!");
            lua_error(L);
        } else {
            d->~LuaCppFunction();
        }
        return 0;
    }

    static int functionToString( lua_State* L )
    {
    LuaCppFunction* d = static_cast< LuaCppFunction *>(luaL_checkudata(L, 1, cpp_function));
    std::stringstream result;
    result << "c++ function: " << d->target_type().name();
    lua_pushstring(L, result.str().c_str());
    return 1;
}

};
#endif
