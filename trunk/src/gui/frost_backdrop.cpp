/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Backdrop source            */
/*                                        */
/*                                        */

#include "gui/frost_backdrop.h"

#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "gui/frost_sprite.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Backdrop::CLASS_NAME = "GUI::Backdrop";

Backdrop::Backdrop( s_ptr<Frame> pParent ) :
    pParent_(pParent)
{
}

Backdrop::~Backdrop()
{
}

void Backdrop::CopyFrom( s_ptr<Backdrop> pBackdrop )
{

}

void Backdrop::SetBackground( const s_str& sBackgroundFile )
{
    pBackground_ = s_refptr<Sprite>(new Sprite(MaterialManager::GetSingleton()->CreateMaterial2D(sBackgroundFile)));
}

void Backdrop::SetBackgroundColor( const Color& mColor )
{
    if (pBackground_)
        pBackground_->SetColor(mColor);
}

const Color& Backdrop::GetBackgroundColor() const
{
    if (pBackground_)
        return pBackground_->GetColor();
    else
        return Color::NaN;
}

void Backdrop::SetBackgroundTilling( const s_bool& bBackgroundTilling )
{
    bBackgroundTilling_ = bBackgroundTilling;
}

const s_bool& Backdrop::IsBackgroundTilling() const
{
    return bBackgroundTilling_;
}

void Backdrop::SetBackgroundInsets( const s_array<s_int,4>& lInsets )
{
    lBackgroundInsets_ = lInsets;
}

void Backdrop::SetBackgroundInsets( const s_int& iLeft, const s_int& iTop, const s_int& iRight, const s_int& iBottom )
{
    lBackgroundInsets_ = (iLeft, iTop, iRight, iBottom);
}

void Backdrop::SetEdge( const s_str& sEdgeFile )
{
    s_refptr<Material> pEdgeMat = MaterialManager::GetSingleton()->CreateMaterial2D(sEdgeFile);

    if (pEdgeMat->GetWidth()/pEdgeMat->GetHeight() == 8.0f)
    {
        for (int i = 0; i < 8; ++i)
            lEdgeList_[i] = s_refptr<Sprite>(new Sprite(pEdgeMat));

        uiEdgeSize_ = uiOriginalEdgeSize_ = s_uint(pEdgeMat->GetHeight());
    }
    else
    {
        lEdgeList_.Set(s_refptr<Sprite>(NULL));

        Error(CLASS_NAME,
            "An edge file's width must be exactly 8 times greater than its height "
            "(in "+sEdgeFile+")."
        );
    }
}

void Backdrop::SetEdgeColor( const Color& mColor )
{
    if (lEdgeList_[0])
    {
        for (int i = 0; i < 8; ++i)
            lEdgeList_[i]->SetColor(mColor);
    }
}

const Color& Backdrop::GetEdgeColor() const
{
    if (lEdgeList_[0])
        return lEdgeList_[0]->GetColor();
    else
        return Color::NaN;
}

void Backdrop::SetEdgeSize( const s_uint& uiEdgeSize )
{
    uiEdgeSize_ = uiEdgeSize;
}

void Backdrop::Render() const
{
    s_float fEdgeScale = s_float(uiEdgeSize_)/s_float(uiOriginalEdgeSize_);
}
