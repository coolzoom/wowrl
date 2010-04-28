/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_scrollframe.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void ScrollFrame::ParseBlock( s_ptr<XML::Block> pBlock )
{
    ParseAttributes_(pBlock);

    ParseSizeBlock_(pBlock);
    ParseResizeBoundsBlock_(pBlock);
    ParseAnchorsBlock_(pBlock);
    ParseTitleRegionBlock_(pBlock);
    ParseBackdropBlock_(pBlock);
    ParseHitRectInsetsBlock_(pBlock);

    ParseScrollChildBlock_(pBlock);

    ParseLayersBlock_(pBlock);
    ParseFramesBlock_(pBlock);
    ParseScriptsBlock_(pBlock);
}

void ScrollFrame::ParseScrollChildBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pScrollChildBlock = pBlock->GetBlock("ScrollChild");
    if (pScrollChildBlock)
    {
        s_ptr<XML::Block> pChildBlock = pScrollChildBlock->First();
        s_ptr<Frame> pScrollChild = GUIManager::GetSingleton()->CreateFrame(pChildBlock->GetName());
        if (pScrollChild)
        {
            try
            {
                pScrollChild->SetAddOn(GUIManager::GetSingleton()->GetCurrentAddOn());
                pScrollChild->SetParent(this);
                pScrollChild->ParseBlock(pChildBlock);
                if (!pScrollChild->IsVirtual())
                    pScrollChild->On("Load");

                s_ptr<XML::Block> pAnchors = pChildBlock->GetBlock("Anchors");
                if (pAnchors)
                {
                    Warning(pAnchors->GetFile()+":"+pAnchors->GetLineNbr(),
                        "Scroll child's anchors are ignored."
                    );
                }

                if (!pChildBlock->GetBlock("Size"))
                {
                    Warning(pChildBlock->GetFile()+":"+pChildBlock->GetLineNbr(),
                        "Scroll child needs its size to be defined in a Size block."
                    );
                }

                SetScrollChild(pScrollChild);
            }
            catch (const GUIException& e)
            {
                pScrollChild.Delete();
                Error("", e.GetDescription());
            }
            catch (...)
            {
                pScrollChild.Delete();
                throw;
            }
        }
    }
}
