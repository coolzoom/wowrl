/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Lua::State header            */
/*                                        */
/*                                        */

#include <lualib.h>
#include <lauxlib.h>
#include <lua.h>

#include <lunar.h>

#ifndef FROST_UTILS_LUASTATE_H
#define FROST_UTILS_LUASTATE_H

#include "frost_utils_types.h"
#include "frost_utils_exception.h"

namespace Frost
{
    class s_var;

    class LuaException : public Exception
    {
    public :

        LuaException(const s_str& sMessage) : Exception(sMessage)
        {
        }

        LuaException(const s_str& sClassName, const s_str& sMessage) : Exception(sClassName, sMessage)
        {
        }
    };

    namespace Lua
    {
        enum Type
        {
            TYPE_NONE,
            TYPE_NIL,
            TYPE_NUMBER,
            TYPE_BOOLEAN,
            TYPE_STRING,
            TYPE_FUNCTION,
            TYPE_TABLE,
            TYPE_THREAD,
            TYPE_LIGHTUSERDATA,
            TYPE_USERDATA
        };

        /// Lua wrapper
        /** Wraps the Lua api into a single class.
        */
        class State
        {
        public :

            /// Constructor
            /** \note Opens a new Lua state and registers all base functions.
            */
            State();

            /// Destructor
            /** \note Definately closes the associated Lua state (all data will be lost).
            */
            ~State();

            /// Return the associated Lua state.
            /** \return The associated Lua state
            */
            lua_State* GetState();

            /// Concatenates a Lua table into a string.
            /** \param sTable The name of the table in Lua
            */
            s_str   ConcTable(const s_str& sTable);

            /// Copy the content of a table from a Lua state to another.
            /** \param pLua      The other Lua state into wich the table will be copied
            *   \param sSrcName  The name of the table in the source state
            *   \param sDestName The name of the table in the destination state
            *   \note If sDestName is ommited, the table will have the same name in both Lua states.
            */
            void    CopyTable(s_ptr<State> pLua , const s_str& sSrcName, const s_str& sDestName = "");

            /// Executes a Lua file.
            /** \param sFile The name of the file
            *   \note This function wil throw an Exception if any error occurs.
            *         Don't forget to catch them.
            */
            void    DoFile(const s_str& sFile);

            /// Executes a string containing Lua instructions.
            /** \param sStr The string to execute
            *   \note This function wil throw an Exception if any error occurs.
            *         Don't forget to catch them.
            */
            void    DoString(const s_str& sStr);

            /// Executes a Lua function.
            /** \param sFunctionName The name of the function to execute
            *   \note This function wil throw an Exception if any error occurs.
            *         Don't forget to catch them.
            */
            void    CallFunction(const s_str& sFunctionName);

            /// Executes a Lua function with arguments.
            /** \param sFunctionName  The name of the function to execute
            *   \param lArgumentStack The agument stack (order matters)
            *   \note This function wil throw an Exception if any error occurs.
            *         Don't forget to catch them.
            */
            void    CallFunction(const s_str& sFunctionName, const s_ctnr<s_var>& lArgumentStack);

            /// Binds a C++ function to a Lua function.
            /** \param sFunctionName The name of the Lua function
            *   \param mFunction     The C++ function to bind
            */
            void    Register(const s_str& sFunctionName, lua_CFunction mFunction);

            /// Registers a Lunar class to be used on this state.
            template<class T>
            void    Register()
            {
                Lunar<T>::Register(pLua_);
            }

            /// Prints an error string in the log file with the Lua tag.
            /** \param sError The error string to output
            *   \note This function will print out the current file and line
            */
            void    PrintError(const s_str& sError);

            /// Formats a raw error string.
            /** \param sError The raw error string
            *   \note Calls the custom error function if provided,
            *         else formats as :<br>
            *         [source]:line: error
            */
            s_str   FormatError(const s_str& sError);

            /// Sets a custom error formatting function.
            /** \param pFunc The error function
            *   \note The string that is passed to this error function only
            *         contains the raw error message from Lua : no indication
            *         on the location of the error.<br>
            *         The default implementation of this function returns :<br>
            *         [source]:line: error<br>
            *         See FormatError().
            */
            void    SetErrorFunction(lua_CFunction pFunc);

            /// Checks if a variable is serializable.
            /** \param iIndex The index on the stack of the variable
            *   \return 'true' for strings, numbers, booleans and tables.
            */
            s_bool  IsSerializable(const s_int& iIndex = -1);

            /// Writes the content of a global variable in a string.
            /** \param sName The name of the global variable
            *   \return The content of the variable
            */
            s_str   SerializeGlobal(const s_str& sName);

            /// Writes the content of a variable in a string.
            /** \param sTab   Number of space to put in front of each line
            *   \param iIndex The index on the stack of the variable
            *   \note Can only serialize strings, numbers, booleans and tables.
            *   \return The content of the variable
            */
            s_str   Serialize(const s_str& sTab = "", const s_int& iIndex = -1);

            /// Puts a number on the stack.
            /** \param iValue The value to push on the stack (converted to float)
            */
            void    PushNumber(const s_int& iValue);

            /// Puts a number on the stack.
            /** \param uiValue The value to push on the stack (converted to float)
            */
            void    PushNumber(const s_uint& uiValue);

            /// Puts a number on the stack.
            /** \param fValue The value to push on the stack
            */
            void    PushNumber(const s_float& fValue);

            /// Puts a number on the stack.
            /** \param dValue The value to push on the stack (converted to float)
            */
            void    PushNumber(const s_double& dValue);

            /// Puts a boolean on the stack.
            /** \param bValue The value to push on the stack
            */
            void    PushBool(const s_bool& bValue);

            /// Puts a string on the stack.
            /** \param sValue The value to push on the stack
            */
            void    PushString(const s_str& sValue);

            /// Puts a value on the stack.
            /** \param vValue The value to push on the stack
            */
            void    PushVar(const s_var& vValue);

            /// Pushes a copy of the value at the given index on the stack.
            /** \param iIndex The index of the value to push
            */
            void    PushValue(const s_int& iIndex);

            /// Puts "nil" (null) on the stack.
            /** \param uiNumber The number of "nil" to push
            */
            void    PushNil(const s_uint& uiNumber = 1);

            /// Puts the value of a global Lua variable on the stack.
            /** \param sName The name of this variable
            */
            void    PushGlobal(const s_str& sName);

            /// Pushes a new Lunar object on the stack.
            /** \return A pointer to the Lunar object
            */
            template<class T>
            s_ptr<T> PushNew()
            {
                return Lunar<T>::PushNew(pLua_);
            }

            /// Sets the value of a global Lua variable.
            /** \param sName The name of this variable
            *   \note The value taken is taken at the top of the stack,
            *         and popped.
            */
            void    SetGlobal(const s_str& sName);

            /// Creates a new empty table and pushes it on the stack.
            void    NewTable();

            /// Iterates over the table at the given index.
            /** \param iIndex The index of the table to iterate on
            *   \note Typical loop (with table at index i) :<br>
            *         for (pLua->PushNil(); pLua->Next(i); pLua->Pop())
            */
            s_bool  Next(const s_int& iIndex = -2);

            /// Removes the value at the top of the stack.
            /** \param uiNumber The number of value to remove
            */
            void    Pop(const s_uint& uiNumber = 1);

            /// Returns the value at the given index converted to a number.
            /** \param iIndex The index at which to search for the value
            *   \return The value at the given index converted to a number
            */
            s_float GetNumber(const s_int& iIndex = -1);

            /// Returns the value at the given index converted to a bool.
            /** \param iIndex The index at which to search for the value
            *   \return The value at the given index converted to a bool
            */
            s_bool  GetBool(const s_int& iIndex = -1);

            /// Returns the value at the given index converted to a string.
            /** \param iIndex The index at which to search for the value
            *   \return The value at the given index converted to a string
            */
            s_str   GetString(const s_int& iIndex = -1);

            /// Returns the value at the given index.
            /** \param iIndex The index at which to search for the value
            *   \return The value at the given index
            */
            s_var   GetValue(const s_int& iIndex = -1);

            /// Returns the Lunar object at the given index.
            /** \param iIndex The index at which to search for the value
            *   \return The Lunar object at the given index
            */
            template<class T>
            s_ptr<T> Get(const s_int& iIndex = -1)
            {
                return Lunar<T>::widecheck(pLua_, iIndex.Get());
            }

            /// Returns the number of value on the stack.
            /** \return The number of value on the stack
            */
            s_uint  GetTop();

            /// Returns the type of the value on the stack.
            /** \param iIndex The index of the value to analyse
            *   \return The type
            */
            Type    GetType(const s_int& iIndex = -1);

            /// Returns the name of a Lua::Type.
            /** \param mType The type to serialize
            *   \return The name of the Lua::Type
            */
            s_str   GetTypeName(Type mType);

            /// Puts a global variable on the stack.
            /** \param sName The name of the global variable
            *   \note The name can contain tables, for example :
            *         "MyTable.MyVariable" is a valid input.
            */
            void    GetGlobal(const s_str& sName);

            /// Reads an int from Lua.
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param iDefaultValue The default value
            *   \return The int value
            */
            s_int   GetGlobalInt(const s_str& sName, const s_bool& bCritical = true, const s_int& iDefaultValue = 0);

            /// Reads a float from Lua.
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param fDefaultValue The default value
            *   \return The float value
            */
            s_float GetGlobalFloat(const s_str& sName, const s_bool& bCritical = true, const s_float& fDefaultValue = 0.0f);

            /// Wrapper to read a string from Lua.
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param sDefaultValue The default value
            *   \return The string value
            */
            s_str   GetGlobalString(const s_str& sName, const s_bool& bCritical = true, const s_str& sDefaultValue = "");

            /// Reads a bool from Lua.
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param bDefaultValue The default value
            *   \return The bool value
            */
            s_bool  GetGlobalBool(const s_str& sName, const s_bool& bCritical = true, const s_bool& bDefaultValue = false);

            /// Puts a value from a Lua table on the stack.
            /** \param sName  The name of the key associated to the value
            *   \param iIndex The position of the table in the stack
            *   \note Puts 'nil' if the key couldn't be found.
            */
            void    GetField(const s_str& sName, const s_int& iIndex = -1);

            /// Puts a value from a Lua table on the stack.
            /** \param iID    The id of the key associated to the value
            *   \param iIndex The position of the table in the stack
            *   \note Puts 'nil' if the key couldn't be found.
            */
            void    GetField(const s_int& iID, const s_int& iIndex = -1);

            /// Reads an int from a Lua table.
            /** \param sName         The name of the key associated to the value
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param iDefaultValue The default value
            *   \param bSetValue     If 'true' and the key wasn't found in the table,
            *                        this function will create that key in the Lua table
            *                        and assign it its default value
            *   \return The int value
            *   \note The table that will be used to read the value should be at the top of
            *         the stack just before you call that function.
            */
            s_int   GetFieldInt(const s_str& sName, const s_bool& bCritical = true, const s_int& iDefaultValue = 0, const s_bool& bSetValue = false);

            /// Reads a float from a Lua table.
            /** \param sName         The name of the key associated to the value
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param fDefaultValue The default value
            *   \param bSetValue     If 'true' and the key wasn't found in the table,
            *                        this function will create that key in the Lua table
            *                        and assign it its default value
            *   \return The float value
            *   \note The table that will be used to read the value should be at the top of
            *         the stack just before you call that function.
            */
            s_float GetFieldFloat(const s_str& sName, const s_bool& bCritical = true, const s_float& fDefaultValue = 0.0f, const s_bool& bSetValue = false);

            /// Reads a string from a Lua table.
            /** \param sName         The name of the key associated to the value
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param sDefaultValue The default value
            *   \param bSetValue     If 'true' and the key wasn't found in the table,
            *                        this function will create that key in the Lua table
            *                        and assign it its default value
            *   \return The string value
            *   \note The table that will be used to read the value should be at the top of
            *         the stack just before you call that function.
            */
            s_str   GetFieldString(const s_str& sName, const s_bool& bCritical = true, const s_str& sDefaultValue = "", const s_bool& bSetValue = false);

            /// Reads a bool from a Lua table.
            /** \param sName         The name of the key associated to the value
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param bDefaultValue The default value
            *   \param bSetValue     If 'true' and the key wasn't found in the table,
            *                        this function will create that key in the Lua table
            *                        and assign it its default value
            *   \return The bool value
            *   \note The table that will be used to read the value should be at the top of
            *         the stack just before you call that function.
            */
            s_bool  GetFieldBool(const s_str& sName, const s_bool& bCritical = true, const s_bool& bDefaultValue = false, const s_bool& bSetValue = false);

            /// Writes a value into a Lua table.
            /** \param sName The name of the key associated to the value
            *   \note The value to put into the table must be at the top of the stack.<br>
            *         The table must be at the index just before the value.<br>
            *         Pops the value from the stack.
            */
            void    SetField(const s_str& sName);

            /// Writes a value into a Lua table.
            /** \param iID The ID of the key associated to the value
            *   \note The value to put into the table must be at the top of the stack.<br>
            *         The table must be at the index just before the value.<br>
            *         Pops the value from the stack.
            */
            void    SetField(const s_int& iID);

            /// Writes an int into a Lua table.
            /** \param sName  The name of the key associated to the value
            *   \param iValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldInt(const s_str& sName, const s_int& iValue);

            /// Writes a float into a Lua table.
            /** \param sName  The name of the key associated to the value
            *   \param fValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldFloat(const s_str& sName, const s_float& fValue);

            /// Writes a string into a Lua table.
            /** \param sName  The name of the key associated to the value
            *   \param sValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldString(const s_str& sName, const s_str& sValue);

            /// Writes a bool into a Lua table.
            /** \param sName  The name of the key associated to the value
            *   \param bValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldBool(const s_str& sName, const s_bool& bValue);

            /// Writes an int into a Lua table.
            /** \param iID    The ID of the key associated to the value
            *   \param iValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldInt(const s_int& iID, const s_int& iValue);

            /// Writes a float into a Lua table.
            /** \param iID    The ID of the key associated to the value
            *   \param fValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldFloat(const s_int& iID, const s_float& fValue);

            /// Writes a string into a Lua table.
            /** \param iID    The ID of the key associated to the value
            *   \param sValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldString(const s_int& iID, const s_str& sValue);

            /// Writes a bool into a Lua table.
            /** \param iID    The ID of the key associated to the value
            *   \param bValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldBool(const s_int& iID, const s_bool& bValue);

            /// Changes the stack size.
            /** \param uiSize The new size of the stack
            *   \note If the stack has more elements, they will be erased.<br>
            *         If is has less, the stack will be filled with nil.
            */
            void    SetTop(const s_uint& uiSize);

            static const s_str CLASS_NAME;

            s_str sComString;

        private :

            lua_State* pLua_;

            lua_CFunction pErrorFunction_;
        };
    }
}

#endif
