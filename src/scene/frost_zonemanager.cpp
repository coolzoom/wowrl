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
#include "material/frost_decal.h"

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

    s_ptr<Zone> ZoneManager::LoadZoneFile( const s_str& sZoneFile )
    {
        s_str sZoneName = sZoneFile.Cut("/").Back().Cut("\\").Back();
        if (sZoneName.EndsWith(".xml"))
        {
            pCurrentZone_.Delete();

            sZoneName.EraseFromEnd(4);
            pCurrentZone_ = new Zone(sZoneName);

            ParseXMLFile_(sZoneFile, pCurrentZone_);

            LightManager::GetSingleton()->SetAmbient(pCurrentZone_->GetAmbientColor());
            LightManager::GetSingleton()->SetSunColor(pCurrentZone_->GetSunColor());

            s_ctnr< s_wptr<Decal> >::iterator iter;
            foreach (iter, lDecalList_)
            {
                pCurrentZone_->AddDecal(*iter);
            }
        }
        else
        {
            Error(CLASS_NAME, "\""+sZoneFile+"\" is not a valid zone file (must be an XML file).");
        }

        return pCurrentZone_;
    }

    s_ptr<Zone> ZoneManager::LoadZone( const s_str& sZoneName )
    {
        pCurrentZone_.Delete();
        pCurrentZone_ = new Zone(sZoneName);

        ParseXMLFile_("Zones/"+sZoneName+"/"+sZoneName+".xml", pCurrentZone_);

        LightManager::GetSingleton()->SetAmbient(pCurrentZone_->GetAmbientColor());
        LightManager::GetSingleton()->SetSunColor(pCurrentZone_->GetSunColor());

        s_ctnr< s_wptr<Decal> >::iterator iter;
        foreach (iter, lDecalList_)
        {
            pCurrentZone_->AddDecal(*iter);
        }

        return pCurrentZone_;
    }

    void ZoneManager::UnloadZone()
    {
        if (pCurrentZone_)
        {
            s_ctnr< s_wptr<Decal> >::iterator iter;
            foreach (iter, lDecalList_)
            {
                pCurrentZone_->AddDecal(*iter);
            }
        }

        pCurrentZone_.Delete();
    }

    void ZoneManager::EnableMouseDecal( s_wptr<Decal> pDecal )
    {
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            pMouseDecal_ = s_refptr<Decal>(new Decal(*pLocked.Get()));
            AddDecalOnGround(pMouseDecal_);
        }
    }

    void ZoneManager::DisableMouseDecal()
    {
        RemoveDecalFromGround(pMouseDecal_);
        pMouseDecal_ = nullptr;
    }

    void ZoneManager::AddDecalOnGround( s_wptr<Decal> pDecal )
    {
        if (pCurrentZone_)
            pCurrentZone_->AddDecal(pDecal);

        lDecalList_.PushBack(pDecal);
    }

    void ZoneManager::RemoveDecalFromGround( s_wptr<Decal> pDecal )
    {
        if (pCurrentZone_)
            pCurrentZone_->RemoveDecal(pDecal);

        s_ctnr< s_wptr<Decal> >::iterator iter = lDecalList_.Get(pDecal);
        if (iter != lDecalList_.End())
            lDecalList_.Erase(iter);
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
