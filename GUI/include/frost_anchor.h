/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               GUI header               */
/*                                        */
/*                                        */


#ifndef FROST_GUI_ANCHOR_H
#define FROST_GUI_ANCHOR_H

#include <frost_utils.h>

namespace Frost
{
    namespace GUI
    {
        class UIObject;

        enum AnchorType
        {
            ANCHOR_ABS,
            ANCHOR_REL
        };

        enum AnchorPoint
        {
            ANCHOR_TOPLEFT,
            ANCHOR_TOP,
            ANCHOR_TOPRIGHT,
            ANCHOR_RIGHT,
            ANCHOR_BOTTOMRIGHT,
            ANCHOR_BOTTOM,
            ANCHOR_BOTTOMLEFT,
            ANCHOR_LEFT,
            ANCHOR_CENTER
        };

        /// Stores a position for a UI widget
        class Anchor
        {
        public :

            /// Default constructor.
            Anchor();

            /// Constructor.
            Anchor(
                s_ptr<UIObject> pObj,
                AnchorPoint     mPoint,
                const s_str&    pParent,
                AnchorPoint     mParentPoint
            );

            /// Returns this Anchor absolute X (in pixel).
            /** \return This Anchor absolute X.
            */
            const s_int&       GetAbsX() const;

            /// Returns this Anchor absolute Y (in pixel).
            /** \return This Anchor absolute Y.
            */
            const s_int&       GetAbsY() const;

            /// Returns this Anchor's base widget.
            /** \return This Anchor's base widget
            */
            s_ptr<const UIObject> GetObject() const;

            /// Returns this Anchor's parent widget.
            /** \return This Anchor's parent widget
            */
            s_ptr<const UIObject> GetParent() const;

            /// Returns this Anchor's parent's raw name (unmodified).
            /** \return This Anchor's parent's raw name (unmodified)
            */
            const s_str&       GetParentRawName() const;

            /// Returns this Anchor's point.
            /** \return This Anchor's point
            */
            AnchorPoint        GetPoint() const;

            /// Returns this Anchor's parent point.
            /** \return This Anchor's parent point
            */
            AnchorPoint        GetParentPoint() const;

            /// Returns the type of this Anchor (abs or rel).
            /** \return The type of this Anchor (abs or rel)
            */
            AnchorType         GetType() const;

            /// Returns this Anchor's absolute horizontal offset.
            /** \return This Anchor's absolute horizontal offset
            */
            const s_int&       GetAbsOffsetX() const;

            /// Returns this Anchor's absolute vertical offset.
            /** \return This Anchor's absolute vertical offset
            */
            const s_int&       GetAbsOffsetY() const;

            /// Returns this Anchor's relative horizontal offset.
            /** \return This Anchor's relative horizontal offset
            */
            const s_float&     GetRelOffsetX() const;

            /// Returns this Anchor's relative vertical offset.
            /** \return This Anchor's relative vertical offset
            */
            const s_float&     GetRelOffsetY() const;

            /// Sets this Anchor's base widget.
            /** \param pObj The new base widget
            */
            void               SetObject(s_ptr<UIObject> pObj);

            /// Sets this Anchor's parent's raw name.
            /** \param sName The parent's raw name
            */
            void               SetParentRawName(const s_str& sName);

            /// Sets this Anchor's point.
            /** \param mPoint The new point
            */
            void               SetPoint(AnchorPoint mPoint);

            /// Sets this Anchor's parent point.
            /** \param mParentPoint The new parent point
            */
            void               SetParentPoint(AnchorPoint mParentPoint);

            /// Sets this Anchor's absolute offset.
            /** \param iX The new horizontal offset
            *   \param iY The new vertical offset
            */
            void               SetAbsOffset(const s_int& iX, const s_int& iY);

            /// Sets this Anchor's relative offset.
            /** \param fX The new horizontal offset
            *   \param fY The new vertical offset
            */
            void               SetRelOffset(const s_float& fX, const s_float& fY);

            /// Prints all relevant information about this anchor in a string.
            /** \param sTab The offset to give to all lines
            *   \return All relevant information about this anchor
            */
            s_str              Serialize(const s_str& sTab) const;

            /// Gets the anchor parent from the parent string.
            void               UpdateParent() const;

            /// Returns the name of an anchor point.
            /** \param mPoint The anchor point
            */
            static s_str       GetStringPoint(AnchorPoint mPoint);

            /// Returns the anchor point from its name.
            /** \param sPoint The name of the anchor point
            */
            static AnchorPoint GetAnchorPoint(const s_str& sPoint);


            static const s_str CLASS_NAME;

        private :

            s_ptr<const UIObject> pObj_;
            AnchorPoint           mParentPoint_;
            AnchorPoint           mPoint_;
            AnchorType            mType_;
            s_int                 iAbsOffX_, iAbsOffY_;
            s_float               fRelOffX_, fRelOffY_;

            mutable s_int iParentWidth_, iParentHeight_;

            mutable s_int   iAbsX_, iAbsY_;
            mutable s_float fRelX_, fRelY_;

            mutable s_ptr<const UIObject> pParent_;
            mutable s_str                 sParent_;
            mutable s_bool                bParentUpdated_;
        };
    }
}

#endif
