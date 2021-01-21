/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Spell header              */
/*                                        */
/*                                        */


#ifndef FROST_SPELL_H
#define FROST_SPELL_H

#include "frost_prereqs.h"

namespace Frost
{
    class Spell
    {
        public :

            /// Constructor.
            /** \param pParent The Unit to which this Spell belongs.
            */
            Spell(s_ptr<Unit> pParent);

            /// Destructor.
            ~Spell();

            static const s_str CLASS_NAME;

        protected :

        s_ptr<Unit> pParent_;

    };
}

#endif
