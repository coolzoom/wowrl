/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           GUIManager header            */
/*                                        */
/*                                        */


#ifndef FROST_GUIMANAGER_H
#define FROST_GUIMANAGER_H

#include <frost_utils.h>
#include <frost_utils_lua.h>
#include <frost_utils_eventreceiver.h>
#include <frost_utils_vector.h>
#include "frost_anchor.h"

namespace Frost
{
    namespace GUI
    {
        class UIObject;
        class Region;
        class LayeredRegion;
        class Frame;
        class EditBox;

        class Sprite;
        class RenderTarget;

        /// This function has to be called before using any GUI class.
        /** \param lParams Implementation dependent set of parameters
        *                  (see implementation file for details)
        */
        void Initialize( const s_map<s_str, s_var>& lParams );

        /// This function has to be called when you don't need the GUI classes anymore.
        /** \note Its main purpose is to free shared resources : if you don't call it,
        *         you'll create a memory leak (or worse).
        */
        void Close();
    };

    class GUIManagerImpl;

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

        s_str sMainDirectory;
        s_str sDirectory;

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
        STRATA_PARENT,
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
        s_ctnr< s_ptr<GUI::Frame> > lFrameList;

        static const s_str CLASS_NAME;
    };

    /// Contains Level
    struct Strata
    {
        s_uint                   uiID;
        s_map<s_int, Level>      lLevelList;
        mutable s_bool           bRedraw;
        s_ptr<GUI::RenderTarget> pRenderTarget;
        s_refptr<GUI::Sprite>    pSprite;
        s_uint                   uiRedrawCount;

        static const s_str CLASS_NAME;
    };

    /// Manages the user interface
    class GUIManager : public EventReceiver
    {
    private :

        template <class T>
        static s_ptr<GUI::Frame> CreateNewFrame(s_ptr<GUIManager> pMgr)
        {
            return new T(pMgr);
        }

        template <class T>
        static s_ptr<GUI::LayeredRegion> CreateNewLayeredRegion(s_ptr<GUIManager> pMgr)
        {
            return new T(pMgr);
        }

    public :

        /// Constructor.
        /** \param lParams A map containing initialization parameters.
        *   \note This map must contain :
        *         * "game_version" = the game version to confront UI files with (s_str)
        *         * "width"        = the width of the render window (s_uint)
        *         * "height"       = the height of the render window (s_uint)
        *   \note Depending on the implementation chosen (SFML, ...), you
        *         might need to provided additionnal values.
        *         See the documentation of the corresponding GUIManagerImpl.
        */
        GUIManager(const s_map<s_str, s_var>& lParams);

        /// Destructor.
        ~GUIManager();

        /// Allows the GUI to receive input and update.
        /** \note See IsEnabled().
        */
        void                 Enable();

        /// Blocks input events.
        /** \note See IsEnabled().
        */
        void                 Disable();

        /// Checks if input can be received.
        /** \return 'true' if input can be received
        *   \note All widgets must call this function and check
        *         its return value before reacting to input events.
        */
        const s_bool&        IsEnabled() const;

        /// Returns the "screen" width.
        /** \return The screen width
        *   \note This is not necessarily the real screen width.
        *         If the GUI is rendered into a small render target
        *         that takes only a portion of the real screen, then
        *         this function returns the width of the render target.
        */
        const s_uint&        GetScreenWidth() const;

        /// Returns the "screen" height.
        /** \return The screen height
        *   \note This is not necessarily the real screen height.
        *         If the GUI is rendered into a small render target
        *         that takes only a portion of the real screen, then
        *         this function returns the height of the render target.
        */
        const s_uint&        GetScreenHeight() const;

        /// Adds a new directory to be parsed for UI AddOns.
        /** \param sDirectory The new directory
        */
        void                 AddAddOnDirectory(const s_str& sDirectory);

        /// Clears the AddOn directory list.
        /** \note This is usefull whenever you need to reload a
        *         completely different UI (for example, when switching
        *         from your game's main menu to the real game).
        */
        void                 ClearAddOnDirectoryList();

        /// Checks the provided string is suitable for naming a widget.
        /** \param sName The string to test
        *   \return 'true' if the provided string can be the name of a widget
        */
        s_bool               CheckUIObjectName(const s_str& sName) const;

        /// Creates a new UIObject.
        /** \param sClassName The class of the UIObject (Frame, FontString, Button, ...)
        *   \return The new UIObject
        *   \note You have the responsability to detroy and initialize the created
        *         UIObject by yourself.
        */
        s_ptr<GUI::UIObject> CreateUIObject(const s_str& sClassName);

        /// Creates a new Frame.
        /** \param sClassName The sub class of the Frame (Button, ...)
        *   \return The new Frame
        *   \note You have the responsability to detroy and initialize the created
        *         Frame by yourself.
        */
        s_ptr<GUI::Frame>    CreateFrame(const s_str& sClassName);

        /// Creates a new Frame, ready for use.
        /** \param sClassName   The sub class of the Frame (Button, ...)
        *   \param sName        The name of this Frame
        *   \param pParent      The parent of the created Frame (can be nullptr)
        *   \param sInheritance The name of the Frame that should be inherited
        *                       (empty if none)
        *   \return The new Frame
        *   \note You don't have the responsability to detroy the created
        *         Frame, it will be done automatically, either by its parent
        *         or by this manager.
        *   \note This function takes care of the basic initializing : the
        *         Frame is directly usable.
        */
        s_ptr<GUI::Frame>    CreateFrame(
            const s_str& sClassName, const s_str& sName,
            s_ptr<GUI::Frame> pParent = nullptr, const s_str& sInheritance = ""
        );

        /// Creates a new LayeredRegion.
        /** \param sClassName The sub class of the LayeredRegion (FontString or Texture)
        *   \return The new LayeredRegion
        *   \note You have the responsability to detroy and initialize the created
        *         LayeredRegion by yourself.
        */
        s_ptr<GUI::LayeredRegion>    CreateLayeredRegion(const s_str& sClassName);

        /// Adds an UIObject to be handled by this manager.
        /** \param pObj The object to add
        *   \return 'false' if the name of the widget was already taken
        */
        s_bool               AddUIObject(s_ptr<GUI::UIObject> pObj);

        /// Removes an UIObject from this manager.
        /** \param pObj The object to remove
        *   \note Only call this function if you have to delete an object
        *         manually (instead of waiting for the UI to close).
        */
        void                 RemoveUIObject(s_ptr<GUI::UIObject> pObj);

        /// Tells this manager that the provided object now has a parent.
        /** \param pObj The object that has recently been given a parent
        *   \note When the GUI closes, the GUIManager deletes all objects
        *         that have no parents first, which then delete all their
        *         children. This function is just here to prevent double
        *         deleting of widgets that once were orphans.
        */
        void                 NotifyObjectHasParent(s_ptr<GUI::UIObject> pObj);

        /// Returns the UIObject associated with the given ID.
        /** \param uiID The unique ID representing the widget
        *   \return The UIObject associated with the given ID
        */
        s_ptr<const GUI::UIObject> GetUIObject(const s_uint& uiID) const;

        /// Returns the UIObject associated with the given ID.
        /** \param uiID The unique ID representing the widget
        *   \return The UIObject associated with the given ID
        */
        s_ptr<GUI::UIObject> GetUIObject(const s_uint& uiID);

        /// Returns the UIObject associated with the given name.
        /** \param sName    The name of the widget you're after
        *   \param bVirtual 'true' to search for a virtual frame
        *   \return The UIObject associated with the given name
        */
        s_ptr<const GUI::UIObject> GetUIObjectByName(const s_str& sName, const s_bool& bVirtual = false) const;

        /// Returns the UIObject associated with the given name.
        /** \param sName    The name of the widget you're after
        *   \param bVirtual 'true' to search for a virtual frame
        *   \return The UIObject associated with the given name
        */
        s_ptr<GUI::UIObject> GetUIObjectByName(const s_str& sName, const s_bool& bVirtual = false);

        /// Tells this manager it must rebuild its strata list.
        void                 FireBuildStrataList();

        /// Prints in the log several performance statistics.
        void                 PrintStatistics();

        /// Prints debug informations in the log file.
        /** \note Calls UIObject::Serialize().
        */
        void                 PrintUI() const;

        /// Returns the AddOn that is being parsed.
        /** \return The AddOn that is being parsed
        */
        s_ptr<AddOn>         GetCurrentAddOn();

        /// Reads a file address and completes it to make a working address.
        /** \param sFileName The raw file name
        *   \return The modified file name
        *   \note All file names are relative to the Engine's executable path,
        *         but sometimes you'd like to use a path that is relative to
        *         your AddOn directory for example. To do so, you need to append
        *         "|" in front of your file name.
        */
        s_str                ParseFileName(const s_str& sFileName) const;

        /// Binds some Lua code to a key.
        /** \param uiKey      The key to bind
        *   \param sLuaString The Lua code that will be executed
        */
        void                 SetKeyBinding(const s_uint& uiKey, const s_str& sLuaString);

        /// Binds some Lua code to a key.
        /** \param uiKey      The key to bind
        *   \param uiModifier The modifier key (shift, ctrl, ...)
        *   \param sLuaString The Lua code that will be executed
        */
        void                 SetKeyBinding(const s_uint& uiKey, const s_uint& uiModifier, const s_str& sLuaString);

        /// Binds some Lua code to a key.
        /** \param uiKey       The key to bind
        *   \param uiModifier1 The first modifier key (shift, ctrl, ...)
        *   \param uiModifier2 The second modifier key (shift, ctrl, ...)
        *   \param sLuaString  The Lua code that will be executed
        */
        void                 SetKeyBinding(
            const s_uint& uiKey, const s_uint& uiModifier1, const s_uint& uiModifier2, const s_str& sLuaString
        );

        /// Unbinds a key.
        /** \param uiKey The key to unbind
        */
        void                 RemoveKeyBinding(const s_uint& uiKey);

        /// Unbinds a key.
        /** \param uiKey      The key to unbind
        *   \param uiModifier The modifier key (shift, ctrl, ...)
        */
        void                 RemoveKeyBinding(const s_uint& uiKey, const s_uint& uiModifier);

        /// Unbinds a key.
        /** \param uiKey      The key to unbind
        *   \param uiModifier1 The first modifier key (shift, ctrl, ...)
        *   \param uiModifier2 The second modifier key (shift, ctrl, ...)
        */
        void                 RemoveKeyBinding(const s_uint& uiKey, const s_uint& uiModifier1, const s_uint& uiModifier2);

        /// Returns the GUI Lua state.
        /** \return The GUI Lua state
        */
        s_ptr<Lua::State>    GetLua();

        /// Creates the Lua::State that will be used to communicate with the GUI.
        /** \note This function is usefull if you need to create additionnal
        *         resources on the Lua::State before the GUI files are loaded.
        *         Else, you can simply use LoadUI().
        */
        void                 CreateLua();

        /// Reads GUI files in the directory list.
        /** \note See AddAddOnDirectory().
        *   \note See LoadUI().
        *   \note See CreateLua().
        */
        void                 ReadFiles();

        /// Loads the UI.
        /** \note Calls CreateLua() then ReadFiles().
        */
        void                 LoadUI();

        /// Closes the UI, deletes widgets.
        void                 CloseUI();

        /// Closes the current UI and load it again.
        /** \note Calls CloseUI() then LoadUI().
        */
        void                 ReloadUI();

        /// Renders the UI into the current render target.
        void                 RenderUI();

        /// Checks if the UI is currently being loaded.
        /** \return 'true' if the UI is currently being loaded
        */
        const s_bool&        IsLoadingUI() const;

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
            Vector::Constraint mConstraint = Vector::CONSTRAINT_NONE
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

        /// Tells this manager to redraw the UI.
        void                 FireRedraw(FrameStrata mStrata) const;

        /// Enables/disables GUI caching.
        /** \param bEnable 'true' to enable
        *   \note See ToggleCaching().
        */
        void                 EnableCaching(const s_bool& bEnable);

        /// Toggles render caching.
        /** \note Enabled by default.
        *   \note Enabling this will most likely improve performances.
        */
        void                 ToggleCaching();

        /// Checks if GUI caching is enabled.
        /** \return 'true' if GUI cachin is enabled
        */
        const s_bool&        IsCachingEnabled() const;

        /// Sets wether the Manager should clear all fonts when closed.
        /** \param bClear 'true' to clear fonts
        *   \note Enabled by default. Note that when enabled, it will also
        *         clear fonts when the UI is reloaded, and load them once
        *         again.
        */
        void                 ClearFontsOnClose(const s_bool& bClear);

        /// Manually create a strata's render target.
        /** \param mFrameStrata The strata to create the render target for
        *   \note Creating a render target can take some times. To
        *         avoid GUI hangs during creation, the GUIManager
        *         automatically creates the render targets when
        *         ReadFiles() is called, but only for strata that
        *         already contain frames when all .xml files are loaded.
        *         Other ones are created when needed, just before
        *         rendering to them. If you know that  one of them is
        *         empty at load time, and is going to be filled later
        *         during run time, you can call this function to ensure
        *         the render target will be created at load time,
        *         causing no GUI hangs.
        */
        void                 CreateStrataRenderTarget(FrameStrata mFrameStrata);

        /// Returns the Frame under the mouse.
        /** \return The Frame under the mouse (nullptr if none)
        */
        s_ptr<const GUI::Frame> GetOveredFrame() const;

        /// Asks this manager for focus.
        /** \param pEditBox The EditBox requesting focus
        */
        void                 RequestFocus(s_ptr<GUI::EditBox> pEditBox);

        /// Returns the highest level on the provided strata.
        /** \param mFrameStrata The strata to inspect
        *   \return The highest level on the provided strata
        */
        s_int                GetHighestLevel(FrameStrata mFrameStrata) const;

        /// Sets the "this" Lua variable.
        /** \param pFrame The Frame to use as "this"
        *   \note Keeps track of the previous "this" values to allow nested calls.
        */
        void                 ThisStackPush(s_ptr<GUI::Frame> pFrame);

        /// Sets the previous value of the "this" Lua variable.
        /** \note Sett ThisStackPush() for more informations.
        */
        void                 ThisStackPop();

        /// Updates this manager and its widgets.
        /** \param fDelta The time elapsed since the last call
        */
        void                 Update(const s_float& fDelta);

        /// Called whenever an Event occurs.
        /** \param mEvent The Event which has occured
        */
        void                 OnEvent(const Event& mEvent);

        /// Tells the underlying graphics engine to start rendering into a new target.
        /** \param pTarget The target to render to (nullptr to render to the screen)
        */
        void                 Begin(s_ptr<GUI::RenderTarget> pTarget = nullptr);

        /// Tells the underlying graphics engine we're done rendering.
        /** \note For most engines, this is when the rendering is actually
        *         done, so don't forget to call it !
        */
        void                 End();

        /// Registers a new Frame type.
        /** \param sClassName The name of the new type (as see in XML files)
        *   \note Use as template argument the C++ type of this Frame.
        */
        template<class T>
        void                 RegisterFrame(const s_str& sClassName)
        {
            lCustomFrameList_[sClassName] = &CreateNewFrame<T>;
            pLua_->Register<typename T::Glue>();
        }

        /// Registers a new LayeredRegion type.
        /** \param sClassName The name of the new type (as see in XML files)
        *   \note Use as template argument the C++ type of this LayeredRegion.
        */
        template<class T>
        void                 RegisterRegion(const s_str& sClassName)
        {
            lCustomRegionList_[sClassName] = &CreateNewLayeredRegion<T>;
            pLua_->Register<typename T::Glue>();
        }

        /// Returns the implementation dependent GUIManagerImpl.
        /** \return The implementation dependent GUIManagerImpl
        */
        s_wptr<const GUIManagerImpl> GetImpl() const;

        /// Returns the implementation dependent GUIManagerImpl.
        /** \return The implementation dependent GUIManagerImpl
        */
        s_wptr<GUIManagerImpl> GetImpl();

        static const s_str CLASS_NAME;

    protected :

        /// Copy constructor.
        GUIManager(const GUIManager& mMgr);

        /// Assignment operator.
        GUIManager& operator = (const GUIManager& mMgr);

    private :

        void LoadAddOnTOC_(const s_str& sAddOnName, const s_str& sAddOnDirectory);
        void LoadAddOnFiles_(s_ptr<AddOn> pAddOn);
        void LoadAddOnDirectory_(const s_str& sDirectory);

        void SaveVariables_(s_ptr<AddOn> pAddOn);

        void CreateStrataRenderTarget_(Strata& mStrata);
        void RenderStrata_(Strata& mStrata);

        void ParseXMLFile_(const s_str& sFile, s_ptr<AddOn> pAddOn);

        s_str  sGameVersion_;
        s_uint uiScreenWidth_;
        s_uint uiScreenHeight_;

        s_bool bClearFontsOnClose_;

        s_ptr<Lua::State> pLua_;
        s_bool            bClosed_;
        s_bool            bEnabled_;
        s_bool            bLoadingUI_;

        s_map< s_uint, s_map< s_uint, s_map<s_uint, s_str> > > lKeyBindingList_;

        s_map< s_str, s_ptr<GUI::UIObject> >  lNamedObjectList_;
        s_map< s_str, s_ptr<GUI::UIObject> >  lNamedVirtualObjectList_;

        s_map< s_uint, s_ptr<GUI::UIObject> > lObjectList_;
        s_map< s_uint, s_ptr<GUI::UIObject> > lMainObjectList_;

        s_ctnr<s_str>                       lGUIDirectoryList_;
        s_map< s_str, s_map<s_str, AddOn> > lAddOnList_;
        s_ptr<AddOn>                        pCurrentAddOn_;

        s_map< s_uint, s_ptr<GUI::Frame> > lFrameList_;
        s_map<FrameStrata, Strata>         lStrataList_;
        s_bool                             bBuildStrataList_;
        s_bool                             bObjectMoved_;
        s_ptr<GUI::Frame>                  pOveredFrame_;
        s_ptr<GUI::EditBox>                pFocusedEditBox_;

        s_ptr<GUI::UIObject> pMovedObject_;
        s_ptr<GUI::UIObject> pSizedObject_;
        s_float              fMouseMovementX_;
        s_float              fMouseMovementY_;

        s_ptr<GUI::Anchor>   pMovedAnchor_;
        s_int                iMovementStartPositionX_;
        s_int                iMovementStartPositionY_;
        Vector::Constraint   mConstraint_;

        s_uint               uiResizeStartW_;
        s_uint               uiResizeStartH_;
        s_bool               bResizeWidth_;
        s_bool               bResizeHeight_;
        s_bool               bResizeFromRight_;
        s_bool               bResizeFromBottom_;

        s_int iThisStackSize_;

        s_uint uiFrameNumber_;

        s_bool bEnableCaching_;

        s_ptr<GUI::RenderTarget> pRenderTarget_;
        s_refptr<GUI::Sprite>    pSprite_;

        s_map<s_str, s_ptr<GUI::Frame>(*)(s_ptr<GUIManager>)>         lCustomFrameList_;
        s_map<s_str, s_ptr<GUI::LayeredRegion>(*)(s_ptr<GUIManager>)> lCustomRegionList_;

        s_refptr<GUIManagerImpl> pImpl_;
    };

    class LuaGUIManager
    {
    public :

        LuaGUIManager(lua_State* luaVM);
        virtual ~LuaGUIManager();

        void SetManager(s_ptr<GUIManager> pMgr);
        s_ptr<GUIManager> GetManager();

        int GetDataTable(lua_State *L);

        static const char className[];
        static const char* classList[];
        static Lunar<LuaGUIManager>::RegType methods[];

    protected :

        lua_State* pLua_;
        int        iRef_;

        s_ptr<GUIManager> pMgr_;
    };
}


#endif
