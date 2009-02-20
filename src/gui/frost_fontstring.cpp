/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "gui/frost_gui_fontstring.h"

#include "gui/frost_gui_layeredregion.h"
#include "frost_guimanager.h"

using namespace std;
using namespace Frost;
using namespace Frost::GUI;

const s_str FontString::CLASS_NAME = "GUI::FontString";

FontString::FontString() : LayeredRegion()
{
}

void FontString::CreateGlue()
{
    s_ptr<Lua::State> pLua = GUIManager::GetSingleton()->GetLua();
    pLua->PushString(sName_);
    LuaFontString* pNewGlue;
    pGlue_ = pNewGlue = new LuaFontString(pLua->GetState());
    Lunar<LuaFontString>::push(pLua->GetState(), pNewGlue);
    pLua->SetGlobal(sName_);
}
