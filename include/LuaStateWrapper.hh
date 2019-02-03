#ifndef LUA_STATE_WRAPPER_HEADER
#define LUA_STATE_WRAPPER_HEADER

//LUA
#include <lua.hpp>

//C++
#include <string>
#include <iostream>
#include <map>
#include <sstream>
#include <memory>

//C
#include <cstring>
namespace lua_wrapper
{

class LuaTable;
class LuaFunction;
class LuaStateWrapper;
union LuaTypeUnion
{
    float f;
    double d;
    int i;
};

enum class LuaDatatype
{
    lua_float,
    lua_double,
    lua_int,
    lua_string,
    lua_table,
    lua_function,
    lua_undefined
};

static std::string to_string(const LuaTable& table);
static std::string to_string(LuaDatatype type){
    switch(type)
    {
        case LuaDatatype::lua_float:
            return "Float";
            break;
        case LuaDatatype::lua_double:
            return "Double";
            break;
        case LuaDatatype::lua_int:
            return "Int";
            break;
        case LuaDatatype::lua_string:
            return "String";
            break;
        case LuaDatatype::lua_table:
            return "Table";
            break;
        case LuaDatatype::lua_function:
            return "Lua Function";
            break;
        default:
            return "Undefined";
    }
};


class LuaFunction
{
    public:
        LuaFunction()
        {}

        LuaFunction(std::shared_ptr<LuaStateWrapper> l, int ref)
            :   m_luaState(l)
            ,   m_ref(ref)
        {
        }

        ~LuaFunction();

        int get_reference(){
            return m_ref;
        }

        void call(const int& numRetValues);

        template < typename ...Types >
        void call(const int& numRetValues, Types ... args);


    private:
        std::weak_ptr<LuaStateWrapper> m_luaState;
        int m_ref;

};

class LuaType
{
    public:
        LuaType()
            :   type(LuaDatatype::lua_undefined)
        {
        }

        LuaType(const LuaType& rtype);
        LuaType& operator=(const LuaType& rtype);

        LuaType(const double& d)
        {
            data.d = d;
            type = LuaDatatype::lua_double;
        }

        LuaType(const float& f)
        {
            data.f = f;
            type = LuaDatatype::lua_float;
        }

        LuaType(const int& i)
        {
            data.i = i;
            type = LuaDatatype::lua_int;
        }

        LuaType(const std::string& _s)
        {
            s = _s; 
            type = LuaDatatype::lua_string;
        }

        LuaType(const char* _s)
        {
            s = std::string(_s);
            type = LuaDatatype::lua_string;
        }

        LuaType(std::shared_ptr<LuaTable> t);

        LuaType(std::shared_ptr<LuaFunction> f)
        {
            func=f;
            type = LuaDatatype::lua_function;
        }

        ~LuaType();

        //Strictly used by stl containers
        friend bool operator<(const LuaType& l, const LuaType& r)
        {
            std::string l_serialized(l.make_hash());
            std::string r_serialized(r.make_hash());
            int ret = strcmp(l_serialized.c_str(), r_serialized.c_str());
            return ret < 0;
        }

        operator double() const { 
            assert(type == LuaDatatype::lua_int || type == LuaDatatype::lua_float || type == LuaDatatype::lua_double); 
            if(type == LuaDatatype::lua_float){
                return data.f;
            } else if (type == LuaDatatype::lua_double){
                return data.d;
            } else if (type == LuaDatatype::lua_int){
                return data.i; 
            } else {
                assert(false);
            }
        }

        operator float() const {
            assert(type == LuaDatatype::lua_int || type == LuaDatatype::lua_float || type == LuaDatatype::lua_double); 
            if(type == LuaDatatype::lua_float){
                return data.f;
            } else if (type == LuaDatatype::lua_double){
                return data.d;
            } else if (type == LuaDatatype::lua_int){
                return data.i; 
            } else {
                assert(false);
            }
        }

        operator std::string() const {
            if(type == LuaDatatype::lua_string)
            {
                return s;
            } else if (type == LuaDatatype::lua_int) {
                return std::to_string(data.i);
            } else if (type == LuaDatatype::lua_float) {
                return std::to_string(data.f);
            } else if (type == LuaDatatype::lua_double) {
                return std::to_string(data.d);
            } else if (type == LuaDatatype::lua_table) {
                return to_string(*t);
            } else if (type == LuaDatatype::lua_function) {
                return std::to_string(func->get_reference());
            }
        }

        std::string make_hash() const{
            std::stringstream streamer;
            streamer << to_string(type);
            switch(type){
                case LuaDatatype::lua_string:
                    streamer << s;
                    break;
                case LuaDatatype::lua_int:
                    streamer << data.i;
                    break;
                case LuaDatatype::lua_float:
                    streamer << data.f;
                    break;
                case LuaDatatype::lua_double:
                    streamer << data.d;
                    break;
                case LuaDatatype::lua_table:
                    streamer << to_string(*t);
                    break;
                case LuaDatatype::lua_function:
                    streamer << func->get_reference();
                    break;
            }
            return streamer.str();
        }

        operator int() const {
            assert(type == LuaDatatype::lua_int || type == LuaDatatype::lua_double || type == LuaDatatype::lua_float);
            if(type == LuaDatatype::lua_float){
                return data.f;
            } else if (type == LuaDatatype::lua_double){
                return data.d;
            } else if (type == LuaDatatype::lua_int){
                return data.i; 
            } else {
                assert(false);
            }
        }

        operator LuaTable() const;

        LuaDatatype get_type(){
            return type;
        }

        void call(const int& numRetValues)
        {
            assert(type == LuaDatatype::lua_function);
            func->call(numRetValues);
        }

        template < typename ...Types > 
        void call(const int& numRetValues, Types ... args)
        {
            assert(type == LuaDatatype::lua_function);
            func->call(numRetValues, args...);
        }

    private:
        LuaTypeUnion data;
        std::string s;
        std::shared_ptr<LuaTable> t;
        std::shared_ptr<LuaFunction> func;
        LuaDatatype type;

};

class LuaTable: public std::map<LuaType, LuaType>
{

    public:
        LuaType& get(const LuaType& key){
            assert(count(key) > 0);
            return std::map<LuaType, LuaType>::operator[](key);
        }

};


class LuaStateWrapper: public std::enable_shared_from_this<LuaStateWrapper>
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
            pushArguments(args...);
            lua_call(m_luaState, m_numArgs, numRetValues); 
            m_numArgs = 0;
        }

        void callFunction(const std::string& module, const std::string& funcName, const int& numRetValues)
        {
            lua_getglobal(m_luaState, module.c_str());
            lua_getfield(m_luaState, -1, funcName.c_str());
            lua_call(m_luaState, 0, numRetValues); 
        }

        template < typename ...Types >
        void callFunction(const int& reference, const int& numRetValues, Types ... args)
        {
           lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, reference); 
           pushArguments(args...);
           lua_call(m_luaState, m_numArgs, numRetValues);
           m_numArgs = 0;
        }

        void callFunction(const int& reference, const int& numRetValues)
        {
            lua_rawgeti(m_luaState, LUA_REGISTRYINDEX, reference);
            lua_call(m_luaState, 0, numRetValues);
        }
        
        LuaType getReturnValue()
        {
            LuaType retVal;
            _getReturnValue(retVal);
            lua_pop(m_luaState, 1);
            return retVal; 
        } 
        void registerFunction(const std::string luaFuncName, lua_CFunction f)
        {
            lua_register( m_luaState, luaFuncName.c_str(), f);
        }

        void removeReferenceFromRegistry(const int& ref){
            std::cout << "Removing reference: " << ref << " from lua registry." << std::endl;;
            luaL_unref(m_luaState, LUA_REGISTRYINDEX, ref);
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

        template< typename ...Types >
        void pushArguments(Types ... args)
        {
            pushArgument(args...);
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

        void _getReturnValue(LuaType& retVal)
        {
            if (lua_isinteger(m_luaState, -1)){
                int i;
                _getReturnValue(i);
                retVal = i;
            }
            else if (lua_isnumber(m_luaState, -1)){
                double d;
                _getReturnValue(d);
                retVal = d;
            }
            else if (lua_isstring(m_luaState, -1)){
                std::string s;
                _getReturnValue(s);
                retVal = s;
            } 
            else if (lua_istable(m_luaState, -1)){
                std::cout << "Reading in a table!" << std::endl;
                std::shared_ptr<LuaTable> t;
                _getReturnValue(t);
                retVal = t;
            } else if (lua_isfunction(m_luaState, -1)){
                std::cout << "Reading in a function!" << std::endl;
                std::shared_ptr<LuaFunction> f;
                _getReturnValue(f);
                retVal = f;
            }
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

        void _getReturnValue(std::string& retVal)
        {
            assert (lua_isstring(m_luaState, -1));
            size_t len;
            const char* cstr = lua_tolstring(m_luaState, -1, &len);
            retVal = std::string(cstr, len);
        }

        void _getReturnValue(std::shared_ptr<LuaTable>& t)
        {
            assert(lua_istable(m_luaState, -1)); 
            t.reset(new LuaTable);
            lua_pushnil(m_luaState);
            while( lua_next(m_luaState, -2))
            {
                lua_pushvalue(m_luaState, -2);
                LuaType key;
                LuaType value;
                _getReturnValue(key);
                lua_pop(m_luaState,1);
                _getReturnValue(value);
                lua_pop(m_luaState,1);
                (*t)[key] = value;
            }
        }

        void _getReturnValue(std::shared_ptr<LuaFunction>& f)
        {
            assert(lua_isfunction(m_luaState, -1));
            lua_pushvalue(m_luaState, -1);
            int ref = luaL_ref(m_luaState, LUA_REGISTRYINDEX);
            f.reset(new LuaFunction(shared_from_this(), ref));
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
        void pushArgument(const double& arg){
            m_numArgs++;
            lua_pushnumber(m_luaState, arg);
        }


        lua_State* m_luaState;
        int m_numArgs;
        bool m_owningInstance;

};

//Free Functions
static std::string to_string(const LuaTable& table){
    std::stringstream sstream;
    auto iter = table.begin();
    sstream << "{";
    bool first_entry = true;
    while(iter != table.end())
    {
        if(!first_entry)
            sstream << ", ";
        first_entry = false;
        sstream <<  "KEY: " << std::string(iter->first);
        sstream << " VAL: " << std::string(iter->second);
        ++iter;
    }
    sstream << "}";
    return sstream.str();
};

//LuaFunction Definitions
template < typename ...Types > 
void LuaFunction::call(const int& numRetValues, Types ... args)
{
    std::shared_ptr<LuaStateWrapper> strongLuaState = m_luaState.lock();
    if(strongLuaState)
        strongLuaState->callFunction(m_ref, numRetValues, args...);
}

void LuaFunction::call(const int& numRetValues)
{
    std::shared_ptr<LuaStateWrapper> strongLuaState = m_luaState.lock();
    if(strongLuaState)
        strongLuaState->callFunction(m_ref, numRetValues);
}

LuaFunction::~LuaFunction()
{
    std::shared_ptr<LuaStateWrapper> strongLuaState = m_luaState.lock();
    if(strongLuaState)
        strongLuaState->removeReferenceFromRegistry(m_ref);
}


//LuaType Definitions
LuaType::LuaType(const LuaType& rtype)
{
    type = rtype.type;
    switch (rtype.type)
    {
        case LuaDatatype::lua_double:
            data.d = rtype.data.d;
            break;
        case LuaDatatype::lua_float:
            data.f = rtype.data.f;
            break;
        case LuaDatatype::lua_int:
            data.i = rtype.data.i;
            break;
        case LuaDatatype::lua_string:
            s = rtype.s;
            break;
        case LuaDatatype::lua_table:
            t = rtype.t;
            break;
        case LuaDatatype::lua_function:
            func = rtype.func;
            break;
    }
}

LuaType& LuaType::operator=(const LuaType& rtype)
{
    type = rtype.type;
    switch (rtype.type)
    {
        case LuaDatatype::lua_double:
            data.d = rtype.data.d;
            break;
        case LuaDatatype::lua_float:
            data.f = rtype.data.f;
            break;
        case LuaDatatype::lua_int:
            data.i = rtype.data.i;
            break;
        case LuaDatatype::lua_string:
            s = rtype.s;
            break;
        case LuaDatatype::lua_table:
            t = rtype.t;
            break;
        case LuaDatatype::lua_function:
            func = rtype.func;
            break;
    }
    return *this;
}

LuaType::LuaType(std::shared_ptr<LuaTable> _t)
{
    t=_t;
    type = LuaDatatype::lua_table;
}

LuaType::~LuaType(){
}

LuaType::operator LuaTable() const 
{
    assert(type == LuaDatatype::lua_table);
    return *t;
}
};

#endif


