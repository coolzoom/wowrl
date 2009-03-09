/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*              Stats header              */
/*                                        */
/*                                        */


#ifndef FROST_STATS_H
#define FROST_STATS_H

#include "frost.h"

namespace Frost
{
    /// Stores an additive and a multiplicative bonus.
    template<class T>
    class Bonus
    {
    public :

        /// Default constructor.
        Bonus() : fMul_(1.0f)
        {
        }

        /// Constructor.
        Bonus(const T& mAdd, const s_float& fMul) : mAdd_(mAdd), fMul_(fMul)
        {
        }

        /// Returns the additive bonus.
        /** \return The additive bonus
        */
        const T& GetAddBonus() const
        {
            return mAdd_;
        }

        /// Returns the multiplicative bonus.
        /** \return The multiplicative bonus
        */
        const s_float& GetMulBonus() const
        {
            return fMul_;
        }

        Bonus operator + (const Bonus& mValue) const
        {
            return Bonus(mAdd_ + mValue.mAdd_, fMul_ + mValue.fMul_);
        }

        Bonus operator - (const Bonus& mValue) const
        {
            return Bonus(mAdd_ - mValue.mAdd_, fMul_ - mValue.fMul_);
        }

        void operator += (const Bonus& mValue)
        {
            mAdd_ += mValue.mAdd_; fMul_ += mValue.fMul_;
        }

        void operator -= (const Bonus& mValue)
        {
            mAdd_ -= mValue.mAdd_; fMul_ -= mValue.fMul_;
        }

        static const s_str CLASS_NAME;

    private :

        T       mAdd_;
        s_float fMul_;
    };

    template<class T>
    const s_str Bonus<T>::CLASS_NAME = "Bonus";

    /// Holds an Item's statistics.
    struct BonusSet
    {
        BonusSet();

        Bonus<s_int> iMaxHealth;
        Bonus<s_int> iMaxPower;

        Bonus<s_int> iAgility;
        Bonus<s_int> iStamina;
        Bonus<s_int> iIntellect;
        Bonus<s_int> iSpirit;
        Bonus<s_int> iStrength;

        Bonus<s_int>   iArmor;
        Bonus<s_int>   iDefense;
        Bonus<s_int>   iBlock;
        Bonus<s_float> fBlockChance;
        Bonus<s_int>   iAttackPower;
        Bonus<s_float> fCriticalHitChance;
        Bonus<s_float> fHitChance;
        Bonus<s_float> fDodgeChance;

        std::map< s_str, Bonus<s_int> >   lSpellDamageBonus;
        std::map< s_str, Bonus<s_float> > lSpellDamageBonusPercent;
        std::map< s_str, Bonus<s_float> > lSpellResistancePercent;
        std::map< s_str, Bonus<s_float> > lSpellCriticalHitChance;
        std::map< s_str, Bonus<s_float> > lSpellHitChance;
        std::map< s_str, Bonus<s_int> >   lSpellPenetration;

        Bonus<s_int> iHealthRenegeration;
        Bonus<s_int> iPowerRegeneration;

        Bonus<s_float> fMovementSpeedPercent;

        static const s_str CLASS_NAME;
    };


    /// Manages an ability score and its bonuses.
    template<class T>
    class Characteristic
    {
    public :

        Characteristic()
        {
        }

        const T& GetBaseValue() const
        {
            return mValue_;
        }

        T GetBonus() const
        {
            return T(s_float(mValue_ + mBonus_.GetAddBonus())*mBonus_.GetMulBonus()) - mValue_;
        }

        T GetValue() const
        {
            return T(s_float(mValue_ + mBonus_.GetAddBonus())*mBonus_.GetMulBonus());
        }

        void AddBonus(const Bonus<T>& mBonus)
        {
            mBonus_ += mBonus;
        }

        void RemoveBonus(const Bonus<T>& mBonus)
        {
            mBonus_ -= mBonus;
        }

        static const s_str CLASS_NAME;

    private :

        T mValue_;
        Bonus<T> mBonus_;
    };

    template<class T>
    const s_str Characteristic<T>::CLASS_NAME = "Characteristic";

    /// Holds a Unit's statistics.
    struct Stats
    {
        Stats();

        void AddBonusSet(BonusSet& mBonusSet);
        void RemoveBonusSet(BonusSet& mBonusSet);

        Characteristic<s_int> iMaxHealth;
        Characteristic<s_int> iMaxPower;

        Characteristic<s_int> iAgility;
        Characteristic<s_int> iStamina;
        Characteristic<s_int> iIntellect;
        Characteristic<s_int> iSpirit;
        Characteristic<s_int> iStrength;

        Characteristic<s_int>   iArmor;
        Characteristic<s_int>   iDefense;
        Characteristic<s_int>   iBlock;
        Characteristic<s_float> fBlockChance;
        Characteristic<s_int>   iAttackPower;
        Characteristic<s_float> fCriticalHitChance;
        Characteristic<s_float> fHitChance;
        Characteristic<s_float> fDodgeChance;

        std::map< s_str, Characteristic<s_int> >   lSpellDamageBonus;
        std::map< s_str, Characteristic<s_float> > lSpellDamageBonusPercent;
        std::map< s_str, Characteristic<s_float> > lSpellResistancePercent;
        std::map< s_str, Characteristic<s_float> > lSpellCriticalHitChance;
        std::map< s_str, Characteristic<s_float> > lSpellHitChance;
        std::map< s_str, Characteristic<s_int> >   lSpellPenetration;

        Characteristic<s_int> iHealthRenegeration;
        Characteristic<s_int> iPowerRegeneration;

        Characteristic<s_float> fMovementSpeedPercent;

        static const s_str CLASS_NAME;
    };
}

#endif
