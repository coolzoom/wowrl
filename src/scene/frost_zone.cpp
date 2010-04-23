/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Zone source              */
/*                                        */
/*                                        */

#include "scene/frost_zone.h"

#include "scene/frost_terrainchunk.h"
#include "scene/frost_doodad.h"
#include "model/frost_modelmanager.h"
#include "camera/frost_cameramanager.h"
#include "camera/frost_camera.h"
#include "material/frost_materialmanager.h"
#include "material/frost_material.h"
#include "material/frost_decal.h"
#include "material/frost_shadermanager.h"
#include "model/frost_model.h"

#include <OgreCamera.h>

using namespace std;

namespace Frost
{
    const s_str Zone::CLASS_NAME = "Zone";

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

        s_map< s_str, s_ptr<Doodad> >::iterator iterDoodad;
        foreach (iterDoodad, lDoodadList_)
        {
            iterDoodad->second.Delete();
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

    s_bool Zone::RegisterModel( const s_str& sName, const s_str& sFile )
    {
        if (!ModelManager::GetSingleton()->LinkModelNameToFile("Zone", sName, sFile))
            return false;

        if (!ModelManager::GetSingleton()->PreloadModel("Zone", sName))
            return false;

        lModelList_[sName] = sFile;

        return true;
    }

    void Zone::SetMaterialInfo( const s_str& sModelName, const s_map<s_int, s_map<s_int, MaterialInfo> >& lMatInfo )
    {
        lMaterialInfoList_[sModelName] = lMatInfo;
    }

    s_ptr<Doodad> Zone::AddDoodad( const s_str& sName, const s_str& sModelName )
    {
        s_ptr<Doodad> pDoodad;

        if (!lDoodadList_.Find(sName) && ModelManager::GetSingleton()->IsModelAvailable("Zone", sModelName))
        {
            pDoodad = new Doodad(sName, sModelName, this);
            lDoodadList_[sName] = pDoodad;

            if (s_refptr<Model> pModel = pDoodad->GetModel().Lock())
            {
                if (lMaterialInfoList_.Find(sModelName))
                {
                    const s_map<s_int, s_map<s_int, MaterialInfo> >& lMap = lMaterialInfoList_[sModelName];
                    s_map<s_int, s_map<s_int, MaterialInfo> >::const_iterator iterMap;
                    foreach (iterMap, lMap)
                    {
                        const s_map<s_int, MaterialInfo>& lSubMap = iterMap->second;
                        s_map<s_int, MaterialInfo>::const_iterator iterMat;
                        foreach (iterMat, lSubMap)
                        {
                            s_refptr<Material> pMat;
                            if (iterMat->second.bDiffuseColor)
                            {
                                pMat = MaterialManager::GetSingleton()->CreateMaterial3D(iterMat->second.mDiffuseColor);
                                pMat->SetShaders("SimpleColor");
                            }
                            else
                            {
                                pMat = MaterialManager::GetSingleton()->CreateMaterial3D(iterMat->second.sDiffuseFile);
                                pMat->SetShaders("SimpleTexture");
                            }

                            if (iterMat->second.bAlphaReject)
                                pMat->SetAlphaReject(true);

                            if (iterMap->first == -1)
                            {
                                pModel->SetMaterial(pMat);
                            }
                            else
                            {
                                s_ptr<ModelPart> pPart = pModel->GetModelPart(s_uint(iterMap->first));
                                if (pPart)
                                {
                                    if (iterMat->first == -1)
                                    {
                                        pPart->SetMaterial(pMat);
                                    }
                                    else
                                    {
                                        pPart->SetMaterial(pMat, s_uint(iterMat->first));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return pDoodad;
    }

    s_ptr<Doodad> Zone::GetDoodadByName( const s_str& sName ) const
    {
        s_map< s_str, s_ptr<Doodad> >::const_iterator iterDoodad = lDoodadList_.Get(sName);
        if (iterDoodad != lDoodadList_.End())
            return iterDoodad->second;
        else
            return nullptr;
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
            if (iterSorted->second->GetRayIntersection(mRayOrigin, mRayDirection, mPos))
                break;
        }

        return mPos;
    }

    s_float Zone::GetTerrainHeight( const s_float& fX, const s_float& fZ ) const
    {
        s_map< s_uint, s_ptr<TerrainChunk> >::const_iterator iterChunk;
        foreach (iterChunk, lChunkList_)
        {
            if (iterChunk->second->ContainsPoint(fX, fZ))
            {
                const Vector& mPosition = iterChunk->second->GetPosition();
                s_float fHeight = iterChunk->second->GetPointHeight(fX - mPosition.X(), fZ - mPosition.Z());
                if (fHeight.IsValid())
                    return fHeight;
            }
        }

        return s_float::NaN;
    }

    void Zone::Update( const s_float& fDelta )
    {
        s_map< s_str, s_ptr<Doodad> >::iterator iterDoodad;
        foreach (iterDoodad, lDoodadList_)
        {
            iterDoodad->second->Update(fDelta);
        }
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
