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
#include "gui/frost_anchor.h"

namespace Frost
{
    /// Exception to be thrown by GUI code.
    /** \note These exceptions should always be handled.<br>
    *         The GUI is not a critical part of the Engine, so
    *         whatever happens there <b>musn't</b> close the
    *         program.
    */
    class GUIException : public Exception
    {
    public :

        GUIException(const s_str& sMessage) : Exception(sMessage)
        {
        }

        GUIException(const s_str& sClassName, const s_str& sMessage) : Exception(sClassName, sMessage)
        {
        }
    };

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

        /// Constraints movement along a particular axis
        enum Constraint
        {
            CONSTRAINT_NONE,
            CONSTRAINT_X,
            CONSTRAINT_Y
        };

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

        /// Returns the AddOn that is being parsed.
        /** \return The AddOn that is being parsed
        */
        s_ptr<AddOn>         GetCurrentAddOn();

        /// Reads a file address and completes it to make a working address.
        /** \param sFileName The raw file name
        *   \return The modified file name
        *   \note All file names are relative to the Engine's executable path,
        *         but sometimes you'd like to use a path that is relative to
        *         your AddOn folder for example. To do so, you need to append
        *         "|" in front of your file name.
        */
        s_str                ParseFileName(const s_str& sFileName) const;

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
        /** \param pObj        The object to move
        *   \param pAnchor     The reference anchor
        *   \param mConstraint The constraint axis if any
        *   \note Movement is handled by the manager itself, you don't
        *         need to do anything.
        */
        void                 StartMoving(
            s_ptr<GUI::UIObject> pObj,
            s_ptr<GUI::Anchor> pAnchor = nullptr,
            Constraint mConstraint = CONSTRAINT_NONE
        );

        /// Stops movement for the given object.
        /** \param pObj The object to stop moving
        */
        void                 StopMoving(s_ptr<GUI::UIObject> pObj);

        /// Checks if the given object is allowed to move.
        /** \param pObj The object to check
        *   \return 'true' if the given object is allowed to move
        */
        s_bool               IsMoving(s_ptr<GUI::UIObject> pObj) const;

        /// Starts resizing a widget.
        /** \param pObj   The object to resize
        *   \param mPoint The sizing point
        *   \note Resizing is handled by the manager itself, you don't
        *         need to do anything.
        */
        void                 StartSizing(s_ptr<GUI::UIObject> pObj, GUI::AnchorPoint mPoint);

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

        /// Tells this manager an object has moved.
        void                 NotifyObjectMoved();

        /// Returns the Frame under the mouse.
        /** \return The Frame under the mouse (nullptr if none)
        */
        s_ptr<GUI::Frame>    GetOveredFrame() const;

        /// Updates this manager and its widgets.
        /** \param fDelta The time elapsed since the last call
        */
        void                 Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
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

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~GUIManager();

        /// Copy constructor.
        GUIManager(const GUIManager& mMgr);

        /// Assignment operator.
        GUIManager& operator = (const GUIManager& mMgr);

    private :

        void LoadAddOnTOC_(const s_str& sAddOnName, const s_str& sAddOnFolder);
        void LoadAddOnFiles_(s_ptr<AddOn> pAddOn);
        void LoadAddOnDirectory_(const s_str& sDirectory);

        void ParseXMLFile_(const s_str& sFile, s_ptr<AddOn> pAddOn);

        s_ptr<Lua::State> pLua_;
        s_bool            bClosed_;

        s_map< s_str, s_ptr<GUI::UIObject> >  lNamedObjectList_;
        s_map< s_str, s_ptr<GUI::UIObject> >  lNamedVirtualObjectList_;

        s_map< s_uint, s_ptr<GUI::UIObject> > lObjectList_;
        s_map< s_uint, s_ptr<GUI::UIObject> > lMainObjectList_;

        s_ctnr<s_str>                       lGUIFolderList_;
        s_map< s_str, s_map<s_str, AddOn> > lAddOnList_;
        s_ptr<AddOn>                        pCurrentAddOn_;

        s_map< s_uint, s_ptr<GUI::Frame> > lFrameList_;
        s_map<FrameStrata, Strata>         lStrataList_;
        s_bool                             bBuildStrataList_;
        s_bool                             bObjectMoved_;
        s_ptr<GUI::Frame>                  pOveredFrame_;

        s_ptr<GUI::UIObject> pMovedObject_;
        s_ptr<GUI::UIObject> pSizedObject_;
        s_float              fMouseMovementX_;
        s_float              fMouseMovementY_;

        s_ptr<GUI::Anchor>   pMovedAnchor_;
        s_int                iMovementStartPositionX_;
        s_int                iMovementStartPositionY_;
        Constraint           mConstraint_;

        s_uint               uiResizeStartW_;
        s_uint               uiResizeStartH_;
        s_bool               bResizeWidth_;
        s_bool               bResizeHeight_;
        s_bool               bResizeFromRight_;
        s_bool               bResizeFromBottom_;
    };
}


#endif
