/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*            Gradient header             */
/*                                        */
/*                                        */


#include "frost.h"

#ifndef FROST_GUI_GRADIENT_H
#define FROST_GUI_GRADIENT_H

namespace Frost
{
    namespace GUI
    {
        enum GradientOrientation
        {
            ORIENTATION_HORIZONTAL,
            ORIENTATION_VERTICAL
        };

        /// An helper to make gradients
        class Gradient
        {
        public :

            /// Default constructor.
            /** \note Makes an empty Gradient.
            */
            Gradient();

            /// Constructor.
            /** \param mOrientation This Gradient's orientation
            *   \param mMinColor    This Gradient's min color
            *   \param mMaxColor    This Gradient's max color
            */
            Gradient(GradientOrientation mOrientation, const Color& mMinColor, const Color& mMaxColor);

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
            GradientOrientation GetOrientation() const;

            /// Checks if this Gradient is an empty one.
            /** \return 'true' if this Gradient is an empty one
            */
            const s_bool& IsEmpty() const;

            static const Gradient EMPTY_GRADIENT;

        private :

            s_bool              bIsEmpty_;
            GradientOrientation mOrientation_;
            Color               mMinColor_, mMaxColor_;

        };
    }
}

#endif
