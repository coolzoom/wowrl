/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           GUIManager header            */
/*                                        */
/*                                        */


#ifndef FROST_GUIMANAGER_H
#define FROST_GUIMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// A piece of the user interface
    struct AddOn
    {
        s_str sName;
        s_str sVersion;
        s_str sUIVersion;
        s_str sAuthor;

        s_bool bEnabled;

        s_str sFolder;

        std::vector<s_str> lFileList;
        std::vector<s_str> lSavedVariableList;

        static const s_str CLASS_NAME;

        AddOn()
        {
            bEnabled = true;
        }
    };

    /// Manages the user interface
    class GUIManager : public Manager<GUIManager>
    {
    friend class Manager<GUIManager>;
    public :

        void           Initialize();

        s_bool               AddUIObject(s_ptr<GUI::UIObject> pObj);
        void                 RemoveUIObject(s_ptr<GUI::UIObject> pObj);
        s_ptr<GUI::UIObject> GetUIObjectByName(const s_str& sName);

        lua_State*     GetLua();

        void           LoadUI();
        void           CloseUI();
        void           ReloadUI();

        void ParseXMLFile(const s_str& sFile);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call GUIManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling GUIManager::Delete() (this is
        *         automatically done by Engine).
        */
        GUIManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~GUIManager();

    private :

        void LoadAddOn(s_str sName, s_str sFolder);

        s_bool ParseFrameAttributes_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseSizeBlock_(s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock);
        s_bool ParseAnchorsBlock_(s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock);

        lua_State* pLua_;

        std::map< s_uint, s_ptr<GUI::UIObject> > lObjectList_;
        std::map< s_str, s_ptr<GUI::UIObject> >  lNamedObjectList_;

        std::map<s_str, AddOn> lAddOnList_;
    };
}


#endif
