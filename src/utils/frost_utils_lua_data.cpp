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

Data::Data() : sName_(""), mLuaType_(Lua::TYPE_NIL), pParent_(nullptr)
{
}

Data::Data( const s_str& name, Lua::Type mLuaType, s_ptr<Argument> pParent ) :
    sName_(name), mLuaType_(mLuaType), pParent_(pParent)
{
}

void Data::Set( s_ptr<Lua::State> pLua, const s_int& iIndex )
{
    if (mLuaType_ == Lua::TYPE_BOOLEAN)
        mValue_ = pLua->GetBool(iIndex);
    else if (mLuaType_ == Lua::TYPE_NUMBER)
        mValue_ = pLua->GetNumber(iIndex);
    else if (mLuaType_ == Lua::TYPE_STRING)
        mValue_ = pLua->GetString(iIndex);
    else if (mLuaType_ == Lua::TYPE_TABLE)
        mValue_ = iIndex;
    else if (mLuaType_ == Lua::TYPE_FUNCTION)
        mValue_ = iIndex;
    else if (mLuaType_ == Lua::TYPE_USERDATA)
        mValue_ = iIndex;
    else if (mLuaType_ == Lua::TYPE_NIL)
        mValue_ = s_var();

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

Lua::Type Data::GetType() const
{
    return mLuaType_;
}
