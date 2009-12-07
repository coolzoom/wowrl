/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_region.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str Region::CLASS_NAME = "GUI::Region";

Region::Region() : UIObject()
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
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    lGlueList_.PushBack(
        pLua->Push<LuaUIObject>(new LuaUIObject(pLua->GetState()))
    );
    pLua->SetGlobal(sName_);
}

s_bool Region::IsInRegion( const s_int& iX, const s_int& iY )
{
    return (iX.IsInRange(lBorderList_[BORDER_LEFT],lBorderList_[BORDER_RIGHT]-1) &&
            iY.IsInRange(lBorderList_[BORDER_TOP], lBorderList_[BORDER_BOTTOM]-1));
}
