/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Editor header              */
/*                                        */
/*                                        */

#ifndef EDITOR_H
#define EDITOR_H

#include "frost.h"
#include <utils/frost_utils_manager.h>
#include <utils/frost_utils_lua.h>

namespace Frost
{
    class EditorAction
    {
    public :

        virtual void Do() = 0;
        virtual void Undo() = 0;
    };

    class Editor : public Manager<Editor>
    {
    friend class Manager<Editor>;
    public :

        /// Initializes the Editor.
        void Initialize();

        /// Adds a new action to the history list and executes it.
        /** \param pEditorAction The action to execute
        *   \note Erases all actions that have been undone.
        */
        void AddEditorAction(s_refptr<EditorAction> pEditorAction);

        /// Undoes the previous action.
        void Undo();

        /// Redoes the action previously undone.
        void Redo();

        /// Creates a new Zone.
        /** \param sName The name of the new Zone
        */
        void NewZone(const s_str& sName);

        /// Loads a Zone.
        /** \param sName The Zone name to load
        */
        void LoadZone(const s_str& sName);

        /// Loads a Zone file.
        /** \param sFile The Zone file to load
        */
        void LoadZoneFile(const s_str& sFile);

        /// Saves the current Zone.
        void SaveZone();

        /// Closes the current Zone.
        void CloseZone();

        /// Adds a new Doodad in the current Zone.
        /** \param sName  The name of the Doodad
        *   \param sModel The model to use
        */
        void AddDoodad(const s_str& sName, const s_str& sModel);

        /// Sets the current Zone's file.
        /** \param sFile The file to save the current Zone into
        *   \note Sets the file the current Zone will be saved into.
        */
        void SetCurrentZoneFile(const s_str& sFile);

        /// Checks if the current Zone has been saved.
        const s_bool& IsZoneSaved();

        /// Returns the current Zone's file.
        const s_str& GetCurrentZoneFile();

        /// Creates a lua glue on the provided Lua state.
        /** \param pLua The lua state
        */
        void CreateGlue(s_ptr<Lua::State> pLua);

    protected :

        /// Default constructor.
        /** \note This is a singleton class. That means that there can
        *         only be a single instance at the same time. That's why
        *         the constructor is not in public scope.<br>
        *         If you need to get a pointer to that unique instance,
        *         call Editor::GetSingleton().<br>
        *         Note that, if GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when you close
        *         your program by calling Editor::Delete().
        */
        Editor();

        /// Destructor.
        /** \note If GetSingleton() has been called at least once,
        *         you'll have to delete the returned pointer when
        *         you close your program by calling Delete().
        */
        ~Editor();

        /// Copy constructor.
        Editor(const Editor& mMgr);

        /// Assignment operator.
        Editor& operator = (const Editor& mMgr);

    private :

        s_str       sCurrentZoneFile_;
        s_bool      bZoneSaved_;

        s_ctnr< s_refptr<EditorAction> >           lHistoryList_;
        s_ctnr< s_refptr<EditorAction> >::iterator iterAction_;
    };

    // Editor Lua glue
    class LuaEditor
    {
    public :

        LuaEditor(lua_State* luaVM);

        int _AddDoodad(lua_State*);
        int _GetCurrentZoneFile(lua_State*);
        int _GetBackgroundColor(lua_State*);
        int _GetModelFile(lua_State*);
        int _GetModelMaterial(lua_State*);
        int _GetZoneModelList(lua_State*);
        int _IsModelLoaded(lua_State*);
        int _IsZoneLoaded(lua_State*);
        int _IsZoneSaved(lua_State*);
        int _LoadZone(lua_State*);
        int _LoadZoneFile(lua_State*);
        int _NewZone(lua_State*);
        int _NotifyDoodadPositioned(lua_State*);
        int _RegisterNewModel(lua_State*);
        int _SaveZone(lua_State*);
        int _SetBackgroundColor(lua_State*);
        int _SetModelMaterial(lua_State*);
        int _ToggleWireframeView(lua_State*);
        int _ToggleShading(lua_State*);
        int _UnloadZone(lua_State*);

        // Lunar function
        int GetDataTable(lua_State *L);

        static const char  className[];
        static const char* classList[];
        static Lunar<LuaEditor>::RegType methods[];
        static const s_str CLASS_NAME;

    protected :

        s_ptr<Editor> pEditor_;

        lua_State* pLua_;
        int        iRef_;
    };
}

#endif
