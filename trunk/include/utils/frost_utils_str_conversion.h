// Warning : If you need to use this file, include frost_utils_types.h
/** \cond NOT_REMOVE_FROM_DOC
*/
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

    template<class T>
    class StringConverter< T, s_str_t<T> >
    {
    public :

        typedef std::basic_string<T> string;

        /// Copy constructor.
        /** \param s The string to copy
        */
        static string Convert(const s_str_t<T>& s)
        {
            return s.Get();
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const s_str_t<T>& s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s.Get();
            }

            return sReturn;
        }
    };

    template<class T>
    class StringConverter< T, std::basic_string<T> >
    {
    public :

        typedef std::basic_string<T> string;

        /// Constructor.
        /** \param s The string to copy
        */
        static string Convert(const string& s)
        {
            return s;
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const string& s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s;
            }

            return sReturn;
        }
    };

    template<class T>
    class StringConverter< T, const T* >
    {
    public :

        typedef std::basic_string<T> string;

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(const T* s)
        {
            return s;
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const T* s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s;
            }

            return sReturn;
        }
    };

    template<class T>
    class StringConverter< T, T* >
    {
    public :

        typedef std::basic_string<T> string;

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(T* s)
        {
            return s;
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(T* s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s;
            }

            return sReturn;
        }
    };

    template<class T, default_uint N>
    class StringConverter< T, T[N] >
    {
    public :

        typedef std::basic_string<T> string;

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(const T* s)
        {
            return s;
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const T* s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s;
            }

            return sReturn;
        }
    };

    template<class T, default_uint N>
    class StringConverter< T, const T[N] >
    {
    public :

        typedef std::basic_string<T> string;

        /// Constructor.
        /** \param s The character array to copy
        */
        static string Convert(const T* s)
        {
            return s;
        }

        /// Creates "uiNbr" copies of the provided string.
        /** \param s     The string to copy
        *   \param uiNbr The number of times to copy the string
        *   \note If uiNbr equals 0, creates an empty string.
        */
        static string Convert(const T* s, const s_uint_t<default_uint>& uiNbr)
        {
            string sReturn;

            if (uiNbr.IsValid())
            {
                for (default_uint uiCounter = 0; uiCounter < uiNbr.Get(); ++uiCounter)
                    sReturn += s;
            }

            return sReturn;
        }
    };

    template<class T>
    class StringConverter<T, T>
    {
    public :

        typedef std::basic_string<T> string;

        /// Constructor.
        /** \param c The character to copy
        */
        static string Convert(const T& c)
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
        static string Convert(const T& c, const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                return string(uiNbr.Get(), c);
            else
                return string();
        }

        /// Constructor.
        /** \param c The character to copy
        */
        static T ConvertChar(const T& c)
        {
            return c;
        }
    };

    template<> class StringConverter<string_element, TypeTraits<string_element>::FrostType>
    {
    public :

        typedef string_object string;

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

        /// Constructor.
        /** \param c The character to copy
        */
        static TypeTraits<string_element>::FrostType ConvertChar(const TypeTraits<string_element>::FrostType& c)
        {
            return c;
        }
    };

    template<class N> class StringConverter< string_element, s_int_t<N> >
    {
    public :

        typedef string_object string;

        /// Converts the provided integer.
        /** \param i The integer to convert
        */
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
        static string Convert(const s_int_t<N>& i, const s_uint_t<default_uint>& uiCharNbr)
        {
            string sReturn;

            if (i >= 0)
            {
                sReturn = StringConverter< string_element, s_int_t<N> >::Convert(i);

                if (uiCharNbr.IsValid())
                {
                    while (sReturn.length() < uiCharNbr.Get())
                        sReturn = '0' + sReturn;
                }
            }
            else
            {
                sReturn = StringConverter< string_element, s_int_t<N> >::Convert(-i);

                if (uiCharNbr.IsValid())
                {
                    while (sReturn.length() < uiCharNbr.Get())
                        sReturn = '0' + sReturn;
                }

                sReturn = '-' + sReturn;
            }

            return sReturn;
        }
    };

    template<> class StringConverter< string_element, TypeTraits<uint>::FrostType >
    {
    public :

        typedef string_object string;

        /// Converts the provided unsigned integer (as a number).
        /** \param ui The unsigned integer to convert
        */
        static string Convert(const TypeTraits<uint>::FrostType& ui)
        {
            switch (ui.GetType())
            {
                case TypeTraits<uint>::FrostType::INTEGER :
                {
                    string_stream sStream;
                    sStream << ui.Get();
                    return sStream.str();
                }
                case TypeTraits<uint>::FrostType::INTEGER_NAN :
                    return "nan";
                case TypeTraits<uint>::FrostType::INTEGER_INF :
                    return "inf";
                default : return "";
            }
        }

        /// Converts and formats the provided unsigned integer (as a number).
        /** \param uiValue   The unsigned integer to convert
        *   \param uiCharNbr The minimum number of character allowed (fills with zeros)
        *   \note If the string representation of ui is larger than uiCharNbr, it won't
        *         be truncated.
        */
        static string Convert(const TypeTraits<uint>::FrostType& ui, const s_uint_t<default_uint>& uiCharNbr)
        {
            string sReturn = StringConverter< string_element, TypeTraits<uint>::FrostType >::Convert(ui);

            if (uiCharNbr.IsValid())
            {
                while (sReturn.length() < uiCharNbr.Get())
                    sReturn = '0' + sReturn;
            }

            return sReturn;
        }

        /// Constructor.
        /** \param c The character to copy
        */
        static TypeTraits<string_element>::FrostType ConvertChar(const TypeTraits<uint>::FrostType& c)
        {
            return UnicodeToUTF8(c);
        }
    };

    template<class N> class StringConverter< string_element, s_uint_t<N> >
    {
    public :

        typedef string_object string;

        /// Converts the provided unsigned integer.
        /** \param ui The unsigned integer to convert
        */
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
        static string Convert(const s_uint_t<N>& ui, const s_uint_t<default_uint>& uiCharNbr)
        {
            string sReturn = StringConverter< string_element, s_uint_t<N> >::Convert(ui);

            if (uiCharNbr.IsValid())
            {
                while (sReturn.length() < uiCharNbr.Get())
                    sReturn = '0' + sReturn;
            }

            return sReturn;
        }
    };

    template<class N> class StringConverter< string_element, s_float_t<N> >
    {
    public :

        typedef string_object string;

        /// Converts the provided floating point number.
        /** \param f The floating point number to convert
        */
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
        static string Convert(const s_float_t<N>& f, const s_uint_t<default_uint>& uiIntCharNbr, const s_uint_t<default_uint>& uiFracCharNbr = s_float_t<N>::DIGIT)
        {
            string sReturn;

            if (f >= 0)
            {
                s_float_t<N> fRoundDown = s_float_t<N>::RoundDown(f);

                sReturn = StringConverter< string_element, s_int_t<default_int> >::Convert(
                    s_int_t<default_int>(fRoundDown), uiIntCharNbr
                );

                if (uiFracCharNbr > 0)
                {
                    s_float_t<N> fPower = 1.0f;
                    for (default_uint i = 0; i < uiFracCharNbr.Get(); ++i)
                        fPower *= 10.0f;

                    sReturn += ".";
                    sReturn += StringConverter< string_element, s_uint_t<default_uint> >::Convert(
                        s_uint_t<default_uint>((f - fRoundDown)*fPower)
                    );
                }
            }
            else
            {
                s_float_t<N> fRoundUp = s_float_t<N>::RoundUp(f);

                sReturn = StringConverter< string_element, s_int_t<default_int> >::Convert(
                    s_int_t<default_int>(fRoundUp), uiIntCharNbr
                );

                if (uiFracCharNbr > 0)
                {
                    s_float_t<N> fPower = 1.0f;
                    for (default_uint i = 0; i < uiFracCharNbr.Get(); ++i)
                        fPower *= 10.0f;

                    sReturn += ".";
                    sReturn += StringConverter< string_element, s_uint_t<default_uint> >::Convert(
                        s_uint_t<default_uint>((fRoundUp - f)*fPower)
                    );
                }
            }

            return sReturn;
        }
    };

    template<class N> class StringConverter< string_element, s_bool_t<N> >
    {
    public :

        typedef string_object string;

        /// Converts the provided boolean.
        /** \param b The boolean to convert
        */
        static string Convert(const s_bool_t<N>& b)
        {
            return b ? "true" : "false";
        }
    };

    template<class N> class StringConverter< string_element, s_ptr<N> >
    {
    public :

        typedef string_object string;

        /// Converts the provided pointer's address.
        /** \param b The pointer to convert
        *   \note The pointer is converted to a ptrdiff_t.
        */
        static string Convert(const s_ptr<N>& p)
        {
            if (p.IsValid())
                return StringConverter< string_element, s_int_t<ptrdiff_t> >::Convert(
                    s_int_t<ptrdiff_t>((ptrdiff_t)p.Get())
                );
            else
                return ("NULL");
        }
    };

    template<class N> class StringConverter< string_element, s_refptr<N> >
    {
    public :

        typedef string_object string;

        /// Converts the provided pointer's address.
        /** \param b The pointer to convert
        *   \note The pointer is converted to a ptrdiff_t.
        */
        static string Convert(const s_refptr<N>& p)
        {
            if (p.IsValid())
                return StringConverter< string_element, s_int_t<ptrdiff_t> >::Convert(
                    s_int_t<ptrdiff_t>((ptrdiff_t)p.Get())
                );
            else
                return ("NULL");
        }
    };

    template<class N> class StringConverter< string_element, s_wptr<N> >
    {
    public :

        typedef string_object string;

        /// Converts the provided pointer's address.
        /** \param b The pointer to convert
        *   \note The pointer is converted to a ptrdiff_t.
        */
        static string Convert(const s_wptr<N>& p)
        {
            if (p.IsValid())
                return StringConverter< string_element, s_int_t<ptrdiff_t> >::Convert(
                    s_int_t<ptrdiff_t>((ptrdiff_t)p.Get())
                );
            else
                return ("NULL");
        }
    };

    template<> class StringConverter< uint, s_str_t<string_element> >
    {
    public :

        typedef std::basic_string<uint> string;

        typedef bool IsDefined;

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const s_str_t<string_element>& s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const s_str_t<string_element>& s)
        {
            return UTF8ToUnicode(s.Get());
        }
    };

    template<> class StringConverter< uint, string_object >
    {
    public :

        typedef std::basic_string<uint> string;

        typedef bool IsDefined;

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const string_object& s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const string_object& s)
        {
            return UTF8ToUnicode(s);
        }
    };

    template<> class StringConverter< uint, const string_element* >
    {
    public :

        typedef std::basic_string<uint> string;

        typedef bool IsDefined;

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const string_element* s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const string_element* s)
        {
            return UTF8ToUnicode(string_object(s));
        }
    };

    template<> class StringConverter< uint, string_element* >
    {
    public :

        typedef std::basic_string<uint> string;

        typedef bool IsDefined;

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(string_element* s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(string_element* s)
        {
            return UTF8ToUnicode(string_object(s));
        }
    };

    template<default_uint N> class StringConverter< uint, const string_element[N] >
    {
    public :

        typedef std::basic_string<uint> string;

        typedef bool IsDefined;

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const string_element* s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const string_element* s)
        {
            return UTF8ToUnicode(string_object(s));
        }
    };

    template<default_uint N> class StringConverter< uint, string_element[N] >
    {
    public :

        typedef std::basic_string<uint> string;

        typedef bool IsDefined;

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to copy
        *   \note Calls UTF8ToUnicode().
        */
        static string Construct(const string_element* s)
        {
            return Convert(s);
        }

        /// Converts the provided UTF8 encoded string.
        /** \param s The string to convert
        *   \note Calls UTF8ToUnicode().
        */
        static string Convert(const string_element* s)
        {
            return UTF8ToUnicode(string_object(s));
        }
    };

    template<> class StringConverter<uint, TypeTraits<uint>::FrostType>
    {
    public :

        typedef std::basic_string<uint> string;

        /// Constructor.
        /** \param c The character to copy
        */
        static string Convert(const TypeTraits<uint>::FrostType& c)
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
        static string Convert(const TypeTraits<uint>::FrostType& c, const s_uint_t<default_uint>& uiNbr)
        {
            if (uiNbr.IsValid())
                return string(uiNbr.Get(), c.Get());
            else
                return string();
        }
    };

    template<> class StringConverter< string_element, std::basic_string<uint> >
    {
    public :

        typedef std::basic_string<string_element> string;

        /// Converts the provided Unicode string to a UTF8 encoded string.
        /** \param s The string to convert
        */
        static string Convert(const std::basic_string<uint>& s)
        {
            return UnicodeToUTF8(s);
        }
    };

    template<> class StringConverter< string_element, uint >
    {
    public :

        typedef string_object string;

        /// Constructor.
        /** \param c The character to copy
        */
        static string Convert(const uint& c)
        {
            std::basic_string<uint> s;
            s.push_back(c);
            return UnicodeToUTF8(s);
        }

        /// Constructor.
        /** \param c The character to copy
        */
        static string_element ConvertChar(const uint& c)
        {
            return UnicodeToUTF8(c);
        }
    };
}
/** \endcond
*/
