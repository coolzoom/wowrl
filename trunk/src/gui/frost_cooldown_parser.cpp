/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_cooldown.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void Cooldown::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);
}
