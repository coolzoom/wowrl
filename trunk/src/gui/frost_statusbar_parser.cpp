/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_statusbar.h"
#include "gui/frost_texture.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void StatusBar::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);

    if (pBlock->IsProvided("minValue") || !bInherits_)
        SetMinValue(s_float(pBlock->GetAttribute("minValue")));
    if (pBlock->IsProvided("maxValue") || !bInherits_)
        SetMaxValue(s_float(pBlock->GetAttribute("maxValue")));
    if (pBlock->IsProvided("defaultValue") || !bInherits_)
        SetValue(s_float(pBlock->GetAttribute("defaultValue")));
    if (pBlock->IsProvided("drawLayer") || !bInherits_)
        SetBarDrawLayer(s_str(pBlock->GetAttribute("drawLayer")));

    if (pBlock->IsProvided("orientation") || !bInherits_)
    {
        s_str sOrientation = pBlock->GetAttribute("orientation");
        if (sOrientation == "HORIZONTAL")
            SetOrientation(ORIENT_HORIZONTAL);
        else if (sOrientation == "VERTICAL")
            SetOrientation(ORIENT_VERTICAL);
        else
        {
            Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                "Unknown StatusBar orientation : \""+sOrientation+"\". Expecting either :\n"
                "\"HORIZONTAL\" or \"VERTICAL\". Attribute ignored."
            );
        }
    }


    s_ptr<XML::Block> pBarBlock = pBlock->GetRadioBlock();
    if (pBarBlock)
    {
        Anchor mAnchor(pBarTexture_, ANCHOR_BOTTOMLEFT, "$parent", ANCHOR_BOTTOMLEFT);
        if (pBarBlock->GetName() == "BarTexture")
        {
            CreateBarTexture_();
            pBarTexture_->ParseBlock(pBarBlock);
            pBarTexture_->ClearAllPoints();
            pBarTexture_->SetPoint(mAnchor);
        }
        else
        {
            SetBarColor(Color(
                s_float(pBarBlock->GetAttribute("a")),
                s_float(pBarBlock->GetAttribute("r")),
                s_float(pBarBlock->GetAttribute("g")),
                s_float(pBarBlock->GetAttribute("b"))
            ));
        }
    }
}
