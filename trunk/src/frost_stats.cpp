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

    BonusSet::BonusSet()
    {
    }

    const s_str Stats::CLASS_NAME = "Stats";

    Stats::Stats()
    {
    }

    void Stats::AddBonusSet( BonusSet& mBonusSet )
    {
        iMaxHealth.AddBonus(mBonusSet.iMaxHealth);
        iMaxPower.AddBonus(mBonusSet.iMaxPower);

        iAgility.AddBonus(mBonusSet.iAgility);
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
        }

        iHealthRenegeration.AddBonus(mBonusSet.iHealthRenegeration);
        iPowerRegeneration.AddBonus(mBonusSet.iPowerRegeneration);

        fMovementSpeedPercent.AddBonus(mBonusSet.fMovementSpeedPercent);
    }

    void Stats::RemoveBonusSet( BonusSet& mBonusSet )
    {
        iMaxHealth.RemoveBonus(mBonusSet.iMaxHealth);
        iMaxPower.RemoveBonus(mBonusSet.iMaxPower);

        iAgility.RemoveBonus(mBonusSet.iAgility);
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
        }

        iHealthRenegeration.RemoveBonus(mBonusSet.iHealthRenegeration);
        iPowerRegeneration.RemoveBonus(mBonusSet.iPowerRegeneration);

        fMovementSpeedPercent.RemoveBonus(mBonusSet.fMovementSpeedPercent);
    }
}
