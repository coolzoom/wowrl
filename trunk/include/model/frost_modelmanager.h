/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*          ModelManager header           */
/*                                        */
/*                                        */


#ifndef FROST_MODELMANAGER_H
#define FROST_MODELMANAGER_H

#include "frost.h"

namespace Frost
{
    /// Handles creation and updating of models
    class ModelManager : public Manager<ModelManager>
    {
    friend class Manager<ModelManager>;
    public :

        /// Creates/loads a Model.
        /** \param sModelName  The name of the model you want to use. This name
        *                      is associated with a unique file at loading time
        *   \param sEntityName The name of the Ogre::Entity that will be created
        *   \return The new Model
        *   \note This function takes care of loading the Model if needed.
        *         If it has already been loaded, it just returns a copy of it.
        */
        s_refptr<Model> CreateModel(const s_str& sModelName, const s_str& sEntityName);

        /// Creates/loads a Model.
        /** \param sCategory   The category into which this model is located
        *   \param sModelName  The name of the model you want to use. This name
        *                      is associated with a unique file at loading time
        *   \param sEntityName The name of the Ogre::Entity that will be created
        *   \return The new Model
        *   \note This function takes care of loading the Model if needed.
        *         If it has already been loaded, it just returns a copy of it.
        */
        s_refptr<Model> CreateModel(const s_str& sCategory, const s_str& sModelName, const s_str& sEntityName);

        /// Links a model name to a file.
        /** \param sModelName The model name to link
        *   \param sFile      The file to link to
        *   \note This function allows you to avoid writing the whole
        *         file name everytime you want to create a new model.
        */
        void         LinkModelNameToFile(const s_str& sModelName, const s_str& sFile);

        /// Links a model name to a file.
        /** \param sCategory  The category into which to store this link
        *   \param sModelName The model name to link
        *   \param sFile      The file to link to
        *   \note This function allows you to avoid writing the whole
        *         file name everytime you want to create a new model.
        */
        void         LinkModelNameToFile(const s_str& sCategory, const s_str& sModelName, const s_str& sFile);

        /// Removes a particular category and all the links it contains.
        /** \param sCategory The category to remove
        */
        void         RemoveCategory(const s_str& sCategory);

        /// Removes a link between a model name and a file.
        /** \param sModelName The model name to unlink
        */
        void         ClearLink(const s_str& sModelName);

        /// Removes a link between a model name and a file.
        /** \param sCategory  The category into which the link is located
        *   \param sModelName The model name to unlink
        */
        void         ClearLink(const s_str& sCategory, const s_str& sModelName);

        static const s_str CLASS_NAME;

    protected :

        /// Default constructor
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call ModelManager::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling ModelManager::Delete() (this is
        *         automatically done by Engine).
        */
        ModelManager();

        /// Destructor
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete() (this is
        *         automatically done by Engine).
        */
        ~ModelManager();

        /// Copy constructor
        ModelManager(const ModelManager& mMgr);

        /// Assignment operator
        ModelManager& operator = (const ModelManager& mMgr);

    private :

        s_map< s_str, std::map<s_str, s_str> > lModelNameToFileMap_;
        s_map< s_str, s_ptr<ModelData> >       lLoadedModelList_;
    };
}

#endif // MODELMANAGER_H
