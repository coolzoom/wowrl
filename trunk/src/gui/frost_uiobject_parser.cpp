/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_uiobject.h"

#include "gui/frost_anchor.h"
#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void UIObject::ParseSizeBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pSizeBlock = pBlock->GetBlock("Size");
    if (pSizeBlock)
    {
        s_ptr<XML::Block> pDimBlock = pSizeBlock->GetRadioBlock();
        if (pDimBlock->GetName() == "AbsDimension")
        {
            if (pDimBlock->IsProvided("x"))
                SetAbsWidth(s_uint(pDimBlock->GetAttribute("x")));
            if (pDimBlock->IsProvided("y"))
                SetAbsHeight(s_uint(pDimBlock->GetAttribute("y")));
        }
        else if (pDimBlock->GetName() == "RelDimension")
        {
            if (pDimBlock->IsProvided("x"))
                SetRelWidth(s_float(pDimBlock->GetAttribute("x")));
            if (pDimBlock->IsProvided("y"))
                SetRelHeight(s_float(pDimBlock->GetAttribute("y")));
        }
    }
}

void UIObject::ParseAnchorsBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pAnchorsBlock = pBlock->GetBlock("Anchors");
    if (pAnchorsBlock)
    {
        s_ctnr<s_str> lFoundPoints;
        s_ptr<XML::Block> pAnchorBlock;
        foreach_block (pAnchorBlock, pAnchorsBlock)
        {
            s_str sPoint = pAnchorBlock->GetAttribute("point");
            s_str sParent = pAnchorBlock->GetAttribute("relativeTo");
            s_str sRelativePoint = pAnchorBlock->GetAttribute("relativePoint");

            if (lFoundPoints.Find(sPoint))
            {
                Warning(pAnchorsBlock->GetFile()+":"+pAnchorsBlock->GetLineNbr(),
                    "Anchor point \""+sPoint+"\" has already been defined "
                    "for \""+sName_+"\". Anchor skipped."
                );
            }
            else
            {
                if (sRelativePoint.IsEmpty())
                    sRelativePoint = sPoint;

                if (sParent.IsEmpty(true))
                {
                    if (pParent_ || IsVirtual())
                        sParent = "$parent";
                    else
                        sParent = "";
                }

                GUI::Anchor mAnchor(
                    this,
                    GUI::Anchor::GetAnchorPoint(sPoint),
                    sParent,
                    GUI::Anchor::GetAnchorPoint(sRelativePoint)
                );

                s_ptr<XML::Block> pOffsetBlock = pAnchorBlock->GetBlock("Offset");
                if (pOffsetBlock)
                {
                    s_ptr<XML::Block> pDimBlock = pOffsetBlock->GetRadioBlock();
                    if (pDimBlock->GetName() == "AbsDimension")
                    {
                        mAnchor.SetAbsOffset(
                            s_int(pDimBlock->GetAttribute("x")),
                            s_int(pDimBlock->GetAttribute("y"))
                        );
                    }
                    else if (pDimBlock->GetName() == "RelDimension")
                    {
                        mAnchor.SetRelOffset(
                            s_float(pDimBlock->GetAttribute("x")),
                            s_float(pDimBlock->GetAttribute("y"))
                        );
                    }
                }

                SetPoint(mAnchor);
            }
        }
    }
}
