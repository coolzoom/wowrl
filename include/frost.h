/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */
/*                                        */


#define GAME_VERSION 0.045

#ifndef NULL
    #define NULL 0
#endif

#include "frost_stableheaders.h"

namespace Frost
{
    // frost_animmanager.h
    struct Animation;
    struct AnimationSequence;
    struct MeshAnimation;
    class  AnimManager;

    // frost_camera.h
    class  Camera;

    // frost_camera_chasingcamera.h
    class  ChasingCamera;

    // frost_camera_freecamera.h
    class  FreeCamera;

    // frost_camera_topcamera.h
    class  TopCamera;

    // frost_cameramanager.h
    class  CameraManager;

    // frost_character.h
    struct Race;
    class  Character;
    class  LuaCharacter;

    // frost_creature.h
    class  Creature;
    class  LuaCreature;

    // frost_decal.h
    class  Decal;

    // frost_engine.h
    class  Engine;

    // frost_fontmanager.h
    class  FontManager;

    // frost_gameplay.h
    class  Gameplay;
    class  LuaGameplay;

    // frost_gameplaymanager.h
    class  GameplayManager;

    namespace GUI
    {
        // frost_gui_anchor.h
        class  Anchor;

        // frost_gui_gradient.h
        class  Gradient;

        // frost_gui_uiobject.h
        class  UIObject;
        class  LuaUIObject;

        // frost_gui_frame.h
        struct Layer;
        struct Level;
        struct Strata;
        class  Frame;
        class  LuaFrame;

        // frost_gui_button.h
        class  Button;
        class  LuaButton;

        // frost_gui_editbox.h
        class  EditBox;
        class  LuaEditBox;

        // frost_gui_scrollingmessageframe.h
        class  ScrollingMessageFrame;
        class  LuaScrollingMessageFrame;

        // frost_gui_slider.h
        class  Slider;
        class  LuaSlider;

        // frost_gui_statusbar.h
        class  StatusBar;
        class  LuaStatusBar;

        // frost_gui_layeredregion.h
        class  LayeredRegion;
        class  LuaLayeredRegion;

        // frost_gui_fontstring.h
        class  FontString;
        class  LuaFontString;

        // frost_gui_texture.h
        class  Texture;
        class  LuaTexture;
    }

    // frost_guimanager.h
    struct AddOn;
    class  GUIManager;

    // frost_healthtype.h
    struct HealthType;
    class  HealthTypeInstance;

    // frost_inputmanager.h
    class  InputManager;

    // frost_light.h
    class  Light;

    // frost_lightmanager.h
    class  LightManager;

    // frost_localemanager.h
    class  LocaleManager;

    // frost_material.h
    class  Material;

    // frost_materialmanager.h
    class  MaterialManager;

    // frost_model.h
    class  Model;
    class  ModelPart;

    // frost_modeldata.h
    class  ModelData;

    // frost_modelmanager.h
    class  ModelManager;

    // frost_movableobject.h
    class  MovableObject;
    class  LuaMovableObject;

    // frost_node.h
    class  Node;

    // frost_path.h
    class  Path;

    // frost_path_directpath.h
    struct DirectPathPoint;
    class  DirectPath;

    // frost_path_manualpath.h
    struct ManualPathPoint;
    class  ManualPath;

    // frost_path_smoothpath.h
    struct SmoothPathPoint;
    class  SmoothPath;

    // frost_pathmanager.h
    class  PathManager;

    // frost_plane.h
    class  Plane;

    // frost_powertype.h
    struct PowerType;
    class  PowerTypeInstance;

    // frost_scenemanager.h
    class  SceneManager;

    // frost_spell.h
    class  Spell;

    // frost_sprite.h
    class  Sprite;

    // frost_spritemanager.h
    struct Vertex;
    struct Quad;
    class  RenderTarget;
    class  SpriteManager;

    // frost_stats.h
    template<class T>
    class  Bonus;
    struct BonusSet;
    template<class T>
    class  Characteristic;
    struct Stats;

    // frost_terrain.h
    class  Terrain;

    // frost_terraindata.h
    class  TerrainData;

    // frost_text.h
    struct Line;
    struct Format;
    class  Text;

    // frost_unit.h
    struct Class;
    class  Unit;
    class  LuaUnit;

    // frost_unitmanager.h
    class  UnitManager;

    namespace XML
    {
        // frost_xml_block.h
        class  Block;
        struct Attribute;
        struct PredefinedBlock;

        // frost_xml_document.h
        class  Document;
    }
}
