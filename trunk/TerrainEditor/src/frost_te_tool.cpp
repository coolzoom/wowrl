#include "frost_te_tool.h"
#include "frost_te_mainwindow.h"

using namespace std;
using namespace Frost;

extern TE::MainWindow* pMainWindow;

TE::Tool::Tool() {}

TE::Tool::~Tool() {}

const s_bool& TE::Tool::HideDecalsWhileDragging() const
{
    return bHideDecalsWhileDragging_;
}

void TE::Tool::OnLeftClicked( const s_float& fX, const s_float& fY ) {}
void TE::Tool::OnRightClicked( const s_float& fX, const s_float& fY ) {}
void TE::Tool::OnBothClicked( const s_float& fX, const s_float& fY ) {}
void TE::Tool::OnLeftDragged( const s_float& fDX, const s_float& fDY ) {}
void TE::Tool::OnRightDragged( const s_float& fDX, const s_float& fDY ) {}
void TE::Tool::OnBothDragged( const s_float& fDX, const s_float& fDY ) {}
void TE::Tool::OnMouseScrolled( const s_int iDirection ) {}

TE::ToolCamera::ToolCamera()
{
    bHideDecalsWhileDragging_ = true;
}

TE::ToolCamera::~ToolCamera()
{
}

void TE::ToolCamera::OnLeftDragged( const s_float& fDX, const s_float& fDY )
{
    pMainWindow->GetCameraTargetNode()->pitch(
        Ogre::Radian(fDY.GetRad().Get()), Ogre::SceneNode::TS_LOCAL
    );
    pMainWindow->GetCameraTargetNode()->yaw(
        Ogre::Radian(fDX.GetRad().Get()), Ogre::SceneNode::TS_WORLD
    );
}
void TE::ToolCamera::OnRightDragged( const s_float& fDX, const s_float& fDY )
{
    pMainWindow->GetCameraTargetNode()->translate(
        Ogre::Vector3(10*fDX.Get(), -10*fDY.Get(), 0), Ogre::Node::TS_LOCAL
    );
}

void TE::ToolCamera::OnBothDragged( const s_float& fDX, const s_float& fDY )
{
    pMainWindow->GetCameraNode()->translate(
        Ogre::Vector3(0, 0, -5*fDY.Get()), Ogre::Node::TS_LOCAL
    );
}
void TE::ToolCamera::OnMouseScrolled( const s_int iDirection )
{
    pMainWindow->GetCameraNode()->translate(
        Ogre::Vector3(0, 0, -iDirection.Get()), Ogre::Node::TS_LOCAL
    );
}


TE::ToolHeight::ToolHeight()
{
}

TE::ToolHeight::~ToolHeight()
{
}

void TE::ToolHeight::OnLeftClicked( const s_float& fX, const s_float& fY ) {}
void TE::ToolHeight::OnRightClicked( const s_float& fX, const s_float& fY ) {}
void TE::ToolHeight::OnBothClicked( const s_float& fX, const s_float& fY ) {}
void TE::ToolHeight::OnMouseScrolled( const s_int iDirection )
{
    if (pMainWindow->IsInfluenceActive())
    {
        pMainWindow->SetInfluence(pMainWindow->GetInfluence()+0.1f*s_float(iDirection));
    }
}
