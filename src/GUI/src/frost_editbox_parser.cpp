/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_editbox.h"
#include "frost_fontstring.h"

#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void EditBox::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);

    ParseTextInsetsBlock_(pBlock);
    ParseFontStringBlock_(pBlock);
    ParseHighlighColorBlock_(pBlock);

    if ((pBlock->IsProvided("letters") || !pBlock->IsProvided("inherits")))
        SetMaxLetters(s_uint(pBlock->GetAttribute("letters")));

    if ((pBlock->IsProvided("blinkSpeed") || !pBlock->IsProvided("inherits")))
        SetBlinkSpeed(s_double(pBlock->GetAttribute("blinkSpeed")));

    if ((pBlock->IsProvided("numeric") || !pBlock->IsProvided("inherits")))
        SetNumericOnly(s_bool(pBlock->GetAttribute("numeric")));

    if ((pBlock->IsProvided("positive") || !pBlock->IsProvided("positive")))
        SetPositiveOnly(s_bool(pBlock->GetAttribute("positive")));

    if ((pBlock->IsProvided("integer") || !pBlock->IsProvided("integer")))
        SetIntegerOnly(s_bool(pBlock->GetAttribute("integer")));

    if ((pBlock->IsProvided("password") || !pBlock->IsProvided("inherits")))
        EnablePasswordMode(s_bool(pBlock->GetAttribute("password")));

    if ((pBlock->IsProvided("multiLine") || !pBlock->IsProvided("inherits")))
        SetMultiLine(s_bool(pBlock->GetAttribute("multiLine")));

    if ((pBlock->IsProvided("historyLines") || !pBlock->IsProvided("inherits")))
        SetMaxHistoryLines(s_uint(pBlock->GetAttribute("historyLines")));

    if ((pBlock->IsProvided("autoFocus") || !pBlock->IsProvided("inherits")))
        EnableAutoFocus(s_bool(pBlock->GetAttribute("autoFocus")));

    if ((pBlock->IsProvided("ignoreArrows") || !pBlock->IsProvided("inherits")))
        SetArrowsIgnored(s_bool(pBlock->GetAttribute("ignoreArrows")));
}

void EditBox::ParseFontStringBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pFontStringBlock = pBlock->GetBlock("FontString");
    if (pFontStringBlock)
    {
        s_ptr<GUI::FontString> pFontString = CreateFontString_();
        pFontString->ParseBlock(pFontStringBlock);

        s_ptr<XML::Block> pErrorBlock = pFontStringBlock->GetBlock("Anchors");
        if (pErrorBlock)
        {
            Warning(pErrorBlock->GetFile()+":"+pErrorBlock->GetLineNbr(),
                "EditBox : FontString's Anchors are ignored."
            );
        }

        pErrorBlock = pFontStringBlock->GetBlock("Size");
        if (pErrorBlock)
        {
            Warning(pErrorBlock->GetFile()+":"+pErrorBlock->GetLineNbr(),
                "EditBox : FontString's Size is ignored."
            );
        }

        pFontString->SetAbsWidth(s_uint::INF);
        pFontString->SetAbsHeight(s_uint::INF);
        pFontString->ClearAllPoints();
        pFontString->SetAbsPoint(
            ANCHOR_TOPLEFT,     "$parent", ANCHOR_TOPLEFT,      lTextInsets_[BORDER_LEFT],   lTextInsets_[BORDER_TOP]
        );
        pFontString->SetAbsPoint(
            ANCHOR_BOTTOMRIGHT, "$parent", ANCHOR_BOTTOMRIGHT, -lTextInsets_[BORDER_RIGHT], -lTextInsets_[BORDER_BOTTOM]
        );

        pFontString->EnableFormatting(false);
    }
}

void EditBox::ParseHighlighColorBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pHighlightBlock = pBlock->GetBlock("HighlightColor");
    if (pHighlightBlock)
    {
        SetHighlightColor(Color(
            s_float(pHighlightBlock->GetAttribute("a")),
            s_float(pHighlightBlock->GetAttribute("r")),
            s_float(pHighlightBlock->GetAttribute("g")),
            s_float(pHighlightBlock->GetAttribute("b"))
        ));
    }
}

void EditBox::ParseTextInsetsBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pTextInsetsBlock = pBlock->GetBlock("TextInsets");
    if (pTextInsetsBlock)
    {
        SetTextInsets(
            s_int(pTextInsetsBlock->GetAttribute("left")),
            s_int(pTextInsetsBlock->GetAttribute("right")),
            s_int(pTextInsetsBlock->GetAttribute("top")),
            s_int(pTextInsetsBlock->GetAttribute("bottom"))
        );
    }
}
