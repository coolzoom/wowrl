/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*                                        */

#include "scene/frost_movableobject.h"
#include "scene/frost_scenemanager.h"
#include "scene/frost_node.h"

#include <OgreSceneNode.h>

#define method(widget, function) {#function, &Lua##widget::_##function}

using namespace std;

namespace Frost
{
    const s_str LuaMovableObject::CLASS_NAME  = "LuaMovableObject";

    LuaMovableObject::LuaMovableObject(lua_State* pLua)
    {
        s_uint uiID = (uint)lua_tonumber(pLua, -1);

        lua_newtable(pLua);
        iRef_ = luaL_ref(pLua, LUA_REGISTRYINDEX);
        pLua_ = pLua;

        pParent_ = SceneManager::GetSingleton()->GetMovableObjectByID(uiID);
    }

    int LuaMovableObject::GetDataTable( lua_State * pLua )
    {
        lua_getref(pLua, iRef_);
        return 1;
    }

    int LuaMovableObject::_Translate(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:Translate", pLua);
        mFunc.Add(0, "vec", Lua::TYPE_TABLE);
        mFunc.Add(1, "local", Lua::TYPE_BOOLEAN, true);
        mFunc.NewParamSet();
        mFunc.Add(0, "x", Lua::TYPE_NUMBER);
        mFunc.Add(1, "y", Lua::TYPE_NUMBER);
        mFunc.Add(2, "z", Lua::TYPE_NUMBER);
        mFunc.Add(3, "local", Lua::TYPE_BOOLEAN, true);
        if (mFunc.Check())
        {
            if (mFunc.GetParamSetRank() == 0)
            {
                s_bool bLocal = false;
                if (mFunc.IsProvided(1))
                    bLocal = mFunc.Get(1)->GetBool();

                s_ptr<Lua::State> pState = mFunc.GetState();
                pState->SetTop(1);

                Vector mVec(
                    pState->GetFieldFloat("x"),
                    pState->GetFieldFloat("y"),
                    pState->GetFieldFloat("z")
                );

                pParent_->Translate(mVec, bLocal);
            }
            else
            {
                s_bool bLocal = false;
                if (mFunc.IsProvided(3))
                    bLocal = mFunc.Get(3)->GetBool();

                pParent_->Translate(Vector(
                        mFunc.Get(0)->GetNumber(),
                        mFunc.Get(1)->GetNumber(),
                        mFunc.Get(2)->GetNumber()
                    ),
                    bLocal
                );
            }
        }

        return mFunc.Return();
    }

    int LuaMovableObject::_SetDirection(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:SetDirection", pLua);
        mFunc.Add(0, "vec", Lua::TYPE_TABLE);
        mFunc.NewParamSet();
        mFunc.Add(0, "x", Lua::TYPE_NUMBER);
        mFunc.Add(1, "y", Lua::TYPE_NUMBER);
        mFunc.Add(2, "z", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            if (mFunc.GetParamSetRank() == 0)
            {
                s_ptr<Lua::State> pState = mFunc.GetState();
                pState->SetTop(1);

                Vector mVec(
                    pState->GetFieldFloat("x"),
                    pState->GetFieldFloat("y"),
                    pState->GetFieldFloat("z")
                );

                pParent_->SetDirection(mVec);
            }
            else
            {
                pParent_->SetDirection(Vector(
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
        }

        return mFunc.Return();
    }

    int LuaMovableObject::_TranslateOrbitNode(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:TranslateOrbitNode", pLua);
        mFunc.Add(0, "vec", Lua::TYPE_TABLE);
        mFunc.Add(1, "local", Lua::TYPE_BOOLEAN, true);
        mFunc.NewParamSet();
        mFunc.Add(0, "x", Lua::TYPE_NUMBER);
        mFunc.Add(1, "y", Lua::TYPE_NUMBER);
        mFunc.Add(2, "z", Lua::TYPE_NUMBER);
        mFunc.Add(3, "local", Lua::TYPE_BOOLEAN, true);
        if (mFunc.Check())
        {
            if (mFunc.GetParamSetRank() == 0)
            {
                s_bool bLocal = false;
                if (mFunc.IsProvided(1))
                    bLocal = mFunc.Get(1)->GetBool();

                s_ptr<Lua::State> pState = mFunc.GetState();
                pState->SetTop(1);

                Vector mVec(
                    pState->GetFieldFloat("x"),
                    pState->GetFieldFloat("y"),
                    pState->GetFieldFloat("z")
                );

                pParent_->GetOrbitNode()->translate(
                    Vector::FrostToOgre(mVec),
                    bLocal ? Ogre::SceneNode::TS_LOCAL : Ogre::SceneNode::TS_PARENT
                );
            }
            else
            {
                s_bool bLocal = false;
                if (mFunc.IsProvided(3))
                    bLocal = mFunc.Get(3)->GetBool();

                pParent_->GetOrbitNode()->translate(Vector::FrostToOgre(Vector(
                        mFunc.Get(0)->GetNumber(),
                        mFunc.Get(1)->GetNumber(),
                        mFunc.Get(2)->GetNumber()
                    )),
                    bLocal ? Ogre::SceneNode::TS_LOCAL : Ogre::SceneNode::TS_PARENT
                );
            }
        }

        return mFunc.Return();
    }

    int LuaMovableObject::_OrbitAround(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:OrbitAround", pLua);
        mFunc.Add(0, "vec", Lua::TYPE_TABLE);
        mFunc.NewParamSet();
        mFunc.Add(0, "x", Lua::TYPE_NUMBER);
        mFunc.Add(1, "y", Lua::TYPE_NUMBER);
        mFunc.Add(2, "z", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            if (mFunc.GetParamSetRank() == 0)
            {
                s_ptr<Lua::State> pState = mFunc.GetState();
                pState->SetTop(1);

                Vector mVec(
                    pState->GetFieldFloat("x"),
                    pState->GetFieldFloat("y"),
                    pState->GetFieldFloat("z")
                );

                pParent_->OrbitAround(mVec);
            }
            else
            {
                pParent_->OrbitAround(Vector(
                    mFunc.Get(0)->GetNumber(),
                    mFunc.Get(1)->GetNumber(),
                    mFunc.Get(2)->GetNumber()
                ));
            }
        }

        return mFunc.Return();
    }

    int LuaMovableObject::_GetDirection(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:GetDirection", pLua, 1);

        Vector mDir = pParent_->GetDirection(false);

        lua_getglobal(pLua, "Vector");
        lua_pushnumber(pLua, mDir.X().Get());
        lua_pushnumber(pLua, mDir.Y().Get());
        lua_pushnumber(pLua, mDir.Z().Get());
        lua_pcall(pLua, 3, 1, 0);
        mFunc.NotifyPushed();

        return mFunc.Return();
    }

    int LuaMovableObject::_Yaw(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:Yaw", pLua);
        mFunc.Add(0, "angle", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            pParent_->Yaw(mFunc.Get(0)->GetNumber());
        }

        return mFunc.Return();
    }

    int LuaMovableObject::_Pitch(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:Pitch", pLua);
        mFunc.Add(0, "angle", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            pParent_->Pitch(mFunc.Get(0)->GetNumber());
        }

        return mFunc.Return();
    }

    int LuaMovableObject::_Roll(lua_State* pLua)
    {
        Lua::Function mFunc("MovableObject:Roll", pLua);
        mFunc.Add(0, "angle", Lua::TYPE_NUMBER);
        if (mFunc.Check())
        {
            pParent_->Roll(mFunc.Get(0)->GetNumber());
        }

        return mFunc.Return();
    }

    s_ptr<MovableObject> LuaMovableObject::GetObject()
    {
        return pParent_;
    }

    const char  LuaMovableObject::className[] = "MovableObject";
    const char* LuaMovableObject::classList[] = {"MovableObject", 0};
    Lunar<LuaMovableObject>::RegType LuaMovableObject::methods[] = {
        {"dt", &LuaMovableObject::GetDataTable},

        method(MovableObject, Translate),
        method(MovableObject, TranslateOrbitNode),
        method(MovableObject, OrbitAround),
        method(MovableObject, GetDirection),
        method(MovableObject, SetDirection),
        method(MovableObject, Yaw),
        method(MovableObject, Pitch),
        method(MovableObject, Roll),

        {0,0}
    };
}
