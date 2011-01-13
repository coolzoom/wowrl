/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          PhysicsManager header         */
/*                                        */
/*                                        */


#ifndef FROST_PHYSICSMANAGER_H
#define FROST_PHYSICSMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Handles all PhysicsHandlers.
    /** \note Collision detection is done by PhysicsHandler and
    *         its derivated classes.
    */
    class PhysicsManager : public Manager<PhysicsManager>
    {
    friend class Manager<PhysicsManager>;
    public :

        /// Reads configuration.
        /** \return 'false' if something went wrong
        */
        s_bool                ReadConfig();

        /// Registers a pre-created handler.
        /** \param pHandler The handler
        *   \note You must create this handler yourself. If you don't need
        *         complexe forces, you can use the default handler and call
        *         CreateHandler() instead.<br><br>
        *         Once this function has been called, the provided handler
        *         must be deleted with DeleteHandler().
        */
        void                  RegisterHandler(s_ptr<PhysicsHandler> pHandler);

        /// Removes a handler.
        /** \param pHandler The handler to remove
        */
        void                  RemoveHandler(s_ptr<PhysicsHandler> pHandler);

        /// Sets the direction and intensity of the gravity field.
        /** \param mGravity The gravity force
        *   \note Default is (0, -9.81, 0).
        */
        void                  SetGravity(const Vector& mGravity);

        /// Returns the gravity force.
        /** \return The gravity force
        */
        const Vector&         GetGravity() const;

        /// Returns the maximum number of collision recursion per frame and object.
        /** \return The maximum number of collision recursion per frame and object
        */
        const s_uint&         GetMaxCollisionRecursion() const;

        /// Checks if the collision detection algorithm is run at a fixed rate.
        /** \return 'true' if it is the case
        *   \note See more comments about this feature in
        *               MovableUnitHandler::Update().
        */
        const s_bool&         IsCollisionDetectionFixedRate() const;

        /// Returns the maximum number of collision detection test done per second.
        /** \return the maximum number of collision detection test done per second
        */
        const s_float&        GetCollisionDetectionRate() const;

        /// Adds a new obstacle to the scene.
        /** \param pObstacle The obstacle to add
        *   \note Automatically enables the obstacle.<br>
        *         Note that you are responsible for deleting this
        *         Obstacle yourself whenever it is no longer necessary.
        *         The obstacle will take care of notifying this manager
        *         of its destruction if you forget to do so.
        */
        void                  AddObstacle(s_ptr<Obstacle> pObstacle);

        /// Removes an obstacle from the scene.
        /** \param pObstacle The obstacle to remove
        *   \note It is still your responsability to destroy the obstacle
        *         yourself.
        */
        void                  RemoveObstacle(s_ptr<Obstacle> pObstacle);

        /// Removes all obstacles from the scene.
        void                  ClearObstacles();

        /// Returns the list of all active obstacles.
        /** \return The list of all active obstacles
        */
        const s_ctnr< s_ptr<Obstacle> >& GetObstacleList() const;

        /// Updates handlers and obstacles.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void                  Update(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call PhysicsManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling PhysicsManager::Delete() (this is
        *         automatically done by Engine).
        */
        PhysicsManager();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~PhysicsManager();

        /// Copy constructor.
        PhysicsManager(const PhysicsManager& mMgr);

        /// Assignment operator.
        PhysicsManager& operator = (const PhysicsManager& mMgr);

    private :

        s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> > lHandlerList_;
        s_ctnr< s_ptr<Obstacle> >                            lObstacleList_;

        Vector  mGravity_;
        s_uint  uiMaxCollisionRecursion_;
        s_bool  bFixedRateCollisionDetection_;
        s_float fCollisionDetectionRate_;

    };
}

#endif
