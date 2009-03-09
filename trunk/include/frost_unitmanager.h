/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           UnitManager source           */
/*                                        */
/*                                        */


#ifndef FROST_UNITMANAGER_H
#define FROST_UNITMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

#include "frost_character.h"

namespace Frost
{
    class UnitManager : public Manager<UnitManager>
    {
    friend class Manager<UnitManager>;
    public :

        /// Prepares this class for being used.
        void                      Initialize();

        /// Creates a new Character.
        s_ptr<Character>          CreateCharacter(const s_str& sName, const s_str& sRace, CharGender mGender);

        /// Creates a new Creature.
        s_ptr<Creature>           CreateCreature(const s_str& sName);

        /// Deletes a particular Unit.
        void                      DeleteUnit(s_ptr<Unit> pUnit);

        /// Returns the Unit associated to the provided ID.
        /** \param uiID The ID
        *   \return The Unit associated to the provided ID (NULL if none)
        */
        s_ptr<Unit>               GetUnitByID(const s_uint& uiID) const;

        /// Reads CharacterModels.xml.
        s_bool                    ParseCharacterModels();

        /// Reads Races.xml.
        s_bool                    ParseRaces();

        /// Reads CreatureModels.xml.
        s_bool                    ParseCreatureModels();

        /// Reads SpellSchools.xml.
        s_bool                    ParseSpellSchools();

        /// Returns the complete list of spell school.
        /** \return The complete list of spell school
        */
        const std::vector<s_str>& GetSpellSchoolList();

        /// Returns the Lua::State used in spell scripts.
        /** \return The Lua::State used in spell scripts
        */
        s_ptr<Lua::State>         GetLua();

        /// Updates all units.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void                      UpdateUnits(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call UnitManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling UnitManager::Delete() (this is
        *         automatically done by Engine).
        */
        UnitManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~UnitManager();

    private :

        s_uint                          uiCounter_;
        std::map< s_uint, s_ptr<Unit> > lUnitList_;
        std::map<s_str, Race>           lRaceList_;

        std::vector<s_str> lSchoolList_;

        s_ptr<Lua::State> pLua_;

    };
}

#endif
