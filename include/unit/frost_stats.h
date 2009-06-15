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

    /// Specialization of the Bonus class.
    template<>
    class Bonus<s_var>
    {
    public :

        /// Default constructor.
        Bonus() : fMul_(1.0f)
        {
        }

        /// Constructor (integer).
        Bonus(const s_int& mAdd, const s_float& fMul) : mAdd_(mAdd), fMul_(fMul)
        {
        }

        /// Constructor (float).
        Bonus(const s_float& mAdd, const s_float& fMul) : mAdd_(mAdd), fMul_(fMul)
        {
        }

        /// Returns the additive bonus (integer).
        /** \return The additive bonus (integer)
        */
        s_int GetAddBonusI() const
        {
            return mAdd_.Get<s_int>();
        }

        /// Returns the additive bonus (float).
        /** \return The additive bonus (float)
        */
        s_float GetAddBonusF() const
        {
            return mAdd_.Get<s_float>();
        }

        /// Returns the multiplicative bonus.
        /** \return The multiplicative bonus
        */
        const s_float& GetMulBonus() const
        {
            return fMul_;
        }

        /// Checks if the contained value is an integer.
        /** \return 'true' if the contained value is an integer
        */
        s_bool IsInteger() const
        {
            return (mAdd_.IsOfType<s_int>());
        }

        Bonus operator + (const Bonus& mValue) const
        {
            if (IsInteger() && mValue.IsInteger())
                return Bonus(mAdd_.Get<s_int>() + mValue.mAdd_.Get<s_int>(), fMul_ + mValue.fMul_);
            else if (!IsInteger() && !mValue.IsInteger())
                return Bonus(mAdd_.Get<s_float>() + mValue.mAdd_.Get<s_float>(), fMul_ + mValue.fMul_);
            else
            {
                Warning(CLASS_NAME, "Trying to add two bonuses with different types.");
                return Bonus();
            }
        }

        Bonus operator - (const Bonus& mValue) const
        {
            if (IsInteger() && mValue.IsInteger())
                return Bonus(mAdd_.Get<s_int>() - mValue.mAdd_.Get<s_int>(), fMul_ - mValue.fMul_);
            else if (!IsInteger() && !mValue.IsInteger())
                return Bonus(mAdd_.Get<s_float>() - mValue.mAdd_.Get<s_float>(), fMul_ - mValue.fMul_);
            else
            {
                Warning(CLASS_NAME, "Trying to substract two bonuses with different types.");
                return Bonus();
            }
        }

        void operator += (const Bonus& mValue)
        {
            if (IsInteger() && mValue.IsInteger())
            {
                mAdd_ = mAdd_.Get<s_int>() + mValue.mAdd_.Get<s_int>();
                fMul_ += mValue.fMul_;
            }
            else if (!IsInteger() && !mValue.IsInteger())
            {
                mAdd_ = mAdd_.Get<s_float>() + mValue.mAdd_.Get<s_float>();
                fMul_ += mValue.fMul_;
            }
            else
            {
                Warning(CLASS_NAME, "Trying to add two bonuses with different types.");
            }
        }

        void operator -= (const Bonus& mValue)
        {
            if (IsInteger() && mValue.IsInteger())
            {
                mAdd_ = mAdd_.Get<s_int>() - mValue.mAdd_.Get<s_int>();
                fMul_ -= mValue.fMul_;
            }
            else if (!IsInteger() && !mValue.IsInteger())
            {
                mAdd_ = mAdd_.Get<s_float>() - mValue.mAdd_.Get<s_float>();
                fMul_ -= mValue.fMul_;
            }
            else
            {
                Warning(CLASS_NAME, "Trying to substract two bonuses with different types.");
            }
        }

        static const s_str CLASS_NAME;

    private :

        s_var   mAdd_;
        s_float fMul_;
    };

    /// Holds an Item's statistics.
    struct BonusSet
    {
        BonusSet();

        // Generic bonuses (needed by the engine)
        Bonus<s_float> fMaxHealth;
        Bonus<s_float> fMaxPower;
        Bonus<s_float> fHealthRegenRatio;
        Bonus<s_float> fPowerRegenRatio;
        Bonus<s_float> fMovementSpeedPercent;

        // Dynamic bonuses
        std::map< s_str, Bonus<s_var> > lBonusList;
        /*Bonus<s_int> iAgility;
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
        std::map< s_str, Bonus<s_int> >   lSpellPenetration;*/

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

        void SetBaseValue(const T& mValue)
        {
            mValue_ = mValue;
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

    /// Specialization of the Characteristic class.
    template<>
    class Characteristic<s_var>
    {
    public :

        Characteristic()
        {
        }

        void SetBaseValue(const s_var& mValue)
        {
            mValue_ = mValue;
        }

        const s_var& GetBaseValue() const
        {
            return mValue_;
        }

        s_int GetBaseValueI() const
        {
            return mValue_.Get<s_int>();
        }

        s_float GetBaseValueF() const
        {
            return mValue_.Get<s_float>();
        }

        s_int GetBonusI() const
        {
            return s_int(s_float(mValue_.Get<s_int>() + mBonus_.GetAddBonusI())*mBonus_.GetMulBonus()) - mValue_.Get<s_int>();
        }

        s_float GetBonusF() const
        {
            return (mValue_.Get<s_float>() + mBonus_.GetAddBonusF())*mBonus_.GetMulBonus() - mValue_.Get<s_float>();
        }

        s_int GetValueI() const
        {
            return s_int(s_float(mValue_.Get<s_int>() + mBonus_.GetAddBonusI())*mBonus_.GetMulBonus());
        }

        s_float GetValueF() const
        {
            return (mValue_.Get<s_float>() + mBonus_.GetAddBonusF())*mBonus_.GetMulBonus();
        }

        void AddBonus(const Bonus<s_var>& mBonus)
        {
            mBonus_ += mBonus;
        }

        void RemoveBonus(const Bonus<s_var>& mBonus)
        {
            mBonus_ -= mBonus;
        }

        s_bool IsInteger() const
        {
            return mBonus_.IsInteger();
        }

        static const s_str CLASS_NAME;

    private :

        s_var        mValue_;
        Bonus<s_var> mBonus_;
    };

    /// Holds a Unit's statistics.
    struct Stats
    {
        Stats();

        void AddBonusSet(BonusSet& mBonusSet);
        void RemoveBonusSet(BonusSet& mBonusSet);

        // Generic characteristics (needed by the engine)
        Characteristic<s_float> fMaxHealth;
        Characteristic<s_float> fMaxPower;
        Characteristic<s_float> fHealthRegenRatio;
        Characteristic<s_float> fPowerRegenRatio;
        Characteristic<s_float> fMovementSpeedPercent;

        // Dynamic characteristics
        std::map< s_str, Characteristic<s_var> > lCharactList;
        /*Characteristic<s_int> iAgility;
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
        std::map< s_str, Characteristic<s_int> >   lSpellPenetration;*/

        static const s_str CLASS_NAME;
    };
}

#endif
