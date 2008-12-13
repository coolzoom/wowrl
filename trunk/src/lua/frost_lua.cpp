/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */
/*  ## : This file contains two things :  */
/*    - a lua interface for frequently    */
/*      used function combination.        */
/*    - the lua glues, to call the progs' */
/*      functions inside lua scripts.     */
/*                                        */


#include "frost_lua.h"

#include "gui/frost_gui_uiobject.h"
#include "gui/frost_gui_frame.h"
#include "gui/frost_gui_layeredregion.h"
#include "gui/frost_gui_button.h"
#include "gui/frost_gui_editbox.h"
#include "gui/frost_gui_scrollingmessageframe.h"
#include "gui/frost_gui_slider.h"
#include "gui/frost_gui_statusbar.h"
#include "gui/frost_gui_fontstring.h"
#include "gui/frost_gui_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

void Lua::RegisterGlobalFuncs( lua_State* pLua )
{
    lua_register(pLua, "Log", l_Log);
    lua_register(pLua, "l_ThrowError", l_ThrowError);
    lua_register(pLua, "RandomInt", l_RandomInt);
    lua_register(pLua, "RandomFloat", l_RandomFloat);
    lua_register(pLua, "StrReplace", l_StrReplace);
    lua_register(pLua, "StrCapitalStart", l_StrCapitalStart);
    lua_register(pLua, "GetDelta", l_GetDelta);
    lua_register(pLua, "GetLocale", l_GetLocale);
    lua_register(pLua, "GetLocalizedString", l_GetLocalizedString);
    lua_register(pLua, "GetTime", l_GetTime);
    lua_register(pLua, "GetTimeOfTheDay", l_GetTimeOfTheDay);
    lua_register(pLua, "GetGlobal", l_GetGlobal);
    lua_register(pLua, "GetMousePos", l_GetMousePos);
    lua_register(pLua, "RunScript", l_DoString);
    lua_register(pLua, "SendString", l_SendString);
    lua_register(pLua, "EmptyString", l_EmptyString);
    lua_register(pLua, "ConcTable", l_ConcTable);
}

void Lua::RegisterGUIClasses( lua_State* pLua )
{
    Lunar<GUI::UIObject>::Register(pLua);
    Lunar<GUI::Frame>::Register(pLua);
    Lunar<GUI::StatusBar>::Register(pLua);
    Lunar<GUI::EditBox>::Register(pLua);
    Lunar<GUI::ScrollingMessageFrame>::Register(pLua);
    Lunar<GUI::Button>::Register(pLua);
    Lunar<GUI::Slider>::Register(pLua);
    Lunar<GUI::LayeredRegion>::Register(pLua);
    Lunar<GUI::Texture>::Register(pLua);
    Lunar<GUI::FontString>::Register(pLua);
}

// Modified openlibs function to load my own libs
const luaL_Reg lualibs[] = {
    {"", luaopen_base},
    {LUA_LOADLIBNAME, luaopen_package},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_IOLIBNAME, luaopen_io},
    {LUA_OSLIBNAME, luaopen_os},
    {LUA_STRLIBNAME, luaopen_string},
    //{LUA_MSTRLIBNAME, OpenString}, // Modified string lib
    {LUA_MATHLIBNAME, luaopen_math},
    {LUA_DBLIBNAME, luaopen_debug},
    {NULL, NULL}
};
void Lua::OpenLibs(lua_State* pLua)
{
    const luaL_Reg *lib = lualibs;
    for ( ; lib->func; lib++)
    {
        lua_pushcfunction(pLua, lib->func);
        lua_pushstring(pLua, lib->name);
        lua_call(pLua, 1, 0);
    }
}


s_str Lua::ConcTable(lua_State* pLua, s_str sTable )
{
    /* [#] This function converts a LUA table into a formated string. It is used
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

    luaL_dostring(pLua, s.c_str());

    return Engine::sLuaComString;
}

void Lua::CopyTable( lua_State* pSrcVM, lua_State* pDestVM, s_str sSrcName, s_str sDestName )
{
    Engine::sLuaComString = "";
    s_str sExec = "str = \"\";\nstr = ConcTable(str, \"" + sSrcName + "\");\n";
    luaL_dostring(pSrcVM, sExec.c_str());

    s_str s = Engine::sLuaComString;

    if (s != "")
    {
        s_str sTab = "    ";
        s_str sTable;
        sTable = sDestName + " = {\n";
        s_uint uiTableIndent = 1u;
        s_bool bTableEnded = false;
        s_uint uiLineNbr = 0u;
        while (!bTableEnded)
        {
            if (uiLineNbr > 1000u)
                break;

            if (uiTableIndent.IsNull())
            {
                bTableEnded = true;
            }
            else
            {
                s_uint i = s.Find(" ");
                s_str sKey = s.Extract(0, i);
                s.Erase(0, ++i);
                if (sKey == "'end'")
                {
                    uiTableIndent--;
                    sTab = sTab.Extract(0, sTab.Length() - 4u);
                    if (uiTableIndent.IsNull())
                        sTable += "}\n";
                    else
                        sTable += sTab + "},\n";
                }
                else
                {
                    sKey = "[" + sKey + "]";

                    i = s.Find(" ");
                    s_str sValue = s.Extract(0, i);
                    s.Erase(0, ++i);

                    int iType;
                    if (sValue == "'table'")
                        iType = LUA_TTABLE;
                    else
                    {
                        iType = LUA_TNUMBER;
                        s_uint j = sValue.Find("\"");
                        if (j)
                        {
                            iType = LUA_TSTRING;
                        }
                        else
                        {
                            j = sValue.Find("'");
                            if (j)
                            {
                                iType = LUA_TBOOLEAN;
                                sValue.Trim('\'');
                            }
                        }
                    }

                    if (iType == LUA_TNUMBER)
                    {
                        sTable += sTab + sKey + " = " + sValue + ";\n";
                    }
                    else if (iType == LUA_TNIL)
                    {
                        sTable += sTab + sKey + " = nil;\n";
                    }
                    else if (iType == LUA_TBOOLEAN)
                    {
                        sTable += sTab + sKey + " = " + sValue + ";\n";
                    }
                    else if (iType == LUA_TSTRING)
                    {
                        sTable += sTab + sKey + " = " + sValue + ";\n";
                    }
                    else if (iType == LUA_TTABLE)
                    {
                        sTable += sTab + sKey + " = {\n";
                        sTab += "    ";
                    }
                }
            }
        }

        luaL_dostring(pDestVM, sTable.c_str());
    }
    else
    {
        lua_newtable(pDestVM);
        lua_setglobal(pDestVM, sDestName.c_str());
    }
}

s_bool Lua::DoFile( lua_State* pLua, s_str sFile )
{
    if (File::Exists(sFile))
    {
        int iError = luaL_dofile(pLua, sFile.c_str());
        if (iError) l_ThrowError(pLua);

        return (iError == 0);
    }
    else
    {
        Log("bad...");
        PrintError(pLua, "Can't open \""+sFile+"\".");
        return false;
    }
}

s_bool Lua::InitLua( lua_State** pLua )
{
    *pLua = lua_open();
    if (*pLua == NULL)
    {
        Error("Lua", "Error while initializing Lua.");
        return false;
    }
    OpenLibs(*pLua);
    // Set panic function
    lua_atpanic(*pLua, l_Log);

    lua_newtable(*pLua);
    lua_setglobal(*pLua, "Functions");

    return true;
}

void Lua::PrintError( lua_State* pLua, s_str sError )
{
    lua_Debug d;
    lua_getstack(pLua, 1, &d);
    lua_getinfo(pLua, "Sl" , &d);
    s_str sDebugStr = s_str(d.short_src) + ", line " + s_str(d.currentline) + " : " + sError;
    lua_pushstring(pLua, sDebugStr.c_str());
    l_ThrowError(pLua);
}

void Frost::lua_pushnumber( lua_State* pLua, s_float fValue )
{
    lua_pushnumber(pLua, fValue.Get());
}

void Frost::lua_pushnumber( lua_State* pLua, s_double dValue )
{
    lua_pushnumber(pLua, dValue.Get());
}

void Frost::lua_pushnumber( lua_State* pLua, s_int iValue )
{
    lua_pushnumber(pLua, iValue.Get());
}

void Frost::lua_pushnumber( lua_State* pLua, s_uint uiValue )
{
    lua_pushnumber(pLua, uiValue.Get());
}

void Frost::lua_pushboolean( lua_State* pLua, s_bool bValue )
{
    lua_pushboolean(pLua, bValue.Get());
}

void Frost::lua_pushstring( lua_State* pLua, s_str sValue )
{
    lua_pushstring(pLua, sValue.c_str());
}
