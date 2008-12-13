#ifndef FROST_TE_MAINWINDOW_H
#define FROST_TE_MAINWINDOW_H

#include "frost_te.h"
#include "frost_te_terrain.h"
#include "frost_te_tool.h"

#include <gtkmm/window.h>
#include <gtkmm/frame.h>
#include <gtkmm/box.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/label.h>
#include <gtkmm/separator.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/table.h>
#include <gtkmm/eventbox.h>
#include <gtkmm/iconfactory.h>

#include "frost_te_gtkogrewindow.h"
#include "frost_sprite.h"

#include <OgreRoot.h>

namespace Frost
{
namespace TE
{
    enum DragType
    {
        DRAG_NONE,
        DRAG_LEFT,
        DRAG_BOTH,
        DRAG_RIGHT
    };

    class MainWindow : public Gtk::Window
    {
    public :

        MainWindow();

        virtual ~MainWindow();

        void Initialize(Ogre::RenderWindow*, uint, uint);

        bool Iteration();

        s_bool Render2D();

        s_ptr<Ogre::RenderWindow> GetOgreRenderWindow();
        s_refptr<Ogre::Root>      GetOgreRoot();
        s_ptr<Ogre::SceneManager> GetOgreSceneMgr();
        s_ptr<Ogre::SceneNode>    GetCameraNode();
        s_ptr<Ogre::SceneNode>    GetCameraTargetNode();
        s_ptr<TE::Terrain>        GetTerrain();

        s_bool                    IsInfluenceActive();
        s_float                   GetInfluence();
        void                      SetInfluence(const s_float& fNewInfluence);

        GtkOgreWidget mOgreWindow;

    protected :

        void MenuFileNew();
        void MenuFileOpen();
        void MenuFileSave();
        void MenuFileSaveAs();
        void MenuFileQuit();

        void MenuEditUndo();
        void MenuEditRedo();

        void MenuToolCamera();
        void MenuToolHeight();

        void MenuViewSwitch2D3D();
        void MenuViewLighting();
        void MenuViewWireframe();
        void MenuView2Sided();
        void MenuViewTextured();
        void MenuViewGrid();

        void MenuHelpHelp();
        void MenuHelpAbout();

        void OnPosChanged();
        void OnParamChanged();
        void OnInfluenceChanged();
        void OnInfluenceChecked();

        void OnApply();

        bool OnMouseMoved(GdkEventMotion* pMotion);
        bool OnButtonPressed(GdkEventButton* pButton);
        bool OnButtonReleased(GdkEventButton* pButton);
        bool OnMouseScrolled(GdkEventScroll* pScroll);

        void CreateMesh();
        void StartDrag(const DragType& mType);
        void Drag(float dx, float dy);
        void StopDrag();

        Vector GetVertexUnderMouse(float x, float y);
        void   UpdateDecal(float x, float y);

        Glib::RefPtr<Gtk::ActionGroup> pActionGroup;
        Glib::RefPtr<Gtk::ActionGroup> pToolActionGroup;
        Glib::RefPtr<Gtk::UIManager>   pUIManager;

        Glib::RefPtr<Gtk::IconFactory> pIconFactory;

        Gtk::VBox mMainBox;
        Gtk::HBox mContentBox;

        // Controls
        Gtk::Frame mSideFrame;
        Gtk::VBox  mSideBox;

        // Terrain dimensions
        Gtk::Frame mTerrainDimFrame;
        Gtk::VBox  mTerrainDimBox;
        Gtk::Frame mTerrainDimNumFrame;
        Gtk::HBox  mTerrainDimNumBox;
        Gtk::Frame mTerrainDimSizeFrame;
        Gtk::VBox  mTerrainDimSizeBox;
        Gtk::HBox  mTerrainDimXYSizeBox;
        Gtk::HBox  mTerrainDimZSizeBox;

        // Number of X points
        Gtk::Label      mNumXPointLabel;
        Gtk::SpinButton mNumXPointEntry;

        // Number of Y points
        Gtk::Label      mNumZPointLabel;
        Gtk::SpinButton mNumZPointEntry;

        // X size
        Gtk::Label      mXSizeLabel;
        Gtk::SpinButton mXSizeEntry;

        // Y size
        Gtk::Label      mYSizeLabel;
        Gtk::SpinButton mYSizeEntry;

        // Z size
        Gtk::HBox       mZSizeBox;
        Gtk::Label      mZSizeLabel;
        Gtk::SpinButton mZSizeEntry;

        // Point editing
        Gtk::Frame mPointEditFrame;
        Gtk::VBox  mPointEditBox;
        Gtk::HBox  mPointPosBox;

        // Pos X
        Gtk::Label      mPosXLabel;
        Gtk::SpinButton mPosXEntry;

        // Pos Y
        Gtk::Label      mPosZLabel;
        Gtk::SpinButton mPosZEntry;

        // Separator
        Gtk::HSeparator mSeparator1;

        // Height
        Gtk::HBox       mHeightBox;
        Gtk::Label      mHeightLabel;
        Gtk::HBox       mHeightAdjust;
        Gtk::SpinButton mHeightEntry;

        // Influence
        Gtk::HBox        mInfluenceBox;
        Gtk::CheckButton mInfluenceCheck;
        Gtk::Label       mInfluenceLabel;
        Gtk::HBox        mInfluenceAdjust;
        Gtk::SpinButton  mInfluenceEntry;

        // Flags
        Gtk::HBox       mFlagsBox;
        Gtk::Label      mFlagsLabel;
        Gtk::HBox       mFlagsAdjust;
        Gtk::SpinButton mFlagsEntry;

        // Apply
        Gtk::HBox       mApplyBox;
        Gtk::Button     mApplyButton;

        // Render
        Gtk::Frame    mRenderFrame;
        Gtk::VBox     mRenderBox;
        Gtk::EventBox mRenderEventBox;


        // Data
        s_refptr<Ogre::Root>      pRoot;
        s_ptr<Ogre::SceneManager> pSceneMgr;

        s_ptr<UtilsManager>    pUtilsMgr;
        s_ptr<TimeManager>     pTimeMgr;
        s_ptr<InputManager>    pInputMgr;
        s_ptr<MaterialManager> pMaterialMgr;
        s_ptr<SpriteManager>   pSpriteMgr;

        s_ptr<TE::Terrain>  pTerrain;
        s_str               sFile;

        s_bool bSaved;

        s_bool bRender2D;

        s_float fBlockSize;
        s_float fBlockSpacing;

        s_refptr<Sprite> pRefSprite;

        s_ptr<Ogre::Entity>    pPlaneEntity;

        s_refptr<Material>     pTerrainMaterial;
        s_refptr<Material>     pTerrainMaterialNoTex;
        s_ptr<Ogre::Entity>    pTerrainEntity;
        s_ptr<Ogre::Mesh>      pTerrainMesh;
        s_ptr<Ogre::SceneNode> pTerrainNode;
        float*  pArray;
        ushort* pIArray;
        Vector* pNArray;

        s_ptr<Ogre::Camera>    pCamera;
        s_ptr<Ogre::SceneNode> pCamTarget;
        s_ptr<Ogre::SceneNode> pCamNode;
        s_bool   bDragging;
        DragType mDragType;
        s_int    iDragStartX;
        s_int    iDragStartY;
        s_float  fLastX;
        s_float  fLastY;

        s_ptr<Ogre::Light> pLight;

        s_ptr<Decal> pDecal;
        s_ptr<Decal> pDecal2;
        s_bool bTextured;
        s_bool bSnapDecalToVertices;
        s_refptr<Timer> pDecalUpdateTimer;

        s_ptr<Ogre::ManualObject> pDbgLine;

        std::map< ToolType, s_ptr<Tool> > lToolList;
        ToolType mTool;
    };
}
}

#endif
