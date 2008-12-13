/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_lua_utils.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

const s_str Function::CLASS_NAME = "Lua::Function";

Function::Function( s_str sName, lua_State* pLua, s_uint uiReturnNbr ) :
    sName_(sName), pLua_(pLua), uiReturnNbr_(uiReturnNbr)
{
}

void Function::Add( s_uint uiIndex, s_str sName, int iLuaType, ValueType mType, s_bool bOptional )
{
    if (bOptional)
    {
        if (MAPFIND(uiIndex, lOptional_))
        {
            lOptional_[uiIndex].Add(sName, iLuaType, mType);
        }
        else
        {
            lOptional_[uiIndex] = Argument(sName, iLuaType, mType, this);
        }
    }
    else
    {
        if (MAPFIND(uiIndex, lArg_))
        {
            lArg_[uiIndex].Add(sName, iLuaType, mType);
        }
        else
        {
            lArg_[uiIndex] = Argument(sName, iLuaType, mType, this);
        }
    }
}

s_ptr<Argument> Function::Get( s_uint uiIndex )
{
    if (MAPFIND(uiIndex, lArg_))
    {
        return &lArg_[uiIndex];
    }
    if (MAPFIND(uiIndex, lOptional_))
    {
        return &lOptional_[uiIndex];
    }

    return NULL;
}

s_bool Function::IsProvided( s_uint uiIndex )
{
    if (MAPFIND(uiIndex, lArg_))
    {
        return lArg_[uiIndex].IsProvided();
    }
    if (MAPFIND(uiIndex, lOptional_))
    {
        return lOptional_[uiIndex].IsProvided();
    }

    return false;
}

s_bool Function::Check( s_bool bPrintError )
{
    uint iArgs = lua_gettop(pLua_); // Get the number of value on the stack
    // Check if that's enough
    if (iArgs < lArg_.size())
    {
        if (bPrintError)
        {
            s_str sArguments;
            map<s_uint, Argument>::iterator iterArg;
            foreach (iterArg, lArg_)
            {
                if (iterArg != lArg_.begin())
                    sArguments += ", ";
                sArguments += iterArg->second.Get()->GetName();
            }
            if (lOptional_.size() > 0)
            {
                if (sArguments != "")
                    sArguments += ", ";
                sArguments += "(+";
                foreach (iterArg, lOptional_)
                {
                    if (iterArg != lOptional_.begin())
                        sArguments += ", ";
                    sArguments += iterArg->second.Get()->GetName();
                }
                sArguments += ")";
            }

            PrintError(pLua_,
                "Too few arguments in \"" +
                sName_ +
                "\" (" +
                s_str(lArg_.size()) +
                " expected : " +
                sArguments +
                ")."
            );
        }
        // So if there isn't enough, just return false
        return false;
    }

    // We then check the value type
    map<s_uint, Argument>::iterator iterArg;
    uint i = 1;
    foreach (iterArg, lArg_)
    {
        iterArg->second.Test(pLua_, i, bPrintError);
        i++;
    }

    // We fill the stack with nil value until there are enough for optional arguments
    uint iMaxArgs = lArg_.size() + lOptional_.size();
    if (iArgs < iMaxArgs)
        lua_pushnil(pLua_, iMaxArgs - iArgs);

    // And we check optional arguments
    foreach (iterArg, lOptional_)
    {
        if (lua_type(pLua_, i) != LUA_TNIL)
        {
            iterArg->second.Test(pLua_, i, bPrintError);
        }
        i++;
    }

    return true;
}

const s_str& Function::GetName() const
{
    return sName_;
}

void Function::Push( ReturnValue mReturn )
{
    if (uiReturnCount_ == uiReturnNbr_)
        uiReturnNbr_++;

    if (mReturn.mType == RETURN_NIL)
        lua_pushnil(pLua_);
    else if (mReturn.mType == RETURN_NUMBER)
        lua_pushnumber(pLua_, mReturn.vValue.GetF());
    else if (mReturn.mType == RETURN_BOOLEAN)
        lua_pushboolean(pLua_, mReturn.vValue.GetB());
    else if (mReturn.mType == RETURN_STRING)
        lua_pushstring(pLua_, mReturn.vValue.GetS());

    uiReturnCount_++;
}

int Function::Return()
{
    if (!uiReturnNbr_.IsNull())
    {
        // Fill with nil value
        if (uiReturnCount_ != uiReturnNbr_)
            lua_pushnil(pLua_, uiReturnNbr_-uiReturnCount_);

        // Return the number of returned value
        return (int)uiReturnNbr_.Get();
    }
    else
        return 0;
}
