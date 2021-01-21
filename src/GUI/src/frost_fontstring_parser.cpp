/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_fontstring.h"

#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void FontString::ParseBlock( s_ptr<XML::Block> pBlock )
{
    LayeredRegion::ParseBlock(pBlock);

    ParseColorBlock_(pBlock);
    ParseShadowBlock_(pBlock);
}

void FontString::ParseAttributes_( s_ptr<XML::Block> pBlock )
{
    LayeredRegion::ParseAttributes_(pBlock);

    SetFont(
        pManager_->ParseFileName(pBlock->GetAttribute("font")),
        s_uint(pBlock->GetAttribute("fontHeight"))
    );

    if (pBlock->IsProvided("text") || !bInherits_)
        SetText(pBlock->GetAttribute("text"));
    if (pBlock->IsProvided("nonspacewrap") || !bInherits_)
        SetNonSpaceWrap(s_bool(pBlock->GetAttribute("nonspacewrap")));
    if (pBlock->IsProvided("spacing") || !bInherits_)
        SetSpacing(s_float(pBlock->GetAttribute("spacing")));

    if (pBlock->IsProvided("outline") || !bInherits_)
    {
        const s_str& sOutline = pBlock->GetAttribute("outline");
        if ( (sOutline == "NORMAL") || (sOutline == "THICK") )
            SetOutlined(true);
        else if (sOutline == "NONE")
            SetOutlined(false);
        else
        {
            Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                "Unkown outline type for "+sName_+" : \""+sOutline+"\"."
            );
        }
    }

    if (pBlock->IsProvided("justifyH") || !bInherits_)
    {
        const s_str& sJustifyH = pBlock->GetAttribute("justifyH");
        if (sJustifyH == "LEFT")
            SetJustifyH(Text::ALIGN_LEFT);
        else if (sJustifyH == "CENTER")
            SetJustifyH(Text::ALIGN_CENTER);
        else if (sJustifyH == "RIGHT")
            SetJustifyH(Text::ALIGN_RIGHT);
        else
        {
            Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                "Unkown horizontal justify behavior for "+sName_+
                " : \""+sJustifyH+"\"."
            );
        }
    }

    if (pBlock->IsProvided("justifyV") || !bInherits_)
    {
        const s_str& sJustifyV = pBlock->GetAttribute("justifyV");
        if (sJustifyV == "TOP")
            SetJustifyV(Text::ALIGN_TOP);
        else if (sJustifyV == "MIDDLE")
            SetJustifyV(Text::ALIGN_MIDDLE);
        else if (sJustifyV == "BOTTOM")
            SetJustifyV(Text::ALIGN_BOTTOM);
        else
        {
            Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                "Unkown vertical justify behavior for "+sName_+
                " : \""+sJustifyV+"\"."
            );
        }
    }
}

void FontString::ParseColorBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pColorBlock = pBlock->GetBlock("Color");
    if (pColorBlock)
    {
        SetTextColor(Color(
            s_float(pColorBlock->GetAttribute("a")),
            s_float(pColorBlock->GetAttribute("r")),
            s_float(pColorBlock->GetAttribute("g")),
            s_float(pColorBlock->GetAttribute("b"))
        ));
    }
}

void FontString::ParseShadowBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pShadowBlock = pBlock->GetBlock("Shadow");
    if (pShadowBlock)
    {
        SetShadow(true);
        s_ptr<XML::Block> pColorBlock = pShadowBlock->GetBlock("Color");
        if (pColorBlock)
        {
            SetShadowColor(Color(
                s_float(pColorBlock->GetAttribute("a")),
                s_float(pColorBlock->GetAttribute("r")),
                s_float(pColorBlock->GetAttribute("g")),
                s_float(pColorBlock->GetAttribute("b"))
            ));
        }

        s_ptr<XML::Block> pOffsetBlock = pShadowBlock->GetBlock("Offset");
        if (pOffsetBlock)
        {
            SetShadowOffsets(
                s_int(pOffsetBlock->GetAttribute("x")),
                s_int(pOffsetBlock->GetAttribute("y"))
            );
        }
    }
}
