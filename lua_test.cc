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

    lua_wrapper::LuaTable result = luaState.getReturnValue();
    //std::cout << "Result: " << result << std::endl;

    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;

    luaState.registerFunction("test", test);
    luaState.loadModule("script/lua_import.lua", "func_test");
    luaState.callFunction("func_test", "test", 0);

    std::cout << "Num Items remaining on stack: " <<
                luaState.getNumItemsOnStack() << std::endl;
    std::cout << "Type of item on stack: " << luaState.getTopStackItemType() << std::endl;

    lua_wrapper::LuaTable table;
    table["foo"] = 1;
    table[1] = "bar";
    table[3.0] = 4;

    std::cout << "DUMP LUA TABLE" << std::endl;
    for(auto i = std::cbegin(result); i != std::cend(result); i++)
    {
        lua_wrapper::LuaType key = i->first;
        lua_wrapper::LuaType value = i->second;
        std::string key_string = key;
        std::string value_string = value;
        std::string value_type = lua_wrapper::to_string(value.get_type());
        std::cout << "TYPE: " << value_type << " KEY: " << key_string << " VAL: " << value_string << std::endl;;

    }

    result["notakey"] = "nowitis";

    auto test = result.get("notakey");

    result["func"].call(1,4);
    int retVal = luaState.getReturnValue();
    std::cout << "Got a RET VALUE OF: " << retVal << std::endl;

    return 0;
}

