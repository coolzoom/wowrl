/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_TITLEREGION_H
#define FROST_GUI_TITLEREGION_H

#include "frost.h"
#include "gui/frost_uiobject.h"

namespace Frost
{
    namespace GUI
    {
        /// Abstract GUI renderable.
        /** \note It is the simplest derivate of UIObject, so if you
        *         just need a widget that has a position and a size
        *         (like Frame's title region), then this is the best
        *         choice.
        */
        class TitleRegion : public UIObject
        {
        public :

            /// Constructor.
            TitleRegion();

            /// Destructor.
            ~TitleRegion();

            /// Renders this widget on the current render target.
            /** \note Does nothing.
            */
            void   Render();

            /// Checks if the provided coordinates are inside this region.
            /** \param iX The horizontal coordinate
            *   \param iY The vertical coordinate
            *   \return 'true' if the provided coordinates are inside this region
            */
            s_bool IsInRegion(const s_int& iX, const s_int& iY);

            /// Creates the associated Lua glue.
            void   CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The Frame's XML::Block
            */
            void   ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            virtual void ParseAttributes_(s_ptr<XML::Block> pBlock);

        };
    }
}

#endif
