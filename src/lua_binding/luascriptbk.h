#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <iostream>

#include <lua.hpp>

class LuaScript {
public:
    LuaScript(const std::string &filename);
    ~LuaScript();
    void printError(const std::string &varname, const std::string &reason);

    template<typename T>
    T get(const std::string &varname) {
        // todo later. template is like, if a function like that gets
        //called, what to do. thus it is independent of data types.
        if(!L) {
            printError(varname,"Couldn't load script");
            return lua_getDefault<T>();
        }

        T result;
        int level = lua_getToStack(varname);
        if(level != -1){
            result = lua_get<T>(varname);
        }
        else {
            result = lua_getDefault<T>();
        }

        lua_pop(L, level+1);
        return result;
    }

    int lua_getToStack(const std::string &varname){
        //later alligator
        int level = 0;
        std::string var = "";
        for (unsigned int i = 0; i < varname.size(); i++){
            if(varname.at(i) == '.'){
                if(level == 0) {
                    lua_getglobal(L, var.c_str());
                }
                else {
                    lua_getfield(L, -1, var.c_str());
                }

                if(lua_isnil(L, -1)){
                    printError(varname, var+" is not defined");
                    return -1;
                }
                else {
                    var = "";
                    level++;
                }
            }
            else {
                var += varname.at(i);
            }
        }
        if(level == 0){
            lua_getglobal(L, var.c_str());
        }
        else {
            lua_getfield(L, -1, var.c_str());
        }

        if(lua_isnil(L, -1)){
            printError(varname, var + " is not defined");
            return -1;
        }

        return level;
    }

    template<typename T>
    T lua_get(const std::string &varname);

    template<class T>
    T lua_getDefault();

    void clean();

    template<class T>
    std::vector<T> getVector(const std::string &name);

    std::vector<std::string> getTableKeys(const std::string &name);

private:
    lua_State* L;
};

LuaScript::LuaScript(const std::string &filename){
    L = luaL_newstate();
    if(luaL_loadfile(L, filename.c_str()) || lua_pcall(L, 0, 0, 0)) {
        std::cout << "Error: script not loaded (" << filename << ")\n";
        L = 0;
    }
}

LuaScript::~LuaScript(){
    if(L) lua_close(L);
}

void LuaScript::printError(const std::string &varname, const std::string &reason){
    std::cout << "Error: Can't get [" << varname << "]: " << reason << '\n'; 
}

template<>
inline std::string LuaScript::lua_getDefault() {
    return "null";
}

template<>
inline int LuaScript::lua_getDefault(){
    return 0;
}

template<>
inline bool LuaScript::lua_get(const std::string &varname){
    return (bool)lua_toboolean(L, -1);
}

template<>
inline float LuaScript::lua_get(const std::string &varname){
    if(!lua_isnumber(L, -1)){
        printError(varname,  "Not a number");
    }
    return (float)lua_tonumber(L, -1);
}

template<>
inline int LuaScript::lua_get(const std::string &varname) {
    if(!lua_isnumber(L, -1)){
        printError(varname, "Not a number");
    }
    return (int)lua_tonumber(L, -1);
}

template<>
inline std::string LuaScript::lua_get(const std::string &varname){
    std::string s = "null";
    if(lua_isstring(L, -1)){
        s = std::string(lua_tostring(L, -1));
    }
    else {
        printError(varname, "Not a string");
    }
    return s;
}

void LuaScript::clean(){
    int n = lua_gettop(L);
    lua_pop(L, n);
}

template<>
std::vector<int> LuaScript::getVector(const std::string &name){
    std::vector<int> v;
    lua_getglobal(L, name.c_str());
    if(lua_isnil(L, -1)){
        return std::vector<int>();
    }
    
    lua_pushnil(L);
    
    while(lua_next(L, -2)){
        v.push_back((int)lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    clean();
    return v;
}

template<>
std::vector<std::string> LuaScript::getVector(const std::string &name){
    std::vector<std::string> v;
    lua_getglobal(L, name.c_str());
    if(lua_isnil(L, -1)){
        return  std::vector<std::string>();
    }
    
    lua_pushnil(L);
    
    while(lua_next(L, -2)){
        v.push_back(std::string(lua_tostring(L, -1)));
        lua_pop(L, 1);
    }
    clean();
    return v;
}

std::vector<std::string> LuaScript::getTableKeys(const std::string& name) {
    std::string code =
        "function getKeys(name) "
        "s = \"\""
        "for k, v in pairs(_G[name]) do "
        "    s = s..k..\",\" "
        "    end "
        "return s "
        "end"; // function for getting table keys
    luaL_loadstring(L,
        code.c_str()); // execute code
    lua_pcall(L,0,0,0);
    lua_getglobal(L, "getKeys"); // get function
    lua_pushstring(L, name.c_str());
    lua_pcall(L, 1 , 1, 0); // execute function
    std::string test = lua_tostring(L, -1);
    std::vector<std::string> strings;
    std::string temp = "";
    std::cout<<"TEMP:"<<test<<std::endl;
    for(unsigned int i = 0; i < test.size(); i++) {
        if(test.at(i) != ',') {
            temp += test.at(i);
        } else {
            strings.push_back(temp);
            temp= "";
        }
    }
    clean();
    return strings;
}

#endif