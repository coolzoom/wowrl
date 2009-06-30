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
    class PhysicsManager : public Manager<PhysicsManager>
    {
    friend class Manager<PhysicsManager>;
    public :

        /// Creates a handler to control a MovableObject.
        /** \param pObj The MovableObject to control
        *   \return The associated handler (creates it if necessary)
        */
        s_ptr<PhysicsHandler> CreateHandler(s_ptr<MovableObject> pObj);

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
        void                  DeleteHandler(s_ptr<PhysicsHandler> pHandler);

        /// Sets the direction and intensity of the gravity field.
        /** \param mGravity The gravity force
        *   \note Default is (0, -9.81, 0).
        */
        void                  SetGravity(const Vector& mGravity);

        /// Returns the gravity force.
        /** \return The gravity force
        */
        const Vector&         GetGravity() const;

        /// Adds a plane to the obstacle list.
        /** \param pPlane The plane to add
        *   \return The new obstacle
        */
        s_ptr<Obstacle>       AddObstacle(s_ptr<Plane> pPlane);

        /// Adds a terrain to the obstacle list.
        /** \param pTerrain The terrain to add
        *   \return The new obstacle
        */
        s_ptr<Obstacle>       AddObstacle(s_ptr<Terrain> pTerrain);

        /// Adds a model to the obstacle list.
        /** \param pModel The model to add
        *   \return The new obstacle
        */
        s_ptr<Obstacle>       AddObstacle(s_ptr<Model> pModel);

        /// Removes an obstacle from the scene.
        /** \param pObstacle The obstacle to remove
        */
        void                  RemoveObstacle(s_ptr<Obstacle> pObstacle);

        /// Removes all obstacles from the scene.
        void                  ClearObstacles();

        /// Updates all handlers.
        /** \param fDelta The time elapsed since the last call
        *   \note Automatically called by Engine.
        */
        void                  UpdateHandlers(const s_float& fDelta);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
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

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~PhysicsManager();

        /// Copy constructor
        PhysicsManager(const PhysicsManager& mMgr);

        /// Assignment operator
        PhysicsManager& operator = (const PhysicsManager& mMgr);

    private :

        s_map< s_ptr<MovableObject>, s_ptr<PhysicsHandler> > lHandlerList_;
        s_map< s_uint, s_ptr<Obstacle> >                     lObstacleList_;
        s_uint                                               uiObstacleCounter_;

        Vector mGravity_;

    };
}

#endif
