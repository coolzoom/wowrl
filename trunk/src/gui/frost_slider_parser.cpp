/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_slider.h"

#include "xml/frost_xml_document.h"
#include "gui/frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void Slider::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);

    if (pBlock->IsProvided("valueStep") || !bInherits_)
        SetValueStep(s_float(pBlock->GetAttribute("valueStep")));
    if (pBlock->IsProvided("minValue") || !bInherits_)
        SetMinValue(s_float(pBlock->GetAttribute("minValue")));
    if (pBlock->IsProvided("maxValue") || !bInherits_)
        SetMaxValue(s_float(pBlock->GetAttribute("maxValue")));
    if (pBlock->IsProvided("defaultValue") || !bInherits_)
        SetValue(s_float(pBlock->GetAttribute("defaultValue")));
    if (pBlock->IsProvided("drawLayer") || !bInherits_)
        SetThumbDrawLayer(s_str(pBlock->GetAttribute("drawLayer")));

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
                "Unknown Slider orientation : \""+sOrientation+"\". Expecting either :\n"
                "\"HORIZONTAL\" or \"VERTICAL\". Attribute ignored."
            );
        }
    }

    s_ptr<XML::Block> pThumbBlock = pBlock->GetBlock("ThumbTexture");
    if (pThumbBlock)
    {
        CreateThumbTexture_();

        pThumbTexture_->ParseBlock(pThumbBlock);
        pThumbTexture_->ClearAllPoints();
        pThumbTexture_->SetPoint(Anchor(
            pThumbTexture_, ANCHOR_CENTER, "$parent",
            GetOrientation() == ORIENT_HORIZONTAL ? ANCHOR_LEFT : ANCHOR_BOTTOM
        ));
    }
}
