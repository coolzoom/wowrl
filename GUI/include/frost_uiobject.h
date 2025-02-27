/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_UIOBJECT_H
#define FROST_GUI_UIOBJECT_H

#include <frost_utils.h>
#include "frost_anchor.h"
#include "frost_guimanager.h"
#include <frost_utils_lua.h>
#include <frost_utils_point.h>
#include <frost_xml.h>

namespace Frost
{
    namespace GUI
    {
        class LuaGlue;

        enum BorderType
        {
            BORDER_LEFT   = 0,
            BORDER_RIGHT  = 1,
            BORDER_TOP    = 2,
            BORDER_BOTTOM = 3
        };

        enum LayerType
        {
            LAYER_BACKGROUND,
            LAYER_BORDER,
            LAYER_ARTWORK,
            LAYER_OVERLAY,
            LAYER_HIGHLIGHT,
            LAYER_SPECIALHIGH
        };

        /// The base of the GUI
        /** This widget (GUI element) is a virtual base.
        *   It doesn't display anything on its own and must
        *   be inherited to allow new features. It provides
        *   several virtual functions : Update, Render...
        *   They should be overriden and contain all the
        *   required logic for it to work.<br>
        *   Look at the provided widgets (Frame, Slider, ...)
        *   to see how it's done.
        */
        class UIObject
        {
        public :

            /// Contructor.
            UIObject(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~UIObject();

            /// Renders this widget on the current render target.
            virtual void    Render() = 0;

            /// Updates this widget's anchors.
            /** \note Must be called on *all* widgets before Update().
            */
            virtual void    UpdateAnchors();

            /// Updates this widget's logic.
            virtual void    Update();

            /// Prints all relevant information about this widget in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this widget
            */
            virtual s_str   Serialize(const s_str& sTab) const;

            /// Copies an UIObject's parameters into this UIObject (inheritance).
            /** \param pObj The UIObject to copy
            */
            virtual void    CopyFrom(s_ptr<UIObject> pObj);

            /// Tells this widget to update its borders.
            virtual void    FireUpdateBorders() const;

            /// Tells this widget to update its dimensions.
            void            FireUpdateDimensions() const;

            /// Returns this widget's name.
            /** \return This widget's name
            */
            const s_str&    GetName() const;

            /// Returns this widget's Lua name.
            /** \return This widget's Lua name
            */
            const s_str&    GetLuaName() const;

            /// Returns this widget's raw name.
            /** \return This widget's raw name
            *   \note This is the name of the widget before "$parent"
            *         has been replaced by its parent's name.
            */
            const s_str&    GetRawName() const;

            /// Sets this widget's name.
            /** \param sName This widget's name
            *   \note Can only be called once.
            */
            void            SetName(const s_str& sName);

            /// Changes this widget's parent.
            /** \param pParent The new parent
            *   \note Default is nullptr.
            */
            virtual void    SetParent(s_ptr<UIObject> pParent);

            /// Returns this widget's parent.
            /** \return This widget's parent
            */
            s_ptr<const UIObject> GetParent() const;

            /// Returns this widget's parent.
            /** \return This widget's parent
            */
            s_ptr<UIObject> GetParent();

            /// Returns the widget this one inherits from.
            /** \return The widget this one inherits from
            */
            s_ptr<UIObject> GetBase();

            /// Changes this widget's alpha (opacity).
            /** \param fAlpha The new alpha value
            *   \note Default is 1.0f.
            */
            void            SetAlpha(const s_float& fAlpha);

            /// Returns this widget's alpha (opacity).
            /** \return This widget's alpha (opacity).
            */
            const s_float&  GetAlpha() const;

            /// Shows this widget.
            /** \note Its parent must be shown for it to appear on
            *         the screen.
            */
            virtual void    Show();

            /// Hides this widget.
            /** \note All its childs won't be visible on the screen
            *         anymore, even if they are still marked as shown.
            */
            virtual void    Hide();

            /// Shows/hides this widget.
            /** \param bIsShown 'true' if you want to show this widget
            *   \note See Show() and Hide() for more infos.
            *   \note Contrary to Show() and Hide(), this function doesn't
            *         trigger any event ("OnShow" or "OnHide"). It should
            *         only be used to set the initial state of the widget.
            */
            virtual void    SetShown(const s_bool& bIsShown);

            /// Checks if this widget is shown.
            /** \return 'true' if this widget is shown
            */
            const s_bool&   IsShown() const;

            /// Checks if this widget can be seen on the screen.
            /** \return 'true' if this widget can be seen on the screen
            */
            virtual s_bool  IsVisible() const;

            /// Changes this widget's absolute width (in pixels).
            /** \param uiAbsWidth The new width
            */
            virtual void    SetAbsWidth(const s_uint& uiAbsWidth);

            /// Changes this widget's absolute height (in pixels).
            /** \param uiAbsHeight The new height
            */
            virtual void    SetAbsHeight(const s_uint& uiAbsHeight);

            /// Checks if this widget's width has been defined as absolute.
            /** \return 'true' if this widget's width has been defined as absolute
            */
            const s_bool&   IsWidthAbsolute() const;

            /// Checks if this widget's height has been defined as absolute.
            /** \return 'true' if this widget's height has been defined as absolute
            */
            const s_bool&   IsHeightAbsolute() const;

            /// Changes this widget's width (relative to its parent).
            /** \param fRelWidth The new width
            *   \note Width will automatically be adjusted to include
            *         all of this widget's childs.
            */
            void            SetRelWidth(const s_float& fRelWidth);

            /// Changes this widget's height (relative to its parent).
            /** \param fRelHeight The new height
            *   \note Height will automatically be adjusted to include
            *         all of this widget's childs.
            */
            void            SetRelHeight(const s_float& fRelHeight);

            /// Returns this widget's absolute width (in pixels).
            /** \return This widget's absolute width (in pixels)
            *   \note If you need to get the size of a widget on the screen,
            *         use GetAppearentWidth() instead, because some
            *         widgets can have an infinite or undefined width.
            */
            const s_uint&   GetAbsWidth() const;

            /// Returns this widget's appearend width (in pixels).
            /** \return This widget's appearend width (in pixels)
            *   \note If you need to get the size of a widget on the screen,
            *         use this function instead of GetAbsWidth(), because
            *         some widgets can have an infinite or undefined width.
            */
            s_uint          GetAppearentWidth() const;

            /// Returns this widget's absolute height (in pixels).
            /** \return This widget's absolute height (in pixels)
            *   \note If you need to get the size of a widget on the screen,
            *         use GetAppearentHeight() instead, because some
            *         widgets can have an infinite or undefined height.
            */
            const s_uint&   GetAbsHeight() const;

            /// Returns this widget's appearend height (in pixels).
            /** \return This widget's appearend height (in pixels)
            *   \note If you need to get the size of a widget on the screen,
            *         use this function instead of GetAbsHeight(), because
            *         some widgets can have an infinite or undefined height.
            */
            s_uint          GetAppearentHeight() const;

            /// Returns this widget's width (relative to its parent).
            /** \return This widget's width (relative to its parent)
            */
            const s_float&  GetRelWidth() const;

            /// Returns this widget's height (relative to its parent).
            /** \return This widget's height (relative to its parent)
            */
            const s_float&  GetRelHeight() const;

            /// Returns the type of this widget.
            /** \return The type of this widget
            */
            const s_str&    GetObjectType() const;

            /// Checks if this widget is of the provided type.
            /** \param sType The type to test
            *   \return 'true' if this widget is of the provided type
            */
            s_bool          IsObjectType(const s_str& sType) const;

            /// Returns an array containing all the types of this widget.
            /** \return  An array containing all the types of this widget
            */
            const s_ctnr<s_str>& GetObjectTypeList() const;

            /// Returns the vertical position of this widget's bottom border.
            /** \return The vertical position of this widget's bottom border
            */
            const s_int&    GetBottom() const;

            /// Returns the position of this widget's center.
            /** \return The position of this widget's center
            */
            Point<s_int>    GetCenter() const;

            /// Returns the horizontal position of this widget's left border.
            /** \return The horizontal position of this widget's left border
            */
            const s_int&    GetLeft() const;

            /// Returns the horizontal position of this widget's right border.
            /** \return The horizontal position of this widget's right border
            */
            const s_int&    GetRight() const;

            /// Returns the vertical position of this widget's top border.
            /** \return The vertical position of this widget's top border
            */
            const s_int&    GetTop() const;

            /// Returns this widget's borders.
            /** \return This widget's borders
            */
            const s_array<s_int,4> GetBorders() const;

            /// Removes all anchors.
            /** \note This widget and its childs won't be visible until you
            *         define at least one anchor.
            */
            void            ClearAllPoints();

            /// Adjusts this widgets anchors to fit the provided widget.
            /** \param pObj A pointer to the object you want to wrap
            *   \note Removes all anchors and defines two new ones.
            */
            void            SetAllPoints(s_ptr<UIObject> pObj);

            /// Adjusts this widgets anchors to fit the provided widget.
            /** \param sObjName The name of the object to fit to
            *   \note Removes all anchors and defines two new ones.<br>
            *         This version is to be used by virtual widgets to
            *         preserve the anchor hierarchy.
            */
            void            SetAllPoints(const s_str& sObjName);

            /// Creates/modifies an anchor.
            /** \param mPoint         The anchor point for this object
            *   \param sParentName    The Anchor's parent
            *   \param mRelativePoint The anchor point for the parent
            *   \param iX             The horizontal offset
            *   \param iY             The vertical offset
            */
            void            SetAbsPoint(AnchorPoint mPoint,
                                        const s_str& sParentName,
                                        AnchorPoint mRelativePoint,
                                        const s_int& iX,
                                        const s_int& iY);

            /// Creates/modifies an anchor.
            /** \param mPoint         The anchor point for this object
            *   \param sParentName    The Anchor's parent
            *   \param mRelativePoint The anchor point for the parent
            *   \param fX             The horizontal offset
            *   \param fY             The vertical offset
            */
            void            SetRelPoint(AnchorPoint mPoint,
                                        const s_str& sParentName,
                                        AnchorPoint mRelativePoint,
                                        const s_float& fX,
                                        const s_float& fY);

            /// Adds/replaces an anchor.
            /** \param mAnchor The Anchor to add
            */
            void            SetPoint(const Anchor& mAnchor);

            /// Checks if this widget depends on another.
            /** \param pObj The widget to test
            *   \note Usefull to detect circular refences.
            */
            s_bool          DependsOn(s_ptr<UIObject> pObj) const;

            /// Returns the number of defined anchors.
            /** \return The number of defined anchors
            */
            s_uint          GetNumPoint() const;

            /// Returns one of this widget's Anchor to modify it.
            /** \param mPoint The anchor point
            *   \return A pointer to the anchor, nullptr if none
            */
            s_ptr<Anchor>   ModifyPoint(AnchorPoint mPoint);

            /// Returns one of this widget's Anchor.
            /** \param mPoint The anchor point
            *   \return A pointer to the anchor, nullptr if none
            */
            s_ptr<const Anchor> GetPoint(AnchorPoint mPoint) const;

            /// Returns all of this widgets's anchors.
            /** \return All of this widgets's anchors
            */
            const s_map<AnchorPoint, Anchor>& GetPointList() const;

            /// Notifies this widget that another one is anchored to it.
            /** \param pObj      The anchored widget
            *   \param bAnchored 'true' if it is anchored, 'false' if it's no longer the case
            */
            void            NotifyAnchoredObject(s_ptr<UIObject> pObj, const s_bool& bAnchored) const;

            /// Checks if this UIObject is virtual.
            /** \return 'true' if this UIObject is virtual
            *   \note A virtual UIObject can be inherited.
            */
            const s_bool&   IsVirtual() const;

            /// Makes this UIObject virtual.
            /** \note See IsVirtual().
            */
            void            SetVirtual();

            /// Returns this widget's unique ID.
            /** \return This widget's unique ID
            */
            const s_uint&   GetID() const;

            /// Sets this widget's unique ID.
            /** \param uiID The ID
            *   \note Can only be called once.
            */
            void            SetID(const s_uint& uiID);

            /// Flags this object as "special".
            /** \note Special objects are not automatically copied
            *         in the Frame inheritance process. They must be
            *         explicitely copied by the derived class
            *         (example : Button will have to copy its button
            *         textures itself).
            */
            void            SetSpecial();

            /// Checks if this object is special.
            /** \return 'true' if this objet is special
            *   \note For more informations, see SetSpecial().
            */
            const s_bool&   IsSpecial() const;

            /// Flags this object as "manually rendered".
            /** \param bManuallyRendered 'true' to flag it as manually rendered
            *   \param pRenderer         The UIObject that will take care of
            *                            rendering this widget
            *   \note Manually rendered objects are not automatically rendered
            *         by their parent (for LayeredRegions) or the GUIManager
            *         (for Frames). They also don't receive automatic input.
            */
            virtual void    SetManuallyRendered(const s_bool& bManuallyRendered, s_ptr<UIObject> pRenderer = nullptr);

            /// Checks if this object is manually rendered.
            /** \return 'true' if this object is manually rendered
            *   \note For more informations, see SetManuallyRendered().
            */
            const s_bool&   IsManuallyRendered() const;

            /// Flags this object as newly created.
            /** \note Newly created objects aren't rendered.
            *         They unflag themselves after the first Update() call.
            *   \note This function is only called on objects created in Lua.
            */
            void            SetNewlyCreated();

            /// Checks if this object has been newly created.
            /** \return 'true' if this object has been newly created
            *   \note For more informations, see SetNewlyCreated().
            */
            const s_bool&   IsNewlyCreated() const;

            /// Notifies the renderer of this widget that it needs to be redrawn.
            /** \note Automatically called by any shape changing function.
            */
            virtual void    NotifyRendererNeedRedraw() const;

            /// Tells this widget that a manually rendered widget requires redraw.
            /** \note This function does nothing by default.
            */
            virtual void    FireRedraw() const;

            /// Adds a Lua variable to copy when derivating.
            /** \param sVariable The name of the variable
            *   \note The variable must be an element of the widget's Lua glue.<br>
            *         If you have a widget called "Test", and in some lua code you do :<br>
            *         Test.someVariable = 2;<br>
            *         ... then you can call this function with "someVariable".
            */
            void            MarkForCopy(const s_str& sVariable);

            /// Removes all anchors that point to this widget and all other kind of links.
            /** \return The list of all widgets that have been cleared
            *   \note Also clears children objects (see Frame::ClearLinks()).
            *   \note Must be called before deleting the widget, except when closing the whole UI.
            */
            virtual s_ctnr< s_ptr<UIObject> > ClearLinks();

            /// Notifies this widget that it has been fully loaded.
            void            NotifyLoaded();

            /// Returns this widget's GUIManager.
            /** \return This widget's GUIManager
            */
            s_ptr<GUIManager> GetManager();

            /// Returns this widget's GUIManager.
            /** \return This widget's GUIManager
            */
            s_ptr<const GUIManager> GetManager() const;

            /// Creates the associated Lua glue.
            /** \note This method is pure virtual : it must be overriden.
            */
            virtual void    CreateGlue() = 0;

            /// Removes the Lua glue.
            void            RemoveGlue();

            /// Pushes this UIObject on the provided Lua::State.
            /** \param pLua The Lua::State on which to push the glue
            */
            virtual void    PushOnLua(s_ptr<Lua::State> pLua) const;

            /// Parses data from an XML::Block.
            /** \param pBlock The UIObject's XML::Block
            */
            virtual void    ParseBlock(s_ptr<XML::Block> pBlock) = 0;

            static const s_str CLASS_NAME;

        protected :

            // XML parsing
            virtual void ParseSizeBlock_(s_ptr<XML::Block> pBlock);
            virtual void ParseAnchorsBlock_(s_ptr<XML::Block> pBlock);

            void         ReadAnchors_(s_int& iLeft, s_int& iRight, s_int& iTop, s_int& iBottom, s_int& iXCenter, s_int& iYCenter) const;
            void         MakeBorders_(s_int& iMin, s_int& iMax, const s_int& iCenter, const s_int& iSize) const;
            virtual void UpdateBorders_() const;
            virtual void UpdateDimensions_() const;

            virtual void NotifyManuallyRenderedObject_(s_ptr<UIObject> pObject, const s_bool& bManuallyRendered);

            s_ptr<GUIManager> pManager_;

            s_str           sName_;
            s_str           sRawName_;
            s_str           sLuaName_;
            s_uint          uiID_;
            s_ptr<UIObject> pParent_;
            s_ptr<UIObject> pInheritance_;
            s_bool          bSpecial_;
            s_bool          bManuallyRendered_;
            s_bool          bNewlyCreated_;
            s_ptr<UIObject> pRenderer_;
            s_bool          bInherits_;

            s_bool          bVirtual_;
            s_bool          bLoaded_;
            mutable s_bool  bReady_;

            s_ctnr< s_ptr<LuaGlue> > lGlueList_;
            s_ctnr<s_str>            lCopyList_;

            s_ctnr<s_str> lType_;

            s_map<AnchorPoint, Anchor>      lAnchorList_;
            s_ctnr< s_ptr<const UIObject> > lPreviousAnchorParentList_;
            s_array<s_bool, 4>              lDefinedBorderList_;
            mutable s_array<s_int, 4>       lBorderList_;

            s_float fAlpha_;
            s_bool  bIsShown_;
            s_bool  bIsVisible_;

            mutable s_bool  bIsWidthAbs_;
            mutable s_bool  bIsHeightAbs_;
            mutable s_uint  uiAbsWidth_;
            mutable s_uint  uiAbsHeight_;
            mutable s_float fRelWidth_;
            mutable s_float fRelHeight_;

            mutable s_bool bUpdateAnchors_;
            mutable s_bool bUpdateBorders_;
            mutable s_bool bUpdateDimensions_;

            mutable s_map< s_uint, s_ptr<UIObject> > lAnchoredObjectList_;
        };

        /** \cond NOT_REMOVE_FROM_DOC
        */

        // Generic Lua glue
        class LuaGlue
        {
        public :

            LuaGlue(lua_State* luaVM);
            virtual ~LuaGlue();

            virtual void NotifyDeleted() = 0;

            int GetDataTable(lua_State *L);

        protected :

            lua_State* pLua_;
            int        iRef_;
        };

        // Virtual widget Lua glue
        class LuaVirtualGlue : public LuaGlue
        {
        public :

            LuaVirtualGlue(lua_State* luaVM);
            virtual ~LuaVirtualGlue();

            virtual void NotifyDeleted();

            int _MarkForCopy(lua_State*);
            int _GetBase(lua_State*);
            int _GetName(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaVirtualGlue>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_uint          uiID_;
            s_ptr<UIObject> pParent_;
        };

        // UIObject Lua glue
        class LuaUIObject : public LuaGlue
        {
        public :

            LuaUIObject(lua_State* luaVM);
            virtual ~LuaUIObject();

            virtual void NotifyDeleted();

            s_ptr<UIObject> GetParent();

            // UIObject
            int _GetAlpha(lua_State*);
            int _GetName(lua_State*);
            int _GetObjectType(lua_State*);
            int _IsObjectType(lua_State*);
            int _SetAlpha(lua_State*);
            // Region
            int _ClearAllPoints(lua_State*);
            int _GetBase(lua_State*);
            int _GetBottom(lua_State*);
            int _GetCenter(lua_State*);
            int _GetHeight(lua_State*);
            int _GetLeft(lua_State*);
            int _GetNumPoint(lua_State*);
            int _GetParent(lua_State*);
            int _GetPoint(lua_State*);
            int _GetRight(lua_State*);
            int _GetTop(lua_State*);
            int _GetWidth(lua_State*);
            int _Hide(lua_State*);
            int _IsShown(lua_State*);
            int _IsVisible(lua_State*);
            int _SetAllPoints(lua_State*);
            int _SetHeight(lua_State*);
            int _SetParent(lua_State*);
            int _SetPoint(lua_State*);
            int _SetRelPoint(lua_State*);
            int _SetWidth(lua_State*);
            int _Show(lua_State*);

            static const char className[];
            static const char* classList[];
            static Lunar<LuaUIObject>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            s_bool CheckParent_();

            s_str           sName_;
            s_ptr<UIObject> pParent_;
        };

        /** \endcond
        */
    }
}

#endif
