/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ZoneManager source           */
/*                                        */
/*                                        */

#include "scene/frost_zonemanager.h"

#include "scene/frost_zone.h"
#include "lua/frost_lua.h"
#include "model/frost_modelmanager.h"
#include "camera/frost_cameramanager.h"
#include "scene/frost_lightmanager.h"
#include "material/frost_decal.h"
#include "frost_inputmanager.h"

using namespace std;

namespace Frost
{
    const s_str ZoneManager::CLASS_NAME = "ZoneManager";

    ZoneManager::ZoneManager()
    {
        Log("Creating "+CLASS_NAME+".");
        RegisterEvent("MOUSE_MOVED");
    }

    ZoneManager::~ZoneManager()
    {
        Log("Closing "+CLASS_NAME+"...");
        UnloadZone();
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
                pCurrentZone_->RemoveDecal(*iter);
            }

            pCurrentZone_.Delete();
            ModelManager::GetSingleton()->RemoveCategory("Zone");
        }
    }

    void ZoneManager::SaveZone( const s_str& sZoneFile )
    {
        if (pCurrentZone_)
        {
            pCurrentZone_->Serialize(sZoneFile);
        }
    }

    void ZoneManager::EnableMouseDecal( s_wptr<Decal> pDecal )
    {
        if (s_refptr<Decal> pLocked = pDecal.Lock())
        {
            if (pMouseDecal_)
                RemoveDecalFromGround(pMouseDecal_);

            pMouseDecal_ = s_refptr<Decal>(new Decal(*pLocked.Get()));
            pMouseDecal_->SetDirection(Vector(0, -1, 0));
            pMouseDecal_->SetProjection(Decal::PROJ_ORTHOGRAPHIC);

            Vector mPos = GetTerrainUnderMouse();
            if (!mPos.IsNaN())
            {
                pMouseDecal_->Show();
                pMouseDecal_->SetPosition(mPos + Vector(0, 5, 0));
            }

            AddDecalOnGround(pMouseDecal_);
        }
    }

    void ZoneManager::DisableMouseDecal()
    {
        RemoveDecalFromGround(pMouseDecal_);
        pMouseDecal_ = nullptr;
    }

    s_wptr<Decal> ZoneManager::GetMouseDecal()
    {
        return pMouseDecal_;
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

    Vector ZoneManager::GetTerrainUnderMouse()
    {
        if (pCurrentZone_)
        {
            return pCurrentZone_->GetTerrainUnderMouse(
                InputManager::GetSingleton()->GetMousePosX()/s_float(Engine::GetSingleton()->GetScreenWidth()),
                InputManager::GetSingleton()->GetMousePosY()/s_float(Engine::GetSingleton()->GetScreenHeight())
            );
        }
        else
            return Vector::NaN;
    }

    s_ptr<Zone> ZoneManager::GetCurrentZone()
    {
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
            pCurrentZone_->Update(fDelta);
            pCurrentZone_->UpdateChunks(CameraManager::GetSingleton()->GetMainCamera());

            if (pMouseDecal_)
            {
                Vector mPos = GetTerrainUnderMouse();
                if (mPos.IsNaN())
                    pMouseDecal_->Hide();
                else
                {
                    if (!pMouseDecal_->IsShown())
                        pMouseDecal_->Show();

                    pMouseDecal_->SetPosition(mPos + Vector(0, 5, 0));
                }
            }
        }
    }

    void ZoneManager::OnEvent( const Event& mEvent )
    {
        if (mEvent.GetName() == "MOUSE_MOVED")
        {

        }
    }
}
