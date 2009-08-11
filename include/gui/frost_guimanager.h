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

        s_ctnr<s_str> lFileList;
        s_ctnr<s_str> lSavedVariableList;

        static const s_str CLASS_NAME;

        AddOn()
        {
            bEnabled = true;
        }
    };

    enum FrameStrata
    {
        STRATA_BACKGROUND,
        STRATA_LOW,
        STRATA_MEDIUM,
        STRATA_HIGH,
        STRATA_DIALOG,
        STRATA_FULLSCREEN,
        STRATA_FULLSCREEN_DIALOG,
        STRATA_TOOLTIP
    };

    /// Contains Frame
    struct Level
    {
        s_ptr<GUI::Frame>                  pTopLevel;
        s_map< s_uint, s_ptr<GUI::Frame> > lFrameList;

        static const s_str CLASS_NAME;
    };

    /// Contains Level
    struct Strata
    {
        s_ptr<GUI::Frame>    pTopStrata;
        s_map<s_uint, Level> lLevelList;

        static const s_str CLASS_NAME;
    };

    /// Manages the user interface
    class GUIManager : public Manager<GUIManager>
    {
    friend class Manager<GUIManager>;
    public :

        /// Initializes this manager.
        void                 Initialize();

        /// Adds a new folder to be parsed for UI AddOns.
        /** \param sFolder The new folder
        */
        void                 AddAddOnFolder(const s_str& sFolder);

        /// Clears the AddOn folder list.
        /** \note This is usefull whenever you need to reload a
        *         completely different UI (for example, when switching
        *         from your game's main menu to the real game).
        */
        void                 ClearAddOnFolderList();

        /// Adds an UIObject to be handled by this manager.
        /** \param pObj The object to add
        *   \return 'false' if the name of the widget was already taken
        */
        s_bool               AddUIObject(s_ptr<GUI::UIObject> pObj);

        /// Returns the UIObject associated with the given name.
        /** \param sName    The name of the widget you're after
        *   \param bVirtual 'true' to search for a virtual frame
        *   \return The UIObject associated with the given name
        */
        s_ptr<GUI::UIObject> GetUIObjectByName(const s_str& sName, const s_bool& bVirtual = false);

        /// Tells this manager it must rebuild its strata list.
        void                 FireBuildStrataList();

        /// Returns a top strata frame.
        /** \param mStrata The strata to search in
        *   \return This strata's top frame
        */
        s_ptr<GUI::Frame>    GetTopStrata(FrameStrata mStrata);

        /// Returns a top level frame.
        /** \param mStrata The strata to search in
        *   \param uiLevel The strat's level to search in
        *   \return This level's top frame
        */
        s_ptr<GUI::Frame>    GetTopLevel(FrameStrata mStrata, const s_uint& uiLevel);

        /// Prints debug informations in the log file.
        /** \note Calls UIObject::Serialize().
        */
        void                 PrintUI();

        /// Returns the GUI Lua state.
        /** \return The GUI Lua state
        */
        s_ptr<Lua::State>    GetLua();

        /// Loads the UI.
        void                 LoadUI();

        /// Closes the UI, deletes widgets.
        void                 CloseUI();

        /// Closes the current UI and load it again.
        void                 ReloadUI();

        /// Renders the UI into the current render target.
        void                 RenderUI();

        /// Ask this manager for movement management.
        /** \param pObj The object to move
        *   \note This manager will not do any movement code.
        *         It will only allow this widget to be moved
        *         through the IsMoving() function.
        */
        void                 StartMoving(s_ptr<GUI::UIObject> pObj);

        /// Stops movement for the given object.
        /** \param pObj The object to stop moving
        */
        void                 StopMoving(s_ptr<GUI::UIObject> pObj);

        /// Checks if the given object is allowed to move.
        /** \param pObj The object to check
        *   \return 'true' if the given object is allowed to move
        */
        s_bool               IsMoving(s_ptr<GUI::UIObject> pObj) const;

        /// Ask this manager for sizing management.
        /** \param pObj The object to resize
        *   \note This manager will not do any sizing code.
        *         It will only allow this widget to be resized
        *         through the IsSizing() function.
        */
        void                 StartSizing(s_ptr<GUI::UIObject> pObj);

        /// Stops sizing for the given object.
        /** \param pObj The object to stop sizing
        */
        void                 StopSizing(s_ptr<GUI::UIObject> pObj);

        /// Checks if the given object is allowed to be resized.
        /** \param pObj The object to check
        *   \return 'true' if the given object is allowed to be resized
        */
        s_bool               IsSizing(s_ptr<GUI::UIObject> pObj) const;

        /// Returns the cumuled horizontal mouse movement.
        /** \return The cumuled horizontal mouse movement
        *   \note This value is reset to zero whenever StartMoving() or
        *         StartSizing() is called.
        */
        s_int                GetMovementX() const;

        /// Returns the cumuled vertical mouse movement.
        /** \return The cumuled vertical mouse movement
        *   \note This value is reset to zero whenever StartMoving() or
        *         StartSizing() is called.
        */
        s_int                GetMovementY() const;

        /// Updates this manager and its widgets.
        /** \param fDelta The time elapsed since the last call
        */
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

        /// Copy constructor
        GUIManager(const GUIManager& mMgr);

        /// Assignment operator
        GUIManager& operator = (const GUIManager& mMgr);

    private :

        void LoadAddOnTOC_(const s_str& sAddOnName, const s_str& sAddOnFolder);
        void LoadAddOnFiles_(s_ptr<AddOn> pAddOn);
        void LoadAddOnDirectory_(const s_str& sDirectory);

        void ParseXMLFile_(const s_str& sFile, s_ptr<AddOn> pAddOn);

        // Attribute parsers
        s_bool ParseFrameAttributes_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseTextureAttributes_(s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pMainBlock);
        s_bool ParseFontStringAttributes_(s_ptr<GUI::FontString> pFontString, s_ptr<XML::Block> pMainBlock);

        // General block parsers
        s_bool ParseSizeBlock_(s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock);
        s_bool ParseAnchorsBlock_(s_ptr<GUI::UIObject> pObject, s_ptr<XML::Block> pMainBlock);

        // Widget block parsers
        s_bool ParseResizeBoundsBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseTitleRegionBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseBackdropBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseHitRectInsetsBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseLayersBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseFramesBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);
        s_bool ParseScriptsBlock_(s_ptr<GUI::Frame> pFrame, s_ptr<XML::Block> pMainBlock);

        // Texture block parsers
        s_bool ParseTexCoordsBlock_(s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock);
        s_bool ParseTextureColorBlock_(s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock);
        s_bool ParseGradientBlock_(s_ptr<GUI::Texture> pTexture, s_ptr<XML::Block> pTextureBlock);

        // FontString block parsers
        s_bool ParseShadowBlock_(s_ptr<GUI::FontString> pFontString, s_ptr<XML::Block> pFontStringBlock);
        s_bool ParseFontStringColorBlock_(s_ptr<GUI::FontString> pFontString, s_ptr<XML::Block> pFontStringBlock);

        // Widget parsers
        s_bool ParseFrameBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseButtonBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseEditBoxBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseSMFBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseSliderBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);
        s_bool ParseStatusBarBlock_(s_ptr<GUI::Frame> pParent, s_ptr<XML::Block> pWidgetBlock);

        // Region parsers
        s_bool ParseFontStringBlock_(s_ptr<GUI::FontString> pFontString, s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock);
        s_bool ParseTextureBlock_(s_ptr<GUI::Texture> pTexture, s_ptr<GUI::Frame> pParent, const s_str& sLevel, s_ptr<XML::Block> pArtBlock);

        s_ptr<Lua::State> pLua_;
        s_bool            bClosed_;

        s_map< s_str, s_ptr<GUI::UIObject> >  lNamedObjectList_;
        s_map< s_str, s_ptr<GUI::UIObject> >  lNamedVirtualObjectList_;

        s_map< s_uint, s_ptr<GUI::UIObject> > lObjectList_;
        s_map< s_uint, s_ptr<GUI::UIObject> > lMainObjectList_;

        s_ctnr<s_str>                       lGUIFolderList_;
        s_map< s_str, s_map<s_str, AddOn> > lAddOnList_;

        s_map< s_uint, s_ptr<GUI::Frame> > lFrameList_;
        s_map<FrameStrata, Strata>         lStrataList_;
        s_bool                             bBuildStrataList_;
        s_ptr<GUI::Frame>                  pOveredFrame_;

        s_ptr<GUI::UIObject> pMovedObject_;
        s_ptr<GUI::UIObject> pSizedObject_;
        s_float              fMouseMovementX_;
        s_float              fMouseMovementY_;
    };
}


#endif
