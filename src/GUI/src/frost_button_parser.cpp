/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_button.h"

#include "frost_texture.h"
#include "frost_fontstring.h"
#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void Button::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);

    s_ptr<XML::Block> pSpecialBlock;
    pSpecialBlock = pBlock->GetBlock("NormalTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateNormalTexture_();
        pTexture->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pTexture->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }

    pSpecialBlock = pBlock->GetBlock("PushedTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreatePushedTexture_();
        pTexture->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pTexture->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }

    pSpecialBlock = pBlock->GetBlock("DisabledTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateDisabledTexture_();
        pTexture->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pTexture->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }

    pSpecialBlock = pBlock->GetBlock("HighlightTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateHighlightTexture_();
        pTexture->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pTexture->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }


    pSpecialBlock = pBlock->GetBlock("NormalText");
    if (pSpecialBlock)
    {
        s_ptr<GUI::FontString> pFontString = CreateNormalText_();
        pFontString->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pFontString->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }

    pSpecialBlock = pBlock->GetBlock("HighlightText");
    if (pSpecialBlock)
    {
        s_ptr<GUI::FontString> pFontString = CreateHighlightText_();
        pFontString->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pFontString->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }

    pSpecialBlock = pBlock->GetBlock("DisabledText");
    if (pSpecialBlock)
    {
        s_ptr<GUI::FontString> pFontString = CreateDisabledText_();
        pFontString->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pFontString->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }

    s_ptr<XML::Block> pOffsetBlock = pBlock->GetBlock("PushedTextOffset");
    if (pOffsetBlock)
    {
        s_ptr<XML::Block> pDimBlock = pOffsetBlock->GetRadioBlock();
        if (pDimBlock->GetName() == "AbsDimension")
        {
            SetPushedTextOffset((
                s_int(pDimBlock->GetAttribute("x")),
                s_int(pDimBlock->GetAttribute("y"))
            ));
        }
        else if (pDimBlock->GetName() == "RelDimension")
        {
            Warning(pDimBlock->GetFile()+":"+pDimBlock->GetLineNbr(),
                "RelDimension for Button:PushedTextOffset is not yet supported ("+sName_+")."
            );
        }
    }

    if ((pBlock->IsProvided("text") || !pBlock->IsProvided("inherits")))
        SetText(pBlock->GetAttribute("text"));
}
