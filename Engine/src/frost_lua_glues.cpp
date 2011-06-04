/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_lua_glues.h"

#include "scene/frost_zonemanager.h"
#include "scene/frost_zone.h"
#include <frost_utils_lua.h>
#include <frost_xml_document.h>
#include <material/frost_decal.h>
#include <model/frost_model.h>
#include <scene/frost_scenemanager.h>
#include <frost_utils_eventmanager.h>
#include <frost_utils_event.h>

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

/** \cond NOT_REMOVE_FROM_DOC
*/

int Lua::_EnableMouseDecal( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Engine:EnableMouseDecal", pLua);
    mFunc.Add(0, "enable", Lua::TYPE_BOOLEAN);
    mFunc.Add(1, "texture", Lua::TYPE_STRING, true);
    mFunc.Add(2, "size", Lua::TYPE_NUMBER, true);
    mFunc.Add(3, "color (red)", Lua::TYPE_NUMBER, true);
    mFunc.Add(4, "color (green)", Lua::TYPE_NUMBER, true);
    mFunc.Add(5, "color (blue)", Lua::TYPE_NUMBER, true);
    mFunc.Add(6, "color (alpha)", Lua::TYPE_NUMBER, true);

    if (mFunc.Check())
    {
        if (mFunc.Get(0)->GetBool())
        {
            if (!mFunc.Get(1)->IsProvided())
            {
                Error(mFunc.GetName(),
                    "You must provide a texture file to enable the mouse decal."
                );
                return mFunc.Return();
            }

            s_refptr<Decal> pDecal = s_refptr<Decal>(new Decal(mFunc.Get(1)->GetString()));

            if (mFunc.IsProvided(2))
            {
                pDecal->SetScale(mFunc.Get(2)->GetNumber());
            }

            if (mFunc.IsProvided(5))
            {
                pDecal->SetSelfIllumination(Color(
                    mFunc.Get(3)->GetNumber(),
                    mFunc.Get(4)->GetNumber(),
                    mFunc.Get(5)->GetNumber()
                ));
            }

            if (mFunc.IsProvided(6))
            {
                pDecal->SetDiffuse(Color(
                    mFunc.Get(6)->GetNumber(), 1, 1, 1
                ));
            }

            ZoneManager::GetSingleton()->EnableMouseDecal(pDecal);
        }
        else
        {
            ZoneManager::GetSingleton()->DisableMouseDecal();
        }
    }

    return mFunc.Return();
}

int Lua::_GetMouseDecalColor( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Engine:GetMouseDecalColor", pLua, 4);

    s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
    if (s_refptr<Decal> pLocked = pDecal.Lock())
    {
        Color mColor = pLocked->GetSelfIllumination();
        mFunc.Push(mColor.GetR());
        mFunc.Push(mColor.GetG());
        mFunc.Push(mColor.GetB());
        mFunc.Push(pLocked->GetDiffuse().GetA());
    }

    return mFunc.Return();
}

int Lua::_GetMouseDecalPosition( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Engine:GetMouseDecalPosition", pLua, 4);

    s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
    if (s_refptr<Decal> pLocked = pDecal.Lock())
    {
        Vector mPos = pLocked->GetPosition(false);
        mFunc.Push(mPos.X());
        mFunc.Push(mPos.Y() - 5.0f);
        mFunc.Push(mPos.Z());
    }

    return mFunc.Return();
}

int Lua::_SetMouseDecalColor( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Engine:SetMouseDecalColor", pLua);
    mFunc.Add(0, "red", Lua::TYPE_NUMBER);
    mFunc.Add(1, "green", Lua::TYPE_NUMBER);
    mFunc.Add(2, "blue", Lua::TYPE_NUMBER);
    mFunc.Add(3, "alpha", Lua::TYPE_NUMBER, true);

    s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
    if (s_refptr<Decal> pLocked = pDecal.Lock())
    {
        if (mFunc.Check())
        {
            pLocked->SetSelfIllumination(Color(
                mFunc.Get(0)->GetNumber(),
                mFunc.Get(1)->GetNumber(),
                mFunc.Get(2)->GetNumber()
            ));

            if (mFunc.IsProvided(3))
            {
                pLocked->SetDiffuse(Color(
                    mFunc.Get(3)->GetNumber(), 1, 1, 1
                ));
            }
        }
    }

    return mFunc.Return();
}

int Lua::_SetMouseDecalTexture( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Engine:SetMouseDecalTexture", pLua);
    mFunc.Add(0, "file", Lua::TYPE_STRING);

    s_wptr<Decal> pDecal = ZoneManager::GetSingleton()->GetMouseDecal();
    if (s_refptr<Decal> pLocked = pDecal.Lock())
    {
        if (mFunc.Check())
        {
            pLocked->SetTextureFile(mFunc.Get(0)->GetString());
        }
    }

    return mFunc.Return();
}

#define register_new(lua, name) \
        (lua)->Register(#name, _##name); \
        (lua)->GetGlobal(#name); \
        (lua)->SetField(#name);

void Lua::RegisterNewEngineFuncs( s_ptr<Lua::State> pLua )
{
    pLua->GetGlobal("Frost");

    register_new(pLua, EnableMouseDecal);
    register_new(pLua, GetMouseDecalColor);
    register_new(pLua, GetMouseDecalPosition);
    register_new(pLua, SetMouseDecalColor);
    register_new(pLua, SetMouseDecalTexture);

    pLua->Pop();
}

void Lua::RegisterNewEditorFuncs( s_ptr<Lua::State> pLua )
{
    pLua->GetGlobal("Editor");

    register_new(pLua, AddDoodad);
    register_new(pLua, GetCurrentZoneFile);
    register_new(pLua, GetModelFile);
    register_new(pLua, GetModelMaterial);
    register_new(pLua, GetZoneModelList);
    register_new(pLua, IsModelLoaded);
    register_new(pLua, RegisterNewModel);
    register_new(pLua, IsZoneLoaded);
    register_new(pLua, IsZoneSaved);
    register_new(pLua, LoadZone);
    register_new(pLua, LoadZoneFile);
    register_new(pLua, NewZone);
    register_new(pLua, NotifyDoodadPositioned);
    register_new(pLua, SaveZone);
    register_new(pLua, SetModelMaterial);
    register_new(pLua, UnloadZone);

    pLua->Pop();
}

int Lua::_AddDoodad( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:AddDoodad", pLua, 1);
    mFunc.Add(0, "name", Lua::TYPE_STRING);
    mFunc.Add(1, "model", Lua::TYPE_STRING);

    if (mFunc.Check())
    {
        try
        {
            Editor::GetSingleton()->AddEditorAction(s_refptr<AddDoodadAction>(new AddDoodadAction(mFunc.Get(0)->GetString(), mFunc.Get(1)->GetString())));
            EventManager::GetSingleton()->FireEvent(Event("OBJECT_ADDED"));
            mFunc.PushNil();
        }
        catch (Exception& e)
        {
            mFunc.Push(e.GetDescription());
        }
    }

    return mFunc.Return();
}

int Lua::_GetCurrentZoneFile( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:GetCurrentZoneFile", pLua, 1);

    mFunc.Push(ZoneManager::GetSingleton()->GetCurrentZoneFile());

    return mFunc.Return();
}

int Lua::_GetModelFile( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:GetModelFile", pLua, 1);
    mFunc.Add(0, "model name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
        if (pZone)
        {
            const s_map<s_str,s_str>& lModelList = pZone->GetModelList();
            s_map<s_str,s_str>::const_iterator iter = lModelList.Get(mFunc.Get(0)->GetString());
            if (iter != lModelList.End())
            {
                mFunc.Push(iter->second);
            }
            else
            {
                Warning(mFunc.GetName(),
                    "No model with the name \""+mFunc.Get(0)->GetString()+"\"."
                );
            }
        }
        else
        {
            Warning(mFunc.GetName(),
                "Can't get a model's file if no zone is loaded."
            );
        }
    }

    return mFunc.Return();
}

int Lua::_GetModelMaterial( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:GetModelMaterial", pLua, 1);
    mFunc.Add(0, "model name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
        if (pZone)
        {
            const s_map<s_str,ModelMaterial>& lMaterialList = pZone->GetMaterialInfoList();
            s_map<s_str,ModelMaterial>::const_iterator iter = lMaterialList.Get(mFunc.Get(0)->GetString());
            if (iter != lMaterialList.End())
            {
                mFunc.Push(iter->second.Serialize());
            }
            else
            {
                Warning(mFunc.GetName(),
                    "No model with the name \""+mFunc.Get(0)->GetString()+"\"."
                );
            }
        }
        else
        {
            Warning(mFunc.GetName(),
                "Can't get a model's file if no zone is loaded."
            );
        }
    }

    return mFunc.Return();
}

int Lua::_GetZoneModelList( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:GetZoneModelList", pLua);

    s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
    if (pZone)
    {
        const s_map<s_str,s_str>& lModelList = pZone->GetModelList();
        s_map<s_str,s_str>::const_iterator iter;
        foreach (iter, lModelList)
        {
            mFunc.Push(iter->first);
        }
    }
    else
    {
        Warning(mFunc.GetName(), "Can't create a model list if no zone is loaded.");
    }

    return mFunc.Return();
}

int Lua::_IsModelLoaded( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:IsModelLoaded", pLua, 1);
    mFunc.Add(0, "name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
        if (pZone)
        {
            const s_map<s_str,s_str>& lModelList = pZone->GetModelList();
            mFunc.Push(lModelList.Find(mFunc.Get(0)->GetString()));
        }
        else
        {
            mFunc.Push(s_bool(false));
        }
    }

    return mFunc.Return();
}

int Lua::_RegisterNewModel( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:RegisterNewModel", pLua, 1);
    mFunc.Add(0, "name", Lua::TYPE_STRING);
    mFunc.Add(1, "file", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
        if (pZone)
        {
            pZone->RegisterModel(mFunc.Get(0)->GetString(), mFunc.Get(1)->GetString());
            EventManager::GetSingleton()->FireEvent(Event("MODEL_CREATED"));
        }
    }

    return mFunc.Return();
}

int Lua::_IsZoneLoaded( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:IsZoneLoaded", pLua, 1);

    mFunc.Push(ZoneManager::GetSingleton()->GetCurrentZone() != nullptr);

    return mFunc.Return();
}

int Lua::_IsZoneSaved( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:IsZoneSaved", pLua, 1);

    mFunc.Push(ZoneManager::GetSingleton()->IsZoneSaved());

    return mFunc.Return();
}

int Lua::_LoadZone( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:LoadZone", pLua, 1);
    mFunc.Add(0, "name", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        try
        {
            ZoneManager::GetSingleton()->LoadZone(mFunc.Get(0)->GetString());
            EventManager::GetSingleton()->FireEvent(Event("ZONE_LOADED"));
            mFunc.PushNil();
        }
        catch (Exception& e)
        {
            mFunc.Push(e.GetDescription());
        }
    }

    return mFunc.Return();
}

int Lua::_LoadZoneFile( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:LoadZoneFile", pLua, 1);
    mFunc.Add(0, "name", Lua::TYPE_STRING);

    if (mFunc.Check())
    {
        try
        {
            ZoneManager::GetSingleton()->LoadZoneFile(mFunc.Get(0)->GetString());
            EventManager::GetSingleton()->FireEvent(Event("ZONE_LOADED"));
            mFunc.PushNil();
        }
        catch (Exception& e)
        {
            mFunc.Push(e.GetDescription());
        }
    }

    return mFunc.Return();
}

int Lua::_NewZone( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:NewZone", pLua, 1);
    mFunc.Add(0, "name", Lua::TYPE_STRING);

    if (mFunc.Check())
    {
        try
        {
            ZoneManager::GetSingleton()->CreateZone(mFunc.Get(0)->GetString());
            EventManager::GetSingleton()->FireEvent(Event("NEW_ZONE"));
            mFunc.PushNil();
        }
        catch (Exception& e)
        {
            mFunc.Push(e.GetDescription());
        }
    }

    return mFunc.Return();
}

int Lua::_NotifyDoodadPositioned( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:NotifyDoodadPositioned", pLua);

    SceneManager::GetSingleton()->StopAllTransformations();

    return mFunc.Return();
}

int Lua::_SaveZone( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:SaveZone", pLua, 2);
    mFunc.Add(0, "file", Lua::TYPE_STRING, true);

    if (mFunc.Check())
    {
        try
        {
            s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();

            if (!pZone)
                throw Exception("No zone to save.");

            if (mFunc.IsProvided(0))
            {
                ZoneManager::GetSingleton()->SaveZone(mFunc.Get(0)->GetString());
            }
            else
            {
                if (ZoneManager::GetSingleton()->GetCurrentZoneFile().IsEmpty())
                    throw Exception("No file to save the zone in.");

                ZoneManager::GetSingleton()->SaveZone();
            }

            EventManager::GetSingleton()->FireEvent(Event("ZONE_SAVED"));

            mFunc.PushNil();
        }
        catch (Exception& e)
        {
            mFunc.Push(e.GetDescription());
        }
    }

    return mFunc.Return();
}

int Lua::_SetModelMaterial( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:RegisterNewModel", pLua, 1);
    mFunc.Add(0, "name", Lua::TYPE_STRING);
    mFunc.Add(1, "material", Lua::TYPE_STRING);
    if (mFunc.Check())
    {
        s_ptr<Zone> pZone = ZoneManager::GetSingleton()->GetCurrentZone();
        if (pZone)
        {
            XML::Document mDoc("DB/ModelMaterial.def");
            mDoc.SetSourceString(mFunc.Get(1)->GetString());
            mDoc.Check();

            pZone->SetMaterialInfo(mFunc.Get(0)->GetString(), ModelMaterial(mDoc.GetMainBlock()));
        }
    }

    return mFunc.Return();
}

int Lua::_UnloadZone( lua_State* pLua )
{
    // Remove "this"
    if (lua_gettop(pLua) >= 1)
        lua_remove(pLua, 1);

    Lua::Function mFunc("Editor:UnloadZone", pLua);

    ZoneManager::GetSingleton()->UnloadZone();
    EventManager::GetSingleton()->FireEvent(Event("ZONE_CLOSED"));

    return mFunc.Return();
}

/** \endcond
*/
