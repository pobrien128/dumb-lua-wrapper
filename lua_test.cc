#include <iostream>
#include <lua.hpp>
#include <cassert>
#include <LuaStateWrapper.hh>

#include <memory>
#include <functional>

class TestClass
{
    public:
        TestClass()
            :   m_num(70)
        {

        }

        int print_num(lua_State* L){
            std::cout << "WE GOt A MEMBER FUNCTION " << m_num << std::endl;
        }
    private:
        int m_num;

};


int test(lua_State *L)
{
    std::shared_ptr<lua_wrapper::LuaStateWrapper> lua_wrapper(new lua_wrapper::LuaStateWrapper(L));
    std::cout << "Number of items on lua stack: " << lua_wrapper->getNumItemsOnStack() << std::endl;

    double arg = lua_wrapper->getArgument<double>();
    std::cout << "TEST " << arg << std::endl;
    return 0;

}

int hello_world(lua_State* L)
{
    std::cout << "WHERE WE IS!" << std::endl;
    std::shared_ptr<lua_wrapper::LuaStateWrapper> lua_wrapper(new lua_wrapper::LuaStateWrapper(L));
    double arg = lua_wrapper->getArgument<double>();
    std::cout << "Hello world! " << arg << std::endl;
    return 0;
}


int main(int argc, char** argv){

    TestClass testclass_inst;
    std::function<int(lua_State*)> cfunc = std::bind(&TestClass::print_num, &testclass_inst, std::placeholders::_1);

    std::string name = cfunc.target_type().name();
    std::cout << name << std::endl;

    std::shared_ptr<lua_wrapper::LuaStateWrapper> luaState = std::make_shared<lua_wrapper::LuaStateWrapper>();
    std::cout << "Num Items remaining on stack: " <<
                luaState->getNumItemsOnStack() << std::endl;
    std::cout << "Opening standard libs" << std::endl;
    luaState->openLibs();
    std::cout << "Num Items remaining on stack: " <<
                luaState->getNumItemsOnStack() << std::endl;
    luaState->loadModule("script/lua_test.lua", "mymath");
    std::cout << "Num Items remaining on stack: " <<
                luaState->getNumItemsOnStack() << std::endl;
    std::cout << "HERE" << std::endl;
    luaState->callFunction("mymath", "fact", 1, 5);
    std::cout << "HERE2" << std::endl;
    std::cout << "Num Items remaining on stack: " <<
                luaState->getNumItemsOnStack() << std::endl;

    std::cout << "HERE3" << std::endl;
    lua_wrapper::LuaTable result = luaState->getReturnValue();

    lua_wrapper::LuaTable test_table;
    test_table["0"]="Please work";
    test_table[3]=2;

    luaState->callFunction("mymath", "table_test", 0, result);

    std::cout << "Num Items remaining on stack: " <<
                luaState->getNumItemsOnStack() << std::endl;

    luaState->registerFunction("test", test);
    luaState->loadModule("script/lua_import.lua", "func_test");
    std::cout << "ABOUT TO DO IT!" << std::endl;
    luaState->callFunction("func_test", "test", 1, cfunc);

    std::cout << "Num Items remaining on stack: " <<
                luaState->getNumItemsOnStack() << std::endl;
    std::cout << "Type of item on stack: " << luaState->getTopStackItemType() << std::endl;

    lua_wrapper::LuaCppFunction f = luaState->getReturnValue();
    std::cout << "THIS IS WHAT CAME out: " << f.target_type().name() << std::endl;
    f(luaState->getLuaState());

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

    result["func"].call(1,6);
    int retVal = luaState->getReturnValue();
    std::cout << "Got a RET VALUE OF: " << retVal << std::endl;

    return 0;
}

