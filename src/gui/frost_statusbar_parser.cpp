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
    ParseAttributes_(pBlock);

    ParseSizeBlock_(pBlock);
    ParseResizeBoundsBlock_(pBlock);
    ParseAnchorsBlock_(pBlock);
    ParseTitleRegionBlock_(pBlock);
    ParseBackdropBlock_(pBlock);
    ParseHitRectInsetsBlock_(pBlock);

    ParseLayersBlock_(pBlock);
    ParseFramesBlock_(pBlock);
    ParseScriptsBlock_(pBlock);

    if (pBlock->IsProvided("minValue") || !bInherits_)
        SetMinValue(s_float(pBlock->GetAttribute("minValue")));
    if (pBlock->IsProvided("maxValue") || !bInherits_)
        SetMaxValue(s_float(pBlock->GetAttribute("maxValue")));
    if (pBlock->IsProvided("defaultValue") || !bInherits_)
        SetValue(s_float(pBlock->GetAttribute("defaultValue")));
    if (pBlock->IsProvided("drawLayer") || !bInherits_)
        SetBarDrawLayer(s_str(pBlock->GetAttribute("drawLayer")));

    CreateBarTexture_();

    Anchor mAnchor(pBarTexture_, ANCHOR_TOPLEFT, this, ANCHOR_TOPLEFT);

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
