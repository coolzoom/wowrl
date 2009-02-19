/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_lua_utils.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

const s_str Data::CLASS_NAME = "Lua::Data";

Data::Data()
{
    sName_ = ""; iLuaType_ = LUA_TNIL; mType_ = VALUE_NONE; pParent_ = NULL;
}

Data::Data( const s_str& name, int iLuaType, ValueType mType, s_ptr<Argument> pParent )
{
    sName_ = name; iLuaType_ = iLuaType; mType_ = mType; pParent_ = pParent;
}

void Data::Set( lua_State* pLua, const s_uint& uiIndex )
{
    if (mType_ == VALUE_BOOL)
        mValue_.SetB(lua_toboolean(pLua, uiIndex.Get()));
    else if (mType_ == VALUE_FLOAT)
        mValue_.SetF(lua_tonumber(pLua, uiIndex.Get()));
    else if (mType_ == VALUE_DOUBLE)
        mValue_.SetD(lua_tonumber(pLua, uiIndex.Get()));
    else if (mType_ == VALUE_INT)
        mValue_.SetI(s_int(lua_tonumber(pLua, uiIndex.Get())));
    else if (mType_ == VALUE_UINT)
        mValue_.SetUI(s_uint(lua_tonumber(pLua, uiIndex.Get())));
    else if (mType_ == VALUE_STRING)
        mValue_.SetS(lua_tostring(pLua, uiIndex.Get()));
    else if (iLuaType_ == LUA_TTABLE)
        mValue_.SetUI(uiIndex);
    else if (iLuaType_ == LUA_TFUNCTION)
        mValue_.SetUI(uiIndex);
    else if (iLuaType_ == LUA_TUSERDATA)
        mValue_.SetUI(uiIndex);
    else if (iLuaType_ == LUA_TNIL)
        mValue_.SetP(NULL);

    pParent_->SetData(this);
}


const s_str& Data::GetName() const
{
    return sName_;
}

const s_var& Data::GetValue() const
{
    return mValue_;
}

const int& Data::GetLuaType() const
{
    return iLuaType_;
}

ValueType Data::GetType() const
{
    return mType_;
}
