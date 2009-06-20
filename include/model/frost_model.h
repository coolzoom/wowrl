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

namespace Frost
{
    /// A sub-mesh
    class ModelPart
    {
    public :

        ModelPart(s_ptr<Model> pParent, const s_uint& uiID);
        ModelPart(const ModelPart &mMP, s_ptr<Model> pParent);

        void               Show();
        void               Hide();
        void               SetMaterial(s_refptr<Material> pMat);
        s_refptr<Material> GetMaterial();

        void               AddSubEntity(const s_uint& uiID);

        static const s_str CLASS_NAME;

    private :

        s_ctnr<s_uint>     lSubEntity_;
        s_ptr<Model>       pParent_;
        s_refptr<Material> pMaterial_;
        s_uint             uiID_;
    };

    /// A 3D element (mesh)
    class Model : public MovableObject
    {
    public :

        /// Default constructor
        Model(const ModelData& mData, const s_str& sEntityName);

        /// Destructor
        ~Model();

        s_refptr<AnimManager> GetAnimMgr();
        const s_str&          GetModelName() const;
        const s_str&          GetEntityName() const;
        s_ptr<ModelPart>      GetModelPart(const s_uint& uiID);
        const s_map< s_uint, s_ptr<ModelPart> >& GetModelPartList() const;
        s_ptr<Ogre::Entity>   GetEntity();

        void                  Show();
        void                  Hide(s_bool bSubMeshes = false);
        void                  SetMaterial(s_refptr<Material> pMat);
        s_refptr<Material>    GetMaterial();

        void                  SetUserObject(s_ptr<Ogre::UserDefinedObject> pObj);

        void                  Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :

        s_ptr<Ogre::Mesh>   pMesh_;
        s_ptr<Ogre::Entity> pEntity_;

        s_refptr<Material> pMaterial_;

        s_refptr<AnimManager> pAnimMgr_;

        s_str sModelName_;
        s_str sEntityName_;

        s_map< s_uint, s_ptr<ModelPart> > lModelPartList_;
    };
}

#endif
