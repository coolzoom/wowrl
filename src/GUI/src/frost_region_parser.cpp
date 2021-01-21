/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_region.h"

#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void Region::ParseBlock( s_ptr<XML::Block> pBlock )
{
    ParseAttributes_(pBlock);

    ParseSizeBlock_(pBlock);
    ParseAnchorsBlock_(pBlock);
}

void Region::ParseAttributes_( s_ptr<XML::Block> pBlock )
{
    if ((pBlock->IsProvided("setAllPoints") || !bInherits_) &&
        (s_bool(pBlock->GetAttribute("setAllPoints"))))
        SetAllPoints("$parent");
}
