/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_scrollframe.h"

#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void ScrollFrame::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);

    ParseScrollChildBlock_(pBlock);
}

void ScrollFrame::ParseScrollChildBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pScrollChildBlock = pBlock->GetBlock("ScrollChild");
    if (pScrollChildBlock)
    {
        s_ptr<XML::Block> pChildBlock = pScrollChildBlock->First();
        s_ptr<Frame> pScrollChild = pManager_->CreateFrame(pChildBlock->GetName());
        if (pScrollChild)
        {
            try
            {
                pScrollChild->SetAddOn(pManager_->GetCurrentAddOn());
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
