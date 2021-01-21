/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_layeredregion.h"

#include "frost_frame.h"
#include <frost_xml_document.h>

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void LayeredRegion::ParseBlock( s_ptr<XML::Block> pBlock )
{
    ParseAttributes_(pBlock);

    ParseSizeBlock_(pBlock);
    ParseAnchorsBlock_(pBlock);
}

void LayeredRegion::ParseAttributes_( s_ptr<XML::Block> pBlock )
{
    s_str sName = pBlock->GetAttribute("name");
    if (!pManager_->CheckUIObjectName(sName))
    {
        throw GUIException(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
            "Can't create an UIObject with an incorrect name. Skipped."
        );
    }

    s_bool bVirtual = s_bool(pBlock->GetAttribute("virtual"));
    s_ptr<GUI::Frame> pFrameParent = s_ptr<GUI::Frame>::DynamicCast(pParent_);
    if (!sName.IsEmpty(true))
    {
        if ( bVirtual || (pFrameParent && pFrameParent->IsVirtual()) )
            SetVirtual();

        SetName(sName);
    }
    else
    {
        throw GUIException(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
            "Can't create an UIObject with a blank name. Skipped."
        );
    }

    if (pManager_->GetUIObjectByName(sName_))
    {
        throw GUIException(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
            s_str(bVirtual ? "A virtual" : "An")+" object with the name "+
            sName_+" already exists. Skipped."
        );
    }

    pManager_->AddUIObject(this);

    if (!bVirtual_)
        CreateGlue();

    if (pFrameParent)
        pFrameParent->AddRegion(this);

    s_str sInheritance = pBlock->GetAttribute("inherits");
    if (!sInheritance.IsEmpty(true))
    {
        s_ctnr<s_str> lObjects = sInheritance.Cut(",");
        s_ctnr<s_str>::iterator iter;
        foreach (iter, lObjects)
        {
            iter->Trim(' ');
            s_ptr<GUI::UIObject> pObj = pManager_->GetUIObjectByName(*iter, true);
            if (pObj)
            {
                if (IsObjectType(pObj->GetObjectType()))
                {
                    // Inherit from the other Region
                    CopyFrom(pObj);
                }
                else
                {
                    Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                        "\""+sName_+"\" ("+lType_.Back()+") cannot inherit "
                        "from \""+(*iter)+"\" ("+pObj->GetObjectType()+"). Inheritance skipped."
                    );
                }
            }
            else
            {
                Warning(pBlock->GetFile()+":"+pBlock->GetLineNbr(),
                    "Couldn't find inherited object \""+(*iter)+"\". Inheritance skipped."
                );
            }
        }
    }

    if ((pBlock->IsProvided("hidden") || !bInherits_) &&
        (s_bool(pBlock->GetAttribute("hidden"))))
        Hide();

    if ((pBlock->IsProvided("setAllPoints") || !bInherits_) &&
        (s_bool(pBlock->GetAttribute("setAllPoints"))))
        SetAllPoints("$parent");
}
