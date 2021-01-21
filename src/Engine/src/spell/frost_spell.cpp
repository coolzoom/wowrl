/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Spell source              */
/*                                        */
/*                                        */

#include "spell/frost_spell.h"

using namespace std;

namespace Frost
{
    const s_str Spell::CLASS_NAME = "Spell";

    Spell::Spell( s_ptr<Unit> pParent ) : pParent_(pParent)
    {
    }

    Spell::~Spell()
    {
    }
}


