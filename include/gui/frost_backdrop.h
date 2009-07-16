/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               XXX header               */
/*                                        */
/*                                        */


#ifndef FROST_BACKDROP_H
#define FROST_BACKDROP_H

#include "frost.h"
#include "gui/frost_sprite.h"

namespace Frost
{
    namespace GUI
    {
        /// Draws borders and background of a Frame.
        class Backdrop
        {
        public :

            enum EdgeType
            {
                EDGE_LEFT,
                EDGE_RIGHT,
                EDGE_TOP,
                EDGE_BOTTOM,
                EDGE_TOP_LEFT,
                EDGE_TOP_RIGHT,
                EDGE_BOTTOM_LEFT,
                EDGE_BOTTOM_RIGHT
            };

            /// Constructor.
            /** \param pParent The Frame it is linked to
            */
            Backdrop(s_ptr<Frame> pParent);

            /// Destructor.
            ~Backdrop();

            /// Copies a Backdrop's parameters into this one (inheritance).
            /** \param pBackdrop The Backdrop to copy
            */
            void CopyFrom(s_ptr<Backdrop> pBackdrop);

            /// Sets the background texture.
            /** \param sBackgroundFile The background texture
            */
            void SetBackground(const s_str& sBackgroundFile);

            /// Returns this Backdrop's background file.
            /** \return This Backdrop's background file
            */
            const s_str& GetBackgroundFile() const;

            /// Sets the background color.
            /** \param mColor The background color
            *   \note This color is only used to tint the background texture.
            */
            void SetBackgroundColor(const Color& mColor);

            /// Returns the background color.
            /** \return The background color
            */
            const Color& GetBackgroundColor() const;

            /// Enables tilling for the background texture.
            /** \param bBackgroundTilling 'true' to enable tilling
            */
            void SetBackgroundTilling(const s_bool& bBackgroundTilling);

            /// Checks if tilling is enabled for the background texture.
            /** \return 'true' if tilling is enabled for the background texture
            */
            const s_bool& IsBackgroundTilling() const;

            /// Sets the appearent tile size.
            /** \param uiTileSize The new edget size
            *   \note Tile will be scaled by uiTileSize/backgroundTextureSize.
            */
            void SetTileSize(const s_uint& uiTileSize);

            /// Returns this Backdrop's tile size.
            /** \return This Backdrop's tile size
            */
            const s_uint& GetTileSize() const;

            /// Sets insets for the background texture.
            /** \param lInsets The insets array
            *   \note Insets are interpreted as (left, right, top, bottom).
            */
            void SetBackgroundInsets(const s_array<s_int,4>& lInsets);

            /// Sets insets for the background texture.
            /** \param iLeft   The left inset
            *   \param iRight  The right inset
            *   \param iTop    The top inset
            *   \param iBottom The bottom inset
            */
            void SetBackgroundInsets(const s_int& iLeft, const s_int& iRight, const s_int& iTop, const s_int& iBottom);

            /// Returns this Backdrop's background insets.
            /** \return This Backdrop's background insets
            */
            const s_array<s_int,4>& GetBackgroundInsets() const;

            /// Sets the edge/corner texture.
            /** \param sEdgeFile The edge/corner texture
            *   \note This texture's width must be 8 times greater than its
            *         height.<br><br>
            *         Texture parts are interpreted as :<br>
            *         - [  0, 1/8] : left edge
            *         - [1/8, 1/4] : right edge
            *         - [1/4, 3/8] : top edge (rotated 90 degrees ccw)
            *         - [3/8, 1/2] : bottom edge (rotated 90 degrees ccw)
            *         - [1/2, 5/8] : top-left corner
            *         - [5/8, 3/4] : top-right corner
            *         - [3/4, 7/8] : bottom-left corner
            *         - [7/8,   1] : bottom-right corner
            */
            void SetEdge(const s_str& sEdgeFile);

            /// Returns this Backdrop's edge file.
            /** \return This Backdrop's edge file
            */
            const s_str& GetEdgeFile() const;

            /// Sets the edge/corner color.
            /** \param mColor The edge/corner color
            *   \note This color is only used to tint the edge/corner texture.
            */
            void SetEdgeColor(const Color& mColor);

            /// Returns the edge/corner color.
            /** \return The edge/corner color
            */
            const Color& GetEdgeColor() const;

            /// Sets the appearent edge size.
            /** \param uiEdgeSize The new edget size
            *   \note Edges will be scaled by uiEdgeSize/edgeTextureHeight.
            */
            void SetEdgeSize(const s_uint& uiEdgeSize);

            /// Returns this Backdrop's edge size.
            /** \return This Backdrop's edge size
            */
            const s_uint& GetEdgeSize() const;

            /// Renders this Backdrop on the current render target.
            void Render() const;

            static const s_str CLASS_NAME;

        private :

            s_ptr<Frame>                pParent_;
            s_refptr<Sprite>            pBackground_;
            s_str                       sBackgroundFile_;
            s_array<s_refptr<Sprite>,8> lEdgeList_;
            s_str                       sEdgeFile_;

            s_bool           bBackgroundTilling_;
            s_uint           uiTileSize_;
            s_uint           uiOriginalTileSize_;
            s_array<s_int,4> lBackgroundInsets_;
            s_uint           uiEdgeSize_;
            s_uint           uiOriginalEdgeSize_;
        };
    }
}

#endif
