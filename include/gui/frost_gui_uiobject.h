/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_UIOBJECT_H
#define FROST_GUI_UIOBJECT_H

#include "frost.h"
#include "frost_gui.h"
#include "frost_gfx.h"

namespace Frost
{
    namespace GUI
    {
        enum ObjectType
        {
            OJBECT_TYPE_UIOBJECT,
            OJBECT_TYPE_REGION,
            OJBECT_TYPE_LAYEREDREGION,
            OJBECT_TYPE_FRAME,
            OJBECT_TYPE_FONTSTRING,
            OJBECT_TYPE_TEXTURE,
            OJBECT_TYPE_BUTTON,
            OJBECT_TYPE_EDITBOX,
            OJBECT_TYPE_SCROLLINGMESSAGE_FRAME,
            OJBECT_TYPE_STATUSBAR,
            OJBECT_TYPE_SLIDER
        };

        enum BorderType
        {
            BORDER_TOP = 0,
            BORDER_RIGHT,
            BORDER_BOTTOM,
            BORDER_LEFT
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
        *   two virtual functions : Update and UpdateMaterial.
        *   Both should be overriden and contain all the
        *   required logic for it to work.<br>
        *   Look at the provided widgets (Frame, Slider, ...)
        *   to see how it's done.
        */
        class UIObject
        {
        public :

            /// Contructor.
            UIObject(lua_State* luaVM);

            /// Destructor.
            virtual ~UIObject();

            /// Updates this widget's logic.
            virtual void    Update();

            /// Updates this widget's graphics.
            /** \param bForceUpdate Update everything
            */
            virtual void    UpdateMaterial(const s_bool& bForceUpdate = false);

            /// Returns this widget's material.
            /** \return This widget's material (texture)
            */
            s_refptr<Material> GetMaterial();

            /// Tells this widget to update its borders.
            void            FireUpdateBorders();

            /// Returns this widget's name.
            /** \return This widget's name
            */
            const s_str&    GetName() const;

            /// Changes this widget's parent.
            /** \param pParent The new parent
            *   \note Default is NULL.
            */
            virtual void    SetParent(s_ptr<UIObject> pParent);

            /// Returns this widget's parent.
            /** \return This widget's parent
            */
            s_ptr<UIObject> GetParent();

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
            void            Show();

            /// Hides this widget.
            /** \note All its childs won't be visible on the screen
            *   anymore, even if they are still marked as shown.
            */
            void            Hide();

            /// Checks if this widget is shown.
            /** \return 'true' if this widget is shown
            */
            const s_bool&   IsShown() const;

            /// Checks if this widget can be seen on the screen.
            /** \return 'true' if this widget can be seen on the screen
            */
            s_bool          IsVisible() const;

            /// Changes this widget's absolute width (in pixels).
            /** \param uiAbsWidth The new width
            *   \note Width will automatically be adjusted to include
            *         all of this widget's childs.
            */
            void            SetAbsWidth(const s_uint& uiAbsWidth);

            /// Changes this widget's absolute height (in pixels).
            /** \param uiAbsHeight The new height
            *   \note Height will automatically be adjusted to include
            *         all of this widget's childs.
            */
            void            SetAbsHeight(const s_uint& uiAbsHeight);

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
            */
            const s_uint&   GetAbsWidth() const;

            /// Returns this widget's absolute height (in pixels).
            /** \return This widget's absolute height (in pixels)
            */
            const s_uint&   GetAbsHeight() const;

            /// Returns this widget's width (relative to its parent).
            /** \return This widget's width (relative to its parent)
            */
            const s_float&  GetRelWidth() const;

            /// Returns this widget's height (relative to its parent).
            /** \return This widget's height (relative to its parent)
            */
            const s_float&  GetRelHeight() const;

            /// Returns the type of this widget.
            /** \return The type of this widget (see ObjectType)
            */
            ObjectType      GetObjectType() const;

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

            /// Creates/modifies an anchor.
            /** \param mPoint         The anchor point for this object
            *   \param pObj           The anchor parent
            *   \param mRelativePoint The anchor point for the parent
            *   \param iX             The horizontal offset
            *   \param iY             The vertical offset
            */
            void            SetPoint(
                                AnchorPoint mPoint,
                                s_ptr<UIObject> pObj,
                                AnchorPoint mRelativePoint,
                                const s_int& iX,
                                const s_int& iY
            );

            /// Returns the number of defined anchors.
            /** \return The number of defined anchors
            */
            s_uint          GetNumPoint() const;

            /// Gets an anchor by its ID.
            /** \param uiPoint The anchor ID
            *   \return A pointer to the anchor, NULL if none
            */
            s_ptr<Anchor>   GetPoint(const s_uint& uiPoint);

            /// Returns this widget's unique ID.
            /** \return This widget's unique ID
            */
            const s_uint&   GetID() const;

            // Glues
            int _GetAlpha(lua_State*);
            int _GetName(lua_State*);
            int _GetObjectType(lua_State*);
            int _IsObjectType(lua_State*);
            int _SetAlpha(lua_State*);
            // Region
            int _ClearAllPoints(lua_State*);
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
            int _RebuildCache(lua_State*);
            int _SetAllPoints(lua_State*);
            int _SetHeight(lua_State*);
            int _SetParent(lua_State*);
            int _SetPoint(lua_State*);
            int _SetWidth(lua_State*);
            int _Show(lua_State*);

            /// Lunar function
            int GetDataTable(lua_State *L);

            static const char className[];
            static Lunar<UIObject>::RegType methods[];
            static const s_str CLASS_NAME;

        protected :

            void UpdateBorders_();

            s_str           sName_;
            s_uint          uiID_;
            ObjectType      mType_;
            s_ptr<UIObject> pParent_;

            std::vector<s_str> lType_;

            std::map<AnchorPoint, Anchor> lAnchorList_;
            std::vector< s_ptr<Anchor> >  lAnchorStack_;
            s_int                         lBorderList_[4];

            s_float fAlpha_;
            s_bool  bIsShown_;
            s_uint  uiAbsWidth_;
            s_uint  uiAbsHeight_;
            s_float fRelWidth_;
            s_float fRelHeight_;

            RenderTarget       mTarget_;
            s_refptr<Material> pMaterial_;

            s_bool bUpdateBorders_;

            // Lunar variables
            lua_State*  pLua_;
            int         iRef_;
        };
    }
}

#endif
