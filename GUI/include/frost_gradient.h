/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Gradient header             */
/*                                        */
/*                                        */


#include <frost_utils.h>

#ifndef FROST_GUI_GRADIENT_H
#define FROST_GUI_GRADIENT_H

#include <frost_utils_color.h>

namespace Frost
{
    namespace GUI
    {
        /// An helper to make gradients
        class Gradient
        {
        public :

            enum Orientation
            {
                HORIZONTAL,
                VERTICAL
            };

            /// Default constructor.
            /** \note Makes an empty Gradient.
            */
            Gradient();

            /// Constructor.
            /** \param mOrientation This Gradient's orientation
            *   \param mMinColor    This Gradient's min color
            *   \param mMaxColor    This Gradient's max color
            */
            Gradient(Orientation mOrientation, const Color& mMinColor, const Color& mMaxColor);

            /// Returns the Gradient's min colors.
            /** \return The Gradient's min colors
            *   \note In horizontal mode, this is the left color, and
            *         in vertical mode this is the top one.
            */
            const Color& GetMinColor() const;

            /// Returns the Gradient's max colors.
            /** \return The Gradient's max colors
            *   \note In horizontal mode, this is the right color, and
            *         in vertical mode this is the bottom one.
            */
            const Color& GetMaxColor() const;

            /// Returns the Gradient's orientation.
            /** \return The Gradient's orientation
            */
            Orientation GetOrientation() const;

            /// Checks if this Gradient is an empty one.
            /** \return 'true' if this Gradient is an empty one
            */
            const s_bool& IsEmpty() const;

            static const Gradient NONE;

        private :

            s_bool      bIsEmpty_;
            Orientation mOrientation_;
            Color       mMinColor_, mMaxColor_;

        };
    }
}

#endif
