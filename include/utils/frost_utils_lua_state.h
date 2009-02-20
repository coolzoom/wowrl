/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Lua::State header            */
/*                                        */
/*                                        */

extern "C"
{
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
    #include "lua/lua.h"
}

#include "lua/lunar.h"

#ifndef FROST_UTILS_LUASTATE_H
#define FROST_UTILS_LUASTATE_H

#include "frost_utils.h"
#include "frost_utils_manager.h"

#include "frost_utils_var.h"
#include "frost_utils_array.h"

namespace Frost
{
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

            /// Concatenates a Lua table into a string
            /** \param pLua The Lua state to use
            *   \param sTable The name of the table in Lua
            */
            s_str   ConcTable(const s_str& sTable);

            /// Copy the content of a table from a Lua state to another
            /** \param pLua      The other Lua state into wich the table will be copied
            *   \param sSrcName  The name of the table in the source state
            *   \param sDestName The name of the table in the destination state
            *   \note If sDestName is ommited, the table will have the same name in both Lua states.
            */
            void    CopyTable(s_ptr<State> pLua , const s_str& sSrcName, const s_str& sDestName = "");

            /// Executes a Lua file
            /** \param pLua  The Lua state to use
            *   \param sFile The name of the file
            *   \return 'false' if there was an error in the file
            *   \note This function prints the errors in the log file
            */
            s_bool  DoFile(const s_str& sFile);

            /// Executes a string containing Lua instructions
            /** \param pLua The Lua state to use
            *   \param sStr The string to execute
            *   \return 'false' if there was an error in the string
            *   \note This function prints the errors in the log file
            */
            s_bool  DoString(const s_str& sStr);

            /// Executes a Lua function
            /** \param pLua          The Lua state to use
            *   \param sFunctionName The name of the function to execute
            *   \return 'false' if there was an error
            *   \note This function prints the errors in the log file
            */
            s_bool  CallFunction(const s_str& sFunctionName);

            /// Executes a Lua function with arguments
            /** \param pLua           The Lua state to use
            *   \param sFunctionName  The name of the function to execute
            *   \param lArgumentStack The agument stack (order matters)
            *   \return 'false' if there was an error
            *   \note This function prints the errors in the log file
            */
            s_bool  CallFunction(const s_str& sFunctionName, const s_ctnr<s_var>& lArgumentStack);

            /// Binds a C++ function to a Lua function.
            /** \param sFunctionName The name of the Lua function
            *   \param mFunction     The C++ function to bind
            */
            void    Register(const s_str& sFunctionName, lua_CFunction mFunction);

            /// Prints an error string in the log file with the Lua tag
            /** \param pLua   The Lua state to use
            *   \param sError The error string to output
            *   \note This function will print out the actual file and line
            */
            void    PrintError(const s_str& sError);

            /// Puts a number on the stack.
            /** \param iValue The value to push on the stack (converted to float)
            */
            void    PushNumber(const s_int& iValue);

            /// Puts a number on the stack.
            /** \param uiValue The value to push on the stack (converted to float)
            */
            void    PushNumber(const s_uint& uiValue);

            /// Puts a number on the stack.
            /** \param iValue The value to push on the stack
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

            /// Puts "nil" (NULL) on the stack.
            /** \param uiNumber The number of "nil" to push
            */
            void    PushNil(const s_uint& uiNumber = 1);

            /// Puts the value of a global Lua variable on the stack.
            /** \param sName The name of this variable
            */
            void    PushGlobal(const s_str& sName);

            /// Sets the value of a global Lua variable.
            /** \param sName The name of this variable
            *   \note The value taken is taken at the top of the stack,
            *         and popped.
            */
            void    SetGlobal(const s_str& sName);

            /// Creates a new empty table and pushes it on the stack.
            void    NewTable();

            /// Removes the value at the top of the stack.
            /** \param uiNumber The number of value to remove
            */
            void    Pop(const s_uint& uiNumber = 1);

            /// Returns the value at the given index converted to a number.
            /** \param uiIndex The index at wich to search for the value
            *   \return The value at the given index converted to a number
            */
            s_float GetNumber(const s_uint& uiIndex = 1);

            /// Returns the value at the given index converted to a bool.
            /** \param uiIndex The index at wich to search for the value
            *   \return The value at the given index converted to a bool
            */
            s_bool  GetBool(const s_uint& uiIndex = 1);

            /// Returns the value at the given index converted to a string.
            /** \param uiIndex The index at wich to search for the value
            *   \return The value at the given index converted to a string
            */
            s_str   GetString(const s_uint& uiIndex = 1);

            /// Returns the number of value on the stack.
            /** \return The number of value on the stack
            */
            s_uint  GetTop();

            /// Returns the type of the value on the stack.
            /** \param uiIndex The index of the value to analyse
            *   \return The type
            */
            Type    GetType(const s_uint& uiIndex = 1);

            /// Returns the name of a Lua::Type.
            /** \param mType The type to serialize
            *   \return The name of the Lua::Type
            */
            s_str   GetTypeName(Type mType);

            /// Wrapper to read an int from Lua
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param iDefaultValue The default value
            *   \return The int value
            */
            s_int   GetGlobalInt(const s_str& sName, const s_bool& bCritical = true, const s_int& iDefaultValue = 0);

            /// Wrapper to read a float from Lua
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param fDefaultValue The default value
            *   \return The float value
            */
            s_float GetGlobalFloat(const s_str& sName, const s_bool& bCritical = true, const s_float& fDefaultValue = 0.0f);

            /// Wrapper to read a string from Lua
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param sDefaultValue The default value
            *   \return The string value
            */
            s_str   GetGlobalString(const s_str& sName, const s_bool& bCritical = true, const s_str& sDefaultValue = "");

            /// Wrapper to read a bool from Lua
            /** \param sName         The name of the variable (global scope)
            *   \param bCritical     If 'true', an error will be printed if
            *                        the variable is not found. Else, it will
            *                        be assigned its default value
            *   \param bDefaultValue The default value
            *   \return The bool value
            */
            s_bool  GetGlobalBool(const s_str& sName, const s_bool& bCritical = true, const s_bool& bDefaultValue = false);

            /// Wrapper to read an int from a Lua table
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

            /// Wrapper to read a float from a Lua table
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

            /// Wrapper to read a string from a Lua table
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

            /// Wrapper to read a bool from a Lua table
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

            /// Wrapper to write an int into a Lua table
            /** \param sName  The name of the key associated to the value
            *   \param iValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldInt(const s_str& sName, const s_int& iValue);

            /// Wrapper to write a float into a Lua table
            /** \param sName  The name of the key associated to the value
            *   \param fValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldFloat(const s_str& sName, const s_float& fValue);

            /// Wrapper to write a string into a Lua table
            /** \param sName  The name of the key associated to the value
            *   \param sValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldString(const s_str& sName, const s_str& sValue);

            /// Wrapper to write a bool into a Lua table
            /** \param sName  The name of the key associated to the value
            *   \param bValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldBool(const s_str& sName, const s_bool& bValue);

            /// Wrapper to write an int into a Lua table
            /** \param iID    The ID of the key associated to the value
            *   \param iValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldInt(const s_int& iID, const s_int& iValue);

            /// Wrapper to write a float into a Lua table
            /** \param iID    The ID of the key associated to the value
            *   \param fValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldFloat(const s_int& iID, const s_float& fValue);

            /// Wrapper to write a string into a Lua table
            /** \param iID    The ID of the key associated to the value
            *   \param sValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldString(const s_int& iID, const s_str& sValue);

            /// Wrapper to write a bool into a Lua table
            /** \param iID    The ID of the key associated to the value
            *   \param bValue The value to set
            *   \note The table that will be used to write the value should be at the top of
            *         the stack just before you call that function.
            */
            void    SetFieldBool(const s_int& iID, const s_bool& bValue);

            static const s_str CLASS_NAME;

            s_str sComString;

        private :

            lua_State* pLua_;
        };
    }
}

#endif
