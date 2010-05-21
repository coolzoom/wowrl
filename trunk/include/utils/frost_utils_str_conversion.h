// Warning : If you need to use this file, include frost_utils_types.h
namespace Frost
{
    s_str_t<uint>           UTF8ToUnicode(const s_str_t<char>& s);
    std::basic_string<uint> UTF8ToUnicode(const std::string& s);
    s_uint_t<uint>          UTF8ToUnicode(const s_int_t<char>& c);
    uint                    UTF8ToUnicode(const char& c);

    s_str_t<char>           UnicodeToUTF8(const s_str_t<uint>& s);
    std::string             UnicodeToUTF8(const std::basic_string<uint>& s);
    s_int_t<char>           UnicodeToUTF8(const s_uint_t<uint>& c);
    char                    UnicodeToUTF8(const uint& c);

    /// Allows explicit string conversion and operator overloading.
    /** This class is used for explicit constructors and +, += and <<
    *   operators. It is specialized for most commonly used string
    *   types : s_str and s_ustr.<br>
    */
    template<class T>
    class StringConverter
    {
        template<class N>
        struct IsDefined
        {
        };
    };

    /// Specialization for standard string.
    /** \note See StringConverter.
    */
    template<> class StringConverter<string_element>
    {
    public :

        typedef std::basic_string<string_element> string;

        template<class N>
        struct IsDefined
        {
        };

        /// Copy constructor.
        /** \param s The string to copy
        */
        static string Convert(const s_str_t<string_element>& s)
        {
            return s.Get();
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const s_str_t<string_element>& s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s.Get();
            }

            return sReturn;
        }

        /// Constructor.
        /** \param s The string to copy
        */
        static string Convert(const string& s)
        {
            return s;
        }

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(const string_element* s)
        {
            return s;
        }

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(string_element* s)
        {
            return s;
        }

        /// Constructor.
        /** \param c The character to copy
        */
        static string Convert(const string_element& c)
        {
            string s;
            s.push_back(c);
            return s;
        }

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const string_element& c, const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                return string(uiNbr.Get(), c);
            else
                return string();
        }

        /// Constructor.
        /** \param c The character to copy
        */
        static string Convert(const TypeTraits<string_element>::FrostType& c)
        {
            string s;
            s.push_back(c.Get());
            return s;
        }

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const TypeTraits<string_element>::FrostType& c, const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                return string(uiNbr.Get(), c.Get());
            else
                return string();
        }

        /// Converts the provided integer.
        /** \param i The integer to convert
        */
        template<class N>
        static string Convert(const s_int_t<N>& i)
        {
            switch (i.GetType())
            {
                case s_int_t<N>::INTEGER :
                {
                    string_stream sStream;
                    sStream << i.Get();
                    return sStream.str();
                }
                case s_int_t<N>::INTEGER_NAN :
                    return "nan";
                case  s_int_t<N>::INTEGER_INF_PLUS :
                    return "inf+";
                case s_int_t<N>::INTEGER_INF_MINUS :
                    return "inf-";
                default : return "";
            }
        }

        /// Converts and formats the provided integer.
        /** \param i         The integer to convert
        *   \param uiCharNbr The minimum number of character allowed (fills with zeros)
        *   \note If the string representation of i is larger than uiCharNbr, it won't
        *         be truncated.
        */
        template<class N>
        static string Convert(const s_int_t<N>& i, const s_uint_t<default_uint>& uiCharNbr)
        {
            string sReturn;

            if (i >= 0)
            {
                sReturn = StringConverter<string_element>::Convert(i);

                if (uiCharNbr.IsValid())
                {
                    while (sReturn.length() < uiCharNbr.Get())
                        sReturn = '0' + sReturn;
                }
            }
            else
            {
                sReturn = StringConverter<string_element>::Convert(-i);

                if (uiCharNbr.IsValid())
                {
                    while (sReturn.length() < uiCharNbr.Get())
                        sReturn = '0' + sReturn;
                }

                sReturn = '-' + sReturn;
            }

            return sReturn;
        }

        /// Converts the provided unsigned integer.
        /** \param ui The unsigned integer to convert
        */
        template<class N>
        static string Convert(const s_uint_t<N>& ui)
        {
            switch (ui.GetType())
            {
                case s_uint_t<N>::INTEGER :
                {
                    string_stream sStream;
                    sStream << ui.Get();
                    return sStream.str();
                }
                case s_uint_t<N>::INTEGER_NAN :
                    return "nan";
                case s_uint_t<N>::INTEGER_INF :
                    return "inf";
                default : return "";
            }
        }

        /// Converts and formats the provided unsigned integer.
        /** \param uiValue   The unsigned integer to convert
        *   \param uiCharNbr The minimum number of character allowed (fills with zeros)
        *   \note If the string representation of ui is larger than uiCharNbr, it won't
        *         be truncated.
        */
        template<class N>
        static string Convert(const s_uint_t<N>& ui, const s_uint_t<default_uint>& uiCharNbr)
        {
            string sReturn = StringConverter<string_element>::Convert(ui);

            if (uiCharNbr.IsValid())
            {
                while (sReturn.length() < uiCharNbr.Get())
                    sReturn = '0' + sReturn;
            }

            return sReturn;
        }

        /// Converts the provided floating point number.
        /** \param f The floating point number to convert
        */
        template<class N>
        static string Convert(const s_float_t<N>& f)
        {
            switch (f.GetType())
            {
                case s_float_t<N>::FLOAT :
                {
                    string_stream sStream;
                    sStream.precision(s_float_t<N>::DIGIT);
                    sStream << f.Get();
                    return sStream.str();
                }
                case s_float_t<N>::FLOAT_NAN :
                    return "nan";
                case s_float_t<N>::FLOAT_INF_PLUS :
                    return "inf+";
                case s_float_t<N>::FLOAT_INF_MINUS :
                    return "inf-";
                default : return "";
            }
        }

        /// s_float conversion constructor.
        /** \param fValue         The s_float to convert
        *   \param uiIntCharNbr   The minimum number of character allowed for the
        *                         integer part (fills with zeros)
        *   \param uinFracCharNbr The number of decimal to show (fills with zeros)
        */
        template<class N>
        static string Convert(const s_float_t<N>& f, const s_uint_t<default_uint>& uiIntCharNbr, const s_uint_t<default_uint>& uiFracCharNbr = s_float_t<N>::DIGIT)
        {
            string sReturn;

            if (f >= 0)
            {
                s_float_t<N> fRoundDown = s_float_t<N>::RoundDown(f);

                sReturn = StringConverter<string_element>::Convert(s_int_t<default_int>(fRoundDown), uiIntCharNbr);

                if (uiFracCharNbr > 0)
                {
                    s_float_t<N> fPower = 1.0f;
                    for (default_uint i = 0; i < uiFracCharNbr.Get(); ++i)
                        fPower *= 10.0f;

                    sReturn += ".";
                    sReturn += StringConverter<string_element>::Convert(s_uint_t<default_uint>((f - fRoundDown)*fPower));
                }
            }
            else
            {
                s_float_t<N> fRoundUp = s_float_t<N>::RoundUp(f);

                sReturn = StringConverter<string_element>::Convert(s_int_t<default_int>(fRoundUp), uiIntCharNbr);

                if (uiFracCharNbr > 0)
                {
                    s_float_t<N> fPower = 1.0f;
                    for (default_uint i = 0; i < uiFracCharNbr.Get(); ++i)
                        fPower *= 10.0f;

                    sReturn += ".";
                    sReturn += StringConverter<string_element>::Convert(s_uint_t<default_uint>((fRoundUp - f)*fPower));
                }
            }

            return sReturn;
        }

        /// Converts the provided boolean.
        /** \param b The boolean to convert
        */
        template<class N>
        static string Convert(const s_bool_t<N>& b)
        {
            return b ? "true" : "false";
        }

        /// Converts the provided pointer's address.
        /** \param b The pointer to convert
        *   \note The pointer is converted to a ptrdiff_t.
        */
        template<class N>
        static string Convert(const s_ptr<N>& p)
        {
            if (p.IsValid())
                return Convert(s_int_t<ptrdiff_t>((ptrdiff_t)p.Get()));
            else
                return ("NULL");
        }

        /// Converts the provided pointer's address.
        /** \param b The pointer to convert
        *   \note The pointer is converted to a ptrdiff_t.
        */
        template<class N>
        static string Convert(const s_refptr<N>& p)
        {
            if (p.IsValid())
                return Convert(s_int_t<ptrdiff_t>((ptrdiff_t)p.Get()));
            else
                return ("NULL");
        }

        /// Converts the provided pointer's address.
        /** \param b The pointer to convert
        *   \note The pointer is converted to a ptrdiff_t.
        */
        template<class N>
        static string Convert(const s_wptr<N>& p)
        {
            if (p.IsValid())
                return Convert(s_int_t<ptrdiff_t>((ptrdiff_t)p.Get()));
            else
                return ("NULL");
        }

        /// Converts the provided string to a UTF8 encoded string.
        /** \param s The string to convert
        */
        static string_object ConvertToStandard(const string& s)
        {
            return s;
        }

        /// Converts the provided character to a UTF8 encoded string.
        /** \param s The string to convert
        */
        static string_element ConvertToStandard(const string_element& c)
        {
            return c;
        }

        /// Converts the provided character to a UTF8 encoded string.
        /** \param s The string to convert
        */
        static TypeTraits<string_element>::FrostType ConvertToStandard(const TypeTraits<string_element>::FrostType& c)
        {
            return c;
        }
    };

    /// Specialization for unicode string.
    /** \note See StringConverter.
    */
    template<> class StringConverter<uint>
    {
    public :

        typedef std::basic_string<uint> string;

        template<class N, class Dummy = void>
        struct IsDefined
        {
        };

        template<class Dummy> struct IsDefined<s_str_t<string_element>, Dummy>         { typedef bool Type; };
        template<class Dummy> struct IsDefined<string_object, Dummy >                  { typedef bool Type; };
        template<class Dummy> struct IsDefined<const string_element*, Dummy>           { typedef bool Type; };
        template<class Dummy> struct IsDefined<string_element*, Dummy>                 { typedef bool Type; };
        template<uint N, class Dummy> struct IsDefined<const string_element[N], Dummy> { typedef bool Type; };
        template<uint N, class Dummy> struct IsDefined<string_element[N], Dummy>       { typedef bool Type; };

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const s_str_t<string_element>& s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const string_object& s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const string_element* s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(string_element* s)
        {
            return Convert(s);
        }

        /// Copy constructor.
        /** \param s The string to copy
        */
        static string Convert(const s_str_t<uint>& s)
        {
            return s.Get();
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const s_str_t<uint>& s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s.Get();
            }

            return sReturn;
        }

        /// Constructor.
        /** \param s The string to copy
        */
        static string Convert(const string& s)
        {
            return s;
        }

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(const uint* s)
        {
            return s;
        }

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(uint* s)
        {
            return s;
        }

        /// Constructor.
        /** \param c The character to copy
        */
        static string Convert(const uint& c)
        {
            string s;
            s.push_back(c);
            return s;
        }

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const uint& c, const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                return string(uiNbr.Get(), c);
            else
                return string();
        }

        /// Constructor.
        /** \param c The character to copy
        */
        static string Convert(const s_uint_t<uint>& c)
        {
            string s;
            s.push_back(c.Get());
            return s;
        }

        /// Creates "uiNbr" copies of the provided character.
        /** \param c     The character to copy
        *   \param uiNbr The number of times to copy the character
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const s_uint_t<uint>& c, const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                return string(uiNbr.Get(), c.Get());
            else
                return string();
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const s_str_t<string_element>& s)
        {
            return UTF8ToUnicode(s.Get());
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const string_object& s)
        {
            return UTF8ToUnicode(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const string_element* s)
        {
            return UTF8ToUnicode(string_object(s));
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(string_element* s)
        {
            return UTF8ToUnicode(string_object(s));
        }

        /// Converts the provided Unicode string to a UTF8 encoded string.
        /** \param s The string to convert
        */
        static string_object ConvertToStandard(const string& s)
        {
            return UnicodeToUTF8(s);
        }

        /// Converts the provided Unicode character to a UTF8 encoded string.
        /** \param s The string to convert
        */
        static string_element ConvertToStandard(const uint& c)
        {
            s_str_t<string_element> s = UnicodeToUTF8(c);
            if (s.GetSize() == 1)
                return s[0];
            else
                return '\0';
        }

        /// Converts the provided Unicode character to a UTF8 encoded string.
        /** \param s The string to convert
        */
        static TypeTraits<string_element>::FrostType ConvertToStandard(const s_uint_t<uint>& c)
        {
            s_str_t<string_element> s = UnicodeToUTF8(c);
            if (s.GetSize() == 1)
                return s[0];
            else
                return '\0';
        }
    };
}
