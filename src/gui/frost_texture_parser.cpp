/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_texture.h"

#include "xml/frost_xml_document.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

void Texture::ParseBlock( s_ptr<XML::Block> pBlock )
{
    ParseAttributes_(pBlock);

    ParseSizeBlock_(pBlock);
    ParseAnchorsBlock_(pBlock);
    ParseTexCoordsBlock_(pBlock);
    ParseColorBlock_(pBlock);
    ParseGradientBlock_(pBlock);
}

void Texture::ParseAttributes_( s_ptr<XML::Block> pBlock )
{
    LayeredRegion::ParseAttributes_(pBlock);

    s_str sFile = pBlock->GetAttribute("file");
    if (!sFile.IsEmpty())
    {
        SetTexture(GUIManager::GetSingleton()->ParseFileName(
            pBlock->GetAttribute("file")
        ));
    }
}

void Texture::ParseTexCoordsBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pTexCoordsBlock = pBlock->GetBlock("TexCoords");
    if (pTexCoordsBlock)
    {
        SetTexCoord(s_array<s_float,4>((
            s_float(pTexCoordsBlock->GetAttribute("left")),
            s_float(pTexCoordsBlock->GetAttribute("right")),
            s_float(pTexCoordsBlock->GetAttribute("top")),
            s_float(pTexCoordsBlock->GetAttribute("bottom"))
        )));
    }
}

void Texture::ParseColorBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pColorBlock = pBlock->GetBlock("Color");
    if (pColorBlock)
    {
        SetColor(Color(
            s_uchar(s_float(pColorBlock->GetAttribute("a"))*255.0f),
            s_uchar(s_float(pColorBlock->GetAttribute("r"))*255.0f),
            s_uchar(s_float(pColorBlock->GetAttribute("g"))*255.0f),
            s_uchar(s_float(pColorBlock->GetAttribute("b"))*255.0f)
        ));
    }
}

void Texture::ParseGradientBlock_( s_ptr<XML::Block> pBlock )
{
    s_ptr<XML::Block> pGradientBlock = pBlock->GetBlock("Gradient");
    if (pGradientBlock)
    {
        s_str sOrientation = pGradientBlock->GetAttribute("orientation");
        GUI::Gradient::Orientation mOrient;
        if (sOrientation == "HORIZONTAL")
            mOrient = GUI::Gradient::HORIZONTAL;
        else if (sOrientation == "VERTICAL")
            mOrient = GUI::Gradient::VERTICAL;
        else
        {
            Warning(pGradientBlock->GetFile()+":"+pGradientBlock->GetLineNbr(),
                "Unknown gradient orientation for "+sName_+" : \""+sOrientation+"\". "
                "No gradient will be shown for this Texture."
            );
            return;
        }

        Color mMinColor, mMaxColor;
        s_ptr<XML::Block> pMinColorBlock = pGradientBlock->GetBlock("MinColor");
        mMinColor.SetA(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("a"))));
        mMinColor.SetR(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("r"))));
        mMinColor.SetG(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("g"))));
        mMinColor.SetB(s_uchar(255.0f*s_float(pMinColorBlock->GetAttribute("b"))));


        s_ptr<XML::Block> pMaxColorBlock = pGradientBlock->GetBlock("MaxColor");
        mMaxColor.SetA(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("a"))));
        mMaxColor.SetR(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("r"))));
        mMaxColor.SetG(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("g"))));
        mMaxColor.SetB(s_uchar(255.0f*s_float(pMaxColorBlock->GetAttribute("b"))));

        SetGradient(GUI::Gradient(mOrient, mMinColor, mMaxColor));
    }
}
