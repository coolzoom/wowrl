/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Plane source              */
/*                                        */
/*                                        */

#include "scene/frost_plane.h"
#include "material/frost_material.h"

#include <OgreSceneManager.h>
#include <OgreMeshManager.h>
#include <OgreNode.h>
#include <OgreEntity.h>

using namespace std;

namespace Frost
{
    const s_str Plane::CLASS_NAME = "Plane";

    Plane::Plane( const s_uint& uiID ) : mInterface_(this), uiID_(uiID)
    {
        // Use the default plane mesh, link it to our node
        pEntity_ = Engine::GetSingleton()->GetOgreSceneManager()->createEntity(
            ("PlaneEntity_"+uiID).Get(), "Plane"
        );
        pNode_->attachObject(pEntity_.Get());
        pEntity_->setMaterialName("Default3D");
        pEntity_->setUserObject(&mInterface_);
    }

    Plane::Plane( const s_uint& uiID, const s_float& fWidth, const s_float& fHeight, const s_float& fDensity ) : mInterface_(this), uiID_(uiID)
    {
        // Create the mesh
        Ogre::Plane mPlane(Ogre::Vector3::UNIT_Y, 0);
        Ogre::MeshManager::getSingleton().createPlane(
            ("Plane_"+uiID).Get(), "Frost", mPlane,
            fWidth.Get(), fHeight.Get(), s_uint(fWidth*fDensity).Get(), s_uint(fHeight*fDensity).Get(),
            true, 1, 1, 1, Ogre::Vector3::UNIT_Z
        );

        // Link it to our node
        pEntity_ = Engine::GetSingleton()->GetOgreSceneManager()->createEntity(
            ("PlaneEntity_"+uiID).Get(), ("Plane_"+uiID).Get()
        );
        pNode_->attachObject(pEntity_.Get());
        pEntity_->setMaterialName("Default3D");
    }

    Plane::~Plane()
    {
        Engine::GetSingleton()->GetOgreSceneManager()->destroyEntity(
            pEntity_.Get()
        );
    }

    void Plane::Show()
    {
        pEntity_->setVisible(true);
    }

    void Plane::Hide()
    {
        pEntity_->setVisible(false);
    }

    void Plane::SetMaterial( s_refptr<Material> pMat )
    {
        pMaterial_ = pMat;
        if (pMat != NULL)
            pEntity_->setMaterialName(pMat->GetOgreMaterialName().Get());
        else
            pEntity_->setMaterialName("Default3D");
    }

    s_refptr<Material> Plane::GetMaterial()
    {
        return pMaterial_;
    }

    const s_uint& Plane::GetID() const
    {
        return uiID_;
    }

    PlaneOgreInterface::PlaneOgreInterface( s_ptr<Plane> pPlane ) : pPlane_(pPlane)
    {
    }

    const Ogre::String& PlaneOgreInterface::getTypeName() const
    {
        static Ogre::String sName = "PLANE";
        return sName;
    }

    s_ptr<Plane> PlaneOgreInterface::GetPlane() const
    {
        return pPlane_;
    }
}
