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

#endif