/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_checkbutton.h"

#include <frost_xml_document.h>
#include "frost_texture.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void CheckButton::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Button::ParseBlock(pBlock);

    s_ptr<XML::Block> pSpecialBlock;
    pSpecialBlock = pBlock->GetBlock("CheckedTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateCheckedTexture_();
        pTexture->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pTexture->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }

    pSpecialBlock = pBlock->GetBlock("DisabledCheckedTexture");
    if (pSpecialBlock)
    {
        s_ptr<GUI::Texture> pTexture = CreateDisabledCheckedTexture_();
        pTexture->ParseBlock(pSpecialBlock);
        if (pSpecialBlock->IsProvided("layer"))
            pTexture->SetDrawLayer(pSpecialBlock->GetAttribute("layer"));
    }
}
