/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           Font manager source          */
/*                                        */
/*  ## : Contains the member functions    */
/*  of the FontManager class.             */
/*       A FontManager is obviously a     */
/*  class that manages font creation and  */
/*  storage.                              */
/*                                        */


#include "frost_localemanager.h"

using namespace std;

namespace Frost
{
    const s_str LocaleManager::CLASS_NAME = "LocaleManager";

    LocaleManager::LocaleManager()
    {
    }

    LocaleManager::~LocaleManager()
    {
    }

    s_str LocaleManager::GetLocalizedString( s_str sStr, s_str sGroup )
    {
        if (MAPFIND(sGroup, lLocalizationTable_))
        {
            s_ptr< map<s_str, s_str> > lSubLocTable = &lLocalizationTable_[sGroup];
            if (MAPFIND(sStr, *lSubLocTable))
            {
                return (*lSubLocTable)[sStr];
            }
        }
        return s_str("<")+sStr+s_str(">");
    }

    void LocaleManager::SetLocale( s_str sLocale )
    {
        if (sLocale_ == "")
            sLocale_ = sLocale;
        else
            Warning(CLASS_NAME, "SetLocale can't be called more than once.");
    }

    const s_str& LocaleManager::GetLocale() const
    {
        return sLocale_;
    }

    s_bool LocaleManager::ReadConfig()
    {
        sLocale_ = LuaManager::GetSingleton()->GetDefaultLua()->GetGlobalString("sLocale");
        if (sLocale_ == "")
        {
            Error(CLASS_NAME, "No locale specified.");
            return false;
        }
        return ReadLocale();
    }

    s_bool LocaleManager::ReadLocale()
    {
        s_str sSection = "";
        s_str sKey = "";
        s_str sValue = "";

        s_str sFile = s_str("Scripts/Locale_") + sLocale_ + s_str(".loc");
        s_str sLine;
        File mFile(sFile, FILE_I);

        while (mFile.IsValid())
        {
            sLine = mFile.GetLine();
            sLine.Trim(' ');

            if (sLine != "")
            {
                if ((sLine.Trim('[') == 1) &&
                    (sLine.Trim(']') == 1))
                {
                    // New section
                    sSection = sLine;
                }
                else
                {
                    vector<s_str> lParams = sLine.Cut("=", 1);
                    if (lParams.size() > 1)
                    {
                        s_str sTemp = lParams[0];
                        sTemp.Trim(' ');
                        sKey = sTemp;
                        sTemp = lParams[1];
                        sTemp.Trim(' ');
                        sValue = sTemp;
                        if (sValue.Trim('"') > 1)
                        {
                            // Store this entry
                            if ( (sSection != "") && (sKey != "") )
                            {
                                lLocalizationTable_[sSection][sKey] = sValue;
                            }
                        }
                    }
                }
            }
        }

        return true;
    }
}
