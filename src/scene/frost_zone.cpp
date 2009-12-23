/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Zone source              */
/*                                        */
/*                                        */

#include "scene/frost_zone.h"

#include "scene/frost_terrainchunk.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "material/frost_decal.h"

#include <OgreCamera.h>

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

    void Zone::AddDecal( s_wptr<Decal> pDecal )
    {
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            s_map< s_uint, s_ptr<TerrainChunk> >::iterator iterChunk;
            foreach (iterChunk, lChunkList_)
            {
                pLocked->AddMaterial(iterChunk->second->GetMaterial());
            }
        }
    }

    void Zone::RemoveDecal( s_wptr<Decal> pDecal )
    {
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            s_map< s_uint, s_ptr<TerrainChunk> >::iterator iterChunk;
            foreach (iterChunk, lChunkList_)
            {
                pLocked->RemoveMaterial(iterChunk->second->GetMaterial());
            }
        }
    }

    Vector Zone::GetTerrainUnderMouse( const s_float& fX, const s_float& fY ) const
    {
        s_map< s_float, s_ptr<TerrainChunk> > lSortedChunkList;
        s_ptr<Camera> pMainCam = CameraManager::GetSingleton()->GetMainCamera();
        Ogre::Ray mRay = pMainCam->GetOgreCamera()->getCameraToViewportRay(fX.Get(), fY.Get());

        Vector mRayOrigin    = Vector::OgreToFrost(mRay.getOrigin());
        Vector mRayDirection = Vector::OgreToFrost(mRay.getDirection());

        s_map< s_uint, s_ptr<TerrainChunk> >::const_iterator iterChunk;
        foreach (iterChunk, lChunkList_)
        {
            s_float fDistance = (iterChunk->second->GetPosition() - pMainCam->GetPosition()).GetNorm();
            lSortedChunkList[fDistance] = iterChunk->second;
        }

        Vector mPos = Vector::NaN;
        s_map< s_float, s_ptr<TerrainChunk> >::iterator iterSorted;
        foreach (iterSorted, lSortedChunkList)
        {
            if (iterSorted->second->GetRayIntersection(mRayOrigin, mRayDirection, mPos));
                break;
        }

        return mPos;
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

                    if (pCamera->IsVisible(pChunk->GetBoundingBox()))
                        pChunk->Show();
                    else
                        pChunk->Hide();
                }
                else
                    pChunk->Unload();
            }
            else
                pChunk->Show();
        }
    }

    const s_str& Zone::GetName()
    {
        return sName_;
    }
}
