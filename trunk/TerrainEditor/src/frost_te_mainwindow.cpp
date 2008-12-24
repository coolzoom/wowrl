#include "frost_te_mainwindow.h"

#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/toggleaction.h>
#include <gtkmm/radioaction.h>
#include <gdkmm/cursor.h>

#include "frost_timemanager.h"
#include "frost_materialmanager.h"
#include "frost_spritemanager.h"
#include "frost_sprite.h"
#include "frost_material.h"
#include "frost_timers.h"
#include "frost_decal.h"

#include <OgreHardwareBufferManager.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreRenderSystem.h>
#include <OgreRenderWindow.h>
#include <OgreEntity.h>
#include <OgreSceneQuery.h>
#include <OgreManualObject.h>

#include <libintl.h>
#include <locale.h>
#define _(text) gettext(text)

#include <iostream>

using namespace std;
using namespace Frost;

#define BLOCK_SIZE 16


extern TE::MainWindow* pMainWindow;

s_refptr<Sprite> pTestSprite;

s_bool Render()
{
    static s_ptr<SpriteManager> pSpriteMgr = SpriteManager::GetSingleton();
    static s_float fTime = 0.0f;
    fTime += s_float(TimeManager::GetSingleton()->GetDelta());

    // Render in the main target
    pSpriteMgr->Begin();

        if (!pMainWindow->Render2D())
            return false;

    pSpriteMgr->End();

    return true;
}

Color GetColorFromHeight(const float& fHeight)
{
    if (fabs(fHeight) < 0.01)
        return Color(0, 255, 0);
    else if (fHeight > 0.0f)
    {
        return Color((uint)(fHeight*255.0f), 255, 0);
    }
    else
    {
        return Color(0, (uint)((1+fHeight)*255.0f), 0);
    }
}

s_bool TE::MainWindow::Render2D()
{
    if (bRender2D)
    {
        if (pTerrain)
        {
            s_uint uiWindowW = mOgreWindow.get_width();
            s_uint uiWindowH = mOgreWindow.get_height();

            s_uint uiTerrainW = pTerrain->GetNumXPoint();
            s_uint uiTerrainH = pTerrain->GetNumZPoint();

            pSpriteMgr->Clear(Color(255, 0, 0, 0));

            for (s_uint uiZ; uiZ < uiTerrainH; uiZ++)
            {
                for (s_uint uiX; uiX < uiTerrainW; uiX++)
                {
                    s_float fX = s_float(uiX)*(fBlockSize + fBlockSpacing);
                    s_float fZ = s_float(uiZ)*(fBlockSize + fBlockSpacing);
                    pRefSprite->SetColor(GetColorFromHeight(pTerrain->GetPointHeight(uiX.Get(), uiZ.Get())));
                    pRefSprite->Render(fX, fZ);
                }
            }
        }
        else
        {
            pSpriteMgr->Clear(Color(255, 128, 128, 128));
        }
    }
    else
    {
        pSpriteMgr->Clear(Color(0, 0, 0, 0));
    }

    return true;
}

TE::MainWindow::MainWindow() :
        mSideFrame("Controls"),
        mTerrainDimFrame("Terrain dimensions"),
        mTerrainDimNumFrame("Number of point"),
        mTerrainDimSizeFrame("Size"),
        mNumXPointLabel("X :"),
        mNumZPointLabel("Z :"),
        mXSizeLabel("X :"),
        mYSizeLabel("Y :"),
        mZSizeLabel("Z :"),
        mPointEditFrame("Point edit"),
        mPosXLabel("X :"),
        mPosZLabel("Z :"),
        mHeightLabel("Height :"),
        mInfluenceLabel("Influence :"),
        mFlagsLabel("Flags :"),
        mApplyButton(Gtk::Stock::APPLY),
        mRenderFrame("Render"),
        bRender2D(true),
        fBlockSize(16.0f),
        fBlockSpacing(3.0f),
        pArray(NULL),
        pIArray(NULL),
        pNArray(NULL),
        mDragType(DRAG_NONE),
        fLastX(-1.0f),
        fLastY(-1.0f),
        bTextured(true),
        mTool(TOOL_CAMERA)
{
    lToolList[TOOL_CAMERA] = new ToolCamera();
    lToolList[TOOL_HEIGHT] = new ToolHeight();

    // Custom icons
    pIconFactory = Gtk::IconFactory::create();

    Glib::RefPtr<Gdk::Pixbuf> pPixbuf = Gdk::Pixbuf::create_from_file("frost-te-camera.png");
    Gtk::IconSet mIconSet1(pPixbuf);
    pIconFactory->add(Gtk::StockID("frost-te-camera"), mIconSet1);

    pPixbuf = Gdk::Pixbuf::create_from_file("frost-te-height.png");
    Gtk::IconSet mIconSet2(pPixbuf);
    pIconFactory->add(Gtk::StockID("frost-te-height"), mIconSet2);

    pIconFactory->add_default();

    // Menu and toolbar
    pActionGroup = Gtk::ActionGroup::create();
    // Tools
    pToolActionGroup = Gtk::ActionGroup::create();

    pActionGroup->add(
        Gtk::Action::create("MenuFile", _("_File"))
    );
    pActionGroup->add(
        Gtk::Action::create("New", Gtk::Stock::NEW),
        sigc::mem_fun(*this, &MainWindow::MenuFileNew)
    );
    pActionGroup->add(
        Gtk::Action::create("Open", Gtk::Stock::OPEN),
        sigc::mem_fun(*this, &MainWindow::MenuFileOpen)
    );
    pActionGroup->add(
        Gtk::Action::create("Save", Gtk::Stock::SAVE),
        sigc::mem_fun(*this, &MainWindow::MenuFileSave)
    );
    pActionGroup->add(
        Gtk::Action::create("SaveAs", Gtk::Stock::SAVE_AS),
        sigc::mem_fun(*this, &MainWindow::MenuFileSaveAs)
    );
    pActionGroup->add(
        Gtk::Action::create("Quit", Gtk::Stock::QUIT),
        sigc::mem_fun(*this, &MainWindow::MenuFileQuit)
    );
    pActionGroup->add(
        Gtk::Action::create("MenuEdit", _("_Edit"))
    );
    pActionGroup->add(
        Gtk::Action::create("Undo", Gtk::Stock::UNDO),
        sigc::mem_fun(*this, &MainWindow::MenuEditUndo)
    );
    pActionGroup->add(
        Gtk::Action::create("Redo", Gtk::Stock::REDO),
        sigc::mem_fun(*this, &MainWindow::MenuEditRedo)
    );
    pActionGroup->add(
        Gtk::Action::create("MenuTool", _("_Tool"))
    );
    Gtk::RadioButtonGroup mRadioGroup;
    //Glib::RefPtr<Gtk::RadioAction> pRadio = Gtk::RadioAction::create_with_icon_name(mRadioGroup, "MoveCamera", "frost-te-camera", _("Move _camera"), "");
    Glib::RefPtr<Gtk::RadioAction> pRadio = Gtk::RadioAction::create(mRadioGroup, "MoveCamera", Gtk::StockID("frost-te-camera"));
    pRadio->property_label().set_value("Move camera");
    pRadio->property_short_label().set_value("Camera");
    pRadio->set_active(true);
    pToolActionGroup->add(
        pRadio,
        sigc::mem_fun(*this, &MainWindow::MenuToolCamera)
    );
    pRadio = Gtk::RadioAction::create(mRadioGroup, "EditHeight", Gtk::StockID("frost-te-height"));
    pRadio->property_label().set_value("Edit height");
    pRadio->property_short_label().set_value("Height");
    pToolActionGroup->add(
        pRadio,
        sigc::mem_fun(*this, &MainWindow::MenuToolHeight)
    );
    pActionGroup->add(
        Gtk::Action::create("MenuView", _("_View"))
    );
    pActionGroup->add(
        Gtk::Action::create_with_icon_name("Switch2D3D", "gtk-refresh", _("2D/3D"), ""),
        sigc::mem_fun(*this, &MainWindow::MenuViewSwitch2D3D)
    );
    Glib::RefPtr<Gtk::ToggleAction> pToggle = Gtk::ToggleAction::create("SwitchWireframe", _("Wireframe"));
    pToggle->set_active(false);
    pActionGroup->add(
        pToggle,
        sigc::mem_fun(*this, &MainWindow::MenuViewWireframe)
    );
    pToggle = Gtk::ToggleAction::create("Switch2Sided", _("2-sided"));
    pToggle->set_active(true);
    pActionGroup->add(
        pToggle,
        sigc::mem_fun(*this, &MainWindow::MenuView2Sided)
    );
    pToggle = Gtk::ToggleAction::create("SwitchLighting", _("Lighting")),
    pToggle->set_active(true);
    pActionGroup->add(
        pToggle,
        sigc::mem_fun(*this, &MainWindow::MenuViewLighting)
    );
    pToggle = Gtk::ToggleAction::create("SwitchTextured", _("Textured")),
    pToggle->set_active(true);
    pActionGroup->add(
        pToggle,
        sigc::mem_fun(*this, &MainWindow::MenuViewTextured)
    );
    pToggle = Gtk::ToggleAction::create("SwitchGrid", _("Grid")),
    pToggle->set_active(true);
    pActionGroup->add(
        pToggle,
        sigc::mem_fun(*this, &MainWindow::MenuViewGrid)
    );
    pActionGroup->add(
        Gtk::Action::create("MenuHelp", _("_Help"))
    );
    pActionGroup->add(
        Gtk::Action::create("Help", Gtk::Stock::HELP),
        sigc::mem_fun(*this, &MainWindow::MenuHelpHelp)
    );
    pActionGroup->add(
        Gtk::Action::create("About", Gtk::Stock::ABOUT),
        sigc::mem_fun(*this, &MainWindow::MenuHelpAbout)
    );

    pUIManager = Gtk::UIManager::create();
    pUIManager->insert_action_group(pActionGroup);
    pUIManager->insert_action_group(pToolActionGroup);
    this->add_accel_group(pUIManager->get_accel_group());

    Glib::ustring sUIInfo =
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='MenuFile'>"
        "      <menuitem action='New'/>"
        "      <menuitem action='Open'/>"
        "      <menuitem action='Save'/>"
        "      <menuitem action='SaveAs'/>"
        "      <separator/>"
        "      <menuitem action='Quit'/>"
        "    </menu>"
        "    <menu action='MenuEdit'>"
        "      <menuitem action='Undo'/>"
        "      <menuitem action='Redo'/>"
        "    </menu>"
        "    <menu action='MenuTool'>"
        "      <menuitem action='MoveCamera'/>"
        "      <menuitem action='EditHeight'/>"
        "    </menu>"
        "    <menu action='MenuView'>"
        "      <menuitem action='Switch2D3D'/>"
        "      <separator/>"
        "      <menuitem action='Switch2Sided'/>"
        "      <menuitem action='SwitchLighting'/>"
        "      <menuitem action='SwitchWireframe'/>"
        "      <menuitem action='SwitchTextured'/>"
        "      <menuitem action='SwitchGrid'/>"
        "    </menu>"
        "    <menu action='MenuHelp'>"
        "      <menuitem action='Help'/>"
        "      <menuitem action='About'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar  name='ToolBar'>"
        "      <toolitem action='New'/>"
        "      <toolitem action='Open'/>"
        "      <toolitem action='Save'/>"
        "      <separator/>"
        "      <toolitem action='Switch2D3D'/>"
        "      <separator/>"
        "      <toolitem action='MoveCamera'/>"
        "      <toolitem action='EditHeight'/>"
        "  </toolbar>"
        "</ui>";

    pUIManager->add_ui_from_string(sUIInfo);

    Gtk::Widget* pMenuBar = pUIManager->get_widget("/MenuBar");
    mMainBox.pack_start(*pMenuBar, Gtk::PACK_SHRINK);
    Gtk::Widget* pToolBar = pUIManager->get_widget("/ToolBar");
    mMainBox.pack_start(*pToolBar, Gtk::PACK_SHRINK);

    Gtk::Widget* pSaveMenu = pUIManager->get_widget("/MenuBar/MenuFile/Save");
    pSaveMenu->set_sensitive(false);

    Gtk::Widget* pSaveAsMenu = pUIManager->get_widget("/MenuBar/MenuFile/SaveAs");
    pSaveAsMenu->set_sensitive(false);

    Gtk::Widget* pSwitchMenu = pUIManager->get_widget("/MenuBar/MenuView/Switch2D3D");
    pSwitchMenu->set_sensitive(false);

    Gtk::Widget* pSwitch2SidedMenu = pUIManager->get_widget("/MenuBar/MenuView/Switch2Sided");
    pSwitch2SidedMenu->set_sensitive(false);

    Gtk::Widget* pSwitchLightingMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchLighting");
    pSwitchLightingMenu->set_sensitive(false);

    Gtk::Widget* pSwitchWireframeMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchWireframe");
    pSwitchWireframeMenu->set_sensitive(false);

    Gtk::Widget* pSwitchTexturedMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchTextured");
    pSwitchTexturedMenu->set_sensitive(false);

    Gtk::Widget* pSwitchGridMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchGrid");
    pSwitchGridMenu->set_sensitive(false);

    Gtk::Widget* pSaveButton = pUIManager->get_widget("/ToolBar/Save");
    pSaveButton->set_sensitive(false);

    Gtk::Widget* pSwitchButton = pUIManager->get_widget("/ToolBar/Switch2D3D");
    pSwitchButton->set_sensitive(false);

    // Content
    // Side bar
    // Terrain dim
    mSideFrame.set_size_request(220);

    mTerrainDimNumBox.pack_start(mNumXPointLabel, Gtk::PACK_EXPAND_PADDING);

    mNumXPointEntry.set_max_length(4);
    mNumXPointEntry.set_numeric(true);
    mNumXPointEntry.set_range(0, 9999);
    mNumXPointEntry.set_increments(1, 10);
    mNumXPointEntry.set_sensitive(false);

    mTerrainDimNumBox.pack_start(mNumXPointEntry, Gtk::PACK_EXPAND_PADDING);
    mTerrainDimNumBox.pack_start(mNumZPointLabel, Gtk::PACK_EXPAND_PADDING);

    mNumZPointEntry.set_max_length(4);
    mNumZPointEntry.set_numeric(true);
    mNumZPointEntry.set_range(0, 9999);
    mNumZPointEntry.set_increments(1, 10);
    mNumZPointEntry.set_sensitive(false);

    mTerrainDimNumBox.pack_start(mNumZPointEntry, Gtk::PACK_EXPAND_PADDING);
    mTerrainDimNumFrame.add(mTerrainDimNumBox);
    mTerrainDimBox.pack_start(mTerrainDimNumFrame, Gtk::PACK_SHRINK);

    mTerrainDimXYSizeBox.pack_start(mXSizeLabel, Gtk::PACK_EXPAND_PADDING);

    mXSizeEntry.set_max_length(6);
    mXSizeEntry.set_digits(1);
    mXSizeEntry.set_numeric(true);
    mXSizeEntry.set_range(0, 9999);
    mXSizeEntry.set_increments(1, 10);
    mXSizeEntry.set_sensitive(false);
    mXSizeEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnParamChanged));

    mTerrainDimXYSizeBox.pack_start(mXSizeEntry, Gtk::PACK_EXPAND_PADDING);
    mTerrainDimXYSizeBox.pack_start(mYSizeLabel, Gtk::PACK_EXPAND_PADDING);

    mYSizeEntry.set_max_length(6);
    mYSizeEntry.set_digits(1);
    mYSizeEntry.set_numeric(true);
    mYSizeEntry.set_range(0, 9999);
    mYSizeEntry.set_increments(1, 10);
    mYSizeEntry.set_sensitive(false);
    mYSizeEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnParamChanged));

    mTerrainDimXYSizeBox.pack_start(mYSizeEntry, Gtk::PACK_EXPAND_PADDING);
    mTerrainDimSizeBox.pack_start(mTerrainDimXYSizeBox, Gtk::PACK_SHRINK);

    mZSizeBox.pack_start(mZSizeLabel, Gtk::PACK_SHRINK);

    mZSizeEntry.set_max_length(6);
    mZSizeEntry.set_digits(1);
    mZSizeEntry.set_numeric(true);
    mZSizeEntry.set_range(0, 999);
    mZSizeEntry.set_increments(1, 10);
    mZSizeEntry.set_sensitive(false);
    mZSizeEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnParamChanged));

    mZSizeBox.pack_start(mZSizeEntry, Gtk::PACK_SHRINK);
    mTerrainDimZSizeBox.pack_start(mZSizeBox, Gtk::PACK_EXPAND_PADDING);
    mTerrainDimSizeBox.pack_start(mTerrainDimZSizeBox, Gtk::PACK_EXPAND_PADDING);

    mTerrainDimSizeFrame.add(mTerrainDimSizeBox);
    mTerrainDimBox.pack_start(mTerrainDimSizeFrame, Gtk::PACK_SHRINK);
    mTerrainDimFrame.add(mTerrainDimBox);

    mSideBox.pack_start(mTerrainDimFrame, Gtk::PACK_SHRINK);

    // Point edit
    mPointPosBox.pack_start(mPosXLabel, Gtk::PACK_EXPAND_PADDING);

    mPosXEntry.set_max_length(4);
    mPosXEntry.set_numeric(true);
    mPosXEntry.set_range(0, 9999);
    mPosXEntry.set_increments(1, 10);
    mPosXEntry.set_sensitive(false);
    mPosXEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnPosChanged));

    mPointPosBox.pack_start(mPosXEntry, Gtk::PACK_EXPAND_PADDING);
    mPointPosBox.pack_start(mPosZLabel, Gtk::PACK_EXPAND_PADDING);

    mPosZEntry.set_max_length(4);
    mPosZEntry.set_numeric(true);
    mPosZEntry.set_range(0, 9999);
    mPosZEntry.set_increments(1, 10);
    mPosZEntry.set_sensitive(false);
    mPosZEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnPosChanged));

    mPointPosBox.pack_start(mPosZEntry, Gtk::PACK_EXPAND_PADDING);

    mPointEditBox.set_spacing(2);
    mPointEditBox.pack_start(mPointPosBox, Gtk::PACK_SHRINK);

    mPointEditBox.pack_start(mSeparator1, Gtk::PACK_SHRINK);

    mHeightBox.pack_start(mHeightLabel, Gtk::PACK_SHRINK);
    mHeightBox.pack_start(mHeightAdjust, Gtk::PACK_EXPAND_WIDGET);

    mHeightEntry.set_max_length(5);
    mHeightEntry.set_digits(2);
    mHeightEntry.set_numeric(true);
    mHeightEntry.set_range(-1, 1);
    mHeightEntry.set_increments(0.01, 0.1);
    mHeightEntry.set_sensitive(false);
    mHeightEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnParamChanged));

    mHeightBox.pack_start(mHeightEntry, Gtk::PACK_SHRINK);
    mPointEditBox.pack_start(mHeightBox, Gtk::PACK_SHRINK);

    mInfluenceCheck.set_sensitive(false);
    mInfluenceCheck.signal_toggled().connect(sigc::mem_fun(*this, &MainWindow::OnInfluenceChecked));

    mInfluenceBox.pack_start(mInfluenceCheck, Gtk::PACK_SHRINK);
    mInfluenceBox.pack_start(mInfluenceLabel, Gtk::PACK_SHRINK);
    mInfluenceBox.pack_start(mInfluenceAdjust, Gtk::PACK_EXPAND_WIDGET);

    mInfluenceEntry.set_max_length(6);
    mInfluenceEntry.set_digits(1);
    mInfluenceEntry.set_numeric(true);
    mInfluenceEntry.set_range(0, 9999);
    mInfluenceEntry.set_increments(0.1, 1);
    mInfluenceEntry.set_sensitive(false);
    mInfluenceEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnInfluenceChanged));

    mInfluenceBox.pack_start(mInfluenceEntry, Gtk::PACK_SHRINK);
    mPointEditBox.pack_start(mInfluenceBox, Gtk::PACK_SHRINK);

    mFlagsBox.pack_start(mFlagsLabel, Gtk::PACK_SHRINK);
    mFlagsBox.pack_start(mFlagsAdjust, Gtk::PACK_EXPAND_WIDGET);

    mFlagsEntry.set_max_length(3);
    mFlagsEntry.set_numeric(true);
    mFlagsEntry.set_range(0, 255);
    mFlagsEntry.set_increments(1, 10);
    mFlagsEntry.set_sensitive(false);
    mFlagsEntry.signal_changed().connect(sigc::mem_fun(*this, &MainWindow::OnParamChanged));

    mFlagsBox.pack_start(mFlagsEntry, Gtk::PACK_SHRINK);
    mPointEditBox.pack_start(mFlagsBox, Gtk::PACK_SHRINK);

    mApplyButton.set_sensitive(false);
    mApplyButton.signal_pressed().connect(sigc::mem_fun(*this, &MainWindow::OnApply));
    mApplyBox.pack_start(mApplyButton, Gtk::PACK_EXPAND_PADDING);
    mPointEditBox.pack_start(mApplyBox, Gtk::PACK_SHRINK);

    mPointEditFrame.add(mPointEditBox);

    mSideBox.pack_start(mPointEditFrame, Gtk::PACK_SHRINK);

    mSideFrame.add(mSideBox);

    // Render frame
    mRenderFrame.set_size_request(556, 256);
    mOgreWindow.set_size_request(500, 200);
    mOgreWindow.queue_draw();
    mRenderEventBox.add_events(Gdk::BUTTON_PRESS_MASK);
    mRenderEventBox.add_events(Gdk::BUTTON_RELEASE_MASK);
    mRenderEventBox.add_events(Gdk::SCROLL_MASK);
    mRenderEventBox.signal_motion_notify_event().connect(sigc::mem_fun(this, &MainWindow::OnMouseMoved));
    mRenderEventBox.signal_button_press_event().connect(sigc::mem_fun(this, &MainWindow::OnButtonPressed));
    mRenderEventBox.signal_button_release_event().connect(sigc::mem_fun(this, &MainWindow::OnButtonReleased));
    mRenderEventBox.signal_scroll_event().connect(sigc::mem_fun(this, &MainWindow::OnMouseScrolled));
    mRenderEventBox.add(mOgreWindow);
    mRenderBox.pack_start(mRenderEventBox, Gtk::PACK_EXPAND_WIDGET);
    mRenderFrame.add(mRenderBox);

    mContentBox.pack_start(mSideFrame, Gtk::PACK_SHRINK);
    mContentBox.pack_start(mRenderFrame, Gtk::PACK_EXPAND_WIDGET);

    mMainBox.pack_start(mContentBox, Gtk::PACK_EXPAND_WIDGET);

    this->add(mMainBox);

    this->show_all_children();

    // Ogre
    pRoot = s_refptr<Ogre::Root>(new Ogre::Root("", ""));

    #ifdef _DEBUG
        pRoot->loadPlugin("RenderSystem_GL_d");
    #else
        pRoot->loadPlugin("RenderSystem_GL");
    #endif

    Ogre::RenderSystem* pRS = pRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
    if (!pRS)
    {
        Error("Main", "No render system with the name \"OpenGL Rendering Subsystem\".");
        return;
    }
    else
        pRoot->setRenderSystem(pRS);

    pRoot->initialise(false);

    pSceneMgr = pRoot->createSceneManager(Ogre::ST_GENERIC, "FrostSceneMgr");

    Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Frost");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

TE::MainWindow::~MainWindow()
{
    Log("Average FPS : "+ pTimeMgr->GetAverageFPS());
    Log("Best FPS : "+ pTimeMgr->GetBestFPS());
    Log("Worst FPS : "+ pTimeMgr->GetWorstFPS());

    SpriteManager::Delete();
    MaterialManager::Delete();
    TimeManager::Delete();
    UtilsManager::Delete();

    std::map< ToolType, s_ptr<Tool> >::iterator iterTool;
    foreach (iterTool, lToolList)
    {
        delete iterTool->second.Get();
    }

    delete pNArray;
    delete pIArray;
    delete pArray;

    if (pTerrainEntity)
    {
        pTerrainEntity->detatchFromParent();
        pSceneMgr->destroyEntity(pTerrainEntity.Get());
        pTerrainEntity = NULL;
    }

    if (pTerrainMesh)
    {
        Ogre::MeshManager::getSingleton().remove("TerrainMesh");
        pTerrainMesh = NULL;
    }

    if (pRoot)
    {
        if (pSceneMgr)
        {
            pRoot->destroySceneManager(pSceneMgr.Get());
        }
        GetOgreRenderWindow()->destroy();
        pRoot.SetNull();
    }

    delete pTerrain.Get();
}

void TE::MainWindow::Initialize(Ogre::RenderWindow* pOgreWindow, uint uiWidth, uint uiHeight)
{
    // Frost
    pUtilsMgr    = UtilsManager::GetSingleton();
    pTimeMgr     = TimeManager::GetSingleton();
    pMaterialMgr = MaterialManager::GetSingleton();
    pSpriteMgr   = SpriteManager::GetSingleton();

    pSpriteMgr->SetRenderFunction(&Render);
    pSpriteMgr->SetYAxisType(AXIS_DOWN);

    pSpriteMgr->Initialize(pSceneMgr, (float)uiWidth, (float)uiHeight);
    pMaterialMgr->Initialize();

    pTimeMgr->Initialize();

    // Create 2D block
    s_refptr<Material> pMat = pMaterialMgr->GetDefault2D();
    pRefSprite = s_refptr<Sprite>(new Sprite(pMat, fBlockSize, fBlockSize));

    // Create camera :
    Ogre::Vector3 mCamPos(0, 0, 10);

    // - Create target orbiting node
    pCamTarget = pSceneMgr->getRootSceneNode()->createChildSceneNode();
    pCamTarget->setPosition(Ogre::Vector3(0, 0, 0));
    pCamTarget->setFixedYawAxis(true);
    pCamTarget->lookAt(-mCamPos, Ogre::SceneNode::TS_WORLD);

    // - Create camera node
    pCamNode = pCamTarget->createChildSceneNode();

    // - Create camera
    pCamera = pSceneMgr->createCamera("Camera");
    pCamera->setAutoAspectRatio(true);
    pCamera->setNearClipDistance(0.2);
    pCamera->setFarClipDistance(200);

    // - Attach camera
    pCamNode->translate(Ogre::Vector3(0, 0, mCamPos.length()));
    pCamNode->setFixedYawAxis(true);
    pCamNode->lookAt(Ogre::Vector3(0,0,0), Ogre::SceneNode::TS_PARENT);
    pCamNode->attachObject(pCamera.Get());

    Frost::s_ptr<Ogre::RenderWindow> pWindow = GetOgreRenderWindow();
    Frost::s_ptr<Ogre::Viewport> pViewport = pWindow->addViewport(pCamera.Get());
    pViewport->setBackgroundColour(Ogre::ColourValue(0.24, 0.35, 0.47));

    // Create light
    pLight = pSceneMgr->createLight("Light");
    pLight->setPosition(5, 10, 5);

    pSceneMgr->setAmbientLight(Ogre::ColourValue(0, 0, 0));

    // Create the grid
    s_refptr<Material> pPlaneMat = pMaterialMgr->CreateMaterial3D("grid.png");
    pPlaneMat->SetAlphaReject(true);
    pPlaneMat->GetOgreMaterial()->setLightingEnabled(false);
    //pPlaneMat->GetOgreMaterial()->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
    pPlaneMat->GetOgreMaterial()->setCullingMode(Ogre::CULL_NONE);
    pPlaneMat->GetOgreMaterial()->setTextureFiltering(Ogre::TFO_NONE);

    Ogre::Plane mPlane(Ogre::Vector3::UNIT_Y, 0);
    Frost::s_ptr<Ogre::Mesh> pPlane = Ogre::MeshManager::getSingletonPtr()->createPlane(
        "PlaneMesh", "General", mPlane, 100, 100, 30, 30, true, 1, 30, 30, Ogre::Vector3::UNIT_Z
    ).get();
    pPlaneEntity = pSceneMgr->createEntity("PlaneEntity", "PlaneMesh");
    pPlaneEntity->setVisible(false);
    pPlaneEntity->setMaterialName(pPlaneMat->GetOgreMaterialName().Get());
    pSceneMgr->getRootSceneNode()->attachObject(pPlaneEntity.Get());

    pDbgLine = pSceneMgr->createManualObject("manual1");
    pDbgLine->setDynamic(true);

    pDbgLine->begin("", Ogre::RenderOperation::OT_LINE_LIST);
    pDbgLine->position(0, 0, 0);
    pDbgLine->position(0, 0, 0);
    // etc
    pDbgLine->end();

    pSceneMgr->getRootSceneNode()->attachObject(pDbgLine.Get());
}

bool TE::MainWindow::Iteration()
{
    if (!Gtk::Main::iteration(false))
        return false;

    if (mOgreWindow.getRenderWindow())
        mOgreWindow.getRenderWindow()->update(true);

    if (!pRoot->_fireFrameStarted())
        return false;

    // Update our own render targets
    if (!pSpriteMgr->RenderTargets())
        return false;

    // Render everyting
    if (!pRoot->_updateAllRenderTargets())
        return false;

    // Update inputs and timers
    pTimeMgr->Update();

    if (!pRoot->_fireFrameEnded())
        return false;

    return true;
}

void TE::MainWindow::MenuFileNew()
{
    Gtk::Dialog mDialog(
        "Creating new terrain...",
        *this,
        true
    );

    // Add response buttons the the dialog:
    mDialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    mDialog.add_button(Gtk::Stock::OK, Gtk::RESPONSE_OK);

    Gtk::VBox* pMainVBox = mDialog.get_vbox();
    pMainVBox->set_spacing(5);

    Gtk::Label mQuestion("Please enter the size of the new terrain :");
    pMainVBox->pack_start(mQuestion);

    Gtk::HBox mMainHBox;

    Gtk::HBox mXBox;
    Gtk::Label mXLabel("X :");
    mXBox.pack_start(mXLabel, Gtk::PACK_SHRINK);
    Gtk::SpinButton mXEntry;
    mXEntry.set_max_length(4);
    mXEntry.set_numeric(true);
    mXEntry.set_range(2, 9999);
    mXEntry.set_increments(1, 10);
    mXBox.pack_start(mXEntry, Gtk::PACK_SHRINK);

    mMainHBox.pack_start(mXBox, Gtk::PACK_EXPAND_PADDING);

    Gtk::HBox mZBox;
    Gtk::Label mZLabel("Z :");
    mZBox.pack_start(mZLabel, Gtk::PACK_SHRINK);
    Gtk::SpinButton mZEntry;
    mZEntry.set_max_length(4);
    mZEntry.set_numeric(true);
    mZEntry.set_range(2, 9999);
    mZEntry.set_increments(1, 10);
    mZBox.pack_start(mZEntry, Gtk::PACK_SHRINK);

    mMainHBox.pack_start(mZBox, Gtk::PACK_EXPAND_PADDING);

    pMainVBox->pack_start(mMainHBox);

    mDialog.show_all_children();

    // Show the dialog and wait for a user response:
    int iResult = mDialog.run();

    // Handle the response:
    switch (iResult)
    {
        case Gtk::RESPONSE_OK :
        {
            sFile = "";
            bSaved = false;

            delete pTerrain.Get();
            pTerrain = new TE::Terrain(
                s_uint(mXEntry.get_text()).Get(),
                s_uint(mZEntry.get_text()).Get(),
                s_float(mXSizeEntry.get_text()).Get(),
                s_float(mYSizeEntry.get_text()).Get(),
                s_float(mZSizeEntry.get_text()).Get()
            );

            mNumXPointEntry.set_text(mXEntry.get_text());
            mNumZPointEntry.set_text(mZEntry.get_text());

            mPosXEntry.set_range(0, pTerrain->GetNumXPoint()-1);
            mPosXEntry.set_text("0");
            mPosZEntry.set_range(0, pTerrain->GetNumZPoint()-1);
            mPosZEntry.set_text("0");

            OnPosChanged();

            Gtk::Widget* pSaveMenu = pUIManager->get_widget("/MenuBar/MenuFile/Save");
            pSaveMenu->set_sensitive(true);

            Gtk::Widget* pSaveAsMenu = pUIManager->get_widget("/MenuBar/MenuFile/SaveAs");
            pSaveAsMenu->set_sensitive(true);

            Gtk::Widget* pSwitch2D3DMenu = pUIManager->get_widget("/MenuBar/MenuView/Switch2D3D");
            pSwitch2D3DMenu->set_sensitive(true);

            Gtk::Widget* pSwitch2SidedMenu = pUIManager->get_widget("/MenuBar/MenuView/Switch2Sided");
            pSwitch2SidedMenu->set_sensitive(true);

            Gtk::Widget* pSwitchLightingMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchLighting");
            pSwitchLightingMenu->set_sensitive(true);

            Gtk::Widget* pSwitchWireframeMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchWireframe");
            pSwitchWireframeMenu->set_sensitive(true);

            Gtk::Widget* pSwitchTexturedMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchTextured");
            pSwitchTexturedMenu->set_sensitive(true);

            Gtk::Widget* pSwitchGridMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchGrid");
            pSwitchGridMenu->set_sensitive(true);

            Gtk::Widget* pSaveButton = pUIManager->get_widget("/ToolBar/Save");
            pSaveButton->set_sensitive(false);

            Gtk::Widget* pSwitchButton = pUIManager->get_widget("/ToolBar/Switch2D3D");
            pSwitchButton->set_sensitive(true);

            mXSizeEntry.set_sensitive(true);
            mYSizeEntry.set_sensitive(true);
            mZSizeEntry.set_sensitive(true);

            mPosXEntry.set_sensitive(true);
            mPosZEntry.set_sensitive(true);

            mHeightEntry.set_sensitive(true);
            mFlagsEntry.set_sensitive(true);

            mInfluenceCheck.set_sensitive(true);
            mInfluenceEntry.set_text("2.5");
            if (mInfluenceCheck.get_active())
            {
                mInfluenceEntry.set_sensitive(true);
            }

            mApplyButton.set_sensitive(true);

            CreateMesh();

            break;
        }
        default:
        {
            break;
        }
    }
}

void TE::MainWindow::MenuFileOpen()
{
    Gtk::FileChooserDialog mDialog(
        "Please choose a file",
        Gtk::FILE_CHOOSER_ACTION_OPEN
    );
    mDialog.set_transient_for(*this);

    // Add response buttons the the dialog:
    mDialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    mDialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

    // Add filters, so that only certain file types can be selected:
    Gtk::FileFilter mFilterTerrain;
    mFilterTerrain.set_name("Terrain files (*.ter)");
    mFilterTerrain.add_pattern("*.ter");
    mDialog.add_filter(mFilterTerrain);

    // Show the dialog and wait for a user response:
    int iResult = mDialog.run();

    // Handle the response:
    switch (iResult)
    {
        case Gtk::RESPONSE_OK :
        {
            sFile = string(mDialog.get_filename());
            bSaved = true;
            delete pTerrain.Get();
            pTerrain = new TE::Terrain(sFile);

            mNumXPointEntry.set_text(s_str(s_uint(pTerrain->GetNumXPoint())).Get());
            mNumZPointEntry.set_text(s_str(s_uint(pTerrain->GetNumZPoint())).Get());

            mXSizeEntry.set_text(s_str(s_float(pTerrain->GetXSize())).Get());
            mYSizeEntry.set_text(s_str(s_float(pTerrain->GetYSize())).Get());
            mZSizeEntry.set_text(s_str(s_float(pTerrain->GetZSize())).Get());

            mPosXEntry.set_range(0, pTerrain->GetNumXPoint()-1);
            mPosXEntry.set_text("0");
            mPosZEntry.set_range(0, pTerrain->GetNumZPoint()-1);
            mPosZEntry.set_text("0");

            OnPosChanged();

            Gtk::Widget* pSaveMenu = pUIManager->get_widget("/MenuBar/MenuFile/Save");
            pSaveMenu->set_sensitive(true);

            Gtk::Widget* pSaveAsMenu = pUIManager->get_widget("/MenuBar/MenuFile/SaveAs");
            pSaveAsMenu->set_sensitive(true);

            Gtk::Widget* pSwitchMenu = pUIManager->get_widget("/MenuBar/MenuView/Switch2D3D");
            pSwitchMenu->set_sensitive(true);

            Gtk::Widget* pSwitch2SidedMenu = pUIManager->get_widget("/MenuBar/MenuView/Switch2Sided");
            pSwitch2SidedMenu->set_sensitive(true);

            Gtk::Widget* pSwitchLightingMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchLighting");
            pSwitchLightingMenu->set_sensitive(true);

            Gtk::Widget* pSwitchWireframeMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchWireframe");
            pSwitchWireframeMenu->set_sensitive(true);

            Gtk::Widget* pSwitchTexturedMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchTextured");
            pSwitchTexturedMenu->set_sensitive(true);

            Gtk::Widget* pSwitchGridMenu = pUIManager->get_widget("/MenuBar/MenuView/SwitchGrid");
            pSwitchGridMenu->set_sensitive(true);

            Gtk::Widget* pSaveButton = pUIManager->get_widget("/ToolBar/Save");
            pSaveButton->set_sensitive(false);

            Gtk::Widget* pSwitchButton = pUIManager->get_widget("/ToolBar/Switch2D3D");
            pSwitchButton->set_sensitive(true);

            mXSizeEntry.set_sensitive(true);
            mYSizeEntry.set_sensitive(true);
            mZSizeEntry.set_sensitive(true);

            mPosXEntry.set_sensitive(true);
            mPosZEntry.set_sensitive(true);

            mHeightEntry.set_sensitive(true);
            mFlagsEntry.set_sensitive(true);

            mInfluenceCheck.set_sensitive(true);
            mInfluenceEntry.set_text("2.5");
            if (mInfluenceCheck.get_active())
            {
                mInfluenceEntry.set_sensitive(true);
            }

            mApplyButton.set_sensitive(true);

            CreateMesh();

            break;
        }
        default:
        {
            break;
        }
    }
}

void TE::MainWindow::MenuFileSave()
{
    if (pTerrain)
    {
        if (sFile != "")
        {
            pTerrain->SetSize(
                s_float(s_str(mXSizeEntry.get_text())).Get(),
                s_float(s_str(mYSizeEntry.get_text())).Get(),
                s_float(s_str(mZSizeEntry.get_text())).Get()
            );

            pTerrain->WriteToFile(sFile);
        }
        else
        {
            MenuFileSaveAs();
        }
        bSaved = true;
        Gtk::Widget* pSaveButton = pUIManager->get_widget("/ToolBar/Save");
        pSaveButton->set_sensitive(false);
    }
}

void TE::MainWindow::MenuFileSaveAs()
{
    if (pTerrain)
    {
        Gtk::FileChooserDialog mDialog(
            "Please choose a file",
            Gtk::FILE_CHOOSER_ACTION_SAVE
        );
        mDialog.set_transient_for(*this);

        // Add response buttons the the dialog:
        mDialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        mDialog.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_OK);

        // Add filters, so that only certain file types can be selected:
        Gtk::FileFilter mFilterTerrain;
        mFilterTerrain.set_name("Terrain files (*.ter)");
        mFilterTerrain.add_pattern("*.ter");
        mDialog.add_filter(mFilterTerrain);

        // Show the dialog and wait for a user response:
        int iResult = mDialog.run();

        // Handle the response:
        switch (iResult)
        {
            case Gtk::RESPONSE_OK :
            {
                pTerrain->SetSize(
                    s_float(s_str(mXSizeEntry.get_text())).Get(),
                    s_float(s_str(mYSizeEntry.get_text())).Get(),
                    s_float(s_str(mZSizeEntry.get_text())).Get()
                );

                sFile = string(mDialog.get_filename());
                pTerrain->WriteToFile(sFile);

                break;
            }
            default:
            {
                break;
            }
        }
        bSaved = true;
        Gtk::Widget* pSaveButton = pUIManager->get_widget("/ToolBar/Save");
        pSaveButton->set_sensitive(false);
    }
}

void TE::MainWindow::MenuFileQuit()
{
    this->hide();
}

void TE::MainWindow::MenuEditUndo()
{
}

void TE::MainWindow::MenuEditRedo()
{
}

void TE::MainWindow::MenuToolCamera()
{
    mTool = TOOL_CAMERA;
}

void TE::MainWindow::MenuToolHeight()
{
    mTool = TOOL_HEIGHT;
}

void TE::MainWindow::MenuViewSwitch2D3D()
{
    if (bRender2D)
    {
        bRender2D = false;
        pTerrainEntity->setVisible(true);
        pPlaneEntity->setVisible(true);
    }
    else
    {
        bRender2D = true;
        pTerrainEntity->setVisible(false);
        pPlaneEntity->setVisible(false);
    }
}

void TE::MainWindow::MenuViewLighting()
{
    Gtk::CheckMenuItem* pSwitchLightingMenu = (Gtk::CheckMenuItem*)pUIManager->get_widget("/MenuBar/MenuView/SwitchLighting");
    if (pSwitchLightingMenu->get_active())
    {
        pSceneMgr->setAmbientLight(Ogre::ColourValue(0.0f, 0.0f, 0.0f));
        pLight->setVisible(true);
    }
    else
    {
        pSceneMgr->setAmbientLight(Ogre::ColourValue(1.0f, 1.0f, 1.0f));
        pLight->setVisible(false);
    }
}

void TE::MainWindow::MenuViewWireframe()
{
    Gtk::CheckMenuItem* pSwitchWireframeMenu = (Gtk::CheckMenuItem*)pUIManager->get_widget("/MenuBar/MenuView/SwitchWireframe");
    if (pSwitchWireframeMenu->get_active())
    {
        pTerrainMaterial->GetOgreMaterial()->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);
        pTerrainMaterialNoTex->GetOgreMaterial()->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);
    }
    else
    {
        pTerrainMaterial->GetOgreMaterial()->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_SOLID);
        pTerrainMaterialNoTex->GetOgreMaterial()->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_SOLID);
    }
}

void TE::MainWindow::MenuViewTextured()
{
    Gtk::CheckMenuItem* pSwitchTexturedMenu = (Gtk::CheckMenuItem*)pUIManager->get_widget("/MenuBar/MenuView/SwitchTextured");
    if (pSwitchTexturedMenu->get_active())
    {
        pTerrainEntity->setMaterialName(pTerrainMaterial->GetOgreMaterialName().Get());
        bTextured = true;
    }
    else
    {
        pTerrainEntity->setMaterialName(pTerrainMaterialNoTex->GetOgreMaterialName().Get());
        bTextured = false;
    }
}

void TE::MainWindow::MenuView2Sided()
{
    Gtk::CheckMenuItem* pSwitch2SidedMenu = (Gtk::CheckMenuItem*)pUIManager->get_widget("/MenuBar/MenuView/Switch2Sided");
    if (pSwitch2SidedMenu->get_active())
    {
        pTerrainMaterial->GetOgreMaterial()->setCullingMode(Ogre::CULL_NONE);
        pTerrainMaterialNoTex->GetOgreMaterial()->setCullingMode(Ogre::CULL_NONE);
    }
    else
    {
        pTerrainMaterial->GetOgreMaterial()->setCullingMode(Ogre::CULL_CLOCKWISE);
        pTerrainMaterialNoTex->GetOgreMaterial()->setCullingMode(Ogre::CULL_CLOCKWISE);
    }
}

void TE::MainWindow::MenuViewGrid()
{
    Gtk::CheckMenuItem* pSwitchGridMenu = (Gtk::CheckMenuItem*)pUIManager->get_widget("/MenuBar/MenuView/SwitchGrid");
    if (pSwitchGridMenu->get_active())
    {
        pPlaneEntity->setVisible(true);
    }
    else
    {
        pPlaneEntity->setVisible(false);
    }
}

void TE::MainWindow::MenuHelpHelp()
{
}

void TE::MainWindow::MenuHelpAbout()
{
}

void TE::MainWindow::OnPosChanged()
{
    s_str sHeight = s_str(s_float(pTerrain->GetPointHeight(
        s_uint(s_str(mPosXEntry.get_text())).Get(),
        s_uint(s_str(mPosZEntry.get_text())).Get()
    )));
    if (sHeight.Length() > 4u)
        sHeight.EraseFromEnd(sHeight.Length()-4);
    mHeightEntry.set_text(sHeight.Get());

    mFlagsEntry.set_text(s_str(s_uint((uint)pTerrain->GetPointFlags(
        s_uint(s_str(mPosXEntry.get_text())).Get(),
        s_uint(s_str(mPosZEntry.get_text())).Get()
    ))).Get());
}

void TE::MainWindow::OnParamChanged()
{
    Gtk::Widget* pSaveButton = pUIManager->get_widget("/ToolBar/Save");
    pSaveButton->set_sensitive(true);
}

void TE::MainWindow::OnInfluenceChanged()
{
    if (pDecal && pDecal2)
    {
        s_float fInf = s_float(s_str(mInfluenceEntry.get_text()));
        pDecal->GetOgreFrustum()->setOrthoWindow(2*fInf.Get(), 2*fInf.Get());
        pDecal2->GetOgreFrustum()->setOrthoWindow(2*fInf.Get(), 2*fInf.Get());
    }
}

void TE::MainWindow::OnInfluenceChecked()
{
    if (mInfluenceCheck.get_active())
        mInfluenceEntry.set_sensitive(true);
    else
        mInfluenceEntry.set_sensitive(false);
}

void TE::MainWindow::OnApply()
{
    if (pTerrain)
    {
        bSaved = false;
        Gtk::Widget* pSaveButton = pUIManager->get_widget("/ToolBar/Save");
        pSaveButton->set_sensitive(true);

        uint uiX = s_uint(s_str(mPosXEntry.get_text())).Get();
        uint uiZ = s_uint(s_str(mPosZEntry.get_text())).Get();
        float fHeight = s_float(s_str(mHeightEntry.get_text())).Get();
        uchar ucFlags = (uchar)(s_uint(s_str(mHeightEntry.get_text())).Get());

        pTerrain->SetPointFlags(uiX, uiZ, ucFlags);

        if (fHeight != pTerrain->GetLastHeight(uiX, uiZ))
        {
            float fDelta = fHeight-pTerrain->GetLastHeight(uiX, uiZ);
            pTerrain->SetPointHeight(uiX, uiZ, fHeight);

            if (mInfluenceCheck.get_active())
            {
                float fDist = s_float(s_str(mInfluenceEntry.get_text())).Get();

                for (uint x = 0; x < pTerrain->GetNumXPoint(); ++x)
                {
                    for (uint z = 0; z < pTerrain->GetNumZPoint(); ++z)
                    {
                        if ( (x != uiX) || (z != uiZ) )
                        {
                            float dX = (pTerrain->GetXSize()/pTerrain->GetNumXPoint())*((float)x-uiX);
                            float dZ = (pTerrain->GetZSize()/pTerrain->GetNumZPoint())*((float)z-uiZ);
                            float fTemp = ::sqrt(dX*dX + dZ*dZ);
                            if (fTemp < fDist)
                            {
                                float fAdd = fDelta*(fDist-fTemp)/fDist;
                                float fOld = pTerrain->GetPointHeight(x, z);
                                float fResult = fAdd+fOld;
                                if (fResult > 1.0f)
                                    fResult = 1.0f;
                                if (fResult < -1.0f)
                                    fResult = -1.0f;
                                pTerrain->SetPointHeight(x, z, fResult);
                            }
                        }
                    }
                }
            }
        }

        CreateMesh();
    }
}

bool TE::MainWindow::OnMouseMoved( GdkEventMotion* pMotion )
{
    /*if (!pDecalUpdateTimer)
    {
        pDecalUpdateTimer = s_refptr<Timer>(new Timer());
        pDecalUpdateTimer->Start();
    }*/

    float fDX;
    float fDY;

    if (fLastX == -1.0f)
        fDX = 0.0f;
    else
        fDX = pMotion->x - fLastX.Get();

    if (fLastY == -1.0f)
        fDY = 0.0f;
    else
        fDY = pMotion->y - fLastY.Get();

    fLastX = pMotion->x;
    fLastY = pMotion->y;

    if (!bRender2D)
    {
        if (bDragging)
        {
            Drag(fDX, fDY);
        }
        if ((!lToolList[mTool]->HideDecalsWhileDragging() && bDragging) || !bDragging)
        {
            //if (pDecalUpdateTimer->GetElapsed() >= 0.05)
            //{
                //pDecalUpdateTimer->Zero();
                uint uiX = pTerrain->GetNumXPoint();
                uint uiZ = pTerrain->GetNumZPoint();
                float fXSize = pTerrain->GetXSize();
                float fZSize = pTerrain->GetZSize();
                float fXCoef = fXSize/uiX;
                float fZCoef = fZSize/uiZ;

                Vector mPos = GetVertexUnderMouse(pMotion->x, pMotion->y);

                if (bSnapDecalToVertices)
                {
                    mPos.XR() += fXSize/2.0f; mPos.ZR() += fZSize/2.0f;
                    mPos.XR() /= fXCoef; mPos.ZR() /= fZCoef;
                    mPos.XR().Round(); mPos.ZR().Round();
                    mPos.XR() *= fXCoef; mPos.ZR() *= fZCoef;
                    mPos.XR() -= fXSize/2.0f; mPos.ZR() -= fZSize/2.0f;
                }

                if (bTextured)
                {
                    if (mPos)
                    {
                        pDecal->Show();
                        pDecal->SetPosition(Vector(mPos.X(), 6, mPos.Z()));
                    }
                    else
                    {
                        pDecal->Hide();
                    }
                }
                else
                {
                    if (mPos)
                    {
                        pDecal2->Show();
                        pDecal2->SetPosition(Vector(mPos.X(), 6, mPos.Z()));
                    }
                    else
                    {
                        pDecal2->Hide();
                    }
                }
            //}
        }
        else
        {
            if (bTextured)
                pDecal->Hide();
            else
                pDecal2->Hide();
        }
    }

    return true;
}

bool TE::MainWindow::OnButtonPressed( GdkEventButton* pButton )
{
    if (pButton->button == 1)
    {
        // Left
        StartDrag(DRAG_LEFT);
        std::map< ToolType, s_ptr<Tool> >::iterator iterTool = lToolList.find(mTool);
        if (iterTool != lToolList.end())
            iterTool->second->OnLeftClicked(fLastX, fLastY);
    }
    else if (pButton->button == 2)
    {
        // Left + right
        StartDrag(DRAG_BOTH);
        std::map< ToolType, s_ptr<Tool> >::iterator iterTool = lToolList.find(mTool);
        if (iterTool != lToolList.end())
            iterTool->second->OnBothClicked(fLastX, fLastY);
    }
    else if (pButton->button == 3)
    {
        // Right
        StartDrag(DRAG_RIGHT);
        std::map< ToolType, s_ptr<Tool> >::iterator iterTool = lToolList.find(mTool);
        if (iterTool != lToolList.end())
            iterTool->second->OnRightClicked(fLastX, fLastY);
    }

    return true;
}

bool TE::MainWindow::OnButtonReleased( GdkEventButton* pButton )
{
    StopDrag();
    return true;
}

bool TE::MainWindow::OnMouseScrolled( GdkEventScroll* pScroll )
{
    std::map< ToolType, s_ptr<Tool> >::iterator iterTool = lToolList.find(mTool);
    if (iterTool != lToolList.end())
    {
        switch (pScroll->direction)
        {
            case GDK_SCROLL_UP :
                iterTool->second->OnMouseScrolled(1); break;
            case GDK_SCROLL_DOWN :
                iterTool->second->OnMouseScrolled(-1); break;
            default :
                break;
        }
    }
    return true;
}

void TE::MainWindow::StartDrag( const DragType& mType )
{
    if (!bDragging)
    {
        // Store the cursor position
        Gdk::ModifierType mModifier;
        Glib::RefPtr<Gdk::Display> pDisplay = Gdk::Display::get_default();
        pDisplay->get_pointer(
            iDragStartX.GetR(), iDragStartY.GetR(),
            mModifier
        );

        bDragging = true;
        mDragType = mType;
    }
}

void TE::MainWindow::Drag( float dx, float dy )
{
    dx /= -mOgreWindow.get_width();
    dy /= -mOgreWindow.get_height();

    std::map< ToolType, s_ptr<Tool> >::iterator iterTool = lToolList.find(mTool);
    if (iterTool != lToolList.end())
    {
        switch (mDragType)
        {
            case DRAG_LEFT :
                iterTool->second->OnLeftDragged(dx, dy); break;
            case DRAG_BOTH :
                iterTool->second->OnBothDragged(dx, dy); break;
            case DRAG_RIGHT :
                iterTool->second->OnRightDragged(dx, dy); break;
            default :
                break;
        }
    }

    // Reposition the cursor
    int x, y;
    Gdk::ModifierType mModifier;
    Glib::RefPtr<Gdk::Display> pDisplay = Gdk::Display::get_default();
    pDisplay->get_pointer(
        x, y,
        mModifier
    );

    Glib::RefPtr<Gdk::Screen> pScreen = pDisplay->get_default_screen();

    int nx = x;
    int ny = y;
    bool bWarp = false;

    if (x == 0)
    {
        nx = pScreen->get_width()-2;
        bWarp = true;
    }
    else if (x == pScreen->get_width()-1)
    {
        nx = 1;
        bWarp = true;
    }
    if (y == 0)
    {
        ny = pScreen->get_height()-2;
        bWarp = true;
    }
    else if (y == pScreen->get_height()-1)
    {
        ny = 1;
        bWarp = true;
    }

    if (bWarp)
    {
        fLastX = -1.0f;
        fLastY = -1.0f;

        pDisplay->warp_pointer(
            pScreen,
            nx, ny
        );
    }
}

void TE::MainWindow::StopDrag()
{
    bDragging = false;
}

s_ptr<Ogre::RenderWindow> TE::MainWindow::GetOgreRenderWindow()
{
    return mOgreWindow.getRenderWindow();
}

s_refptr<Ogre::Root> TE::MainWindow::GetOgreRoot()
{
    return pRoot;
}

s_ptr<Ogre::SceneManager> TE::MainWindow::GetOgreSceneMgr()
{
    return pSceneMgr;
}

s_ptr<Ogre::SceneNode> TE::MainWindow::GetCameraNode()
{
    return pCamNode;
}

s_ptr<Ogre::SceneNode> TE::MainWindow::GetCameraTargetNode()
{
    return pCamTarget;
}

s_ptr<TE::Terrain> TE::MainWindow::GetTerrain()
{
    return pTerrain;
}

s_bool TE::MainWindow::IsInfluenceActive()
{
    return mInfluenceCheck.get_active();
}

s_float TE::MainWindow::GetInfluence()
{
    return s_float(s_str(mInfluenceEntry.get_text()));
}

void TE::MainWindow::SetInfluence(const s_float& fNewInfluence)
{
    s_str sInfluence = s_str(fNewInfluence);
    while (sInfluence.Size() >= 6)
        sInfluence.EraseFromEnd(1);
    mInfluenceEntry.set_text(sInfluence.Get());
}

void TE::MainWindow::CreateMesh()
{
    if (pTerrainEntity)
    {
        pTerrainEntity->detatchFromParent();
        pSceneMgr->destroyEntity(pTerrainEntity.Get());
        pTerrainEntity = NULL;
    }
    if (pTerrainMesh)
    {
        Ogre::MeshManager::getSingleton().remove("TerrainMesh");
        pTerrainMesh = NULL;
    }
    if (!pTerrainMaterial)
    {
        pTerrainMaterial = pMaterialMgr->CreateMaterial3D("checker.png");
        pTerrainMaterial->SetTilling(5, 5);
        pDecal = pTerrainMaterial->AddDecal("decal.png");
        pDecal->SetPosition(Vector(0, 6, 0));
        pDecal->SetDirection(Vector(0, -1, 0));
        pDecal->GetOgreFrustum()->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        pDecal->GetOgreFrustum()->setOrthoWindow(5, 5);
        pTerrainMaterial->GetOgreMaterial()->setCullingMode(Ogre::CULL_NONE);

        pTerrainMaterialNoTex = pMaterialMgr->CreateMaterial3D("TerrainMat (no texture)", 255, 255, 255);
        pDecal2 = pTerrainMaterialNoTex->AddDecal(pDecal);
        pDecal2->GetOgreFrustum()->setProjectionType(Ogre::PT_ORTHOGRAPHIC);
        pDecal2->GetOgreFrustum()->setOrthoWindow(5, 5);
        pTerrainMaterialNoTex->GetOgreMaterial()->setCullingMode(Ogre::CULL_NONE);
    }

    uint uiX = pTerrain->GetNumXPoint();
    uint uiZ = pTerrain->GetNumZPoint();
    int iVertexCount = uiX*uiZ;
    int iIndexCount = 6*(uiX-1)*(uiZ-1);
    float fXSize = pTerrain->GetXSize();
    float fYSize = pTerrain->GetYSize();
    float fZSize = pTerrain->GetZSize();
    float fXCoef = fXSize/(uiX-1);
    float fZCoef = fZSize/(uiZ-1);

    delete pNArray;
    delete pIArray;
    delete pArray;

    pTerrainMesh = Ogre::MeshManager::getSingleton().createManual("TerrainMesh", "General").get();
    Ogre::SubMesh* pSubMesh = pTerrainMesh->createSubMesh();
    pSubMesh->useSharedVertices = true;
    Ogre::VertexData* data = new Ogre::VertexData();
    pTerrainMesh->sharedVertexData = data;
    data->vertexCount = iVertexCount;
    Ogre::VertexDeclaration* decl = data->vertexDeclaration;
    decl->addElement(
        0, 0,
        Ogre::VET_FLOAT3, Ogre::VES_POSITION
    );
    decl->addElement(
        0, (3)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
        Ogre::VET_FLOAT3, Ogre::VES_NORMAL
    );
    decl->addElement(
        0, (3+3)*Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT1),
        Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES
    );

    Ogre::HardwareVertexBufferSharedPtr vbuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(
        decl->getVertexSize(0),
        iVertexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
    );

    int iParamNbr = 3+3+2;
    pArray = new float[iVertexCount*iParamNbr];

    // Positions and texture coordinates
    for (uint x = 0; x < uiX; x++)
    {
        for (uint z = 0; z < uiZ; z++)
        {
            // x y z -> y z x
            int index = (x*uiZ+z)*iParamNbr;

            // Position
            pArray[index + 0] = x*fXCoef - fXSize/2.0f;
            pArray[index + 1] = pTerrain->GetPointHeight(x, z)*fYSize;
            pArray[index + 2] = z*fZCoef - fZSize/2.0f;

            // Texture coordinates
            pArray[index + 6] = x*fXCoef;
            pArray[index + 7] = z*fZCoef;
        }
    }

    pIArray = new ushort[iIndexCount];

    // Indices
    for (uint x = 0; x < uiX-1; x++)
    {
        for (uint z = 0; z < uiZ-1; z++)
        {
            int index = (x*(uiZ-1) + z)*6;
            pIArray[index + 0] = x*uiZ+z;
            pIArray[index + 1] = x*uiZ+z+1;
            pIArray[index + 2] = (x+1)*uiZ+z+1;

            pIArray[index + 3] = x*uiZ+z;
            pIArray[index + 4] = (x+1)*uiZ+z+1;
            pIArray[index + 5] = (x+1)*uiZ+z;
        }
    }

    pNArray = new Vector[iVertexCount]();

    // Normals
    // Calculation code taken from :
    // http://www.devmaster.net/forums/showthread.php?t=1783
    for( int i = 0; i < iIndexCount; i += 3 )
    {
        // Get the three vertices that make the faces
        ushort i1 = pIArray[i+0];
        ushort i2 = pIArray[i+1];
        ushort i3 = pIArray[i+2];

        Vector mV1 = Vector(pArray[i1*iParamNbr+0], pArray[i1*iParamNbr+1], pArray[i1*iParamNbr+2]);
        Vector mV2 = Vector(pArray[i2*iParamNbr+0], pArray[i2*iParamNbr+1], pArray[i2*iParamNbr+2]);
        Vector mV3 = Vector(pArray[i3*iParamNbr+0], pArray[i3*iParamNbr+1], pArray[i3*iParamNbr+2]);

        // Calculate the normal
        Vector mNormal = (mV2-mV1)^(mV3-mV1);

        // Sum up the face's normal for each of the vertices that make up the face.
        pNArray[i1] += mNormal;
        pNArray[i2] += mNormal;
        pNArray[i3] += mNormal;
    }

    // Normals
    for (uint x = 0; x < uiX; x++)
    {
        for (uint z = 0; z < uiZ; z++)
        {
            // x y z -> y z x
            int index1 = (x*uiZ+z)*iParamNbr;
            int index2 = x*uiZ+z;

            // Normalize the sum at the very end
            pNArray[index2].Normalize();

            pArray[index1 + 3] = pNArray[index2].X().Get();
            pArray[index1 + 4] = pNArray[index2].Y().Get();
            pArray[index1 + 5] = pNArray[index2].Z().Get();
        }
    }

    vbuf->writeData(0, vbuf->getSizeInBytes(), pArray, true);

    Ogre::VertexBufferBinding* bind = data->vertexBufferBinding;
    bind->setBinding(0, vbuf);

    Ogre::HardwareIndexBufferSharedPtr ibuf = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(
        Ogre::HardwareIndexBuffer::IT_16BIT,
        iIndexCount,
        Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY
    );

    pSubMesh->indexData->indexBuffer = ibuf;
    pSubMesh->indexData->indexCount = iIndexCount;
    pSubMesh->indexData->indexStart = 0;

    ibuf->writeData(0, ibuf->getSizeInBytes(), pIArray, true);

    if (bTextured)
        pSubMesh->setMaterialName(pTerrainMaterial->GetOgreMaterialName().Get());
    else
        pSubMesh->setMaterialName(pTerrainMaterialNoTex->GetOgreMaterialName().Get());

    pTerrainMesh->_setBounds(Ogre::AxisAlignedBox(
        -fXSize/2.0f, -fYSize, -fZSize/2.0f,
         fXSize/2.0f,  fYSize,  fZSize/2.0f
    ));

    pTerrainMesh->_setBoundingSphereRadius(
        std::max(fXSize, std::max(fYSize, fZSize))/2.0f
    );

    pTerrainMesh->load();

    pTerrainEntity = pSceneMgr->createEntity("TerrainEntity", "TerrainMesh");
    if (bRender2D)
        pTerrainEntity->setVisible(false);
    else
        pTerrainEntity->setVisible(true);

    pTerrainNode = pSceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3::ZERO);
    pTerrainNode->attachObject(pTerrainEntity.Get());
    pTerrainNode->showBoundingBox(true);
}

Vector TE::MainWindow::GetVertexUnderMouse( float x, float y )
{
    if (pTerrain)
    {
        Ogre::Ray ray = pCamera->getCameraToViewportRay(x/mOgreWindow.get_width(), y/mOgreWindow.get_height());
        Ogre::AxisAlignedBox box = pTerrainMesh->getBounds();
        Ogre::Vector3 point = ray.getOrigin();
        Ogre::Vector3 dir = ray.getDirection();

        // first, does the ray start from inside the terrain extents?
        if (!box.contains(point))
        {
            // not inside the box, so let's see if we are actually
            // colliding with it
            pair<bool, Ogre::Real> res = ray.intersects(box);
            if (!res.first)
                return Vector(s_float::NaN);
            // update point to the collision position
            point = ray.getPoint(res.second);
        }

        // now move along the ray until we intersect or leave the bounding box
        while (true)
        {
            // have we arived at or under the terrain height?
            // note that this approach means that ray queries from below won't work
            // correctly, but then again, that shouldn't be a usual case...
            if (pTerrain->TestPoint(point.x, point.z))
            {
                float height = pTerrain->CalcPointHeight(point.x, point.z);
                if (point.y <= height)
                {
                    point.y = height;
                    return Vector::OgreToFrost(point);
                }
            }

            // move further...
            point += dir;

            // check if we are still inside the boundaries
            if (point.x < box.getMinimum().x || point.z < box.getMinimum().z
                || point.x > box.getMaximum().x || point.z > box.getMaximum().z)
                return Vector(s_float::NaN);
        }

        // Ask Ogre for the mouse ray
        /*Ogre::Ray mMouseRay = pCamera->getCameraToViewportRay(x/mOgreWindow.get_width(), y/mOgreWindow.get_height());
        Vector mTest = Vector::OgreToFrost(mMouseRay.getOrigin());
        Vector mDirection = Vector::OgreToFrost(mMouseRay.getDirection());

        // First test if the ray intersects the terrain's bounding box
        pair<bool, Ogre::Real> mResult = mMouseRay.intersects(pTerrainMesh->getBounds());
        if (mResult.first)
        {
            Log("1");
            // Then, search for the intersection
            mTest += s_float(mResult.second)*mDirection;
            bool bFirstIteration = true;

            if (mDirection.Y() > 0.0f)
            {
                Log("2"+mTest);
                // Trying to select from the bottom of the terrain
                while ( (mTest.Y() < pTerrain->GetYSize()) && pTerrain->TestPoint(mTest.X().Get(), mTest.Z().Get()) )
                {
                    float fTestHeight = pTerrain->CalcPointHeight(mTest.X().Get(), mTest.Z().Get());
                    if (mTest.Y() > fTestHeight)
                    {
                        if (!bFirstIteration)
                        {
                            Log("2.");
                            return mTest;
                        }
                        else
                        {
                            Log("2..");
                            break;
                        }
                    }

                    mTest += mDirection;
                    Log(" "+mTest);
                    bFirstIteration = false;
                }
            }
            else
            {
                Log("3 : "+mTest);
                // Trying to select from the top of the terrain
                while ( (mTest.Y() > -pTerrain->GetYSize()) && pTerrain->TestPoint(mTest.X().Get(), mTest.Z().Get()) )
                {
                    float fTestHeight = pTerrain->CalcPointHeight(mTest.X().Get(), mTest.Z().Get());
                    if (mTest.Y() < fTestHeight)
                    {
                        if (!bFirstIteration)
                        {
                            Log("3.");
                            return mTest;
                        }
                        else
                        {
                            Log("3..");
                            break;
                        }
                    }

                    mTest += mDirection;
                    Log(" "+mTest);
                    bFirstIteration = false;
                }
            }
        }*/
    }

    return Vector(s_float::NaN);
}

/*Vector TE::MainWindow::GetVertexUnderMouse( float x, float y )
{
    if (pTerrain)
    {
        // Ask Ogre for the mouse ray
        Ogre::Ray mMouseRay = pCamera->getCameraToViewportRay(x/mOgreWindow.get_width(), y/mOgreWindow.get_height());
        Vector mOrigin = Vector::OgreToFrost(mMouseRay.getOrigin());
        Vector mDirection = Vector::OgreToFrost(mMouseRay.getDirection());

        // Prepare the iteration
        Vector mV1, mV2, mV3;
        Vector mE1, mE2, mTemp, mV1Dir;
        s_float fDet, fInvDet, fU, fV, fT;
        s_float fBestT = s_float::INFPLUS;
        ushort i1, i2, i3;
        bool bIntersect = false;
        int iIndexCount = 6*(pTerrain->GetNumXPoint()-1)*(pTerrain->GetNumZPoint()-1);
        int iParamNbr = 3+3+2;

        // Test each face of the terrain mesh
        for( int i = 0; i < iIndexCount; i += 3 )
        {
            // Algorithm and implementation taken from :
            // http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
            // Comments and integration are mine.

            // Get the three vertices that make the triangle
            i1 = pIArray[i+0]; i2 = pIArray[i+1]; i3 = pIArray[i+2];
            mV1 = Vector(pArray[i1*iParamNbr+0], pArray[i1*iParamNbr+1], pArray[i1*iParamNbr+2]);
            mV2 = Vector(pArray[i2*iParamNbr+0], pArray[i2*iParamNbr+1], pArray[i2*iParamNbr+2]);
            mV3 = Vector(pArray[i3*iParamNbr+0], pArray[i3*iParamNbr+1], pArray[i3*iParamNbr+2]);

            // Get the base of the triangle (local 2D space)
            mE1 = mV2 - mV1;
            mE2 = mV3 - mV1;

            // Calculate the determinant
            mTemp = mDirection^mE2;
            fDet = mE1*mTemp;

            if (fDet > -0.000001f && fDet < 0.000001f)
            {
                // The ray is contained inside the triangle's plane. Skip it.
                continue;
            }

            fInvDet = 1.0f/fDet;

            // Get the direction from the origin of the ray to the first vertex
            mV1Dir = mOrigin - mV1;

            // U and V are the coordinates of the intersection point in the local 2D space
            // Calculate U
            fU = fInvDet*(mV1Dir*mTemp);

            if (fU < 0.0f || fU > 1.0f)
            {
                // By definition, U and V are positive (U > 0), and U+V < 1 (U < 1).
                // This U doesn't meet the conditions : the intersection is inside the
                // triangle's plane, but not inside the triangle itself. Skip it.
                continue;
            }

            // Calculate V
            mTemp = mV1Dir^mE1;
            fV = fInvDet*(mDirection*mTemp);

            if (fV < 0.0f || fU + fV > 1.0f)
            {
                // Again, V must be positive and U+V < 1, so this V doesn't meet the
                // conditions. Skip this triangle.
                continue;
            }

            // We know the intersection is inside the triangle, now let's calculate
            // it's position from the ray's point of view :
            // Point = Origin + T*Direction
            fT = fInvDet*(mE2*mTemp);
            if (fT > 0.000001f)
            {
                // T is positive : the point is in the good direction.
                // We've found an intersection ! Keep on searching for
                // a smaller T : in case there are overlapping triangles,
                // we will pick the closest one.
                bIntersect = true;
                fBestT = s_float::Min(fBestT, fT);
                continue;
            }
            else
            {
                // T is negative : the point is "behind" the ray.
                // It's not an acceptable intersection. Skip this triangle.
                continue;
            }
        }

        if (bIntersect)
        {
            // Intersection was found
            return mOrigin + mDirection*fBestT;
        }
    }

    return Vector(s_float::NaN);
}*/
