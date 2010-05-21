/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_types.h"
#include "frost_utils_stdhelper.h"

namespace Frost
{
    s_ustr UTF8ToUnicode( const s_str& s )
    {
        return UTF8ToUnicode(s.Get());
    }

    s_ustr::string UTF8ToUnicode( const s_str::string& s )
    {
        static uchar MAX_ANSI = 127;
        static uchar ESC_C2   = 194;
        static uchar ESC_C3   = 195;

        s_ustr::string sResult;

        s_str::string::const_iterator iter;
        uchar cEscape = 0;

        foreach (iter, s)
        {
            uchar c = *iter;
            if (c <= MAX_ANSI)
            {
                sResult.push_back(c);
            }
            else
            {
                if (c == ESC_C2 || c == ESC_C3)
                {
                    cEscape = c;
                    continue;
                }

                if (cEscape != 0)
                {
                    if (cEscape == ESC_C3)
                    {
                        // 192 : offset of for "c3" (195) escaped characters (accentuated)
                        // 128 : start offset for these characters (the first one is "c3 80")
                        sResult.push_back(192 + c - 128);
                    }
                    else if (cEscape == ESC_C2)
                    {
                        // 192 : offset of for "c2" (194) escaped characters (misc)
                        // 128 : start offset for these characters (the first one is "c2 80")
                        sResult.push_back(128 + c - 128);
                    }
                }
            }
        }

        return sResult;
    }

    s_uint UTF8ToUnicode( const s_char& c )
    {
        return UTF8ToUnicode(c.Get());
    }

    uint UTF8ToUnicode( const char& c )
    {
        static uchar MAX_ANSI = 127;

        uchar uc = (uchar)c;

        if (uc <= MAX_ANSI)
            return uc;
        else
            return 0u;
    }

    s_str UnicodeToUTF8( const s_ustr& s )
    {
        return UnicodeToUTF8(s.Get());
    }

    s_str::string UnicodeToUTF8( const s_ustr::string& s )
    {
        static uint MAX_ANSI = 127;
        static uint ESC_C2   = 194;
        static uint ESC_C3   = 195;

        s_str::string sResult;

        s_ustr::string::const_iterator iter;

        foreach (iter, s)
        {
            uint c = *iter;
            if (c <= MAX_ANSI)
            {
                sResult.push_back(c);
            }
            else
            {
                if (c < 192)
                {
                    sResult.push_back((uchar)ESC_C2);
                    sResult.push_back((uchar)(128 + c - 128));
                }
                else
                {
                    sResult.push_back((uchar)ESC_C3);
                    sResult.push_back((uchar)(128 + c - 192));
                }
            }
        }

        return sResult;
    }

    s_char UnicodeToUTF8( const s_uint& c )
    {
        return UnicodeToUTF8(c.Get());
    }

    char UnicodeToUTF8( const uint& c )
    {
        static uint MAX_ANSI = 127;
        if (c <= MAX_ANSI)
            return (uchar)c;
        else
            return '\0';
    }
}
