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

        /// Sets the minimum sliding angle.
        /** \param fMinSlidingAngle The new minimum sliding angle
        *   \note See GetMinSlidingAngle() for more information.
        *   \note Only functionnal when sliding is enabled (gravity).
        */
        void                  SetMinSlidingAngle(const s_float& fMinSlidingAngle);

        /// Returns the minimum sliding angle.
        /** \return The minimum sliding angle (non dimensional angle)
        *   \note Whenever there is a collision, you calculate the absolute angle
        *         between the motion vector and the collision plane's normal.
        *         If this angle is greater than the value returned by this
        *         function, then the object can slide along this plane.
        *   \note Only functionnal when sliding is enabled (gravity).
        */
        const s_float&        GetMinSlidingAngle() const;

        /// Sets the maximum climbing angle.
        /** \param fMaxClimbingAngle The new maximum climbing angle
        *   \note See GetMaxClimbingAngle() for more information.
        *   \note Only functionnal when climbing is enabled (unit movement).
        */
        void                  SetMaxClimbingAngle(const s_float& fMaxClimbingAngle);

        /// Returns the maximum climbing angle.
        /** \return The maximum climbing angle (non dimensional angle)
        *   \note Whenever there is a collision, you calculate the absolute angle
        *         between the motion vector and the collision plane's normal.
        *         If this angle is smaller than the value returned by this
        *         function, then the object can climb along this plane.
        *   \note Only functionnal when climbing is enabled (unit movement).
        */
        const s_float&        GetMaxClimbingAngle() const;

        /// Returns the maximum number of collision recursion per frame and object.
        /** \return The maximum number of collision recursion per frame and object
        */
        const s_uint&         GetMaxCollisionRecursion() const;

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
        s_float fMinSlidingAngle_;
        s_float fMaxClimbingAngle_;

    };
}

#endif
