/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "frost_region.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Region::CLASS_NAME = "GUI::Region";

Region::Region(s_ptr<GUIManager> pManager) : UIObject(pManager)
{
    lType_.PushBack("Region");
}

Region::~Region()
{
}

void Region::Render()
{
}

void Region::CreateGlue()
{
    s_ptr<Lua::State> pLua = pManager_->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(pLua->PushNew<LuaUIObject>());
    pLua->SetGlobal(sName_);
    pLua->Pop();
}

s_bool Region::IsInRegion( const s_int& iX, const s_int& iY ) const
{
    return (iX.IsInRange(lBorderList_[BORDER_LEFT], lBorderList_[BORDER_RIGHT]  - 1) &&
            iY.IsInRange(lBorderList_[BORDER_TOP],  lBorderList_[BORDER_BOTTOM] - 1));
}
