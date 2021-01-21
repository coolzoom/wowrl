/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Editor header              */
/*                                        */
/*                                        */

#ifndef EDITOR_H
#define EDITOR_H

#include "frost.h"
#include <frost_utils_manager.h>
#include <frost_utils_lua.h>

namespace Frost
{
    class EditorAction
    {
    public :

        EditorAction();
        virtual ~EditorAction();

        virtual void Do() = 0;
        virtual void Undo() = 0;

        const s_bool& CallDoWhenAdded() const;

    protected :

        s_bool bCallDoWhenAdded_;
    };

    class Editor : public Manager<Editor>
    {
    friend class Manager<Editor>;
    public :

        enum Tool
        {
            TOOL_NONE,
            TOOL_MOVE,
            TOOL_SCALE,
            TOOL_ROTATE
        };

        /// Initializes the Editor.
        void Initialize();

        /// Adds a new action to the history list and executes it.
        /** \param pEditorAction The action to execute
        *   \note Erases all actions that have been undone.
        */
        void AddEditorAction(s_refptr<EditorAction> pEditorAction);

        /// Undoes the previous action.
        s_bool Undo();

        /// Redoes the action previously undone.
        s_bool Redo();

        /// Checks if there are any editor actions to undo
        /** \return 'true' if it is the case
        */
        s_bool CanUndo() const;

        /// Checks if there are any editor actions to redo
        /** \return 'true' if it is the case
        */
        s_bool CanRedo() const;

        /// Sets the new tool to use.
        /** \param mTool The tool to use (see Editor::Tool)
        */
        void SetCurrentTool(const Tool& mTool);

        /// Returns the tool that is currently selected.
        /** \return The tool that is currently selected
        */
        const Tool& GetCurrentTool() const;

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

        Tool mTool_;

        s_ctnr< s_refptr<EditorAction> >           lHistoryList_;
        s_ctnr< s_refptr<EditorAction> >::iterator iterAction_;
    };

    // Editor Lua glue
    class LuaEditor
    {
    public :

        LuaEditor(lua_State* luaVM);

        int _CanRedo(lua_State*);
        int _CanUndo(lua_State*);
        int _DeleteObject(lua_State*);
        int _DeselectObject(lua_State*);
        int _GetBackgroundColor(lua_State*);
        int _GetObjectList(lua_State*);
        int _GetSelectedObjectCount(lua_State*);
        int _GetSelectedObjects(lua_State*);
        int _HideObject(lua_State*);
        int _Redo(lua_State*);
        int _SelectObject(lua_State*);
        int _SetBackgroundColor(lua_State*);
        int _SetCurrentTool(lua_State*);
        int _ShowObject(lua_State*);
        int _ToggleWireframeView(lua_State*);
        int _ToggleShading(lua_State*);
        int _Undo(lua_State*);

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
