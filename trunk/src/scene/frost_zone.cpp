/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Zone source              */
/*                                        */
/*                                        */

#include "scene/frost_zone.h"

#include "scene/frost_terrainchunk.h"
#include "camera/frost_camera.h"

using namespace std;

namespace Frost
{
    Zone::Zone( const s_str& sName ) :
        sName_(sName)
    {
    }

    Zone::~Zone()
    {
        s_map< s_uint, s_ptr<TerrainChunk> >::iterator iterChunk;
        foreach (iterChunk, lChunkList_)
        {
            iterChunk->second.Delete();
        }
    }

    void Zone::SetAmbientColor( const Color& mColor )
    {
        mAmbientColor_ = mColor;
    }

    const Color& Zone::GetAmbientColor() const
    {
        return mAmbientColor_;
    }

    void Zone::SetSunColor( const Color& mColor )
    {
        mSunColor_ = mColor;
    }

    const Color& Zone::GetSunColor() const
    {
        return mSunColor_;
    }

    s_uint Zone::AddChunk( s_ptr<TerrainChunk> pChunk )
    {
        lChunkList_[lChunkList_.GetSize()] = pChunk;
        return lChunkList_.GetSize()-1;
    }

    void Zone::UpdateChunks( s_ptr<Camera> pCamera )
    {
        s_map< s_uint, s_ptr<TerrainChunk> >::iterator iterChunk;
        foreach (iterChunk, lChunkList_)
        {
            s_ptr<TerrainChunk> pChunk = iterChunk->second;
            if (!pChunk->IsAlwaysVisible())
            {
                s_float fDist = (pChunk->GetPosition() - pCamera->GetPosition(false)).GetNorm();
                if (fDist < pCamera->GetFarClipDistance())
                {
                    pChunk->Load();

                    AxisAlignedBox mBox(
                        pChunk->GetPosition() - pChunk->GetSize()/2.0f,
                        pChunk->GetPosition() + pChunk->GetSize()/2.0f
                    );

                    if (pCamera->IsVisible(mBox))
                    {
                        pChunk->Show();
                    }
                    else
                    {
                        pChunk->Hide();
                    }
                }
                else
                {
                    pChunk->Unload();
                }
            }
            else
            {
                pChunk->Show();
            }
        }
    }

    const s_str& Zone::GetName()
    {
        return sName_;
    }
}


