/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#define GAME_VERSION 0.045

#ifndef NULL
    #if !MSVC
        #define NULL ((void*)0)
    #endif
#endif

#include "frost_stableheaders.h"

namespace Frost
{
    /* CAMERA ********************************************/
    // camera/frost_camera.h
    class  Camera;

    // camera/frost_cameramanager.h
    class  CameraManager;

    /* GAMEPLAY ******************************************/
    // gameplay/frost_gameplay.h
    class  Gameplay;
    class  LuaGameplay;

    // gameplay/frost_gameplaymanager.h
    class  GameplayManager;

    /* GUI ***********************************************/
    // gui/frost_modelframe.h
    namespace GUI
    {
        class ModelFrame;
        class LuaModelFrame;
    }

    /* MATERIAL ******************************************/
    // material/frost_decal.h
    class  Decal;

    // material/frost_material.h
    class  Material;
    class  MaterialDefinition;

    // material/frost_materialmanager.h
    class  MaterialManager;

    // material/frost_rendertarget.h
    class  RenderTarget;

    // material/frost_shader.h
    struct AutoParam;
    struct SamplerParam;
    class  Shader;
    class  VertexShader;
    class  PixelShader;

    // material/frost_shadermanager.h
    class  ShaderManager;

    /* MODEL *********************************************/
    // model/frost_animmanager.h
    struct Animation;
    struct AnimationSequence;
    struct MeshAnimation;
    class  AnimManager;

    // model/frost_fmmodeldata.h
    class  FMModelData;

    // model/frost_m2modeldata.h
    class  M2ModelData;

    // model/frost_manualmodeldata.h
    namespace ManualModel
    {
        struct Vertex;
        struct Triangle;
        struct SubMesh;
        struct Bone;
        struct Animation;
        struct Model;
    }

    class ManualModelData;

    // model/frost_model.h
    class  Model;
    class  ModelPart;
    class  ModelMaterial;

    // model/frost_modeldata.h
    class  ModelData;

    // model/frost_modelmanager.h
    class  ModelManager;

    // model/frost_ogremeshmodeldata.h
    class  OgreMeshModelData;

    /* PATH **********************************************/
    // path/frost_path.h
    class  Path;

    // path/frost_directpath.h
    class  DirectPath;

    // path/frost_manualpath.h
    class  ManualPath;

    // path/frost_smoothpath.h
    class  SmoothPath;

    /* SCENE *********************************************/
    // scene/frost_axisalignedobstacle.h
    class  AxisAlignedObstacle;

    // scene/frost_boxobstacle.h;
    class  BoxObstacle;

    // scene/frost_cylinderobstacle.h
    class  CylinderObstacle;

    // scene/frost_guizmo.h
    class  Gizmo;

    // scene/frost_light.h
    class  Light;

    // scene/frost_lightmanager.h
    class  LightManager;

    // scene/frost_meshobstacle.h
    class  MeshObstacle;

    // scene/frost_movableobject.h
    class  MovableObject;
    class  LuaMovableObject;

    // scene/frost_node.h
    class  Node;

    // scene/frost_obstacle.h
    struct CollisionData;
    class  Obstacle;
    class  MovableObstacle;

    // scene/frost_physicshandler.h
    class  PhysicsHandler;

    // scene/frost_physicsmanager.h
    class  PhysicsManager;

    // scene/frost_plane.h
    class  Plane;

    // scene/frost_planeobstacle.h
    class  PlaneObstacle;

    // scene/frost_scenemanager.h
    class  SceneManager;

    // scene/frost_sceneobject.h
    class  OgreInterface;
    class  SceneObject;

    // scene/frost_sphereobstacle.h
    class  SphereObstacle;

    // scene/frost_terrainchunk.h
    class  TerrainChunk;

    // scene/frost_terrainobstacle.h
    class  TerrainObstacle;

    /* ***************************************************/

    // frost_engine.h
    class  Engine;

    // frost_engine_glues.h
    class  LuaEngine;

    /* ***************************************************/

    // frost_inputmanager.h
    class  InputManager;

    // frost_localemanager.h
    class  LocaleManager;
}
