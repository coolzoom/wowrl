/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Lua header                */
/*                                        */
/*                                        */

/* Warning : This is a precompiled header.
*            Modifying it will trigger a
*            full rebuild !
*/

extern "C"
{
	#include "lua/lualib.h"
	#include "lua/lauxlib.h"
	#include "lua/lua.h"
}

#include "lua/lunar.h"

#ifndef FROST_LUA_H
#define FROST_LUA_H

#include "frost.h"
#include "frost_lua_utils.h"

namespace Frost
{
    namespace Lua
    {
        /// Creates a whole new Lua state
        /** \param[out] ppLua A pointer to a Lua state pointer
        */
        s_bool  InitLua(lua_State** ppLua);

        /// Open the base libraries
        /** \param pLua The Lua state to use
        */
        void    OpenLibs(lua_State* pLua);

        /// Allow a Lua state to use Frost's global functions
        /** \param pLua The Lua state to use
        */
        void    RegisterGlobalFuncs(lua_State* pLua);

        /// Allow a Lua state to use Frost's GUI classes
        /** \param pLua The Lua state to use
        */
        void    RegisterGUIClasses(lua_State* pLua);

        /// Concatenates a Lua table into a string
        /** \param pLua The Lua state to use
        *   \param sTable The name of the table in Lua
        */
        s_str   ConcTable(lua_State* pLua, s_str sTable);

        /// Copy the content of a table from a Lua state to another
        /** \param pSrc      The source state
        *   \param pDest     The destination state
        *   \param sSrcName  The name of the table in the source state
        *   \param sDestName The name of the table in the destination state
        */
        void    CopyTable(lua_State* pSrc, lua_State* pDest, s_str sSrcName, s_str sDestName);

        /// Executes a Lua file
        /** \param pLua  The Lua state to use
        *   \param sFile The name of the file
        *   \return 'false' if there was an error in the file
        *   \note This function prints the errors in the log file
        */
        s_bool  DoFile(lua_State* pLua, s_str sFile);

        /// Executes a string containing Lua instructions
        /** \param pLua The Lua state to use
        *   \param sStr The string to execute
        *   \return 'false' if there was an error in the string
        *   \note This function prints the errors in the log file
        */
        s_bool  DoString(lua_State* pLua, s_str sStr);

        /// Prints an error string in the log file with the Lua tag
        /** \param pLua   The Lua state to use
        *   \param sError The error string to output
        *   \note This function will print out the actual file and line
        */
        void    PrintError(lua_State* pLua, s_str sError);

        /// Wrapper to read an int from Lua
        /** \param sName         The name of the variable (global scope)
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param iDefaultValue The default value
        *   \return The int value
        */
        s_int   GetGlobalInt(s_str sName, s_bool bCritical = true, s_int iDefaultValue = 0);

        /// Wrapper to read a float from Lua
        /** \param sName         The name of the variable (global scope)
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param fDefaultValue The default value
        *   \return The float value
        */
        s_float GetGlobalFloat(s_str sName, s_bool bCritical = true, s_float fDefaultValue = 0.0f);

        /// Wrapper to read a string from Lua
        /** \param sName         The name of the variable (global scope)
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param sDefaultValue The default value
        *   \return The string value
        */
        s_str   GetGlobalString(s_str sName, s_bool bCritical = true, s_str sDefaultValue = "");

        /// Wrapper to read a bool from Lua
        /** \param sName         The name of the variable (global scope)
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param bDefaultValue The default value
        *   \return The bool value
        */
        s_bool  GetGlobalBool(s_str sName, s_bool bCritical = true, s_bool bDefaultValue = false);

        /// Wrapper to read an int from a Lua table
        /** \param sName         The name of the key associated to the value
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param iDefaultValue The default value
        *   \param bSetValue     If 'true' and the key wasn't found in the table,
        *                        this function will create that key in the Lua table
        *                        and assign it its default value
        *   \param pLua          The Lua state to use, if different from the main one
        *   \return The int value
        *   \note The table that will be used to read the value should be at the top of
        *         the stack just before you call that function.
        */
        s_int   GetFieldInt(s_str sName, s_bool bCritical = true, s_int iDefaultValue = 0, s_bool bSetValue = false, lua_State* pLua = NULL);

        /// Wrapper to read a float from a Lua table
        /** \param sName         The name of the key associated to the value
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param fDefaultValue The default value
        *   \param bSetValue     If 'true' and the key wasn't found in the table,
        *                        this function will create that key in the Lua table
        *                        and assign it its default value
        *   \param pLua          The Lua state to use, if different from the main one
        *   \return The float value
        *   \note The table that will be used to read the value should be at the top of
        *         the stack just before you call that function.
        */
        s_float GetFieldFloat(s_str sName, s_bool bCritical = true, s_float fDefaultValue = 0.0f, s_bool bSetValue = false, lua_State* pLua = NULL);

        /// Wrapper to read a string from a Lua table
        /** \param sName         The name of the key associated to the value
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param sDefaultValue The default value
        *   \param bSetValue     If 'true' and the key wasn't found in the table,
        *                        this function will create that key in the Lua table
        *                        and assign it its default value
        *   \param pLua          The Lua state to use, if different from the main one
        *   \return The string value
        *   \note The table that will be used to read the value should be at the top of
        *         the stack just before you call that function.
        */
        s_str   GetFieldString(s_str sName, s_bool bCritical = true, s_str sDefaultValue = "", s_bool bSetValue = false, lua_State* pLua = NULL);

        /// Wrapper to read a bool from a Lua table
        /** \param sName         The name of the key associated to the value
        *   \param bCritical     If 'true', an error will be printed if
        *                        the variable is not found. Else, it will
        *                        be assigned its default value
        *   \param bDefaultValue The default value
        *   \param bSetValue     If 'true' and the key wasn't found in the table,
        *                        this function will create that key in the Lua table
        *                        and assign it its default value
        *   \param pLua          The Lua state to use, if different from the main one
        *   \return The bool value
        *   \note The table that will be used to read the value should be at the top of
        *         the stack just before you call that function.
        */
        s_bool  GetFieldBool(s_str sName, s_bool bCritical = true, s_bool bDefaultValue = false, s_bool bSetValue = false, lua_State* pLua = NULL);

        /// Wrapper to write an int into a Lua table
        /** \param sName  The name of the key associated to the value
        *   \param iValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetFieldInt(s_str sName, s_int iValue, lua_State* pLua = NULL);

        /// Wrapper to write a float into a Lua table
        /** \param sName  The name of the key associated to the value
        *   \param fValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetFieldFloat(s_str sName, s_float fValue, lua_State* pLua = NULL);

        /// Wrapper to write a string into a Lua table
        /** \param sName  The name of the key associated to the value
        *   \param sValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetFieldString(s_str sName, s_str sValue, lua_State* pLua = NULL);

        /// Wrapper to write a bool into a Lua table
        /** \param sName  The name of the key associated to the value
        *   \param bValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetFieldBool(s_str sName, s_bool bValue, lua_State* pLua = NULL);

        /// Wrapper to write an int into a Lua table
        /** \param iID    The ID of the key associated to the value
        *   \param iValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetIFieldInt(s_int iID, s_int iValue, lua_State* pLua = NULL);

        /// Wrapper to write a float into a Lua table
        /** \param iID    The ID of the key associated to the value
        *   \param fValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetIFieldFloat(s_int iID, s_float fValue, lua_State* pLua = NULL);

        /// Wrapper to write a string into a Lua table
        /** \param iID    The ID of the key associated to the value
        *   \param sValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetIFieldString(s_int iID, s_str sValue, lua_State* pLua = NULL);

        /// Wrapper to write a bool into a Lua table
        /** \param iID    The ID of the key associated to the value
        *   \param bValue The value to set
        *   \param pLua   The Lua state to use, if different from the main one
        *   \note The table that will be used to write the value should be at the top of
        *         the stack just before you call that function.
        */
        void    SetIFieldBool(s_int iID, s_bool bValue, lua_State* pLua = NULL);
    }

    // Global glues
    int l_RandomInt(lua_State* pLua);
    int l_RandomFloat(lua_State* pLua);
    int l_StrReplace(lua_State* pLua);
    int l_StrCapitalStart(lua_State* pLua);
    int l_GetDelta(lua_State* pLua);
    int l_GetLocale(lua_State* pLua);
    int l_GetLocalizedString(lua_State* pLua);
    int l_GetTime(lua_State* pLua);
    int l_GetTimeOfTheDay(lua_State* pLua);
    int l_GetGlobal(lua_State* pLua);
    int l_GetMousePos(lua_State* pLua);
    int l_DoString(lua_State* pLua);

    // Glues, but also used direcly in C++
    int l_ThrowError(lua_State* pLua);
    int l_Log(lua_State* pLua);
    int l_SendString(lua_State* pLua);
    int l_EmptyString(lua_State* pLua);
    int l_ConcTable(lua_State* pLua);

    // Handy function to push nil several times
    inline void lua_pushnil(lua_State* pLua, s_uint uiNbr )
    {
        for (s_uint j; j < uiNbr; j++)
            lua_pushnil(pLua);
    }

    // Overloading
    void lua_pushnumber(lua_State* pLua, s_float fValue);
    void lua_pushnumber(lua_State* pLua, s_double dValue);
    void lua_pushnumber(lua_State* pLua, s_int fValue);
    void lua_pushnumber(lua_State* pLua, s_uint fValue);
    void lua_pushboolean(lua_State* pLua, s_bool bValue);
    void lua_pushstring(lua_State* pLua, s_str sValue);
}

#endif
