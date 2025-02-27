/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*           ZoneManager header           */
/*                                        */
/*                                        */


#ifndef FROST_ZONEMANAGER_H
#define FROST_ZONEMANAGER_H

#include "frost_prereqs.h"

#include <frost_editor.h>
#include <frost_utils_manager.h>
#include <frost_utils_eventreceiver.h>

namespace Frost
{
    /// Handles zone loading/unloading.
    class ZoneManager : public Manager<ZoneManager>, public EventReceiver
    {
    friend class Manager<ZoneManager>;
    public :

        /// Initializes this manager.
        void Initialize();

        /// Creates a new zone.
        /** \param sZoneName The name of the new zone to create
        *   \return A pointer to the new zone
        *   \note Unloads the previous zone (if any).
        *   \note Throws Exception if something goes wrong.
        */
        s_ptr<Zone> CreateZone(const s_str& sZoneName);

        /// Loads a new zone.
        /** \param sZoneName The name of the new zone to load
        *   \return A pointer to the new zone
        *   \note Unloads the previous zone (if any).
        */
        s_ptr<Zone> LoadZone(const s_str& sZoneName);

        /// Loads a new zone.
        /** \param sZoneFile The XML file containing the zone data
        *   \return A pointer to the new zone
        *   \note Unloads the previous zone (if any).
        */
        s_ptr<Zone> LoadZoneFile(const s_str& sZoneFile);

        /// Unloads the current Zone.
        /** \note This is automatically called by LoadZone() and
        *         LoadZoneFile().
        */
        void        UnloadZone();

        /// Saves the current Zone in a file.
        /** \param sZoneFile The file into which to save the Zone
        *   \note If sZoneFile is ommitted, the Zone is saved in the
        *         current zone file (if any).
        */
        void        SaveZone(const s_str& sZoneFile = "");

        /// Shows a Decal on the ground following the mouse.
        /** \param pDecal The decal to use
        */
        void        EnableMouseDecal(s_wptr<Decal> pDecal);

        /// Hides the mouse decal.
        void        DisableMouseDecal();

        /// Returns the mouse decal.
        /** \return The mouse decal
        */
        s_wptr<Decal> GetMouseDecal();

        /// Adds a new Decal on the ground.
        /** \param pDecal The decal to put on the ground
        */
        void        AddDecalOnGround(s_wptr<Decal> pDecal);

        /// Removes a Decal from the ground.
        /** \param pDecal The decal to remove
        */
        void        RemoveDecalFromGround(s_wptr<Decal> pDecal);

        /// Casts a ray to find the terrain position under the mouse.
        /** \return The terrain position under the mouse
        */
        Vector      GetTerrainUnderMouse();

        /// Returns the current Zone.
        /** \return The current Zone
        */
        s_ptr<Zone> GetCurrentZone();

        /// Returns the current Zone's file.
        const s_str& GetCurrentZoneFile();

        /// Checks if the current Zone has been saved.
        const s_bool& IsZoneSaved();

        /// Returns the Lua state used by zones.
        /** \return The Lua state used by zones
        */
        s_ptr<Lua::State> GetLua();

        /// Updates displayed chunks.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void        Update(const s_float& fDelta);

        /// Calls the OnEvent script.
        /** \param mEvent The Event that occured
        */
        void        OnEvent(const Event& mEvent);

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

        s_ptr<Zone> pCurrentZone_;
        s_str       sCurrentZoneFile_;
        s_bool      bZoneSaved_;

        s_ptr<Lua::State> pLua_;

        s_refptr<Decal> pMouseDecal_;

        s_ctnr< s_wptr<Decal> > lDecalList_;
    };

    class AddDoodadAction : public EditorAction
    {
    public :

        AddDoodadAction(const s_str& sName, const s_str& sModel);

        virtual void Do();
        virtual void Undo();

    private :

        s_str sName_;
        s_str sModel_;
    };
}

#endif
