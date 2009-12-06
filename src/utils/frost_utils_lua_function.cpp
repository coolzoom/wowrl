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
    s_map< s_uint, s_ptr<Argument> >::iterator iter;
    foreach (iter, pArgList_->lOptional_)
    {
        iter->second.Delete();
    }
    foreach (iter, pArgList_->lArg_)
    {
        iter->second.Delete();
    }
}

void Function::Add( const s_uint& uiIndex, const s_str& sName, Lua::Type mLuaType, const s_bool& bOptional )
{
    if (bOptional)
    {
        if (pArgList_->lOptional_.Find(uiIndex))
        {
            pArgList_->lOptional_[uiIndex]->Add(sName, mLuaType);
        }
        else
        {
            pArgList_->lOptional_[uiIndex] = new Argument(sName, mLuaType, this);
        }
    }
    else
    {
        if (pArgList_->lArg_.Find(uiIndex))
        {
            pArgList_->lArg_[uiIndex]->Add(sName, mLuaType);
        }
        else
        {
            pArgList_->lArg_[uiIndex] = new Argument(sName, mLuaType, this);
        }
    }
}

void Function::NewParamSet()
{
    lArgListStack_.PushBack(ArgumentList());
    pArgList_ = &lArgListStack_.Back();
    pArgList_->uiRank_ = lArgListStack_.GetSize()-1;
}

const s_uint& Function::GetParamSetRank()
{
    return pArgList_->uiRank_;
}

s_ptr<Argument> Function::Get( const s_uint& uiIndex )
{
    if (pArgList_->lArg_.Find(uiIndex))
    {
        return pArgList_->lArg_[uiIndex];
    }
    if (pArgList_->lOptional_.Find(uiIndex))
    {
        return pArgList_->lOptional_[uiIndex];
    }

    return nullptr;
}

s_bool Function::IsProvided( const s_uint& uiIndex ) const
{
    s_map< s_uint, s_ptr<Argument> >::const_iterator iter = pArgList_->lArg_.Get(uiIndex);
    if (iter != pArgList_->lArg_.End())
    {
        return iter->second->IsProvided();
    }
    iter = pArgList_->lOptional_.Get(uiIndex);
    if (iter != pArgList_->lOptional_.End())
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
    s_ctnr<ArgumentList>::iterator iterArgList;
    s_ctnr< s_ptr<ArgumentList> > lValidArgList;
    foreach (iterArgList, lArgListStack_)
    {
        if (uiArgumentCount_ >= iterArgList->lArg_.GetSize())
            lValidArgList.PushBack(&(*iterArgList));
    }

    if (lValidArgList.IsEmpty())
    {
        if (bPrintError)
        {
            s_str sError;
            foreach (iterArgList, lArgListStack_)
            {
                s_str sArguments = "\n  - ["+iterArgList->lArg_.GetSize()+"] : ";
                map< s_uint, s_ptr<Argument> >::iterator iterArg;
                foreach (iterArg, iterArgList->lArg_)
                {
                    if (iterArg != iterArgList->lArg_.Begin())
                        sArguments += ", ";
                    sArguments += iterArg->second->GetData()->GetName();
                }
                if (iterArgList->lOptional_.GetSize() > 0)
                {
                    if (sArguments != "")
                        sArguments += ", ";
                    sArguments += "(+";
                    foreach (iterArg, iterArgList->lOptional_)
                    {
                        if (iterArg != iterArgList->lOptional_.Begin())
                            sArguments += ", ";
                        sArguments += iterArg->second->GetData()->GetName();
                    }
                    sArguments += ")";
                }
                sError += sArguments;
            }

            if (lArgListStack_.GetSize() == 1)
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
    s_map< s_uint, s_ptr<Argument> >::iterator iterArg;
    if (lValidArgList.GetSize() > 1)
    {
        s_ctnr< s_ptr<ArgumentList> >::iterator iterArgListPtr;
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
                    s_str sArguments = "\n  - ["+iterArgList->lArg_.GetSize()+"] : ";
                    s_map< s_uint, s_ptr<Argument> >::iterator iterArg;
                    foreach (iterArg, iterArgList->lArg_)
                    {
                        if (iterArg != iterArgList->lArg_.Begin())
                            sArguments += ", ";
                        sArguments += iterArg->second->GetData()->GetName();
                    }
                    if (iterArgList->lOptional_.GetSize() > 0)
                    {
                        if (sArguments != "")
                            sArguments += ", ";
                        sArguments += "(+";
                        foreach (iterArg, iterArgList->lOptional_)
                        {
                            if (iterArg != iterArgList->lOptional_.Begin())
                                sArguments += ", ";
                            sArguments += iterArg->second->GetData()->GetName();
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
        pArgList_ = lValidArgList.Front();
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
    s_uint uiMaxArgs = pArgList_->lArg_.GetSize() + pArgList_->lOptional_.GetSize();
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

void Function::Push( const s_var& vValue )
{
    if (uiReturnCount_ == uiReturnNbr_)
        ++uiReturnNbr_;

    pLua_->PushVar(vValue);

    ++uiReturnCount_;
}

void Function::PushNil( const s_uint& uiNbr )
{
    for (s_uint ui = 0; ui < uiNbr; ++ui)
    {
        if (uiReturnCount_ == uiReturnNbr_)
            ++uiReturnNbr_;

        pLua_->PushNil();

        ++uiReturnCount_;
    }
}

void Function::NotifyPushed()
{
    if (uiReturnCount_ == uiReturnNbr_)
        ++uiReturnNbr_;

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
        return static_cast<int>(uiReturnNbr_.Get());
    }
    else
        return 0;
}

s_ptr<Lua::State> Function::GetState() const
{
    return pLua_;
}
