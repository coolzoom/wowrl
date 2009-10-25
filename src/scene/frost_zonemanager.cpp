/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ZoneManager source           */
/*                                        */
/*                                        */

#include "scene/frost_zonemanager.h"

#include "scene/frost_zone.h"
#include "lua/frost_lua.h"
#include "camera/frost_cameramanager.h"
#include "scene/frost_lightmanager.h"

using namespace std;

namespace Frost
{
    const s_str ZoneManager::CLASS_NAME = "ZoneManager";

    ZoneManager::ZoneManager()
    {
    }

    ZoneManager::~ZoneManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        pCurrentZone_.Delete();
    }

    void ZoneManager::Initialize()
    {
        pLua_ = LuaManager::GetSingleton()->CreateLua();
        Lua::RegisterGlobalFuncs(pLua_);
    }

    s_ptr<Zone> ZoneManager::LoadZone( const s_str& sZoneName )
    {
        pCurrentZone_.Delete();
        pCurrentZone_ = new Zone(sZoneName);

        ParseXMLFile_("Zones/"+sZoneName+"/"+sZoneName+".xml", pCurrentZone_);

        LightManager::GetSingleton()->SetAmbient(pCurrentZone_->GetAmbientColor());
        LightManager::GetSingleton()->SetSunColor(pCurrentZone_->GetSunColor());

        return pCurrentZone_;
    }

    s_ptr<Lua::State> ZoneManager::GetLua()
    {
        return pLua_;
    }

    void ZoneManager::Update( const s_float& fDelta )
    {
        if (pCurrentZone_)
        {
            pCurrentZone_->UpdateChunks(CameraManager::GetSingleton()->GetMainCamera());
        }
    }
}


