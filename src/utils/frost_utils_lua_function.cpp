/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_utils_lua_state.h"
#include "frost_utils_lua_function.h"
#include "frost_utils_luamanager.h"
#include "frost_utils_stdhelper.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

const s_str Function::CLASS_NAME = "Lua::Function";

Function::Function( const s_str& sName, lua_State* pLua, const s_uint& uiReturnNbr ) :
    sName_(sName), uiReturnNbr_(uiReturnNbr)
{
    pLua_ = LuaManager::GetSingleton()->GetState(pLua);
    NewParamSet();
}

Function::Function( const s_str& sName, s_ptr<Lua::State> pLua, const s_uint& uiReturnNbr ) :
    sName_(sName), pLua_(pLua), uiReturnNbr_(uiReturnNbr)
{
    NewParamSet();
}

Function::~Function()
{
    map< s_uint, s_ptr<Argument> >::iterator iter;
    foreach (iter, pArgList_->lOptional_)
    {
        iter->second.Delete();
    }
    foreach (iter, pArgList_->lArg_)
    {
        iter->second.Delete();
    }
}

void Function::Add( const s_uint& uiIndex, const s_str& sName, Lua::Type mLuaType, ValueType mType, const s_bool& bOptional )
{
    if (bOptional)
    {
        if (MAPFIND(uiIndex, pArgList_->lOptional_))
        {
            pArgList_->lOptional_[uiIndex]->Add(sName, mLuaType, mType);
        }
        else
        {
            pArgList_->lOptional_[uiIndex] = new Argument(sName, mLuaType, mType, this);
        }
    }
    else
    {
        if (MAPFIND(uiIndex, pArgList_->lArg_))
        {
            pArgList_->lArg_[uiIndex]->Add(sName, mLuaType, mType);
        }
        else
        {
            pArgList_->lArg_[uiIndex] = new Argument(sName, mLuaType, mType, this);
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
        return pArgList_->lArg_[uiIndex];
    }
    if (MAPFIND(uiIndex, pArgList_->lOptional_))
    {
        return pArgList_->lOptional_[uiIndex];
    }

    return NULL;
}

s_bool Function::IsProvided( const s_uint& uiIndex ) const
{
    map< s_uint, s_ptr<Argument> >::const_iterator iter = pArgList_->lArg_.find(uiIndex);
    if (iter != pArgList_->lArg_.end())
    {
        return iter->second->IsProvided();
    }
    iter = pArgList_->lOptional_.find(uiIndex);
    if (iter != pArgList_->lOptional_.end())
    {
        return iter->second->IsProvided();
    }

    return false;
}

const s_uint& Function::GetArgumentCount() const
{
    return uiArgumentCount_;
}

s_bool Function::Check( const s_bool& bPrintError )
{
    uiArgumentCount_ = pLua_->GetTop();

    // Check if that's enough
    vector<ArgumentList>::iterator iterArgList;
    vector< s_ptr<ArgumentList> > lValidArgList;
    foreach (iterArgList, lArgListStack_)
    {
        if (uiArgumentCount_ >= iterArgList->lArg_.size())
            lValidArgList.push_back(&(*iterArgList));
    }

    if (lValidArgList.empty())
    {
        if (bPrintError)
        {
            s_str sError;
            foreach (iterArgList, lArgListStack_)
            {
                s_str sArguments = "\n  - ["+s_uint((uint)iterArgList->lArg_.size())+"] : ";
                map< s_uint, s_ptr<Argument> >::iterator iterArg;
                foreach (iterArg, iterArgList->lArg_)
                {
                    if (iterArg != iterArgList->lArg_.begin())
                        sArguments += ", ";
                    sArguments += iterArg->second->Get()->GetName();
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
                        sArguments += iterArg->second->Get()->GetName();
                    }
                    sArguments += ")";
                }
                sError += sArguments;
            }

            if (lArgListStack_.size() == 1)
            {
                pLua_->PrintError(
                    "Too few arguments in \""+sName_+"\". Expected :"+sError
                );
            }
            else
            {
                pLua_->PrintError(
                    "Too few arguments in \""+sName_+"\". Expected either :"+sError
                );
            }
        }
        // So if there isn't enough, just return false
        return false;
    }

    // We then check the value type
    s_int i;
    map< s_uint, s_ptr<Argument> >::iterator iterArg;
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
                if (!iterArg->second->Test(pLua_, i, false))
                {
                    bValid = false;
                    break;
                }
                ++i;
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
                    s_str sArguments = "\n  - ["+s_uint((uint)iterArgList->lArg_.size())+"] : ";
                    map< s_uint, s_ptr<Argument> >::iterator iterArg;
                    foreach (iterArg, iterArgList->lArg_)
                    {
                        if (iterArg != iterArgList->lArg_.begin())
                            sArguments += ", ";
                        sArguments += iterArg->second->Get()->GetName();
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
                            sArguments += iterArg->second->Get()->GetName();
                        }
                        sArguments += ")";
                    }
                    sError += sArguments;
                }

                pLua_->PrintError(
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
            if (!iterArg->second->Test(pLua_, i, bPrintError))
                bValid = false;
            ++i;
        }

        if (!bValid)
        {
            return false;
        }
    }

    // We fill the stack with nil value until there are enough for optional arguments
    s_uint uiMaxArgs = pArgList_->lArg_.size() + pArgList_->lOptional_.size();
    if (uiArgumentCount_ < uiMaxArgs)
        pLua_->PushNil(uiMaxArgs - uiArgumentCount_);

    // And we check optional arguments
    s_bool bValid = true;
    foreach (iterArg, pArgList_->lOptional_)
    {
        if (pLua_->GetType(i) != Lua::TYPE_NIL)
        {
            if (!iterArg->second->Test(pLua_, i, bPrintError))
                bValid = false;
        }
        ++i;
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
        ++uiReturnNbr_;

    if (mReturn.mType == RETURN_NIL)
        pLua_->PushNil();
    else if (mReturn.mType == RETURN_NUMBER)
        pLua_->PushNumber(mReturn.vValue.GetF());
    else if (mReturn.mType == RETURN_BOOLEAN)
        pLua_->PushBool(mReturn.vValue.GetB());
    else if (mReturn.mType == RETURN_STRING)
        pLua_->PushString(mReturn.vValue.GetS());

    ++uiReturnCount_;
}

int Function::Return()
{
    if (uiReturnNbr_ != 0)
    {
        // Fill with nil value
        if (uiReturnCount_ < uiReturnNbr_)
            pLua_->PushNil(uiReturnNbr_ - uiReturnCount_);

        // Return the number of returned value
        return (int)uiReturnNbr_.Get();
    }
    else
        return 0;
}
