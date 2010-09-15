/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_messageframe.h"

#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void MessageFrame::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);
}
