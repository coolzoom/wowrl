/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_titleregion.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void TitleRegion::ParseBlock( s_ptr<XML::Block> pBlock )
{
    ParseAttributes_(pBlock);

    ParseSizeBlock_(pBlock);
    ParseAnchorsBlock_(pBlock);
}

void TitleRegion::ParseAttributes_( s_ptr<XML::Block> pBlock )
{
    if ((pBlock->IsProvided("setAllPoints") || !bInherits_) &&
        (s_bool(pBlock->GetAttribute("setAllPoints"))))
        SetAllPoints(pParent_);
}
