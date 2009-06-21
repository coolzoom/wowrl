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

#include "unit/frost_character.h"

namespace Frost
{
    /// Handles Unit creation.
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
        /** \param uiID The Unit's ID
        *   \return The Unit associated to the provided ID (NULL if none)
        */
        s_ptr<Unit>               GetUnitByID(const s_uint& uiID) const;

        void                      SelectOneUnit(s_ptr<Unit> pUnit);

        void                      AddUnitToSelection(s_ptr<Unit> pUnit);

        void                      AddUnitListToSelection(s_ctnr< s_ptr<Unit> > lUnitList);

        void                      ClearSelection();

        /// Reads all xml files related to Units.
        /** \return 'true' if everything went fine
        */
        s_bool                    ParseData();

        /// Returns the complete list of spell school.
        /** \return The complete list of spell school
        */
        const s_ctnr<s_str>&      GetSpellSchoolList();

        /// Returns the Class linked to the provided name.
        /** \param sClassName The name of the Class to get
        *   \return The Class
        */
        s_ptr<const Class>        GetClassByName(const s_str& sClassName) const;

        /// Returns the HealthType linked to the provided name.
        /** \param sHealthTypeName The name of the HealthType to get
        *   \return The HealthType
        */
        s_ptr<const HealthType>   GetHealthTypeByName(const s_str& sHealthTypeName) const;

        /// Returns the PowerType linked to the provided name.
        /** \param sPowerTypeName The name of the PowerType to get
        *   \return The PowerType
        */
        s_ptr<const PowerType>    GetPowerTypeByName(const s_str& sPowerTypeName) const;

        /// Returns the Lua::State used in spell scripts.
        /** \return The Lua::State used in spell scripts
        */
        s_ptr<Lua::State>         GetLua();

        /// Creates a Lua glue on the provided Lua::State.
        /** \param pLua The Lua::State on which to create this glue
        */
        void                      CreateGlue(s_ptr<Lua::State> pLua);

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

        /// Copy constructor
        UnitManager(const UnitManager& mMgr);

        /// Assignment operator
        UnitManager& operator = (const UnitManager& mMgr);

    private :

        s_bool ParseCharacterModels_();
        s_bool ParseRaces_();
        s_bool ParseClasses_();
        s_bool ParseHealthTypes_();
        s_bool ParsePowerTypes_();
        s_bool ParseCreatureModels_();
        s_bool ParseSpellSchools_();

        s_uint                       uiCounter_;
        s_map< s_uint, s_ptr<Unit> > lUnitList_;
        s_map<s_str, Race>           lRaceList_;
        s_map<s_str, Class>          lClassList_;
        s_map<s_str, HealthType>     lHealthTypeList_;
        s_map<s_str, PowerType>      lPowerTypeList_;

        s_ptr<Unit>                  pMouseOveredUnit_;
        s_map< s_uint, s_ptr<Unit> > lSelectedUnitList_;

        s_ctnr<s_str> lSchoolList_;

        s_ptr<Lua::State> pLua_;

    };

    /** \cond NOT_REMOVE_FROM_DOC
    */

    class LuaUnitManager
    {
    public :

        LuaUnitManager(lua_State* pLua);

        int _GetSelectedUnit(lua_State*);
        int _GetNearestUnit(lua_State*);

        int    GetDataTable(lua_State*);
        static const char className[];
        static Lunar<LuaUnitManager>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        lua_State*  pLua_;
        int         iRef_;
    };

    /** \endcond
    */
}

#endif
