/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              GUI header                */
/*                                        */
/*                                        */


#ifndef FROST_GUI_REGION_H
#define FROST_GUI_REGION_H

#include <frost_utils.h>
#include "frost_uiobject.h"

namespace Frost
{
    namespace GUI
    {
        /// Simple GUI region (not renderable).
        /** \note It is the simplest derivate of UIObject, so if you
        *         just need a widget that has a position and a size
        *         (like Frame's title region), then this is the best
        *         choice.
        */
        class Region : public UIObject
        {
        public :

            /// Constructor.
            Region(s_ptr<GUIManager> pManager);

            /// Destructor.
            virtual ~Region();

            /// Renders this widget on the current render target.
            /** \note Does nothing.
            */
            virtual void   Render();

            /// Checks if the provided coordinates are inside this region.
            /** \param iX The horizontal coordinate
            *   \param iY The vertical coordinate
            *   \return 'true' if the provided coordinates are inside this region
            */
            virtual s_bool IsInRegion(const s_int& iX, const s_int& iY) const;

            /// Creates the associated Lua glue.
            virtual void   CreateGlue();

            /// Parses data from an XML::Block.
            /** \param pBlock The Frame's XML::Block
            */
            virtual void   ParseBlock(s_ptr<XML::Block> pBlock);

            static const s_str CLASS_NAME;

        protected :

            virtual void ParseAttributes_(s_ptr<XML::Block> pBlock);

        };
    }
}

#endif
