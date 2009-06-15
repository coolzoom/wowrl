/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_utils_lua_state.h"
#include "frost_utils_lua_function.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

const s_str Data::CLASS_NAME = "Lua::Data";

Data::Data()
{
    sName_ = ""; mLuaType_ = Lua::TYPE_NIL; mType_ = VALUE_NONE; pParent_ = NULL;
}

Data::Data( const s_str& name, Lua::Type mLuaType, ValueType mType, s_ptr<Argument> pParent )
{
    sName_ = name; mLuaType_ = mLuaType; mType_ = mType; pParent_ = pParent;
}

void Data::Set( s_ptr<Lua::State> pLua, const s_int& iIndex )
{
    if (mType_ == VALUE_BOOL)
        mValue_.SetB(pLua->GetBool(iIndex));
    else if (mType_ == VALUE_FLOAT)
        mValue_.SetF(pLua->GetNumber(iIndex));
    else if (mType_ == VALUE_DOUBLE)
        mValue_.SetD(s_double(pLua->GetNumber(iIndex)));
    else if (mType_ == VALUE_INT)
        mValue_.SetI(s_int(pLua->GetNumber(iIndex)));
    else if (mType_ == VALUE_UINT)
        mValue_.SetUI(s_uint(pLua->GetNumber(iIndex)));
    else if (mType_ == VALUE_STRING)
        mValue_.SetS(pLua->GetString(iIndex));
    else if (mLuaType_ == Lua::TYPE_TABLE)
        mValue_.SetI(iIndex);
    else if (mLuaType_ == Lua::TYPE_FUNCTION)
        mValue_.SetI(iIndex);
    else if (mLuaType_ == Lua::TYPE_USERDATA)
        mValue_.SetI(iIndex);
    else if (mLuaType_ == Lua::TYPE_NIL)
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

Lua::Type Data::GetLuaType() const
{
    return mLuaType_;
}

ValueType Data::GetType() const
{
    return mType_;
}
