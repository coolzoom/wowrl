/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ZoneManager header           */
/*                                        */
/*                                        */


#ifndef FROST_ZONEMANAGER_H
#define FROST_ZONEMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Handles zone loading/unloading.
    class ZoneManager : public Manager<ZoneManager>
    {
    friend class Manager<ZoneManager>;
    public :

        /// Initializes this manager.
        void Initialize();

        /// Loads a new zone.
        /** \param sZoneName The name of the new zone to load
        *   \return A pointer to the new zone
        *   \note Unloads the previous zone (if any).
        */
        s_ptr<Zone> LoadZone(const s_str& sZoneName);

        /// Returns the Lua state used by zones.
        /** \return The Lua state used by zones
        */
        s_ptr<Lua::State> GetLua();

        /// Updates displayed chunks.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call ZoneManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling ZoneManager::Delete() (this is
        *         automatically done by Engine).
        */
        ZoneManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~ZoneManager();

        /// Copy constructor.
        ZoneManager(const ZoneManager& mMgr);

        /// Assignment operator.
        ZoneManager& operator = (const ZoneManager& mMgr);

    private :

        void ParseXMLFile_(const s_str& sFile, s_ptr<Zone> pCurrentZone);
        void ParseTerrain_(s_ptr<Zone> pCurrentZone, s_ptr<XML::Block> pTerrainBlock);

        s_ptr<Zone> pCurrentZone_;
        s_ptr<Lua::State> pLua_;

    };
}

#endif
