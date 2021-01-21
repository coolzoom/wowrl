/* ###################################### */
/* ###     Frost Engine, by Kalith    ### */
/* ###################################### */
/*             Editor source              */
/*                                        */
/*                                        */

#include "frost_editor.h"

#include <frost_utils_eventmanager.h>
#include <frost_utils_event.h>

using namespace std;

namespace Frost
{
    Editor::Editor()
    {
        iterAction_ = lHistoryList_.Begin();
    }

    Editor::~Editor()
    {
    }

    void Editor::Initialize()
    {
    }

    void Editor::AddEditorAction( s_refptr<EditorAction> pEditorAction )
    {
        if (pEditorAction)
        {
            if (pEditorAction->CallDoWhenAdded())
                pEditorAction->Do();

            lHistoryList_.EraseRange(iterAction_, lHistoryList_.End());
            lHistoryList_.PushBack(pEditorAction);
            iterAction_ = lHistoryList_.End();

            EventManager::GetSingleton()->FireEvent(Event("ZONE_MODIFIED"));
        }
    }

    s_bool Editor::Undo()
    {
        if (iterAction_ != lHistoryList_.Begin())
        {
            --iterAction_;
            (*iterAction_)->Undo();

            EventManager::GetSingleton()->FireEvent(Event("UNDO"));
            return true;
        }

        return false;
    }

    s_bool Editor::Redo()
    {
        if (iterAction_ != lHistoryList_.End())
        {
            (*iterAction_)->Do();
            ++iterAction_;

            EventManager::GetSingleton()->FireEvent(Event("REDO"));
            return true;
        }

        return false;
    }

    s_bool Editor::CanUndo() const
    {
        return iterAction_ != lHistoryList_.Begin();
    }

    s_bool Editor::CanRedo() const
    {
        return iterAction_ != lHistoryList_.End();
    }

    void Editor::SetCurrentTool( const Tool& mTool )
    {
        mTool_ = mTool;
    }

    const Editor::Tool& Editor::GetCurrentTool() const
    {
        return mTool_;
    }

    void Editor::CreateGlue( s_ptr<Lua::State> pLua )
    {
        pLua->Register<LuaEditor>();
        pLua->PushNew<LuaEditor>();
        pLua->SetGlobal("Editor");
    }

    EditorAction::EditorAction() : bCallDoWhenAdded_(true)
    {
    }

    EditorAction::~EditorAction()
    {
    }

    const s_bool& EditorAction::CallDoWhenAdded() const
    {
        return bCallDoWhenAdded_;
    }
}
