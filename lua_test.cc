#include <iostream>
#include <lua.hpp>
#include <cassert>
#include <LuaStateWrapper.hh>

int test(lua_State *L)
{
    lua_wrapper::LuaStateWrapper lua_wrapper(L);
    std::cout << "Number of items on lua stack: " << lua_wrapper.getNumItemsOnStack() << std::endl;

    int arg = lua_wrapper.getArgument<int>();
    std::cout << "TEST " << arg << std::endl;

}

int main(int argc, char** argv){

    lua_wrapper::LuaStateWrapper luaState;
    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;
    std::cout << "Opening standard libs" << std::endl;
    luaState.openLibs();
    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;
    luaState.loadModule("script/lua_test.lua", "mymath");
    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;
    luaState.callFunction("mymath", "fact", 1, 5);
    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;

    int result = luaState.getReturnValue<int>();
    std::cout << "Result: " << result << std::endl;

    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;

    luaState.registerFunction("test", test);
    luaState.loadModule("script/lua_import.lua", "func_test");
    luaState.callFunction("func_test", "test", 0);

    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;
    std::cout << "Type of item on stack: " << luaState.getTopStackItemType() << std::endl;

    return 0;
}

