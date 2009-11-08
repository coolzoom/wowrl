/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_button.h"

#include "gui/frost_texture.h"
#include "gui/frost_fontstring.h"
#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void Button::ParseBlock( s_ptr<XML::Block> pBlock )
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

    s_ptr<XML::Block> pSpecialBlock;
    pSpecialBlock = pBlock->GetBlock("NormalTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateNormalTexture_();
        pTexture->ParseBlock(pSpecialBlock);
    }

    pSpecialBlock = pBlock->GetBlock("PushedTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreatePushedTexture_();
        pTexture->ParseBlock(pSpecialBlock);
    }

    pSpecialBlock = pBlock->GetBlock("DisabledTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateDisabledTexture_();
        pTexture->ParseBlock(pSpecialBlock);
    }

    pSpecialBlock = pBlock->GetBlock("HighlightTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateHighlightTexture_();
        pTexture->ParseBlock(pSpecialBlock);
    }


    pSpecialBlock = pBlock->GetBlock("NormalText");
    if (pSpecialBlock)
    {
        s_ptr<GUI::FontString> pFontString = CreateNormalText_();
        pFontString->ParseBlock(pSpecialBlock);
    }

    pSpecialBlock = pBlock->GetBlock("HighlightText");
    if (pSpecialBlock)
    {
        s_ptr<GUI::FontString> pFontString = CreateHighlightText_();
        pFontString->ParseBlock(pSpecialBlock);
    }

    pSpecialBlock = pBlock->GetBlock("DisabledText");
    if (pSpecialBlock)
    {
        s_ptr<GUI::FontString> pFontString = CreateDisabledText_();
        pFontString->ParseBlock(pSpecialBlock);
    }

    if ((pBlock->IsProvided("text") || !pBlock->IsProvided("inherits")))
        SetText(pBlock->GetAttribute("text"));
}
