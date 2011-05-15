/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ZoneManager source           */
/*                                        */
/*                                        */

#include "scene/frost_zonemanager.h"

#include "scene/frost_zone.h"
#include "scene/frost_doodad.h"
#include <lua/frost_lua.h>
#include <model/frost_modelmanager.h>
#include <camera/frost_cameramanager.h>
#include <scene/frost_lightmanager.h>
#include <scene/frost_scenemanager.h>
#include <material/frost_decal.h>
#include <frost_inputmanager.h>
#include <frost_engine.h>

#include <frost_utils_event.h>
#include <frost_utils_eventmanager.h>

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

    s_ptr<Zone> ZoneManager::CreateZone( const s_str& sZoneName )
    {
        s_str::const_iterator iterName;
        foreach (iterName, sZoneName)
        {
            s_char c = *iterName;
            if (!s_str::IsAlphaNumeric(c) && c != '_' && c != ' ' && c != '\'')
            {
                throw Exception(
                    "A zone's name can only contain alpha numeric symbols, spaces, underscores and apostrophes.\n"
                    "\""+sZoneName+"\" is forbidden."
                );
            }
        }

        pCurrentZone_ = new Zone(sZoneName);

        LightManager::GetSingleton()->SetAmbient(pCurrentZone_->GetAmbientColor());
        LightManager::GetSingleton()->SetSunColor(pCurrentZone_->GetSunColor());

        s_ctnr< s_wptr<Decal> >::iterator iter;
        foreach (iter, lDecalList_)
        {
            pCurrentZone_->AddDecal(*iter);
        }

        bZoneSaved_ = false;

        return pCurrentZone_;
    }

    s_ptr<Zone> ZoneManager::LoadZoneFile( const s_str& sZoneFile )
    {
        if (sZoneFile.EndsWith(".xml"))
        {
            UnloadZone();

            pCurrentZone_ = new Zone();

            Log<1>("Loading zone : "+sZoneFile);
            pCurrentZone_->ParseXMLFile(sZoneFile);
            Log<1>("Done.");

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

        bZoneSaved_ = true;

        return pCurrentZone_;
    }

    s_ptr<Zone> ZoneManager::LoadZone( const s_str& sZoneName )
    {
        UnloadZone();

        pCurrentZone_ = new Zone();

        Log<1>("Loading zone : "+sZoneName);
        pCurrentZone_->ParseXMLFile("Zones/"+sZoneName+"/"+sZoneName+".xml");
        Log<1>("Done.");

        LightManager::GetSingleton()->SetAmbient(pCurrentZone_->GetAmbientColor());
        LightManager::GetSingleton()->SetSunColor(pCurrentZone_->GetSunColor());

        s_ctnr< s_wptr<Decal> >::iterator iter;
        foreach (iter, lDecalList_)
        {
            pCurrentZone_->AddDecal(*iter);
        }

        bZoneSaved_ = true;

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
            sCurrentZoneFile_ = "";
        }
    }

    void ZoneManager::SaveZone( const s_str& sZoneFile )
    {
        if (pCurrentZone_)
            pCurrentZone_->Serialize(sZoneFile);

        bZoneSaved_ = true;
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

    const s_str& ZoneManager::GetCurrentZoneFile()
    {
        return sCurrentZoneFile_;
    }

    const s_bool& ZoneManager::IsZoneSaved()
    {
        return bZoneSaved_;
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

    AddDoodadAction::AddDoodadAction(const s_str& sName, const s_str& sModel) :
        sName_(sName), sModel_(sModel)
    {
    }

    void AddDoodadAction::Do()
    {
        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
        if (!pZone)
            throw Exception("No Zone loaded, can't create doodad.");

        s_ptr<Doodad> pDoodad = pZone->AddDoodad(sName_, sModel_);

        if (pDoodad)
            SceneManager::GetSingleton()->StartMoving(pDoodad, Vector::CONSTRAINT_NONE);

        EventManager::GetSingleton()->FireEvent(Event("DOODAD_ADDED"));
    }

    void AddDoodadAction::Undo()
    {
        // TODO : # Implement AddDoodad::Undo.
    }
}
