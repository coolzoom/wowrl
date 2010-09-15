/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_modelframe.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void ModelFrame::ParseBlock( s_ptr<XML::Block> pBlock )
{
    Frame::ParseBlock(pBlock);

    if ((pBlock->IsProvided("file") || !pBlock->IsProvided("file")))
        SetModelFile(pBlock->GetAttribute("file"));

    if ((pBlock->IsProvided("scale") || !pBlock->IsProvided("scale")))
        SetModelScale(s_float(pBlock->GetAttribute("scale")));

    ParseMaterialsBlock_(pBlock);
}

void ModelFrame::ParseMaterialsBlock_( s_ptr<XML::Block> pBlock )
{
    if (!pModel_)
        return;

    s_ptr<XML::Block> pMaterialsBlock = pBlock->GetBlock("Materials");
    if (pMaterialsBlock)
    {
        s_map< s_int, s_ctnr<s_int> > lMatList;

        s_ptr<XML::Block> pMaterialBlock;
        foreach_block (pMaterialBlock, pMaterialsBlock)
        {
            s_int iID = s_int(pMaterialBlock->GetAttribute("subMeshID"));
            s_int iEntityID = s_int(pMaterialBlock->GetAttribute("subEntityID"));
            if (iID < 0)
                iID = -1;
            if (iEntityID < 0)
                iEntityID = -1;

            if (lMatList.Find(iID) && lMatList[iID].Find(iEntityID))
            {
                Warning(pMaterialBlock->GetFile()+":"+pMaterialBlock->GetLineNbr(),
                    "A material has already been defined for \""+sName_+"\""+
                    (iID < 0 ? "." : ("'s submesh "+iID+
                    (iEntityID < 0 ? "." : ("(subentity "+iEntityID+")."))))
                );
                continue;
            }

            s_ptr<XML::Block> pDiffuseBlock = pMaterialBlock->GetRadioBlock();
            if (pDiffuseBlock->GetName() == "DiffuseColor")
            {
                Color mColor(
                    s_float(pDiffuseBlock->GetAttribute("a")),
                    s_float(pDiffuseBlock->GetAttribute("r")),
                    s_float(pDiffuseBlock->GetAttribute("g")),
                    s_float(pDiffuseBlock->GetAttribute("b"))
                );

                if (iID < 0)
                    SetModelTexture(mColor);
                else if (iEntityID < 0)
                    SetSubMeshTexture(s_uint(iID), mColor);
                else
                    SetSubEntityTexture(s_uint(iID), s_uint(iEntityID), mColor);
            }
            else
            {
                s_str sFile = pDiffuseBlock->GetAttribute("file");
                s_bool bAlphaReject = s_bool(pDiffuseBlock->GetAttribute("alphaReject"));

                if (iID < 0)
                    SetModelTexture(sFile, bAlphaReject);
                else if (iEntityID < 0)
                    SetSubMeshTexture(s_uint(iID), sFile, bAlphaReject);
                else
                    SetSubEntityTexture(s_uint(iID), s_uint(iEntityID), sFile, bAlphaReject);
            }

            lMatList[iID].PushBack(iEntityID);
        }
    }
}
