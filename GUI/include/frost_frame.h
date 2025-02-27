/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_FRAME_H
#define FROST_GUI_FRAME_H

#include <frost_utils.h>
#include "frost_region.h"

namespace Frost
{
    namespace GUI
    {
        class Backdrop;

        /// Contains LayeredRegion
        struct Layer
        {
            s_bool                         bDisabled;
            s_ctnr< s_ptr<LayeredRegion> > lRegionList;

            static LayerType GetLayerType(const s_str& sLayer);

            static const s_str CLASS_NAME;
        };

        /// GUI container
        /** Can contain other Frames, or LayeredRegions
        *   (text, images, ...).
        */
        class Frame : public EventReceiver, public Region
        {
        public :

            /// Constructor.
            Frame(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~Frame();

            /// Renders this widget on the current render target.
            virtual void        Render();

            /// Updates this widget's logic.
            virtual void        Update();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            virtual s_str       Serialize(const s_str& sTab) const;

            /// Returns 'true' if this Frame can use a script.
            /** \param sScriptName The name of the script
            *   \note This method can be overriden if needed.
            */
            virtual s_bool      CanUseScript(const s_str& sScriptName) const;

            /// Checks if this Frame's position is valid.
            void                CheckPosition();

            /// Copies an UIObject's parameters into this Frame (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void        CopyFrom(s_ptr<UIObject> pObj);

            /// Creates a new title region for this Frame.
            /** \note You can get it by calling GetTitleRegion().
            */
            void                CreateTitleRegion();

            /// Disables a Layer.
            /** \param mLayerID The id of the layer to disable
            */
            void                DisableDrawLayer(LayerType mLayerID);

            /// Enables a Layer.
            /** \param mLayerID The id of the layer to enable
            */
            void                EnableDrawLayer(LayerType mLayerID);

            /// Sets if this Frame can receive keyboard input.
            /** \param bIsKeyboardEnabled 'true' to enable
            */
            void                EnableKeyboard(const s_bool& bIsKeyboardEnabled);

            /// Sets if this Frame can receive mouse input.
            /** \param bIsMouseEnabled 'true' to enable
            *   \param bAllowWorldInput 'true' to allow world input
            */
            virtual void        EnableMouse(const s_bool& bIsMouseEnabled, const s_bool& bAllowWorldInput = false);

            /// Sets if this Frame can receive mouse wheel input.
            /** \param bIsMouseWheelEnabled 'true' to enable
            */
            virtual void        EnableMouseWheel(const s_bool& bIsMouseWheelEnabled);

            /// Checks if this Frame has a script defined.
            /** \param sScriptName The name of the script to check
            *   \return 'true' if this script is defined
            */
            s_bool              HasScript(const s_str& sScriptName) const;

            /// Adds a LayeredRegion to this Frame's children.
            /** \param pRegion The LayeredRegion to add
            */
            void                AddRegion(s_ptr<LayeredRegion> pRegion);

            /// Removes a LayeredRegion from this Frame's children.
            /** \param pRegion The LayeredRegion to remove
            */
            void                RemoveRegion(s_ptr<LayeredRegion> pRegion);

            /// Creates a new region as child of this Frame.
            /** \param mLayer       The layer on which to create the region
            *   \param sClassName   The name of the region class ("FontString", ...)
            *   \param sName        The name of the region
            *   \param sInheritance The name of the region to inherit from
            *                       (empty if none)
            *   \return The created region.
            *   \note You don't have the reponsability to delete this region.
            *         It will be done automatically when the Frame is deleted.
            *   \note This function takes care of the basic initializing :
            *         you can directly use the created region.
            */
            s_ptr<LayeredRegion> CreateRegion(
                LayerType mLayer, const s_str& sClassName, const s_str& sName, const s_str& sInheritance = ""
            );

            /// Adds a Frame to this Frame's children.
            /** \param pChild The Frame to add
            */
            void                AddChild(s_ptr<Frame> pChild);

            /// Removes a Frame from this Frame's children.
            /** \param pChild The Frame to remove
            */
            void                RemoveChild(s_ptr<Frame> pChild);

            /// Returns the child list.
            /** \return The child list
            */
            const s_map< s_uint, s_ptr<Frame> >& GetChildren() const;

            /// Returns one of this Frame's children.
            /** \param sName The name of the child
            *   \return One of this Frame's children
            */
            s_ptr<Frame>        GetChild(const s_str& sName) const;

            /// Returns one of this Frame's region.
            /** \param sName The name of the region
            *   \return One of this Frame's region
            */
            s_ptr<LayeredRegion> GetRegion(const s_str& sName) const;

            /// Calculates effective alpha.
            /** \return Effective alpha (alpha*parent->alpha)
            */
            s_float             GetEffectiveAlpha() const;

            /// Calculates effective scale.
            /** \return Effective scale (scale*parent->scale)
            */
            s_float             GetEffectiveScale() const;

            /// Returns this Frame's level.
            /** \return This Frame's level
            */
            const s_int&        GetFrameLevel() const;

            /// Returns this Frame's strata.
            /** \return This Frame's strata
            */
            FrameStrata         GetFrameStrata() const;

            /// Returns this Frame's backdrop.
            /** \return This Frame's backdrop
            */
            s_wptr<Backdrop>    GetBackdrop() const;

            /// Returns this Frame's type.
            /** \return This Frame's type (Frame, Slider, ...)
            */
            const s_str&        GetFrameType() const;

            /// Returns this Frame's absolute hit rect insets.
            /** \return This Frame's absolute hit rect insets
            */
            const s_array<s_int,4>& GetAbsHitRectInsets() const;

            /// Returns this Frame's relative hit rect insets.
            /** \return This Frame's relative hit rect insets
            */
            const s_array<s_float,4>& GetRelHitRectInsets() const;

            /// Returns this Frame's max dimensions.
            /** \return This Frame's max dimensions
            */
            s_array<s_uint,2>   GetMaxResize() const;

            /// Returns this Frame's min dimensions.
            /** \return This Frame's min dimensions
            */
            s_array<s_uint,2>   GetMinResize() const;

            /// Returns the number of children of this Frame.
            /** \return The number of children of this Frame
            */
            s_uint              GetNumChildren() const;

            /// Returns the number of region of this Frame.
            /** \return The number of region of this Frame
            */
            s_uint              GetNumRegions() const;

            /// Returns this Frame's scale.
            /** \return This Frame's scale
            *   \note If you want it's true scale on the screen,
            *         use GetEffectiveScale().
            */
            const s_float&      GetScale() const;

            /// Returns this Frame's title region.
            s_ptr<Region>       GetTitleRegion() const;

            /// Checks if this Frame is clamped to screen.
            /** \return 'true' if this Frame is clamed to screen
            */
            const s_bool&       IsClampedToScreen() const;

            /// Checks if the provided coordinates are in the Frame.
            /** \param iX           The horizontal coordinate
            *   \param iY           The vertical coordinate
            *   \return 'true' if the provided coordinates are in the Frame
            */
            virtual s_bool      IsInFrame(const s_int& iX, const s_int& iY) const;

            /// Checks if this Frame can receive keyboard input.
            /** \return 'true' if this Frame can receive keyboard input
            */
            const s_bool&       IsKeyboardEnabled() const;

            /// Checks if this Frame can receive mouse input.
            /** \return 'true' if this Frame can receive mouse input
            */
            const s_bool&       IsMouseEnabled() const;

            /// Checks if this Frame allows world input.
            /** \return 'true' if this Frame allows world input
            */
            const s_bool&       IsWorldInputAllowed() const;

            /// Checks if this Frame can receive mouse wheel input.
            /** \return 'true' if this Frame can receive mouse wheel input
            */
            const s_bool&       IsMouseWheelEnabled() const;

            /// Checks if this Frame can be moved.
            /** \return 'true' if this Frame can be moved
            */
            const s_bool&       IsMovable() const;

            /// Checks if this Frame can be resized.
            /** \return 'true' if this Frame can be resized
            */
            const s_bool&       IsResizable() const;

            /// Checks if this Frame is at top level.
            /** \return 'true' if this Frame is at top level
            */
            const s_bool&       IsTopLevel() const;

            /// Checks if this Frame has been moved by the user.
            /** \return 'true' if this Frame has been moved by the user
            */
            const s_bool&       IsUserPlaced() const;

            /// Registers a handler script to this Frame.
            /** \param sScriptName The name of the script
            *   \param sContent    The content ot the script
            */
            void                DefineScript(const s_str& sScriptName, const s_str& sContent);

            /// Tells this Frame that a script has been defined.
            /** \param sScriptName The name of the script
            *   \param bDefined    'true' if the script is defined
            */
            void                NotifyScriptDefined(const s_str& sScriptName, const s_bool& bDefined);

            /// Calls a script.
            /** \param sScriptName The name of the script
            *   \param pEvent      Stores scripts arguments
            */
            virtual void        On(const s_str& sScriptName, s_ptr<Event> pEvent = nullptr);

            /// Calls the OnEvent script.
            /** \param mEvent The Event that occured
            */
            virtual void        OnEvent(const Event& mEvent);

            /// Tells this Frame to react to every event in the game.
            void                RegisterAllEvents();

            /// Tells this Frame to react to a certain event.
            /** \param sEventName The name of the event
            */
            void                RegisterEvent(const s_str& sEventName);

            /// Tells this Frame to react to mouse drag.
            /** \param lButtonList The list of mouse button allowed
            */
            void                RegisterForDrag(const s_ctnr<s_str>& lButtonList);

            /// Sets if this Frame is clamped to screen.
            /** \param bIsClampedToScreen 'true' if this Frame is clamped to screen
            *   \note If 'true', the Frame can't go out of the screen.
            */
            void                SetClampedToScreen(const s_bool& bIsClampedToScreen);

            /// Sets this Frame's strata.
            /** \param mStrata The new strata
            */
            void                SetFrameStrata(FrameStrata mStrata);

            /// Sets this Frame's strata.
            /** \param sStrata The new strata
            */
            void                SetFrameStrata(const s_str& sStrata);

            /// Sets this Frames' backdrop.
            /** \param pBackdrop The new backdrop
            */
            void                SetBackdrop(s_refptr<Backdrop> pBackdrop);

            /// Sets this Frame's absolute hit rect insets.
            /** \param iLeft   Offset from the left border
            *   \param iRight  Offset from the right border
            *   \param iTop    Offset from the top border
            *   \param iBottom Offset from the bottom border
            *   \note This is the zone on which you can click.
            */
            void                SetAbsHitRectInsets(const s_int& iLeft, const s_int& iRight, const s_int& iTop, const s_int& iBottom);

            /// Sets this Frame's relative hit rect insets.
            /** \param fLeft   Offset from the left border
            *   \param fRight  Offset from the right border
            *   \param fTop    Offset from the top border
            *   \param fBottom Offset from the bottom border
            *   \note This is the zone on which you can click.
            */
            void                SetRelHitRectInsets(const s_float& fLeft, const s_float& fRight, const s_float& fTop, const s_float& fBottom);

            /// Sets this Frame's level.
            /** \param iLevel The new level
            */
            void                SetLevel(const s_int& iLevel);

            /// Set this Frame's maximum size.
            /** \param uiMaxWidth  The maximum width this Frame can have
            *   \param uiMaxHeight The maximum height this Frame can have
            */
            void                SetMaxResize(const s_uint& uiMaxWidth, const s_uint& uiMaxHeight);

            /// Set this Frame's minimum size.
            /** \param uiMinWidth  The minimum width this Frame can have
            *   \param uiMinHeight The minimum height this Frame can have
            */
            void                SetMinResize(const s_uint& uiMinWidth, const s_uint& uiMinHeight);

            /// Set this Frame's maximum height.
            /** \param uiMaxHeight The maximum height this Frame can have
            */
            void                SetMaxHeight(const s_uint& uiMaxHeight);

            /// Set this Frame's maximum width.
            /** \param uiMaxWidth  The maximum width this Frame can have
            */
            void                SetMaxWidth(const s_uint& uiMaxWidth);

            /// Set this Frame's minimum height.
            /** \param uiMinHeight The minimum height this Frame can have
            */
            void                SetMinHeight(const s_uint& uiMinHeight);

            /// Set this Frame's minimum width.
            /** \param uiMinWidth  The minimum width this Frame can have
            */
            void                SetMinWidth(const s_uint& uiMinWidth);

            /// Sets if this Frame can be moved by the user.
            /** \param bIsMovable 'true' to allow the user to move this Frame
            */
            void                SetMovable(const s_bool& bIsMovable);

            /// Changes this widget's parent.
            /** \param pParent The new parent
            *   \note Default is nullptr.<br>
            *         Overrides UIObject's implementation.
            */
            void                SetParent(s_ptr<UIObject> pParent);

            /// Sets if this Frame can be resized by the user.
            /** \param bIsResizable 'true' to allow the user to resize this Frame
            */
            void                SetResizable(const s_bool& bIsResizable);

            /// Sets this Frame's scale.
            /** \param fScale The new scale
            */
            void                SetScale(const s_float& fScale);

            /// Sets if this Frame is at top level.
            /** \param bIsTopLevel 'true' to put the Frame at top level
            */
            void                SetTopLevel(const s_bool& bIsTopLevel);

            /// Decreases this Frame's level so it's the lowest of the strata.
            /** \note All its children are lowered of the same ammount.
            *   \note Only works for top level frames.
            */
            void                Lower();

            /// Increases this Frame's level so it's the highest of the strata.
            /** \note All its children are raised of the same ammount.
            *   \note Only works for top level frames.
            */
            void                Raise();

            /// Sets if this Frame has been moved by the user.
            /** \param bIsUserPlaced 'true' if this Frame has been moved by the user
            */
            void                SetUserPlaced(const s_bool& bIsUserPlaced);

            /// Starts moving this Frame with the mouse.
            void                StartMoving();

            /// Ends moving this Frame.
            void                StopMoving();

            /// Starts resizing this Frame with the mouse.
            /** \param mPoint The corner to move
            */
            void                StartSizing(const AnchorPoint& mPoint);

            /// Ends resizing this Frame.
            void                StopSizing();

            /// Shows this widget.
            /** \note Its parent must be shown for it to appear on
            *         the screen.
            */
            virtual void        Show();

            /// Hides this widget.
            /** \note All its childs won't be visible on the screen
            *         anymore, even if they are still marked as shown.
            */
            virtual void        Hide();

            /// Shows/hides this widget.
            /** \param bIsShown 'true' if you want to show this widget
            *   \note See Show() and Hide() for more infos.
            *   \note Contrary to Show() and Hide(), this function doesn't
            *         trigger any event ("OnShow" or "OnHide"). It should
            *         only be used to set the initial state of the widget.
            */
            virtual void    SetShown(const s_bool& bIsShown);

            /// Flags this object as "manually rendered".
            /** \param bManuallyRendered 'true' to flag it as manually rendered
            *   \param pRenderer         The UIObject that will take care of
            *                            rendering this widget
            *   \note Manually rendered objects are not automatically rendered
            *         by their parent (for LayeredRegions) or the GUIManager
            *         (for Frames). They also don't receive automatic input.
            *   \note This function propagates the manually rendered flag to
            *         this Frame's children.
            */
            virtual void        SetManuallyRendered(const s_bool& bManuallyRendered, s_ptr<UIObject> pRenderer = nullptr);

            /// Changes this widget's absolute width (in pixels).
            /** \param uiAbsWidth The new width
            */
            virtual void        SetAbsWidth(const s_uint& uiAbsWidth);

            /// Changes this widget's absolute height (in pixels).
            /** \param uiAbsHeight The new height
            */
            virtual void        SetAbsHeight(const s_uint& uiAbsHeight);

            /// Tells this Frame it is being overed by the mouse.
            /** \param bMouseInFrame 'true' if the mouse is above this Frame
            *   \param iX            The horizontal mouse coordinate
            *   \param iY            The vertical mouse coordinate
            *   \note Always use the mouse position set by this function and
            *         not the one returned by the InputManager, because there
            *         can be an offset to apply (for example with ScrollFrame).
            */
            virtual void        NotifyMouseInFrame(const s_bool& bMouseInFrame, const s_int& iX, const s_int& iY);

            /// Tells this Frame that at least one of its children has modified its strata or level.
            /** \param pChild The child that has changed its strata (can also be a child of this child)
            *   \note If this Frame has no parent, it calls GUIManager::FireBuildStrataList(). Else it
            *         notifies its parent.
            */
            virtual void        NotifyChildStrataChanged(s_ptr<Frame> pChild);

            /// Notifies the renderer of this widget that it needs to be redrawn.
            /** \note Automatically called by any shape changing function.
            */
            virtual void        NotifyRendererNeedRedraw() const;

            /// Tells this Frame to rebuilt the layer list.
            /** \note Automatically called by AddRegion(), RemoveRegion(), and
            *         LayeredRegion::SetDrawLayer().
            */
            void                FireBuildLayerList();

            /// Tells the Frame not to react to all events.
            void                UnregisterAllEvents();

            /// Tells the Frame not to react to a certain event.
            /** \param sEventName The name of the event
            */
            void                UnregisterEvent(const s_str& sEventName);

            /// Sets the addon this Frame belongs to.
            /** \param pAddOn The addon this Frame belongs to
            */
            void                SetAddOn(s_ptr<AddOn> pAddOn);

            /// Returns this Frame's addon.
            /** \return This Frame's addon
            *   \note Returns "nullptr" if the Frame has been created
            *         by Lua code and wasn't assigned a parent.
            */
            s_ptr<AddOn>        GetAddOn() const;

            /// Removes all anchors that point to this widget and all other kind of links.
            /** \return The list of all widgets that have been cleared
            *   \note Also clears children objects (see Frame::ClearLinks()).
            *   \note Must be called before deleting the widget, except when closing the whole UI.
            */
            virtual s_ctnr< s_ptr<UIObject> > ClearLinks();

            /// Creates the associated Lua glue.
            virtual void        CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The Frame's XML::Block
            */
            virtual void        ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            // XML parsing
            virtual void ParseAttributes_(s_ptr<XML::Block> pBlock);
            virtual void ParseResizeBoundsBlock_(s_ptr<XML::Block> pBlock);
            virtual void ParseTitleRegionBlock_(s_ptr<XML::Block> pBlock);
            virtual void ParseBackdropBlock_(s_ptr<XML::Block> pBlock);
            virtual void ParseHitRectInsetsBlock_(s_ptr<XML::Block> pBlock);
            virtual void ParseLayersBlock_(s_ptr<XML::Block> pBlock);
            virtual void ParseFramesBlock_(s_ptr<XML::Block> pBlock);
            virtual void ParseScriptsBlock_(s_ptr<XML::Block> pBlock);

            void NotifyVisible_(const s_bool& bTriggerEvents = true);
            void NotifyInvisible_(const s_bool& bTriggerEvents = true);
            void NotifyStrataChanged_();

            void NotifyTopLevelParent_(const s_bool& bTopLevel, s_ptr<Frame> pParent);

            void AddLevel_(const s_int& iAmount);

            s_map< s_uint, s_ptr<Frame> >         lChildList_;
            s_map< s_uint, s_ptr<LayeredRegion> > lRegionList_;
            s_map<LayerType, Layer>               lLayerList_;
            s_map<s_str, s_str>                   lDefinedScriptList_;
            s_ctnr<s_str>                         lQueuedEventList_;
            s_map<s_str, s_bool>                  lRegEventList_;
            s_map<s_str, s_bool>                  lRegDragList_;

            s_ptr<AddOn> pAddOn_;

            s_int iLevel_;

            FrameStrata  mStrata_;
            s_bool       bIsTopLevel_;
            s_ptr<Frame> pTopLevelParent_;

            s_refptr<Backdrop> pBackdrop_;

            s_bool bHasAllEventsRegistred_;

            s_bool bIsKeyboardEnabled_;
            s_bool bIsMouseEnabled_;
            s_bool bAllowWorldInput_;
            s_bool bIsMouseWheelEnabled_;
            s_bool bIsMovable_;
            s_bool bIsClampedToScreen_;
            s_bool bIsResizable_;
            s_bool bIsUserPlaced_;

            s_bool bBuildLayerList_;

            s_array<s_int,  4> lAbsHitRectInsetList_;
            s_array<s_float,4> lRelHitRectInsetList_;

            s_uint uiMinWidth_;
            s_uint uiMaxWidth_;
            s_uint uiMinHeight_;
            s_uint uiMaxHeight_;

            s_float fScale_;

            s_bool bMouseInFrame_;
            s_bool bMouseInTitleRegion_;
            s_int  iMousePosX_, iMousePosY_;

            s_ptr<Region> pTitleRegion_;

            s_ptr<Frame> pParentFrame_;

            s_ctnr<s_str> lMouseButtonList_;
            s_bool        bMouseDragged_;
        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        /// Frame Lua glue
        class LuaFrame : public LuaUIObject
        {
        public :

            /// Constructor.
            LuaFrame(lua_State* pLua);

            int _CreateFontString(lua_State*);
            int _CreateTexture(lua_State*);
            int _CreateTitleRegion(lua_State*);
            int _DisableDrawLayer(lua_State*);
            int _EnableDrawLayer(lua_State*);
            int _EnableKeyboard(lua_State*);
            int _EnableMouse(lua_State*);
            int _EnableMouseWheel(lua_State*);
            int _GetBackdrop(lua_State*);
            int _GetBackdropBorderColor(lua_State*);
            int _GetBackdropColor(lua_State*);
            int _GetChildren(lua_State*);
            int _GetEffectiveAlpha(lua_State*);
            int _GetEffectiveScale(lua_State*);
            int _GetFrameLevel(lua_State*);
            int _GetFrameStrata(lua_State*);
            int _GetFrameType(lua_State*);
            int _GetHitRectInsets(lua_State*);
            int _GetID(lua_State*);
            int _GetMaxResize(lua_State*);
            int _GetMinResize(lua_State*);
            int _SetMaxWidth(lua_State*);
            int _SetMaxHeight(lua_State*);
            int _SetMinWidth(lua_State*);
            int _SetMinHeight(lua_State*);
            int _GetNumChildren(lua_State*);
            int _GetNumRegions(lua_State*);
            int _GetScale(lua_State*);
            int _GetScript(lua_State*);
            int _GetTitleRegion(lua_State*);
            int _HasScript(lua_State*);
            int _HookScript(lua_State*) { return 0; } // WBI
            int _IsClampedToScreen(lua_State*);
            int _IsFrameType(lua_State*);
            int _IsKeyboardEnabled(lua_State*);
            int _IsMouseEnabled(lua_State*);
            int _IsMouseWheelEnabled(lua_State*);
            int _IsMovable(lua_State*);
            int _IsResizable(lua_State*);
            int _IsTopLevel(lua_State*);
            int _IsUserPlaced(lua_State*);
            int _Lower(lua_State*);
            int _On(lua_State*);
            int _Raise(lua_State*);
            int _RegisterAllEvents(lua_State*);
            int _RegisterEvent(lua_State*);
            int _RegisterForDrag(lua_State*);
            int _SetBackdrop(lua_State*);
            int _SetBackdropBorderColor(lua_State*);
            int _SetBackdropColor(lua_State*);
            int _SetClampedToScreen(lua_State*);
            int _SetFrameLevel(lua_State*);
            int _SetFrameStrata(lua_State*);
            int _SetHitRectInsets(lua_State*);
            int _SetID(lua_State*) { return 0; } // WBI
            int _SetMaxResize(lua_State*);
            int _SetMinResize(lua_State*);
            int _SetMovable(lua_State*);
            int _SetResizable(lua_State*);
            int _SetScale(lua_State*);
            int _SetScript(lua_State*);
            int _SetTopLevel(lua_State*);
            int _SetUserPlaced(lua_State*);
            int _StartMoving(lua_State*);
            int _StartSizing(lua_State*);
            int _StopMovingOrSizing(lua_State*);
            int _UnregisterAllEvents(lua_State*);
            int _UnregisterEvent(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaFrame>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_ptr<Frame> pFrameParent_;
        };

        /** \endcond
        */
    }
}

#endif
