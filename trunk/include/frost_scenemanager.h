/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          SceneManager header           */
/*                                        */
/*                                        */


#ifndef FROST_SCENEMANAGER_H
#define FROST_SCENEMANAGER_H

#include "frost.h"
#include "utils/frost_utils_manager.h"

namespace Frost
{
    /// Manages creation of 3D objects
    class SceneManager : public Manager<SceneManager>
    {
    friend class Manager<SceneManager>;
    public :

        /// Initializes this manager.
        /** \note Automatically called by Engine.
        */
        void   Initialize();

        /// Creates a new Plane.
        /** \note The created plane will be "infinite".
        *         Actually, its width and height will be
        *         set to 1500. If you need a larger one,
        *         use the other CreatePlane method.
        *   \return The created Plane
        */
        s_ptr<Plane>   CreatePlane();

        /// Creates a new Plane.
        /** \param fWidth   The width of this plane
        *   \param fHeight  The height of this plane
        *   \param fDensity The number of segment per meter
        *   \return The created Plane
        */
        s_ptr<Plane>   CreatePlane(const s_float& fWidth, const s_float& fHeight, const s_float& fDensity = 0.5f);

        /// Creates a new Terrain.
        /** \param sFileName The path to the file containing terrain info
        *   \return The created Terrain
        */
        s_ptr<Terrain> CreateTerrain(const s_str& sFileName);

        /// Creates a new Node.
        /** \param mPos The position to give to this Node.
        *   \return The created Node
        */
        s_ptr<Node>    CreateNode(const Vector& mPos = Vector::ZERO);

        /// Deletes a particular Plane.
        /** \param pPlane The Plane to delete
        *   \note All planes created by this manager are automatically deleted
        *         when the application closes.
        */
        void           DeletePlane(s_ptr<Plane> pPlane);

        /// Deletes a particular Terrain.
        /** \param pTerrain The Terrain to delete
        *   \note All terrains created by this manager are automatically deleted
        *         when the application closes.
        */
        void           DeleteTerrain(s_ptr<Terrain> pTerrain);

        /// Deletes a particular Node.
        /** \param pNode The Node to delete
        *   \note All nodes created by this manager are automatically deleted
        *         when the application closes.
        */
        void           DeleteNode(s_ptr<Node> pNode);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call SceneManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling SceneManager::Delete() (this is
        *         automatically done by Engine).
        */
        SceneManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~SceneManager();

        /// Copy constructor
        SceneManager(const SceneManager& mMgr);

        /// Assignment operator
        SceneManager& operator = (const SceneManager& mMgr);

    private :

        std::map< s_uint, s_ptr<Plane> > lPlaneList_;
        s_uint                           uiPlaneCounter_;

        std::map< s_str, s_ptr<TerrainData> > lLoadedModelList_;
        std::map< s_uint, s_ptr<Terrain> >    lTerrainList_;
        s_uint                                uiTerrainCounter_;

        std::map< s_uint, s_ptr<Node> > lNodeList_;
        s_uint                          uiNodeCounter_;
    };
}

#endif
