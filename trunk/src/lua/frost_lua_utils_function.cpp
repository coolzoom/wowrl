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

Function::Function( const s_str& sName, lua_State* pLua, const s_uint& uiReturnNbr ) :
    sName_(sName), pLua_(pLua), uiReturnNbr_(uiReturnNbr)
{
    NewParamSet();
}

void Function::Add( const s_uint& uiIndex, const s_str& sName, int iLuaType, ValueType mType, const s_bool& bOptional )
{
    if (bOptional)
    {
        if (MAPFIND(uiIndex, pArgList_->lOptional_))
        {
            pArgList_->lOptional_[uiIndex].Add(sName, iLuaType, mType);
        }
        else
        {
            pArgList_->lOptional_[uiIndex] = Argument(sName, iLuaType, mType, this);
        }
    }
    else
    {
        if (MAPFIND(uiIndex, pArgList_->lArg_))
        {
            pArgList_->lArg_[uiIndex].Add(sName, iLuaType, mType);
        }
        else
        {
            pArgList_->lArg_[uiIndex] = Argument(sName, iLuaType, mType, this);
        }
    }
}

void Function::NewParamSet()
{
    lArgListStack_.push_back(ArgumentList());
    pArgList_ = &lArgListStack_.back();
    pArgList_->uiRank_ = lArgListStack_.size()-1;
}

const s_uint& Function::GetParamSetRank()
{
    return pArgList_->uiRank_;
}

s_ptr<Argument> Function::Get( const s_uint& uiIndex )
{
    if (MAPFIND(uiIndex, pArgList_->lArg_))
    {
        return &pArgList_->lArg_[uiIndex];
    }
    if (MAPFIND(uiIndex, pArgList_->lOptional_))
    {
        return &pArgList_->lOptional_[uiIndex];
    }

    return NULL;
}

s_bool Function::IsProvided( const s_uint& uiIndex ) const
{
    map<s_uint, Argument>::const_iterator iter = pArgList_->lArg_.find(uiIndex);
    if (iter != pArgList_->lArg_.end())
    {
        return iter->second.IsProvided();
    }
    iter = pArgList_->lOptional_.find(uiIndex);
    if (iter != pArgList_->lOptional_.end())
    {
        return iter->second.IsProvided();
    }

    return false;
}

const s_uint& Function::GetParamCount() const
{
    return uiParamCount_;
}

s_bool Function::Check( const s_bool& bPrintError )
{
    // TODO : tester ça
    uint iArgs = lua_gettop(pLua_); // Get the number of value on the stack
    uiParamCount_ = iArgs;

    // Check if that's enough
    vector<ArgumentList>::iterator iterArgList;
    vector< s_ptr<ArgumentList> > lValidArgList;
    foreach (iterArgList, lArgListStack_)
    {
        if (iArgs >= iterArgList->lArg_.size())
            lValidArgList.push_back(&(*iterArgList));
    }
    if (lValidArgList.empty())
    {
        if (bPrintError)
        {
            s_str sError;
            foreach (iterArgList, lArgListStack_)
            {
                s_str sArguments = "\n  - ["+s_uint(iterArgList->lArg_.size())+"] : ";
                map<s_uint, Argument>::iterator iterArg;
                foreach (iterArg, iterArgList->lArg_)
                {
                    if (iterArg != iterArgList->lArg_.begin())
                        sArguments += ", ";
                    sArguments += iterArg->second.Get()->GetName();
                }
                if (iterArgList->lOptional_.size() > 0)
                {
                    if (sArguments != "")
                        sArguments += ", ";
                    sArguments += "(+";
                    foreach (iterArg, iterArgList->lOptional_)
                    {
                        if (iterArg != iterArgList->lOptional_.begin())
                            sArguments += ", ";
                        sArguments += iterArg->second.Get()->GetName();
                    }
                    sArguments += ")";
                }
                sError += sArguments;
            }

            if (lArgListStack_.size() == 1)
            {
                PrintError(pLua_,
                    "Too few arguments in \""+sName_+"\". Expected :"+sError
                );
            }
            else
            {
                PrintError(pLua_,
                    "Too few arguments in \""+sName_+"\". Expected either :"+sError
                );
            }
        }
        // So if there isn't enough, just return false
        return false;
    }

    // We then check the value type
    uint i;
    map<s_uint, Argument>::iterator iterArg;
    if (lValidArgList.size() > 1)
    {
        vector< s_ptr<ArgumentList> >::iterator iterArgListPtr;
        pArgList_.SetNull();
        foreach (iterArgListPtr, lValidArgList)
        {
            i = 1;
            s_bool bValid = true;
            foreach (iterArg, (*iterArgListPtr)->lArg_)
            {
                if (!iterArg->second.Test(pLua_, i, false))
                {
                    bValid = false;
                    break;
                }
                i++;
            }
            if (bValid)
            {
                pArgList_ = (*iterArgListPtr);
                break;
            }
        }

        if (!pArgList_)
        {
            if (bPrintError)
            {
                s_str sError;
                foreach (iterArgList, lArgListStack_)
                {
                    s_str sArguments = "\n  - ["+s_uint(iterArgList->lArg_.size())+"] : ";
                    map<s_uint, Argument>::iterator iterArg;
                    foreach (iterArg, iterArgList->lArg_)
                    {
                        if (iterArg != iterArgList->lArg_.begin())
                            sArguments += ", ";
                        sArguments += iterArg->second.Get()->GetName();
                    }
                    if (iterArgList->lOptional_.size() > 0)
                    {
                        if (sArguments != "")
                            sArguments += ", ";
                        sArguments += "(+";
                        foreach (iterArg, iterArgList->lOptional_)
                        {
                            if (iterArg != iterArgList->lOptional_.begin())
                                sArguments += ", ";
                            sArguments += iterArg->second.Get()->GetName();
                        }
                        sArguments += ")";
                    }
                    sError += sArguments;
                }

                PrintError(pLua_,
                    "Wrong arguments provided to \""+sName_+"\". Expected either :"+sError
                );
            }
            return false;
        }
    }
    else
    {
        pArgList_ = lValidArgList.front();
        i = 1;
        s_bool bValid = true;
        foreach (iterArg, pArgList_->lArg_)
        {
            if (!iterArg->second.Test(pLua_, i, bPrintError))
                bValid = false;
            i++;
        }

        if (!bValid)
            return false;
    }

    // We fill the stack with nil value until there are enough for optional arguments
    uint iMaxArgs = pArgList_->lArg_.size() + pArgList_->lOptional_.size();
    if (iArgs < iMaxArgs)
        lua_pushnil(pLua_, iMaxArgs - iArgs);

    // And we check optional arguments
    s_bool bValid = true;
    foreach (iterArg, pArgList_->lOptional_)
    {
        if (lua_type(pLua_, i) != LUA_TNIL)
        {
            if (!iterArg->second.Test(pLua_, i, bPrintError))
                bValid = false;
        }
        i++;
    }

    return bValid;
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
    if (uiReturnNbr_ != 0)
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
