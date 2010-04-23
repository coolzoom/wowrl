/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Model header              */
/*                                        */
/*                                        */


#ifndef FROST_MODEL_H
#define FROST_MODEL_H

#include "frost.h"
#include "scene/frost_movableobject.h"

#include <OgreVector4.h>

namespace Frost
{
    /// A sub-mesh
    class ModelPart
    {
    public :

        ModelPart(s_ptr<Model> pParent, const s_uint& uiID);
        ModelPart(const ModelPart &mMP, s_ptr<Model> pParent);

        void             Show();
        void             Hide();
        void             Highlight();
        void             Unlight();
        void             SetMaterial(s_refptr<Material> pMat, const s_uint& uiSubEntity = s_uint::NaN);
        s_wptr<Material> GetMaterial();
        void             SetCustomShaderParameter(const s_uint& uiID, const Ogre::Vector4& mVec);

        void             AddSubEntity(const s_uint& uiID);
        const s_ctnr<s_uint>& GetSubEntityList() const;

        static const s_str CLASS_NAME;

    private :

        s_uint             uiID_;
        s_ptr<Model>       pParent_;
        s_refptr<Material> pMaterial_;

        s_bool bHighlighted_;

        s_ctnr<s_uint>                      lSubEntity_;
        s_map< s_uint, s_refptr<Material> > lSubEntityMaterialList_;
    };

    /// A 3D element (mesh)
    class Model : public MovableObject
    {
    public :

        /// Default constructor.
        Model(const ModelData& mData, const s_str& sEntityName);

        /// Destructor.
        ~Model();

        s_wptr<AnimManager> GetAnimMgr();
        const s_str&        GetOgreModelName() const;
        const s_str&        GetEntityName() const;
        s_ptr<ModelPart>    GetModelPart(const s_uint& uiID);
        const s_map< s_uint, s_ptr<ModelPart> >& GetModelPartList() const;
        s_ptr<Ogre::Entity> GetEntity();
        s_ptr<Ogre::Mesh>   GetMesh();

        void              Show();
        void              Hide(s_bool bSubMeshes = false);
        void              Highlight();
        void              Unlight();
        void              SetMaterial(s_refptr<Material> pMat);
        s_wptr<Material>  GetMaterial();
        void              SetCustomShaderParameter(const s_uint& uiID, const Ogre::Vector4& mVec);

        const Vector&     GetEllipsoidRadius() const;

        void              Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :

        s_ptr<Ogre::Mesh>   pMesh_;

        s_refptr<Material> pMaterial_;

        s_refptr<AnimManager> pAnimMgr_;

        s_str sOgreModelName_;
        s_str sEntityName_;

        s_bool bHighlighted_;

        s_map< s_uint, s_ptr<ModelPart> > lModelPartList_;
    };
}

#endif
