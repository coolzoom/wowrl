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
            s_int iX = s_int(pDimBlock->GetAttribute("x"));
            s_int iY = s_int(pDimBlock->GetAttribute("y"));
            if (iX >= 0)
                SetAbsWidth(s_uint(iX));
            if (iY >= 0)
                SetAbsHeight(s_uint(iY));
        }
        else if (pDimBlock->GetName() == "RelDimension")
        {
            SetRelWidth(s_float(pDimBlock->GetAttribute("x")));
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

                GUI::Anchor mAnchor(
                    this,
                    GUI::Anchor::GetAnchorPoint(sPoint),
                    sParent.IsEmpty(true) && !pAnchorBlock->IsProvided("relativeTo") ? "$parent" : sParent,
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
