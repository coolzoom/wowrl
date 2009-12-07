/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_checkbutton.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void CheckButton::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);
}
