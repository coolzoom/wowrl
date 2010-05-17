/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_utils_types.h"
#include "frost_utils_stdhelper.h"

namespace Frost
{
    s_ustr UTF8ToUnicode( const s_str& sStr )
    {
        static uchar MAX_ANSI = 127;
        static uchar ESC_C2   = 194;
        static uchar ESC_C3   = 195;

        s_ustr sResult;

        s_str::const_iterator iter;
        s_uchar cEscape = s_uchar::NaN;

        foreach (iter, sStr)
        {
            uchar c = *iter;
            if (c <= MAX_ANSI)
            {
                sResult.PushBack(c);
            }
            else
            {
                if (c == ESC_C2 || c == ESC_C3)
                {
                    cEscape = c;
                    continue;
                }

                if (cEscape.IsValid())
                {
                    if (cEscape == ESC_C3)
                    {
                        // 192 : offset of for "c3" (195) escaped characters (accentuated)
                        // 128 : start offset for these characters (the first one is "c3 80")
                        sResult.PushBack(192 + *iter - 128);
                    }
                    else if (cEscape == ESC_C2)
                    {
                        // 192 : offset of for "c2" (194) escaped characters (misc)
                        // 128 : start offset for these characters (the first one is "c2 80")
                        sResult.PushBack(128 + *iter - 128);
                    }
                }
            }
        }

        return sResult;
    }

    s_str UnicodeToUTF8( const s_ustr& sUStr )
    {
        static uchar MAX_ANSI = 127;
        static uchar ESC_C2   = 194;
        static uchar ESC_C3   = 195;

        s_str sResult;

        s_ustr::const_iterator iter;

        foreach (iter, sUStr)
        {
            uint c = (uchar)*iter;
            if (c <= MAX_ANSI)
            {
                sResult.PushBack(c);
            }
            else
            {
                if (c <= 191)
                {
                    sResult.PushBack(ESC_C2);
                    sResult.PushBack(128 + c - 128);
                }
                else
                {
                    sResult.PushBack(ESC_C3);
                    sResult.PushBack(128 + c - 192);
                }
            }
        }

        return sResult;
    }
}
