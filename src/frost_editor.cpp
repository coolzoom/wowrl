/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Editor source              */
/*                                        */
/*                                        */

#include "frost_editor.h"

#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"
#include "scene/frost_doodad.h"
#include "scene/frost_scenemanager.h"

#include "frost_editor_actions.h"

using namespace std;

namespace Frost
{
    Editor::Editor()
    {
        iterAction_ = lHistoryList_.Begin();
    }

    Editor::~Editor()
    {
    }

    void Editor::Initialize()
    {
    }

    void Editor::AddEditorAction( s_refptr<EditorAction> pEditorAction )
    {
        if (pEditorAction)
        {
            pEditorAction->Do();
            lHistoryList_.EraseRange(iterAction_, lHistoryList_.End());
            lHistoryList_.PushBack(pEditorAction);
            iterAction_ = lHistoryList_.End();
        }
    }

    void Editor::Undo()
    {
        if (iterAction_ != lHistoryList_.Begin())
        {
            --iterAction_;
            (*iterAction_)->Undo();

            EventManager::GetSingleton()->FireEvent(Event("UNDO"));
        }
    }

    void Editor::Redo()
    {
        if (iterAction_ != lHistoryList_.End())
        {
            (*iterAction_)->Do();
            ++iterAction_;

            EventManager::GetSingleton()->FireEvent(Event("REDO"));
        }
    }

    void Editor::SetCurrentTool( const Tool& mTool )
    {
        mTool_ = mTool;
    }

    const Editor::Tool& Editor::GetCurrentTool() const
    {
        return mTool_;
    }

    const s_bool& Editor::IsZoneSaved()
    {
        return bZoneSaved_;
    }

    const s_str& Editor::GetCurrentZoneFile()
    {
        return sCurrentZoneFile_;
    }

    void Editor::SaveZone()
    {
        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();

        if (!pZone)
            throw Exception("No zone to save.");

        if (sCurrentZoneFile_.IsEmpty())
            throw Exception("No file to save the zone.");

        ZoneManager::GetSingleton()->SaveZone(sCurrentZoneFile_);
        bZoneSaved_ = true;

        EventManager::GetSingleton()->FireEvent(Event("ZONE_SAVED"));
    }

    void Editor::NewZone( const s_str& sName )
    {
        ZoneManager::GetSingleton()->CreateZone(sName);
        sCurrentZoneFile_ = "";
        bZoneSaved_ = false;

        EventManager::GetSingleton()->FireEvent(Event("NEW_ZONE"));
    }

    void Editor::LoadZone( const s_str& sName )
    {
        ZoneManager::GetSingleton()->LoadZone(sName);
        bZoneSaved_ = true;
        sCurrentZoneFile_ = "Zones/"+sName+"/"+sName+".xml";

        EventManager::GetSingleton()->FireEvent(Event("ZONE_LOADED"));
    }

    void Editor::LoadZoneFile( const s_str& sFile )
    {
        ZoneManager::GetSingleton()->LoadZoneFile(sFile);
        bZoneSaved_ = true;
        sCurrentZoneFile_ = sFile;

        EventManager::GetSingleton()->FireEvent(Event("ZONE_LOADED"));
    }

    void Editor::CloseZone()
    {
        ZoneManager::GetSingleton()->UnloadZone();
        sCurrentZoneFile_ = "";

        EventManager::GetSingleton()->FireEvent(Event("ZONE_CLOSED"));
    }

    void Editor::AddDoodad( const s_str& sName, const s_str& sModel )
    {
        AddEditorAction(s_refptr<ActionAddDoodad>(new ActionAddDoodad(sName, sModel)));
    }

    void Editor::SetCurrentZoneFile( const s_str& sFile )
    {
        sCurrentZoneFile_ = sFile;
    }

    void Editor::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->Register<LuaEditor>();
        pLua->PushNew<LuaEditor>();
        pLua->SetGlobal("Editor");
    }
}
