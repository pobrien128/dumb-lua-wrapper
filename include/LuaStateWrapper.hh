#ifndef LUA_STATE_WRAPPER_HEADER
#define LUA_STATE_WRAPPER_HEADER

//LUA
#include <lua.hpp>

//C++
#include <string>
#include <iostream>
namespace lua_wrapper
{
class LuaStateWrapper
{
    public:
        LuaStateWrapper()
            :   m_luaState(luaL_newstate())
            ,   m_numArgs(0)
            ,   m_owningInstance(true)
        {
        }

        LuaStateWrapper(lua_State* luaState)
            :  m_luaState(luaState) 
            ,  m_numArgs(0)
            ,  m_owningInstance(false)
        {
        }

        ~LuaStateWrapper()
        {
            if(m_owningInstance)
                lua_close(m_luaState);
        }

        void openLibs()
        {
            luaL_openlibs(m_luaState);
        }

        void loadModule(const std::string& filename, const std::string modulename)
        { 
            std::cout << "Loading module " << modulename << " from file; " << filename << std::endl;
            int status = luaL_dofile(m_luaState, filename.c_str());
            assert(status == 0);
            lua_setglobal(m_luaState, modulename.c_str());
            lua_settop(m_luaState,0);
        }

        template < typename ...Types >
        void callFunction(const std::string& module, const std::string& funcName, const int& numRetValues, Types ... args)
        {
            lua_getglobal(m_luaState, module.c_str());
            lua_getfield(m_luaState, -1, funcName.c_str());
            std::cout << "About to push arguments on the stack" << std::endl;
            pushArguments(args...);
            std::cout << "Done pushing arguments, about to call " << funcName << " with " << m_numArgs << " arguments and " << numRetValues << " return values." <<  std::endl;
            lua_call(m_luaState, m_numArgs, numRetValues); 
            m_numArgs = 0;
        }

        void callFunction(const std::string& module, const std::string& funcName, const int& numRetValues)
        {
            lua_getglobal(m_luaState, module.c_str());
            lua_getfield(m_luaState, -1, funcName.c_str());
            lua_call(m_luaState, 0, numRetValues); 
        }
        
        template < typename T >
        T getReturnValue()
        {
            T retVal;
            _getReturnValue(retVal);
            lua_pop(m_luaState, 1);
            return retVal;
        }

        void registerFunction(const std::string luaFuncName, lua_CFunction f)
        {
            lua_register( m_luaState, luaFuncName.c_str(), f);
        }

        int getNumItemsOnStack()
        {
           return lua_gettop(m_luaState);
        }

        std::string getTopStackItemType()
        {
           if(lua_isinteger(m_luaState, -1))
                return "INTEGER";
           else if(lua_isnumber(m_luaState, -1)) 
               return "NUMBER";
           else if(lua_isboolean(m_luaState, -1))
               return "BOOLEAN";
           else if(lua_iscfunction(m_luaState, -1))
               return "C FUNCTION";
           else if(lua_isfunction(m_luaState, -1))
                return "FUNCTION";
           else if(lua_isstring(m_luaState, -1))
                return "STRING";
           else if(lua_istable(m_luaState, -1))
                return "TABLE";
           else
               return "UNKNOWN";
        }

        template< typename T >
        T getArgument()
        {
           T retVal; 
           _getArgument(retVal);
           return retVal;
        }

    private:

        void _getArgument(int& retVal)
        {
            assert(lua_isnumber(m_luaState, -1));
            retVal = (int)lua_tointeger(m_luaState, -1);
        }

        void _getArgument(float& retVal)
        {
            assert(lua_isnumber(m_luaState, -1));
            retVal = (float)lua_tonumber(m_luaState, -1);
        }

        void _getArgument(double& retVal)
        {
            assert(lua_isnumber(m_luaState, -1));
            retVal = (double)lua_tonumber(m_luaState, -1);
        }

        void _getReturnValue(int& retVal)      
        {
            assert (lua_isnumber(m_luaState, -1));
            retVal = (int)lua_tointeger(m_luaState, -1);
        }
        void _getReturnValue(float& retVal)
        {
            assert (lua_isnumber(m_luaState, -1));
            retVal = (float)lua_tonumber(m_luaState, -1);
        }

        void _getReturnValue(double& retVal)
        {
            assert (lua_isnumber(m_luaState, -1));
            retVal = (double)lua_tonumber(m_luaState, -1);
        }

        template< typename ...Types >
        void pushArguments(Types ... args)
        {
            pushArgument(args...);
        }

        template< typename T, typename... Types >
        void pushArgument(T t, Types... args )
        {
            pushArgument(t);
            pushArgument(args...);
        }

        void pushArgument(const int& arg){
            m_numArgs++;
            lua_pushnumber(m_luaState, arg);
        }
        void pushArgument(const float& arg){
            m_numArgs++;
            lua_pushnumber(m_luaState, arg);
        }


        lua_State* m_luaState;
        int m_numArgs;
        bool m_owningInstance;

};
};


#endif


