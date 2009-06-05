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

Argument::Argument( const s_str& sName, Lua::Type mLuaType, ValueType mType, s_ptr<Function> pParent )
{
    lData_.push_back(Data(sName, mLuaType, mType, this));
    pData_ = &lData_[0];
    pParent_ = pParent;
}

void Argument::Add( const s_str& sName, Lua::Type mLuaType, ValueType mType )
{
    lData_.push_back(Data(sName, mLuaType, mType, this));
    if (lData_.size() == 1)
        pData_ = &lData_[0];
}

s_int Argument::GetI() const
{
    return pData_->GetValue().GetI();
}

s_uint Argument::GetUI() const
{
    return pData_->GetValue().GetUI();
}

s_float Argument::GetF() const
{
    return pData_->GetValue().GetF();
}

s_bool Argument::GetB() const
{
    return pData_->GetValue().GetB();
}

s_str Argument::GetS() const
{
    return pData_->GetValue().GetS();
}

const void* Argument::GetP() const
{
    return pData_->GetValue().GetP();
}

Lua::Type Argument::GetType() const
{
    return pData_->GetLuaType();
}

const s_bool& Argument::IsProvided() const
{
    return bSet_;
}

s_bool Argument::Test( s_ptr<Lua::State> pLua, const s_uint& uiIndex, const s_bool& bPrintError )
{
    s_bool bSeveralChoices = (lData_.size() > 1);

    Lua::Type mType = pLua->GetType(uiIndex);
    vector<Data>::iterator iterData;
    foreach (iterData, lData_)
    {
        if (mType != iterData->GetLuaType())
        {
            if (bPrintError && !bSeveralChoices)
            {
                pLua->PrintError(
                    "Argument " +
                    s_str(uiIndex) +
                    " of \"" +
                    pParent_->GetName() +
                    "\" must be a " +
                    s_str(pLua->GetTypeName(iterData->GetLuaType())) +
                    " (" +
                    iterData->GetName() +
                    ")."
                );
            }
        }
        else
        {
            iterData->Set(pLua, uiIndex);
            break;
        }
    }

    if (!bSet_)
    {
        if (bPrintError && bSeveralChoices)
        {
            s_str sEnum = "";
            uint i = 0;
            foreach (iterData, lData_)
            {
                if (i != lData_.size()-2)
                {
                    if (iterData != lData_.begin())
                        sEnum += ", ";
                    sEnum += "a ";
                }
                else
                    sEnum += " or a ";

                sEnum += s_str(pLua->GetTypeName(iterData->GetLuaType())) +
                    " (" + iterData->GetName() + ")";
                ++i;
            }
            pLua->PrintError(
                "Argument " +
                s_str(uiIndex) +
                " of \"" +
                pParent_->GetName() +
                "\" must be " +
                sEnum +
                "."
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

s_ptr<Data> Argument::Get() const
{
    return pData_;
}
