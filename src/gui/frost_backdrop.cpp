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
#include "gui/frost_frame.h"

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

void Backdrop::CopyFrom( const Backdrop& mBackdrop )
{
    this->SetBackground(mBackdrop.GetBackgroundFile());
    this->SetEdge(mBackdrop.GetEdgeFile());
    this->SetBackgroundTilling(mBackdrop.IsBackgroundTilling());
    this->SetTileSize(mBackdrop.GetTileSize());

    this->SetBackgroundColor(mBackdrop.GetBackgroundColor());
    this->SetBackgroundInsets(mBackdrop.GetBackgroundInsets());

    this->SetEdgeColor(mBackdrop.GetEdgeColor());
    this->SetEdgeSize(mBackdrop.GetEdgeSize());
}

void Backdrop::SetBackground( const s_str& sBackgroundFile )
{
    if (!sBackgroundFile.IsEmpty())
    {
        s_refptr<Material> pMat = MaterialManager::GetSingleton()->CreateMaterial2D(sBackgroundFile);

        uiTileSize_ = uiOriginalTileSize_ = s_uint(pMat->GetWidth());

        pBackground_ = s_refptr<Sprite>(new Sprite(pMat));

    }

    sBackgroundFile_ = sBackgroundFile;
}

const s_str& Backdrop::GetBackgroundFile() const
{
    return sBackgroundFile_;
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

    if (!bBackgroundTilling_ && pBackground_)
    {
        pBackground_->SetTextureRect(0.0f, 0.0f, 1.0f, 1.0f, true);
    }
}

const s_bool& Backdrop::IsBackgroundTilling() const
{
    return bBackgroundTilling_;
}

void Backdrop::SetTileSize( const s_uint& uiTileSize )
{
    uiTileSize_ = uiTileSize;
}

const s_uint& Backdrop::GetTileSize() const
{
    return uiTileSize_;
}

void Backdrop::SetBackgroundInsets( const s_array<s_int,4>& lInsets )
{
    lBackgroundInsets_ = lInsets;
}

void Backdrop::SetBackgroundInsets( const s_int& iLeft, const s_int& iRight, const s_int& iTop, const s_int& iBottom )
{
    lBackgroundInsets_ = (iLeft, iRight, iTop, iBottom);
}

const s_array<s_int,4>& Backdrop::GetBackgroundInsets() const
{
    return lBackgroundInsets_;
}

void Backdrop::SetEdge( const s_str& sEdgeFile )
{
    if (!sEdgeFile.IsEmpty())
    {
        if (File::Exists(sEdgeFile))
        {
            s_refptr<Material> pEdgeMat = MaterialManager::GetSingleton()->CreateMaterial2D(sEdgeFile);

            if (pEdgeMat->GetWidth()/pEdgeMat->GetHeight() == 8.0f)
            {
                uiEdgeSize_ = uiOriginalEdgeSize_ = s_uint(pEdgeMat->GetHeight());

                for (int i = 0; i < 8; ++i)
                {
                    lEdgeList_[i] = s_refptr<Sprite>(new Sprite(
                        pEdgeMat, pEdgeMat->GetHeight()*i, 0.0f,
                        s_float(uiEdgeSize_), s_float(uiEdgeSize_))
                    );
                }

                lEdgeList_[EDGE_TOPLEFT]->SetHotSpot(
                    0.0f, 0.0f
                );
                lEdgeList_[EDGE_TOPRIGHT]->SetHotSpot(
                    s_float(uiOriginalEdgeSize_), 0.0f
                );
                lEdgeList_[EDGE_BOTTOMLEFT]->SetHotSpot(
                    0.0f, s_float(uiOriginalEdgeSize_)
                );
                lEdgeList_[EDGE_BOTTOMRIGHT]->SetHotSpot(
                    s_float(uiOriginalEdgeSize_), s_float(uiOriginalEdgeSize_)
                );

                sEdgeFile_ = sEdgeFile;
            }
            else
            {
                lEdgeList_.Set(s_refptr<Sprite>(NULL));
                sEdgeFile_ = "";

                Error(CLASS_NAME,
                    "An edge file's width must be exactly 8 times greater than its height "
                    "(in "+sEdgeFile+"). No edge will be drawn for "+pParent_->GetName()+"'s backdrop."
                );
            }
        }
        else
        {
            lEdgeList_.Set(s_refptr<Sprite>(NULL));
            sEdgeFile_ = "";

            Warning(CLASS_NAME,
                "Couldn't find file : \""+sEdgeFile+"\" for "+pParent_->GetName()+"'s backdrop edge. "
                "No edge will be drawn."
            );
        }
    }
    else
    {
        sEdgeFile_ = sEdgeFile;
    }
}

const s_str& Backdrop::GetEdgeFile() const
{
    return sEdgeFile_;
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

const s_uint& Backdrop::GetEdgeSize() const
{
    return uiEdgeSize_;
}

void Backdrop::Render() const
{
    if (pParent_)
    {
        const s_array<s_int,4>& lParentBorders = pParent_->GetBorders();

        if (pBackground_)
        {
            if (bBackgroundTilling_)
            {
                pBackground_->SetTextureRect(
                    0.0f, 0.0f,
                    s_float(
                        lParentBorders[BORDER_RIGHT] + lBackgroundInsets_[BORDER_RIGHT] -
                        (lParentBorders[BORDER_LEFT] + lBackgroundInsets_[BORDER_LEFT])
                    )*s_float(uiOriginalTileSize_)/s_float(uiTileSize_),
                    s_float(
                        lParentBorders[BORDER_BOTTOM] - lBackgroundInsets_[BORDER_BOTTOM] -
                        (lParentBorders[BORDER_TOP]   - lBackgroundInsets_[BORDER_TOP])
                    )*s_float(uiOriginalTileSize_)/s_float(uiTileSize_)
                );
            }

            pBackground_->Render2V(
                s_float(lParentBorders[BORDER_LEFT]   + lBackgroundInsets_[BORDER_LEFT]),
                s_float(lParentBorders[BORDER_TOP]    + lBackgroundInsets_[BORDER_TOP]),

                s_float(lParentBorders[BORDER_RIGHT]  - lBackgroundInsets_[BORDER_RIGHT]),
                s_float(lParentBorders[BORDER_BOTTOM] - lBackgroundInsets_[BORDER_BOTTOM])
            );
        }

        if (lEdgeList_[0])
        {
            s_float fEdgeScale = s_float(uiEdgeSize_)/s_float(uiOriginalEdgeSize_);

            // Render corners
            lEdgeList_[EDGE_TOPLEFT]->RenderEx(
                s_float(lParentBorders[BORDER_LEFT]),
                s_float(lParentBorders[BORDER_TOP]),
                0.0f, fEdgeScale, fEdgeScale
            );

            lEdgeList_[EDGE_TOPRIGHT]->RenderEx(
                s_float(lParentBorders[BORDER_RIGHT]),
                s_float(lParentBorders[BORDER_TOP]),
                0.0f, fEdgeScale, fEdgeScale
            );

            lEdgeList_[EDGE_BOTTOMLEFT]->RenderEx(
                s_float(lParentBorders[BORDER_LEFT]),
                s_float(lParentBorders[BORDER_BOTTOM]),
                0.0f, fEdgeScale, fEdgeScale
            );

            lEdgeList_[EDGE_BOTTOMRIGHT]->RenderEx(
                s_float(lParentBorders[BORDER_RIGHT]),
                s_float(lParentBorders[BORDER_BOTTOM]),
                0.0f, fEdgeScale, fEdgeScale
            );

            // Render sides
            s_float fEdgeHeight = s_float(lParentBorders[BORDER_BOTTOM] - lParentBorders[BORDER_TOP] - s_int(2*uiEdgeSize_));

            if (fEdgeHeight > 0.0f)
            {
                lEdgeList_[EDGE_LEFT]->SetTextureRect(
                    0.0f, 0.0f, s_float(uiOriginalEdgeSize_), fEdgeHeight
                );

                lEdgeList_[EDGE_RIGHT]->SetTextureRect(
                    s_float(uiOriginalEdgeSize_), 0.0f, s_float(2*uiOriginalEdgeSize_), fEdgeHeight
                );

                lEdgeList_[EDGE_LEFT]->Render2V(
                    s_float(lParentBorders[BORDER_LEFT]),
                    s_float(lParentBorders[BORDER_TOP]    + s_int(uiEdgeSize_)),

                    s_float(lParentBorders[BORDER_LEFT]   + s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_BOTTOM] - s_int(uiEdgeSize_))
                );

                lEdgeList_[EDGE_RIGHT]->Render2V(
                    s_float(lParentBorders[BORDER_RIGHT]  - s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_TOP]    + s_int(uiEdgeSize_)),

                    s_float(lParentBorders[BORDER_RIGHT]),
                    s_float(lParentBorders[BORDER_BOTTOM] - s_int(uiEdgeSize_))
                );
            }

            s_float fEdgeWidth = s_float(lParentBorders[BORDER_RIGHT] - lParentBorders[BORDER_LEFT] - s_int(2*uiEdgeSize_));

            if (fEdgeWidth > 0.0f)
            {
                lEdgeList_[EDGE_TOP]->SetTextureRect(
                    s_float(2*uiOriginalEdgeSize_), 0.0f, s_float(3*uiOriginalEdgeSize_), fEdgeWidth
                );

                lEdgeList_[EDGE_BOTTOM]->SetTextureRect(
                    s_float(3*uiOriginalEdgeSize_), 0.0f, s_float(4*uiOriginalEdgeSize_), fEdgeWidth
                );

                lEdgeList_[EDGE_TOP]->Render4V(
                    s_float(lParentBorders[BORDER_RIGHT] - s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_TOP]),

                    s_float(lParentBorders[BORDER_RIGHT] - s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_TOP]   + s_int(uiEdgeSize_)),

                    s_float(lParentBorders[BORDER_LEFT]  + s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_TOP]   + s_int(uiEdgeSize_)),

                    s_float(lParentBorders[BORDER_LEFT]  + s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_TOP])
                );

                lEdgeList_[EDGE_BOTTOM]->Render4V(
                    s_float(lParentBorders[BORDER_RIGHT]  - s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_BOTTOM] - s_int(uiEdgeSize_)),

                    s_float(lParentBorders[BORDER_RIGHT]  - s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_BOTTOM]),

                    s_float(lParentBorders[BORDER_LEFT]   + s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_BOTTOM]),

                    s_float(lParentBorders[BORDER_LEFT]   + s_int(uiEdgeSize_)),
                    s_float(lParentBorders[BORDER_BOTTOM] - s_int(uiEdgeSize_))
                );
            }
        }
    }
}
