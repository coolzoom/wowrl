/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_utils_lua_state.h"
#include "frost_utils_lua_function.h"
#include "frost_utils_stdhelper.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

const s_str Argument::CLASS_NAME = "Lua::Argument";

Argument::Argument( const s_str& sName, Lua::Type mLuaType, s_ptr<Function> pParent )
{
    lData_.PushBack(Data(sName, mLuaType, this));
    pData_ = &lData_[0];
    pParent_ = pParent;
    pLua_ = pParent_->GetState();
}

void Argument::Add( const s_str& sName, Lua::Type mLuaType )
{
    lData_.PushBack(Data(sName, mLuaType, this));
    if (lData_.GetSize() == 1)
        pData_ = &lData_[0];
}

s_float Argument::GetNumber() const
{
    return pData_->GetValue().Get<s_float>();
}

s_bool Argument::GetBool() const
{
    return pData_->GetValue().Get<s_bool>();
}

s_str Argument::GetString() const
{
    return pData_->GetValue().Get<s_str>();
}

s_int Argument::GetIndex() const
{
    return pData_->GetValue().Get<s_int>();
}

Lua::Type Argument::GetType() const
{
    return pData_->GetType();
}

const s_bool& Argument::IsProvided() const
{
    return bSet_;
}

s_bool Argument::Test( s_ptr<Lua::State> pLua, const s_int& iIndex, const s_bool& bPrintError )
{
    s_bool bSeveralChoices = (lData_.GetSize() > 1);

    Lua::Type mType = pLua->GetType(iIndex);
    s_ctnr<Data>::iterator iterData;
    foreach (iterData, lData_)
    {
        if (mType != iterData->GetType())
        {
            if (bPrintError && !bSeveralChoices)
            {
                pLua->PrintError(
                    "Argument "+iIndex+" of \""+pParent_->GetName()+"\" "
                    "must be a "+pLua->GetTypeName(iterData->GetType())+" "
                    "("+iterData->GetName()+") (got a "+pLua->GetTypeName(mType)+")."
                );
            }
        }
        else
        {
            iterData->Set(pLua, iIndex);
            break;
        }
    }

    if (!bSet_)
    {
        if (bPrintError && bSeveralChoices)
        {
            s_str sEnum = "";
            s_uint i = 0;
            foreach (iterData, lData_)
            {
                if (i == 0)
                    sEnum += "a ";
                else
                {
                    if (i == lData_.GetSize()-1)
                        sEnum += ", or a ";
                    else
                        sEnum += ", a ";
                }

                sEnum += pLua->GetTypeName(iterData->GetType())+" ("+iterData->GetName()+")";
                ++i;
            }
            pLua->PrintError(
                "Argument "+iIndex+" of \""+pParent_->GetName()+"\" "
                "must be "+sEnum+" (got a "+pLua->GetTypeName(mType)+")."
            );
        }
    }

    return bSet_;
}

void Argument::SetData( s_ptr<Data> pData )
{
    bSet_ = true;
    pData_ = pData;
}

s_ptr<Data> Argument::GetData() const
{
    return pData_;
}
