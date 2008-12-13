/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*               Lua source               */
/*                                        */

#include "frost_lua_utils.h"

using namespace std;
using namespace Frost;
using namespace Frost::Lua;

const s_str Argument::CLASS_NAME = "Lua::Argument";

Argument::Argument()
{
    bSet_ = false;
    pData_ = NULL;
    pParent_ = NULL;
}

Argument::Argument( s_str sName, int iLuaType, ValueType mType, s_ptr<Function> pParent )
{
    bSet_ = false;
    lData_.push_back(Data(sName, iLuaType, mType, this));
    pData_ = &lData_[0];
    pParent_ = pParent;
}

void Argument::Add( s_str sName, int iLuaType, ValueType mType )
{
    lData_.push_back(Data(sName, iLuaType, mType, this));
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

int Argument::GetType() const
{
    return pData_->GetLuaType();
}

s_bool Argument::IsProvided() const
{
    return bSet_;
}

s_bool Argument::Test( lua_State* pLua, s_uint uiIndex, s_bool bPrintError )
{
    s_bool bSeveralChoices = (lData_.size() > 1);

    int iType = lua_type(pLua, uiIndex.Get());
    vector<Data>::iterator iterData;
    foreach (iterData, lData_)
    {
        if (iType != iterData->GetLuaType())
        {
            if (bPrintError && !bSeveralChoices)
            {
                PrintError(pLua,
                    "Argument " +
                    s_str(uiIndex) +
                    " of \"" +
                    pParent_->GetName() +
                    "\" must be a " +
                    s_str(lua_typename(pLua, iterData->GetLuaType())) +
                    " value (" +
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

                sEnum += s_str(lua_typename(pLua, iterData->GetLuaType())) +
                    " value (" + iterData->GetName() + ")";
                i++;
            }
            PrintError(pLua,
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
