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

        void                 Initialize();

        s_bool               AddUIObject(s_ptr<GUI::UIObject> pObj);
        void                 RemoveUIObject(s_ptr<GUI::UIObject> pObj);
        s_ptr<GUI::UIObject> GetUIObjectByName(const s_str& sName, const s_bool& bVirtual = false);

        void                 PrintUI();

        s_ptr<Lua::State>    GetLua();

        void                 LoadUI();
        void                 CloseUI();
        void                 ReloadUI();

        void                 Update(const s_float& fDelta);

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

        void LoadAddOnTOC_(const s_str& sAddOnName, const s_str& sAddOnFolder);
        void LoadAddOnFiles_(s_ptr<AddOn> pAddOn);
        void LoadAddOnDirectory_(const s_str& sDirectory);

        void ParseXMLFile_(const s_str& sFile, s_ptr<AddOn> pAddOn);

        // Attribute parsers
        s_bool ParseFrameAttributes_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);

        // Block parsers
        s_bool ParseSizeBlock_(s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock);
        s_bool ParseAnchorsBlock_(s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock);
        s_bool ParseTitleRegionBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseBackdropBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseHitRectInsetsBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseLayersBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseFramesBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseScriptsBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);

        // Widget parsers
        s_bool ParseFrameBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseButtonBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseEditBoxBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseSMFBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseSliderBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseStatusBarBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);

        // Art parsers
        s_bool ParseFontStringBlock_(s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock);
        s_bool ParseTextureBlock_(s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock);

        s_ptr<Lua::State> pLua_;

        std::map< s_str, s_ptr<GUI::UIObject> >  lNamedObjectList_;
        std::map< s_str, s_ptr<GUI::UIObject> >  lNamedVirtualObjectList_;

        std::map< s_uint, s_ptr<GUI::UIObject> > lObjectList_;

        std::map<s_str, AddOn> lAddOnList_;
    };
}


#endif
