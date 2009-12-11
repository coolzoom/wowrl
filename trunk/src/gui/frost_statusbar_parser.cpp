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

    CreateBarTexture_();

    Anchor mAnchor(pBarTexture_, ANCHOR_BOTTOMLEFT, "$parent", ANCHOR_BOTTOMLEFT);

    s_ptr<XML::Block> pBarBlock = pBlock->GetRadioBlock();
    if (pBarBlock->GetName() == "BarTexture")
    {
        pBarTexture_->ParseBlock(pBarBlock);
        pBarTexture_->ClearAllPoints();
        pBarTexture_->SetPoint(mAnchor);
    }
    else
    {
        pBarTexture_->SetName("$parentBarTexture");
        GUIManager::GetSingleton()->AddUIObject(pBarTexture_);

        if (!bVirtual_)
            pBarTexture_->CreateGlue();

        AddRegion(pBarTexture_);

        pBarTexture_->SetPoint(mAnchor);
        SetBarColor(Color(
            s_uchar(s_float(pBarBlock->GetAttribute("a"))*255.0f),
            s_uchar(s_float(pBarBlock->GetAttribute("r"))*255.0f),
            s_uchar(s_float(pBarBlock->GetAttribute("g"))*255.0f),
            s_uchar(s_float(pBarBlock->GetAttribute("b"))*255.0f)
        ));
    }
}
