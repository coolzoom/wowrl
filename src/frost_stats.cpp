/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Stats source              */
/*                                        */
/*                                        */

#include "frost_stats.h"
#include "frost_unitmanager.h"

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

        /*iAgility.AddBonus(mBonusSet.iAgility);
        iStamina.AddBonus(mBonusSet.iStamina);
        iIntellect.AddBonus(mBonusSet.iIntellect);
        iSpirit.AddBonus(mBonusSet.iSpirit);
        iStrength.AddBonus(mBonusSet.iStrength);

        iArmor.AddBonus(mBonusSet.iArmor);
        iDefense.AddBonus(mBonusSet.iDefense);
        iBlock.AddBonus(mBonusSet.iBlock);
        fBlockChance.AddBonus(mBonusSet.fBlockChance);
        iAttackPower.AddBonus(mBonusSet.iAttackPower);
        fCriticalHitChance.AddBonus(mBonusSet.fCriticalHitChance);
        fHitChance.AddBonus(mBonusSet.fHitChance);
        fDodgeChance.AddBonus(mBonusSet.fDodgeChance);

        const vector<s_str>& lSchoolList = UnitManager::GetSingleton()->GetSpellSchoolList();
        vector<s_str>::const_iterator iterSchool;
        foreach (iterSchool, lSchoolList)
        {
            lSpellDamageBonus[*iterSchool].AddBonus(mBonusSet.lSpellDamageBonus[*iterSchool]);
            lSpellDamageBonusPercent[*iterSchool].AddBonus(mBonusSet.lSpellDamageBonusPercent[*iterSchool]);
            lSpellResistancePercent[*iterSchool].AddBonus(mBonusSet.lSpellResistancePercent[*iterSchool]);
            lSpellCriticalHitChance[*iterSchool].AddBonus(mBonusSet.lSpellCriticalHitChance[*iterSchool]);
            lSpellHitChance[*iterSchool].AddBonus(mBonusSet.lSpellHitChance[*iterSchool]);
            lSpellPenetration[*iterSchool].AddBonus(mBonusSet.lSpellPenetration[*iterSchool]);
        }*/
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

        /*iAgility.RemoveBonus(mBonusSet.iAgility);
        iStamina.RemoveBonus(mBonusSet.iStamina);
        iIntellect.RemoveBonus(mBonusSet.iIntellect);
        iSpirit.RemoveBonus(mBonusSet.iSpirit);
        iStrength.RemoveBonus(mBonusSet.iStrength);

        iArmor.RemoveBonus(mBonusSet.iArmor);
        iBlock.RemoveBonus(mBonusSet.iBlock);
        iAttackPower.RemoveBonus(mBonusSet.iAttackPower);
        fCriticalHitChance.RemoveBonus(mBonusSet.fCriticalHitChance);
        fHitChance.RemoveBonus(mBonusSet.fHitChance);

        const vector<s_str>& lSchoolList = UnitManager::GetSingleton()->GetSpellSchoolList();
        vector<s_str>::const_iterator iterSchool;
        foreach (iterSchool, lSchoolList)
        {
            lSpellDamageBonus[*iterSchool].RemoveBonus(mBonusSet.lSpellDamageBonus[*iterSchool]);
            lSpellDamageBonusPercent[*iterSchool].RemoveBonus(mBonusSet.lSpellDamageBonusPercent[*iterSchool]);
            lSpellResistancePercent[*iterSchool].RemoveBonus(mBonusSet.lSpellResistancePercent[*iterSchool]);
            lSpellCriticalHitChance[*iterSchool].RemoveBonus(mBonusSet.lSpellCriticalHitChance[*iterSchool]);
            lSpellHitChance[*iterSchool].RemoveBonus(mBonusSet.lSpellHitChance[*iterSchool]);
            lSpellPenetration[*iterSchool].RemoveBonus(mBonusSet.lSpellPenetration[*iterSchool]);
        }*/
    }
}
