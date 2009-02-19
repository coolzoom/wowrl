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

#ifndef FROST_LUA_UTILS_H
#define FROST_LUA_UTILS_H

#include "frost.h"

namespace Frost
{
    namespace Lua
    {
        class Argument;
        class Function;

        /// Lua data
        class Data
        {
        public :

            /// Default constructor
            Data();

            /// Main constructor
            /** \param sName    The name associated to this data
            *   \param iLuaType The expected type in Lua
            *   \param mType    The C++ type you'll be using
            *   \param pParent  A pointer to the Argument that'll be using it
            *   \note You shouldn't have to call it. Consider using Function instead.
            */
            Data(const s_str& sName, int iLuaType, ValueType mType, s_ptr<Argument> pParent);

            /// Gets data from Lua
            /** \param pLua    The Lua state to use
            *   \param uiIndex The index at which to get the value
            *   \note Only called on a valid Data (expected type is found).
            */
            void         Set(lua_State* pLua, const s_uint& uiIndex);

            /// Returns this argument's name.
            /** \return This argument's name
            */
            const s_str& GetName() const;

            /// Returns this argument's value.
            /** \return This argument's value
            */
            const s_var& GetValue() const;

            /// Returns this argument's Lua type.
            /** \return This argument's Lua type
            */
            const int&   GetLuaType() const;

            /// Returns this argument's type.
            /** \return This argument's type
            */
            ValueType    GetType() const;

            static const s_str CLASS_NAME;

        private :

            s_str           sName_;
            s_var           mValue_;
            int             iLuaType_;
            ValueType       mType_;
            s_ptr<Argument> pParent_;
        };

        /// Argument of a Lua glue
        /** Used internally by Function.<br>
        *   If, for some reason, you need to add an argument
        *   without using Function's interface, please let me
        *   know on the forums and I'll try to see what can be
        *   done.
        */
        class Argument
        {
        public :

            /// Default constructor.
            Argument();

            /// Main constructor.
            /** \param sName    The name of this argument (used to print errors in the log)
            *   \param iLuaType The expected type in Lua
            *   \param mType    The C++ type you'll be using (conversion is done automatically)
            *   \param pParent  A pointer to the function that'll be using it
            *   \note You shouldn't have to call it. Consider using Function instead.
            */
            Argument(const s_str& sName, int iLuaType, ValueType mType, s_ptr<Function> pParent);

            /// Adds an alternative to this argument.
            /** \param sName    The name of this alternative argument (used to print errors in the log)
            *   \param iLuaType The expected type in Lua
            *   \param mType    The C++ type you'll be using (conversion is done automatically)
            */
            void          Add(const s_str& sName, int iLuaType, ValueType mType);

            /// Returns the associated Data.
            /** \return The associated Data
            */
            s_ptr<Data>   Get() const;

            /// Returns the value and converts it to an int.
            /** \return The value and converts it to an int
            */
            s_int         GetI() const;

            /// Returns the value and converts it to an unsigned int.
            /** \return The value and converts it to an unsigned int
            */
            s_uint        GetUI() const;

            /// Returns the value and converts it to a float.
            /** \return The value and converts it to a float
            */
            s_float       GetF() const;

            /// Returns the value and converts it to a double.
            /** \return The value and converts it to a double
            */
            s_double      GetD() const;

            /// Returns the value and converts it to a bool.
            /** \return The value and converts it to a bool
            */
            s_bool        GetB() const;

            /// Returns the value and converts it to a string.
            /** \return The value and converts it to a string
            */
            s_str         GetS() const;

            /// Returns the value and converts it to a void pointer.
            /** \return The value and converts it to a void pointer
            */
            const void*   GetP() const;

            /// Returns the actual type of this value.
            /** \return The actual type of this value
            */
            int           GetType() const;

            /// Checks if this argument has been provided (in case it is optional).
            /** \return 'true' if the user has provided this argument
            */
            const s_bool& IsProvided() const;

            /// Checks if this argument has the expected type(s).
            /** \param pLua        The Lua state to use
            *   \param uiIndex     The index to check
            *   \param bPrintError Set to 'false' if you don't want that function
            *                      to print errors in the log
            *   \return 'true' if everything went fine
            */
            s_bool        Test(lua_State* pLua, const s_uint& uiIndex, const s_bool& bPrintError = true);

            /// Sets this argument's data
            /** \param pData A pointer to the good Data
            */
            void          SetData(s_ptr<Data> pData);

            static const s_str CLASS_NAME;

        private :

            s_bool            bSet_;
            s_ptr<Data>       pData_;
            std::vector<Data> lData_;
            s_ptr<Function>   pParent_;
        };

        enum ReturnValueType
        {
            /// Returns nil (= NULL in Lua)
            RETURN_NIL,
            /// Returns a number (promoted to a float in Lua)
            RETURN_NUMBER,
            /// Returns a string
            RETURN_STRING,
            /// Returns a boolean
            RETURN_BOOLEAN,
            /// Returns a Lua object (a table, ...)
            RETURN_OBJECT
        };

        /// Return value of a Lua glue
        struct ReturnValue
        {
            ReturnValue();
            ReturnValue(ReturnValueType type);
            ReturnValue(const s_var& vVal);
            ReturnValue(const s_int& iVal);
            ReturnValue(const s_uint& uiVal);
            ReturnValue(const s_float& fVal);
            ReturnValue(const s_double& dVal);
            ReturnValue(const s_bool& bVal);
            ReturnValue(const s_str& sVal);

            ReturnValueType mType;
            s_var           vValue;
        };

        struct ArgumentList
        {
            std::map<s_uint, Argument> lArg_;
            std::map<s_uint, Argument> lOptional_;
            s_uint                     uiRank_;
        };

        /// A helper to write Lua glues
        /** A glue is a C++ function that is executed
        *   in Lua. Due to the way Lua communicates with
        *   C++, creating such functions can become boring
        *   if you check every argument's type, or if you
        *   allow optional arguments, or even two possible
        *   types for a single argument.<br>
        *   This is done quite easilly with this class.<br>
        *   Look at the source code for existing glues to see
        *   how things work (UIObject::_SetPoint() is a good
        *   example).
        */
        class Function
        {
        public :

            /// Default constructor.
            /** \param sName       The name of your function (used to print errors in the log)
            *   \param pLua        The Lua state to use
            *   \param uiReturnNbr The maximum number of returned values
            *   \note Call this at the beginning of your glue
            */
            Function(const s_str& sName, lua_State* pLua, const s_uint& uiReturnNbr = 0u);

            /// Adds an argument to that function.
            /** \param uiIndex    The index of this argument
            *   \param sName      The name of this argument (used to print errors in the log)
            *   \param iLuaType   The expected type in Lua
            *   \param mType      The C++ type you'll be using (conversion is done automatically)
            *   \param bOptional 'true' if this argument is not essential and can be ommited
            *   \note Optional arguments work just like in C++ : if you say argument n is optional,
            *         then all the following arguments will have to be optional too.<br>
            *         You can add several different arguments with the same index, but they must
            *         have different Lua types. The function will then choose the right one acording
            *         to the actual Lua type that the user has provided.
            */
            void            Add(const s_uint& uiIndex, const s_str& sName, int iLuaType, ValueType mType, const s_bool& bOptional = false);

            /// Tells the Function there is another parameter set that will be provided.
            void            NewParamSet();

            /// Returns the indice of the paramater set that the user has provided.
            /** \return The indice of the paramater set that the user has provided
            *   \note Should be called after Check() has been called.
            */
            const s_uint&   GetParamSetRank();

            /// Checks if all the arguments types and retreive them.
            /** \param bPrintError If set to 'false', this function will fail
            *          silently
            *   \return 'true' if everything went fine
            *   \note You should always check the return value of this function.
            */
            s_bool          Check(const s_bool& bPrintError = true);

            /// Returns the argument at the provided index.
            /** \param uiIndex The index of the argument
            *   \return The argument at the provided index
            */
            s_ptr<Argument> Get(const s_uint& uiIndex);

            /// Checks if an argument has been provided (in case it is optional).
            /** \param uiIndex The index of the argument
            *   \return 'true' if the user has provided this argument
            *   \note You should always check the return value of this function
            *         whenever you want to use an optional argument.
            */
            s_bool          IsProvided(const s_uint& uiIndex) const;

            /// Returns the number of provided parameters.
            /** \return The number of provided parameters
            *   \note Should be called after Check() has been called.
            */
            const s_uint&   GetParamCount() const;

            /// Adds a return value
            /** \param mReturn The returned value
            *   \note - If you want to return a Lua object, <b>you must put it on the
            *         stack yourself</b>, and then call this function :<br>
            *         Push(Lua::ReturnValue(Lua::RETURN_OBJECT));<br>
            *         - Values are immediatly pushed onto the Lua stack, so
            *         the order in which you return your values is important.<br>
            *         - If, for some reason, your can't push one of your return
            *         values, push nil instead.<br>
            *         - Note that this class will automatically fill the stack with
            *         nil until the proper number of return values is reached when
            *         Return() is called.
            */
            void            Push(ReturnValue mReturn);

            /// Ends the function
            /** \return The number of returned values
            *   \note Use the return of this function as return value of your glue.<br>
            *         Note that this class will automatically fill the stack with
            *         nil until the proper number of return values is reached.
            */
            int             Return();

            /// Returns the name of this function.
            /** \return The name of this function
            */
            const s_str&    GetName() const;

            static const s_str CLASS_NAME;

        private :

            s_str                      sName_;
            lua_State*                 pLua_;
            s_uint                     uiParamCount_;
            s_uint                     uiReturnNbr_;
            s_uint                     uiReturnCount_;
            std::vector<ArgumentList>  lArgListStack_;
            s_ptr<ArgumentList>        pArgList_;
        };
    }
}


#endif
