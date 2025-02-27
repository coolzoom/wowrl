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
#include "material/frost_material.h"

#include <frost_utils_axisalignedbox.h>
#include <frost_xml.h>

#include <OgreVector4.h>

namespace Frost
{
    class ModelMaterial
    {
    public :

        ModelMaterial();
        ModelMaterial(s_ptr<XML::Block> pBlock);

        void AddMaterialDefinition(const MaterialDefinition& mMatDef, const s_int& iSubMesh = -1, const s_int& iSubEntity = -1);
        void ApplyOn(s_wptr<Model> pModel, const s_bool& bPostProcess = true) const;

        s_str Serialize() const;
        void  SerializeIn(File& mFile) const;

        void Clear(const s_int& iSubMesh = -1, const s_int& iSubEntity = -1);

    private :

        s_map< s_int, s_map<s_int, MaterialDefinition> > lMaterialList_;

    };

    /// A sub-mesh
    class ModelPart
    {
    public :

        ModelPart(s_ptr<Model> pParent, const s_uint& uiID);
        ModelPart(const ModelPart &mMP, s_ptr<Model> pParent);

        void             Show(const s_uint& uiSubEntity = s_uint::NaN);
        void             Hide(const s_uint& uiSubEntity = s_uint::NaN);
        void             Highlight();
        void             Unlight();
        void             SetMaterial(s_refptr<Material> pMat, const s_uint& uiSubEntity = s_uint::NaN);
        s_wptr<Material> GetMaterial(const s_uint& uiSubEntity = s_uint::NaN);
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
        Model(const ModelData& mData, const s_str& sEntityName, s_ptr<Ogre::SceneManager> pSceneManager);

        /// Destructor.
        ~Model();

        const s_bool&       HasAnimation() const;
        s_wptr<AnimManager> GetAnimMgr();
        const s_str&        GetOgreModelName() const;
        const s_str&        GetEntityName() const;
        s_ptr<ModelPart>    GetModelPart(const s_uint& uiID);
        const s_map< s_uint, s_ptr<ModelPart> >& GetModelPartList() const;
        s_ptr<Ogre::Entity> GetEntity();
        s_ptr<Ogre::Mesh>   GetMesh();

        void              CreateObstacles();
        void              CreateObstacle(const s_uint& uiSubMeshID);
        s_ptr<Obstacle>   GetObstacle(const s_uint& uiSubMeshID) const;
        const s_map< s_uint, s_ptr<Obstacle> >& GetObstacleList() const;
        void              Show(s_bool bSubMeshes = false);
        void              Hide(s_bool bSubMeshes = false);
        void              Highlight();
        void              Unlight();
        void              SetMaterial(s_refptr<Material> pMat);
        s_wptr<Material>  GetMaterial();
        void              SetCustomShaderParameter(const s_uint& uiID, const Ogre::Vector4& mVec);

        const AxisAlignedBox& GetBoundingBox() const;

        void              Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    private :

        s_ptr<const ModelData> pModelData_;
        s_ptr<Ogre::Entity>    pEntity_;
        s_ptr<Ogre::Mesh>      pMesh_;
        AxisAlignedBox         mBoundingBox_;

        s_refptr<Material> pMaterial_;

        s_bool                bHasAnimation_;
        s_refptr<AnimManager> pAnimMgr_;

        s_str sOgreModelName_;
        s_str sEntityName_;

        s_bool bHighlighted_;

        s_map< s_uint, s_ptr<ModelPart> > lModelPartList_;
        s_map< s_uint, s_ptr<Obstacle> >  lObstacleList_;
    };
}

#endif
