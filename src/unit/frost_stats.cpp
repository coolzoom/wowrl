/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Stats source              */
/*                                        */
/*                                        */

#include "unit/frost_stats.h"
#include "unit/frost_unitmanager.h"

using namespace std;

namespace Frost
{
    const s_str BonusSet::CLASS_NAME = "BonusSet";
    const s_str Bonus<s_var>::CLASS_NAME = "Bonus<s_var>";
    const s_str Characteristic<s_var>::CLASS_NAME = "Characteristic<s_var>";

    BonusSet::BonusSet()
    {
    }

    const s_str Stats::CLASS_NAME = "Stats";

    Stats::Stats()
    {
        // NOTE : Default values are there for testing purposes.
        fMaxHealth.SetBaseValue(1000.0f);
        fMaxPower.SetBaseValue(2000.0f);
        fHealthRegenRatio.SetBaseValue(1.0f);
        fPowerRegenRatio.SetBaseValue(1.0f);
        fMovementSpeedPercent.SetBaseValue(1.0f);
    }

    void Stats::AddBonusSet( BonusSet& mBonusSet )
    {
        fMaxHealth.AddBonus(mBonusSet.fMaxHealth);
        fMaxPower.AddBonus(mBonusSet.fMaxPower);
        fHealthRegenRatio.AddBonus(mBonusSet.fHealthRegenRatio);
        fPowerRegenRatio.AddBonus(mBonusSet.fPowerRegenRatio);
        fMovementSpeedPercent.AddBonus(mBonusSet.fMovementSpeedPercent);

        map< s_str, Bonus<s_var> >::iterator iterBonus;
        foreach (iterBonus, mBonusSet.lBonusList)
        {
            if (MAPFIND(iterBonus->first, lCharactList))
            {
                lCharactList[iterBonus->first].AddBonus(iterBonus->second);
            }
            else
            {
                Warning(CLASS_NAME,
                    "Characteristic \""+iterBonus->first+"\" doesn't exist."
                );
            }
        }
    }

    void Stats::RemoveBonusSet( BonusSet& mBonusSet )
    {
        fMaxHealth.RemoveBonus(mBonusSet.fMaxHealth);
        fMaxPower.RemoveBonus(mBonusSet.fMaxPower);
        fHealthRegenRatio.RemoveBonus(mBonusSet.fHealthRegenRatio);
        fPowerRegenRatio.RemoveBonus(mBonusSet.fPowerRegenRatio);
        fMovementSpeedPercent.RemoveBonus(mBonusSet.fMovementSpeedPercent);

        map< s_str, Bonus<s_var> >::iterator iterBonus;
        foreach (iterBonus, mBonusSet.lBonusList)
        {
            if (MAPFIND(iterBonus->first, lCharactList))
            {
                lCharactList[iterBonus->first].RemoveBonus(iterBonus->second);
            }
            else
            {
                Warning(CLASS_NAME,
                    "Characteristic \""+iterBonus->first+"\" doesn't exist."
                );
            }
        }
    }
}
