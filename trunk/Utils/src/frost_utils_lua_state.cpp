/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Lua::State source           */
/*                                        */
/*                                        */

#include "frost_utils_lua_state.h"
#include "frost_utils_lua_glues.h"
#include "frost_utils_file.h"
#include "frost_utils_eventmanager.h"
#include "frost_utils_event.h"
#include "frost_utils_stdhelper.h"
#include "frost_utils_log.h"
#include "frost_utils_var.h"

//#define DEBUG_STACK

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

const luaL_Reg lualibs[] = {
    {"", luaopen_base},
    {LUA_LOADLIBNAME, luaopen_package},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_IOLIBNAME, luaopen_io},
    {LUA_OSLIBNAME, luaopen_os},
    {LUA_STRLIBNAME, luaopen_string},
    {LUA_MATHLIBNAME, luaopen_math},
    {LUA_DBLIBNAME, luaopen_debug},
    {NULL, NULL}
};

void OpenLibs( lua_State* pLua_ )
{
    for (const luaL_Reg* lib = lualibs; lib->func; ++lib)
    {
        lua_pushcfunction(pLua_, lib->func);
        lua_pushstring(pLua_, lib->name);
        lua_call(pLua_, 1, 0);
    }
}

const s_str State::CLASS_NAME = "Lua::State";

int l_TreatError(lua_State*);

State::State()
{
    pErrorFunction_ = &l_TreatError;

    pLua_ = lua_open();
    if (!pLua_)
    {
        Error("Lua", "Error while initializing Lua.");
        return;
    }

    OpenLibs(pLua_);

    Register("Log", l_Log);
    Register("Error", l_ThrowError);
    Register("Warning", l_Warning);
    Register("RandomInt", l_RandomInt);
    Register("RandomFloat", l_RandomFloat);
    Register("StrReplace", l_StrReplace);
    Register("StrCapitalStart", l_StrCapitalStart);
    Register("GetGlobal", l_GetGlobal);
    Register("RunScript", l_DoString);
    Register("SendString", l_SendString);
    Register("EmptyString", l_EmptyString);
    Register("ConcTable", l_ConcTable);
}

State::~State()
{
    if (pLua_)
        lua_close(pLua_);
}

lua_State* State::GetState()
{
    return pLua_;
}

s_str State::ConcTable( const s_str& sTable )
{
    /* [#] This function converts a Lua table into a formated string. It is used
    *  to save the content of the table in the SavedVariables.
    */
    s_str s = "tbl = \"" + sTable + "\";\n"
              "temp = \"\";\n"
              "for k, v in pairs (" + sTable + ") do\n"
                  "local s, t;\n"
                  "if (type(k) == \"number\") then\ns = k;\nend\n"
                  "if (type(k) == \"string\") then\ns = \"\\\"\"..k..\"\\\"\";\nend\n"
                  "if (type(v) == \"number\") then\nt = v;\nend\n"
                  "if (type(v) == \"string\") then\nt = \"\\\"\"..v..\"\\\"\";\nend\n"
                  "if (type(v) == \"boolean\") then\nif v then\nt = \"'true'\";\nelse\nt = \"'false'\";\nend\nend\n"
                  "if (type(v) == \"table\") then\n"
                      "t = \"'table'\";\nSendString(s..\" \"..t..\" \");\nConcTable(temp, tbl..\"[\"..s..\"]\");\n"
                  "else\n"
                      "SendString(s..\" \"..t..\" \");\n"
                  "end\n"
              "end\n"
              "SendString(\"'end' \");\n";

    luaL_dostring(pLua_, s.c_str());

    return sComString;
}

void State::CopyTable( s_ptr<State> pLua, const s_str& sSrcName, const s_str& sDestName )
{
    #ifdef DEBUG_STACK
    Log("CopyTable : "+GetTop());
    #endif
    s_str sNewName;
    if (sDestName == "") sNewName = sSrcName;
    else                 sNewName = sDestName;

    sComString = "";
    try
    {
        pLua->DoString("str = \"\";\nstr = ConcTable(str, \"" + sSrcName + "\");\n");
    }
    catch (LuaException& e)
    {
        Error(CLASS_NAME, "CopyTable : "+e.GetDescription());
    }

    s_str s = sComString;

    if (s != "")
    {
        s_str sTab = "    ";
        s_str sTable;
        sTable = sNewName + " = {\n";
        s_uint uiTableIndent = 1u;
        s_bool bTableEnded = false;
        s_uint uiLineNbr = 0u;
        while (!bTableEnded)
        {
            if (uiLineNbr > 1000u)
                break;

            if (uiTableIndent == 0)
            {
                bTableEnded = true;
            }
            else
            {
                s_uint i = s.FindPos(" ");
                s_str sKey = s.Extract(0, i);
                ++i;
                s.Erase(0, i);
                if (sKey == "'end'")
                {
                    uiTableIndent--;
                    sTab = sTab.Extract(0, sTab.GetLength() - 4u);
                    if (uiTableIndent == 0)
                        sTable += "}\n";
                    else
                        sTable += sTab + "},\n";
                }
                else
                {
                    sKey = "[" + sKey + "]";

                    i = s.FindPos(" ");
                    s_str sValue = s.Extract(0, i);
                    ++i;
                    s.Erase(0, i);

                    int iType;
                    if (sValue == "'table'")
                        iType = Lua::TYPE_TABLE;
                    else
                    {
                        iType = Lua::TYPE_NUMBER;
                        if (sValue.Find("\""))
                        {
                            iType = Lua::TYPE_STRING;
                        }
                        else
                        {
                            if (sValue.Find("'"))
                            {
                                iType = Lua::TYPE_BOOLEAN;
                                sValue.Trim('\'');
                            }
                        }
                    }

                    if (iType == Lua::TYPE_NUMBER)
                    {
                        sTable += sTab + sKey + " = " + sValue + ";\n";
                    }
                    else if (iType == Lua::TYPE_NIL)
                    {
                        sTable += sTab + sKey + " = nil;\n";
                    }
                    else if (iType == Lua::TYPE_BOOLEAN)
                    {
                        sTable += sTab + sKey + " = " + sValue + ";\n";
                    }
                    else if (iType == Lua::TYPE_STRING)
                    {
                        sTable += sTab + sKey + " = " + sValue + ";\n";
                    }
                    else if (iType == Lua::TYPE_TABLE)
                    {
                        sTable += sTab + sKey + " = {\n";
                        sTab += "    ";
                    }
                }
            }
        }

        pLua->DoString(sTable);
    }
    else
    {
        pLua->NewTable();
        pLua->SetGlobal(sNewName);
    }
    #ifdef DEBUG_STACK
    Log("~CopyTable : "+GetTop());
    #endif
}

int l_TreatError( lua_State* pLua )
{
    if (!lua_isstring(pLua, -1))
        return 0;

    lua_Debug d;

    lua_getstack(pLua, 1, &d);
    lua_getinfo(pLua, "Sl", &d);

    s_str sError = s_str(d.short_src) + ":" + s_int(d.currentline) + ": " + s_str(lua_tostring(pLua, -1));
    lua_pushstring(pLua, sError.c_str());

    return 1;
}

void State::DoFile( const s_str& sFile )
{
    #ifdef DEBUG_STACK
    Log("DoFile : "+GetTop()+" "+sFile);
    #endif
    if (File::Exists(sFile))
    {
        lua_pushcfunction(pLua_, pErrorFunction_);
        s_uint uiFuncPos = GetTop();

        if (luaL_loadfile(pLua_, sFile.c_str()) != 0)
        {
            if (lua_isstring(pLua_, -1))
            {
                s_str sError = lua_tostring(pLua_, -1);
                lua_pop(pLua_, 1);
                lua_remove(pLua_, uiFuncPos.Get());
                throw LuaException(sError);
            }
            else
            {
                lua_remove(pLua_, uiFuncPos.Get());
                throw LuaException(CLASS_NAME, "Cannot load file : \""+sFile+"\"");
            }
        }

        int iError = lua_pcall(pLua_, 0, LUA_MULTRET, -2);
        if (iError != 0)
        {
            if (lua_isstring(pLua_, -1))
            {
                s_str sError = lua_tostring(pLua_, -1);
                lua_pop(pLua_, 1);
                lua_remove(pLua_, uiFuncPos.Get());
                throw LuaException(sError);
            }
            else
            {
                lua_pop(pLua_, 1);
                lua_remove(pLua_, uiFuncPos.Get());
                throw LuaException(CLASS_NAME, "Unhandled error.");
            }
        }

        lua_remove(pLua_, uiFuncPos.Get());
    }
    else
    {
        throw LuaException(CLASS_NAME, "DoFile : can't open \""+sFile+"\".");
    }
    #ifdef DEBUG_STACK
    Log("~DoFile : "+GetTop()+" "+sFile);
    #endif
}

void State::DoString( const s_str& sStr )
{
    #ifdef DEBUG_STACK
    Log("DoString : "+GetTop()+" "+sStr);
    #endif
    lua_pushcfunction(pLua_, pErrorFunction_);
    s_uint uiFuncPos = GetTop();

    if (luaL_loadstring(pLua_, sStr.c_str()) != 0)
    {
        if (lua_isstring(pLua_, -1))
        {
            s_str sError = lua_tostring(pLua_, -1);
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(sError);
        }
        else
        {
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(CLASS_NAME, "Unhandled error.");
        }
    }

    int iError = lua_pcall(pLua_, 0, LUA_MULTRET, -2);
    if (iError != 0)
    {
        if (lua_isstring(pLua_, -1))
        {
            s_str sError = lua_tostring(pLua_, -1);
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(sError);
        }
        else
        {
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(CLASS_NAME, "Unhandled error.");
        }
    }

    lua_remove(pLua_, uiFuncPos.Get());
    #ifdef DEBUG_STACK
    Log("~DoString : "+GetTop()+" "+sStr);
    #endif
}

void State::CallFunction( const s_str& sFunctionName )
{
    #ifdef DEBUG_STACK
    Log("CallFunction : "+GetTop()+" "+sFunctionName);
    #endif
    lua_pushcfunction(pLua_, pErrorFunction_);
    s_uint uiFuncPos = GetTop();

    GetGlobal(sFunctionName);

    if (lua_isnil(pLua_, -1))
    {
        lua_settop(pLua_, (uiFuncPos-1).Get());
        throw LuaException(CLASS_NAME, "\""+sFunctionName+"\" doesn't exist.");
    }

    if (!lua_isfunction(pLua_, -1))
    {
        LuaException mExcept(CLASS_NAME, "\""+sFunctionName+"\" is not a function ("+GetTypeName(GetType())+" : "+GetValue().ToString()+").");
        lua_settop(pLua_, (uiFuncPos-1).Get());
        throw mExcept;
    }

    int iError = lua_pcall(pLua_, 0, LUA_MULTRET, -2);
    if (iError != 0)
    {
        if (lua_isstring(pLua_, -1))
        {
            s_str sError = lua_tostring(pLua_, -1);
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(sError);
        }
        else
        {
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(CLASS_NAME, "Unhandled error.");
        }
    }

    lua_remove(pLua_, uiFuncPos.Get());
    #ifdef DEBUG_STACK
    Log("~CallFunction : "+GetTop()+" "+sFunctionName);
    #endif
}

void State::CallFunction( const s_str& sFunctionName, const s_ctnr<s_var>& lArgumentStack )
{
    #ifdef DEBUG_STACK
    Log("CallFunction : "+GetTop()+" "+sFunctionName+"(args)");
    #endif
    lua_pushcfunction(pLua_, pErrorFunction_);
    s_uint uiFuncPos = GetTop();

    GetGlobal(sFunctionName);

    if (lua_isnil(pLua_, -1))
    {
        lua_settop(pLua_, (uiFuncPos-1).Get());
        throw LuaException(CLASS_NAME, "\""+sFunctionName+"\" doesn't exist.");
    }

    if (lua_isfunction(pLua_, -1))
    {
        lua_settop(pLua_, (uiFuncPos-1).Get());
        throw LuaException(CLASS_NAME, "\""+sFunctionName+"\" is not a function ("+GetTypeName(GetType())+" : "+GetValue().ToString()+")");
    }

    s_ctnr<s_var>::const_iterator iter;
    foreach (iter, lArgumentStack)
        PushVar(*iter);

    int iError = lua_pcall(pLua_, lArgumentStack.GetSize().Get(), LUA_MULTRET, -2-lArgumentStack.GetSize().Get());
    if (iError != 0)
    {
        if (lua_isstring(pLua_, -1))
        {
            s_str sError = lua_tostring(pLua_, -1);
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(sError);
        }
        else
        {
            lua_settop(pLua_, (uiFuncPos-1).Get());
            throw LuaException(CLASS_NAME, "Unhandled error.");
        }
    }

    lua_remove(pLua_, uiFuncPos.Get());
    #ifdef DEBUG_STACK
    Log("~CallFunction : "+GetTop()+" "+sFunctionName+"(args)");
    #endif
}

void State::Register( const s_str& sFunctionName, lua_CFunction mFunction )
{
    lua_register(pLua_, sFunctionName.c_str(), mFunction);
}

s_str State::FormatError( const s_str& sError )
{
    PushString(sError);
    (*pErrorFunction_)(pLua_);
    s_str sResult = GetString();
    Pop(1);

    return sResult;
}

void State::PrintError( const s_str& sError )
{
    s_str sDebugStr = FormatError(sError);

    Log(sDebugStr);

    Event e("LUA_ERROR");
    e.Add(s_var(sDebugStr));
    e.Add(s_var(sError));

    EventManager::GetSingleton()->FireEvent(e);
}

void State::SetErrorFunction( lua_CFunction pFunc )
{
    if (pFunc != 0)
        pErrorFunction_ = pFunc;
    else
        pErrorFunction_ = &l_TreatError;
}

s_bool State::IsSerializable( const s_int& iIndex )
{
    if (lua_getmetatable(pLua_, iIndex.Get()))
    {
        Pop();
        GetField("Serialize", iIndex.Get());
        if (GetType() == TYPE_FUNCTION)
        {
            Pop();
            return true;
        }
        Pop();
    }

    Type mType = GetType(iIndex);
    return (mType == TYPE_BOOLEAN) || (mType == TYPE_NUMBER) ||
           (mType == TYPE_STRING)  || (mType == TYPE_TABLE);
}

s_str State::SerializeGlobal( const s_str& sName )
{
    GetGlobal(sName);

    s_str sContent;
    if (IsSerializable())
        sContent = sName+" = "+Serialize()+";";

    Pop();

    return sContent;
}

s_str State::Serialize( const s_str& sTab, const s_int& iIndex )
{
    #ifdef DEBUG_STACK
    Log("Serialize : "+GetTop()+" "+sTab);
    #endif
    s_int iAbsoluteIndex = iIndex >= 0 ? iIndex : s_int(GetTop()+1) + iIndex;
    s_str sResult;

    if (lua_getmetatable(pLua_, iAbsoluteIndex.Get()))
    {
        Pop();
        GetField("Serialize", iAbsoluteIndex);
        if (GetType() == TYPE_FUNCTION)
        {
            PushValue(iAbsoluteIndex);
            PushString(sTab);
            int iError = lua_pcall(pLua_, 2, 1, 0);
            if (iError == 0)
            {
                sResult << GetString();
                Pop();
                return sResult;
            }
        }
        else
            Pop();
    }

    Type mType = GetType(iIndex);
    switch (mType)
    {
        case TYPE_BOOLEAN :
            sResult << GetBool(iIndex);
            break;

        case TYPE_NUMBER :
            sResult << GetNumber(iIndex);
            break;

        case TYPE_STRING :
            sResult << "\""+GetString(iIndex)+"\"";
            break;

        case TYPE_TABLE :
        {
            sResult << "{";

            s_str sContent = "\n";
            PushValue(iIndex);
            PushNil();
            while (lua_next(pLua_, -2) != 0)
            {
                if (IsSerializable())
                {
                    sContent << sTab << "    [" << Serialize(sTab + "    ", -2) << "] = "
                            << Serialize(sTab + "    ", -1) << ";\n";
                }
                Pop();
            }
            Pop();

            if (sContent != "\n")
                sResult << sContent << sTab;

            sResult << "}";
            break;
        }

        default : break;
    }

    #ifdef DEBUG_STACK
    Log("~Serialize : "+GetTop()+" "+sTab);
    #endif

    return sResult;
}

void State::PushNumber( const s_int& iValue )
{
    if (iValue.IsValid())
        lua_pushnumber(pLua_, iValue.Get());
    else
        lua_pushnil(pLua_);
}

void State::PushNumber( const s_uint& uiValue)
{
    if (uiValue.IsValid())
        lua_pushnumber(pLua_, uiValue.Get());
    else
        lua_pushnil(pLua_);
}

void State::PushNumber( const s_float& fValue )
{
    if (fValue.IsValid())
        lua_pushnumber(pLua_, fValue.Get());
    else
        lua_pushnil(pLua_);
}

void State::PushNumber( const s_double& dValue )
{
    if (dValue.IsValid())
        lua_pushnumber(pLua_, dValue.Get());
    else
        lua_pushnil(pLua_);
}

void State::PushBool( const s_bool& bValue )
{
    lua_pushboolean(pLua_, bValue.Get());
}

void State::PushString( const s_str& sValue )
{
    lua_pushstring(pLua_, sValue.c_str());
}

void State::PushNil( const s_uint& uiNumber )
{
    for (s_uint ui; ui < uiNumber; ++ui)
        lua_pushnil(pLua_);
}

void State::PushVar( const s_var& vValue )
{
    const s_type& mType = vValue.GetType();
    if (mType == s_var::VALUE_INT) PushNumber(vValue.Get<s_int>());
    else if (mType == s_var::VALUE_UINT) PushNumber(vValue.Get<s_uint>());
    else if (mType == s_var::VALUE_FLOAT) PushNumber(vValue.Get<s_float>());
    else if (mType == s_var::VALUE_DOUBLE) PushNumber(vValue.Get<s_double>());
    else if (mType == s_var::VALUE_STRING) PushString(vValue.Get<s_str>());
    else if (mType == s_var::VALUE_BOOL) PushBool(vValue.Get<s_bool>());
    else PushNil();
}

void State::PushValue( const s_int& iIndex )
{
    lua_pushvalue(pLua_, iIndex.Get());
}

void State::PushGlobal( const s_str& sName )
{
    GetGlobal(sName);
}

void State::SetGlobal( const s_str& sName )
{
    #ifdef DEBUG_STACK
    Log("SetGlobal : "+GetTop()+" "+sName);
    #endif
    s_ctnr<s_str> lDecomposedName;
    s_str sVarName;
    s_ctnr<s_str> lWords = sName.Cut(":");
    s_ctnr<s_str>::iterator iter1;
    foreach (iter1, lWords)
    {
        s_ctnr<s_str> lSubWords = iter1->Cut(".");
        s_ctnr<s_str>::iterator iter2;
        foreach (iter2, lSubWords)
        {
            lDecomposedName.PushBack(*iter2);
        }
    }

    // Start at 1 to pop the value the user has put on the stack.
    s_uint uiCounter = 1;

    sVarName = lDecomposedName.Back();
    lDecomposedName.PopBack();

    if (lDecomposedName.GetSize() >= 1)
    {
        lua_getglobal(pLua_, lDecomposedName.Begin()->c_str());
        lDecomposedName.PopFront();
        ++uiCounter;

        if (!lua_isnil(pLua_, -1))
        {
            s_ctnr<s_str>::iterator iterWords;
            foreach (iterWords, lDecomposedName)
            {
                lua_getfield(pLua_, -1, iterWords->c_str());
                ++uiCounter;
                if (lua_isnil(pLua_, -1))
                {
                    Pop(uiCounter);
                    return;
                }
            }
        }

        lua_pushvalue(pLua_, (-uiCounter).Get());
        lua_setfield(pLua_, -2, sVarName.c_str());
        Pop(uiCounter);
    }
    else
    {
        lua_setglobal(pLua_, sName.c_str());
    }
    #ifdef DEBUG_STACK
    Log("~SetGlobal : "+GetTop()+" "+sName);
    #endif
}

void State::NewTable()
{
    #ifdef DEBUG_STACK
    Log("NewTable : "+GetTop());
    #endif
    lua_newtable(pLua_);
    #ifdef DEBUG_STACK
    Log("~NewTable : "+GetTop());
    #endif
}

s_bool State::Next( const s_int& iIndex )
{
    #ifdef DEBUG_STACK
    Log("Next : "+GetTop());
    #endif
    int res = lua_next(pLua_, iIndex.Get());
    #ifdef DEBUG_STACK
    Log("~Next : "+GetTop());
    #endif
    return res != 0;
}

void State::Pop( const s_uint& uiNumber )
{
    #ifdef DEBUG_STACK
    Log("Pop : "+GetTop()+" "+uiNumber);
    #endif
    lua_pop(pLua_, static_cast<int>(uiNumber.Get()));
    #ifdef DEBUG_STACK
    Log("~Pop : "+GetTop()+" "+uiNumber);
    #endif
}

s_float State::GetNumber( const s_int& iIndex )
{
    return static_cast<float>(lua_tonumber(pLua_, iIndex.Get()));
}

s_bool State::GetBool( const s_int& iIndex )
{
    return lua_toboolean(pLua_, iIndex.Get()) != 0;
}

s_str State::GetString( const s_int& iIndex )
{
    return lua_tostring(pLua_, iIndex.Get());
}

s_var State::GetValue( const s_int& iIndex )
{
    int type = lua_type(pLua_, iIndex.Get());
    switch (type)
    {
        case LUA_TBOOLEAN : return GetBool(iIndex);
        case LUA_TNUMBER : return GetNumber(iIndex);
        case LUA_TSTRING : return GetString(iIndex);
        default : return s_var();
    }
}

s_uint State::GetTop()
{
    return lua_gettop(pLua_);
}

Type State::GetType( const s_int& iIndex )
{
    int type = lua_type(pLua_, iIndex.Get());
    switch (type)
    {
        case LUA_TBOOLEAN : return Lua::TYPE_BOOLEAN;
        case LUA_TFUNCTION : return Lua::TYPE_FUNCTION;
        case LUA_TLIGHTUSERDATA : return Lua::TYPE_LIGHTUSERDATA;
        case LUA_TNIL : return Lua::TYPE_NIL;
        case LUA_TNONE : return Lua::TYPE_NONE;
        case LUA_TNUMBER : return Lua::TYPE_NUMBER;
        case LUA_TSTRING : return Lua::TYPE_STRING;
        case LUA_TTABLE : return Lua::TYPE_TABLE;
        case LUA_TTHREAD : return Lua::TYPE_THREAD;
        case LUA_TUSERDATA : return Lua::TYPE_USERDATA;
        default : return Lua::TYPE_NONE;
    }
}

s_str State::GetTypeName( Type mType )
{
    switch (mType)
    {
        case Lua::TYPE_BOOLEAN : return lua_typename(pLua_, LUA_TBOOLEAN);
        case Lua::TYPE_FUNCTION : return lua_typename(pLua_, LUA_TFUNCTION);
        case Lua::TYPE_LIGHTUSERDATA : return lua_typename(pLua_, LUA_TLIGHTUSERDATA);
        case Lua::TYPE_NIL : return lua_typename(pLua_, LUA_TNIL);
        case Lua::TYPE_NONE : return lua_typename(pLua_, LUA_TNONE);
        case Lua::TYPE_NUMBER : return lua_typename(pLua_, LUA_TNUMBER);
        case Lua::TYPE_STRING : return lua_typename(pLua_, LUA_TSTRING);
        case Lua::TYPE_TABLE : return lua_typename(pLua_, LUA_TTABLE);
        case Lua::TYPE_THREAD : return lua_typename(pLua_, LUA_TTHREAD);
        case Lua::TYPE_USERDATA : return lua_typename(pLua_, LUA_TUSERDATA);
        default : return "";
    }
}

void State::GetGlobal( const s_str& sName )
{
    #ifdef DEBUG_STACK
    Log("GetGlobal : "+GetTop()+" "+sName);
    #endif
    s_ctnr<s_str> lDecomposedName;
    s_ctnr<s_str> lWords = sName.Cut(":");
    s_ctnr<s_str>::iterator iter1;
    foreach (iter1, lWords)
    {
        s_ctnr<s_str> lSubWords = iter1->Cut(".");
        s_ctnr<s_str>::iterator iter2;
        foreach (iter2, lSubWords)
        {
            lDecomposedName.PushBack(*iter2);
        }
    }

    lua_getglobal(pLua_, lDecomposedName.Front().c_str());

    if (lDecomposedName.GetSize() > 1)
    {
        if (lua_isnil(pLua_, -1))
            return;

        lDecomposedName.Erase(lDecomposedName.Begin());

        s_ctnr<s_str>::iterator iterWords;
        foreach (iterWords, lDecomposedName)
        {
            lua_getfield(pLua_, -1, iterWords->c_str());
            lua_remove(pLua_, -2);

            if (lua_isnil(pLua_, -1))
                return;

            if (!lua_istable(pLua_, -1) && iterWords+1 != lDecomposedName.End())
            {
                lua_pop(pLua_, 1);
                lua_pushnil(pLua_);
                return;
            }
        }
    }

    #ifdef DEBUG_STACK
    Log("~GetGlobal : "+GetTop()+" "+sName);
    #endif
}

s_int State::GetGlobalInt( const s_str& sName, const s_bool& bCritical, const s_int& iDefaultValue )
{
    #ifdef DEBUG_STACK
    Log("GetGlobalInt : "+GetTop()+" "+sName);
    #endif
    s_int i;
    GetGlobal(sName);
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
        {
            PrintError("Missing " + sName + " attribute");
            i = iDefaultValue;
        }
        else
            i = iDefaultValue;
    }
    else if (!lua_isnumber(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        PrintError("\"" + sName + "\" is expected to be a number");
        i = iDefaultValue;
    }
    else
    {
        i = (int)lua_tonumber(pLua_, -1);
        lua_pop(pLua_, 1);
    }
    #ifdef DEBUG_STACK
    Log("~GetGlobalInt : "+GetTop()+" "+sName);
    #endif
    return i;
}

s_float State::GetGlobalFloat( const s_str& sName, const s_bool& bCritical, const s_float& fDefaultValue )
{
    #ifdef DEBUG_STACK
    Log("GetGlobalFloat : "+GetTop()+" "+sName);
    #endif
    s_float f;
    GetGlobal(sName);
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
        {
            PrintError("Missing " + sName + " attribute");
            f = fDefaultValue;
        }
        else
            f = fDefaultValue;
    }
    else if (!lua_isnumber(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        PrintError("\"" + sName + "\" is expected to be a number");
        f = fDefaultValue;
    }
    else
    {
        f = static_cast<float>(lua_tonumber(pLua_, -1));
        lua_pop(pLua_, 1);
    }
    #ifdef DEBUG_STACK
    Log("~GetGlobalFloat : "+GetTop()+" "+sName);
    #endif
    return f;
}

s_str State::GetGlobalString( const s_str& sName, const s_bool& bCritical, const s_str& sDefaultValue )
{
    #ifdef DEBUG_STACK
    Log("GetGlobalString : "+GetTop()+" "+sName);
    #endif
    s_str s;
    GetGlobal(sName);
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
        {
            PrintError("Missing " + sName + " attribute");
            s = sDefaultValue;
        }
        else
            s = sDefaultValue;
    }
    else if (!lua_isstring(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        PrintError("\"" + sName + "\" is expected to be a string");
        s = sDefaultValue;
    }
    else
    {
        s = lua_tostring(pLua_, -1);
        lua_pop(pLua_, 1);
    }
    #ifdef DEBUG_STACK
    Log("~GetGlobalString : "+GetTop()+" "+sName);
    #endif
    return s;
}

s_bool State::GetGlobalBool( const s_str& sName, const s_bool& bCritical, const s_bool& bDefaultValue )
{
    #ifdef DEBUG_STACK
    Log("GetGlobalBool : "+GetTop()+" "+sName);
    #endif
    s_bool b;
    GetGlobal(sName);
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
        {
            PrintError("Missing " + sName + " attribute");
            b = bDefaultValue;
        }
        else
            b = bDefaultValue;
    }
    else if (!lua_isboolean(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        PrintError("\"" + sName + "\" is expected to be a bool");
        b = bDefaultValue;
    }
    else
    {
        b = (lua_toboolean(pLua_, -1) != 0);
        lua_pop(pLua_, 1);
    }
    #ifdef DEBUG_STACK
    Log("~GetGlobalBool : "+GetTop()+" "+sName);
    #endif
    return b;
}

void State::GetField( const s_str& sName, const s_int& iIndex )
{
    #ifdef DEBUG_STACK
    Log("GetField : "+GetTop()+" "+sName);
    #endif
    lua_getfield(pLua_, iIndex.Get(), sName.c_str());
    #ifdef DEBUG_STACK
    Log("~GetField : "+GetTop()+" "+sName);
    #endif
}

void State::GetField( const s_int& iID, const s_int& iIndex )
{
    #ifdef DEBUG_STACK
    Log("GetField : "+GetTop()+" "+iID);
    #endif
    lua_pushnumber(pLua_, iID.Get());
    if (iIndex >= 0)
        lua_gettable(pLua_, iIndex.Get());
    else
        lua_gettable(pLua_, iIndex.Get() - 1);
    #ifdef DEBUG_STACK
    Log("~GetField : "+GetTop()+" "+iID);
    #endif
}

s_int State::GetFieldInt( const s_str& sName, const s_bool& bCritical, const s_int& iDefaultValue, const s_bool& bSetValue )
{
    #ifdef DEBUG_STACK
    Log("GetFieldInt : "+GetTop()+" "+sName);
    #endif
    s_int i;
    lua_getfield(pLua_, -1, sName.c_str());
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
            PrintError("Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldInt(sName, iDefaultValue);
            i = iDefaultValue;
        }
        else
            i = iDefaultValue;
    }
    else if (!lua_isnumber(pLua_, -1))
    {
        PrintError("Field is expected to be a number");
        lua_pop(pLua_, 1);
        i = iDefaultValue;
    }
    else
    {
        i = (int)lua_tonumber(pLua_, -1);
        lua_pop(pLua_, 1);
    }
    #ifdef DEBUG_STACK
    Log("~GetFieldInt : "+GetTop()+" "+sName);
    #endif
    return i;
}

s_float State::GetFieldFloat( const s_str& sName, const s_bool& bCritical, const s_float& fDefaultValue, const s_bool& bSetValue )
{
    #ifdef DEBUG_STACK
    Log("GetFieldFloat : "+GetTop()+" "+sName);
    #endif
    s_float f;
    lua_getfield(pLua_, -1, sName.c_str());
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
            PrintError("Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldFloat(sName, fDefaultValue);
            f = fDefaultValue;
        }
        else
            f = fDefaultValue;
    }
    else if (!lua_isnumber(pLua_, -1))
    {
        PrintError("Field is expected to be a number");
        lua_pop(pLua_, 1);
        f = fDefaultValue;
    }
    else
    {
        f = static_cast<float>(lua_tonumber(pLua_, -1));
        lua_pop(pLua_, 1);

    }
    #ifdef DEBUG_STACK
    Log("~GetFieldFloat : "+GetTop()+" "+sName);
    #endif
    return f;
}

s_str State::GetFieldString( const s_str& sName, const s_bool& bCritical, const s_str& sDefaultValue, const s_bool& bSetValue )
{
    #ifdef DEBUG_STACK
    Log("GetFieldString : "+GetTop()+" "+sName);
    #endif
    s_str s;
    lua_getfield(pLua_, -1, sName.c_str());
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
            PrintError("Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldString(sName, sDefaultValue);
            s = sDefaultValue;
        }
        else
            s = sDefaultValue;
    }
    else if (!lua_isstring(pLua_, -1))
    {
        PrintError("Field is expected to be a string");
        lua_pop(pLua_, 1);
        s = sDefaultValue;
    }
    else
    {
        s = lua_tostring(pLua_, -1);
        lua_pop(pLua_, 1);
    }
    #ifdef DEBUG_STACK
    Log("~GetFieldString : "+GetTop()+" "+sName);
    #endif
    return s;
}

s_bool State::GetFieldBool( const s_str& sName, const s_bool& bCritical, const s_bool& bDefaultValue, const s_bool& bSetValue )
{
    #ifdef DEBUG_STACK
    Log("GetFieldBool : "+GetTop()+" "+sName);
    #endif
    s_bool b;
    lua_getfield(pLua_, -1, sName.c_str());
    if (lua_isnil(pLua_, -1))
    {
        lua_pop(pLua_, 1);
        if (bCritical)
            PrintError("Missing " + sName + " attribute");
        else if (bSetValue)
        {
            SetFieldBool(sName, bDefaultValue);
            b = bDefaultValue;
        }
        else
            b = bDefaultValue;
    }
    else if (!lua_isboolean(pLua_, -1))
    {
        PrintError("Field is expected to be a bool");
        lua_pop(pLua_, 1);
        b = bDefaultValue;
    }
    else
    {
        b = (lua_toboolean(pLua_, -1) != 0);
        lua_pop(pLua_, 1);
    }
    #ifdef DEBUG_STACK
    Log("~GetFieldBool : "+GetTop()+" "+sName);
    #endif
    return b;
}

void State::SetField( const s_str& sName )
{
    #ifdef DEBUG_STACK
    Log("SetField : "+GetTop()+" "+sName);
    #endif
    lua_pushstring(pLua_, sName.c_str());
    lua_pushvalue(pLua_, -2);
    lua_settable(pLua_, -4);
    lua_pop(pLua_, 1);
    #ifdef DEBUG_STACK
    Log("~SetField : "+GetTop()+" "+sName);
    #endif
}

void State::SetField( const s_int& iID )
{
    #ifdef DEBUG_STACK
    Log("SetField : "+GetTop()+" "+iID);
    #endif
    lua_pushnumber(pLua_, iID.Get());
    lua_pushvalue(pLua_, -2);
    lua_settable(pLua_, -4);
    lua_pop(pLua_, 1);
    #ifdef DEBUG_STACK
    Log("~SetField : "+GetTop()+" "+iID);
    #endif
}

void State::SetFieldInt( const s_str& sName, const s_int& iValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldInt : "+GetTop()+" "+sName);
    #endif
    lua_pushstring(pLua_, sName.c_str());
    lua_pushnumber(pLua_, iValue.Get());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldInt : "+GetTop()+" "+sName);
    #endif
}

void State::SetFieldFloat( const s_str& sName, const s_float& fValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldFloat : "+GetTop()+" "+sName);
    #endif
    lua_pushstring(pLua_, sName.c_str());
    lua_pushnumber(pLua_, fValue.Get());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldFloat : "+GetTop()+" "+sName);
    #endif
}

void State::SetFieldString( const s_str& sName, const s_str& sValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldString : "+GetTop()+" "+sName);
    #endif
    lua_pushstring(pLua_, sName.c_str());
    lua_pushstring(pLua_, sValue.c_str());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldString : "+GetTop()+" "+sName);
    #endif
}

void State::SetFieldBool( const s_str& sName, const s_bool& bValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldBool : "+GetTop()+" "+sName);
    #endif
    lua_pushstring(pLua_, sName.c_str());
    lua_pushboolean(pLua_, bValue.Get());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldBool : "+GetTop()+" "+sName);
    #endif
}

void State::SetFieldInt( const s_int& iID, const s_int& iValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldInt : "+GetTop()+" "+iID);
    #endif
    lua_pushnumber(pLua_, iID.Get());
    lua_pushnumber(pLua_, iValue.Get());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldInt : "+GetTop()+" "+iID);
    #endif
}

void State::SetFieldFloat( const s_int& iID, const s_float& fValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldFloat : "+GetTop()+" "+iID);
    #endif
    lua_pushnumber(pLua_, iID.Get());
    lua_pushnumber(pLua_, fValue.Get());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldFloat : "+GetTop()+" "+iID);
    #endif
}

void State::SetFieldString( const s_int& iID, const s_str& sValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldString : "+GetTop()+" "+iID);
    #endif
    lua_pushnumber(pLua_, iID.Get());
    lua_pushstring(pLua_, sValue.c_str());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldString : "+GetTop()+" "+iID);
    #endif
}

void State::SetFieldBool( const s_int& iID, const s_bool& bValue )
{
    #ifdef DEBUG_STACK
    Log("SetFieldBool : "+GetTop()+" "+iID);
    #endif
    lua_pushnumber(pLua_, iID.Get());
    lua_pushboolean(pLua_, bValue.Get());
    lua_settable(pLua_, -3);
    #ifdef DEBUG_STACK
    Log("~SetFieldBool : "+GetTop()+" "+iID);
    #endif
}

void State::SetTop( const s_uint& uiSize )
{
    #ifdef DEBUG_STACK
    Log("SetTop : "+uiSize);
    #endif
    lua_settop(pLua_, uiSize.Get());
}
