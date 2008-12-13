#ifndef FROST_TE_TOOL_H
#define FROST_TE_TOOL_H

#include "frost_te.h"

namespace Frost
{
namespace TE
{
    class Tool
    {
    public :

        Tool();
        virtual ~Tool();

        virtual void OnLeftClicked(const s_float& fX, const s_float& fY);
        virtual void OnRightClicked(const s_float& fX, const s_float& fY);
        virtual void OnBothClicked(const s_float& fX, const s_float& fY);
        virtual void OnLeftDragged(const s_float& fDX, const s_float& fDY);
        virtual void OnRightDragged(const s_float& fDX, const s_float& fDY);
        virtual void OnBothDragged(const s_float& fDX, const s_float& fDY);
        virtual void OnMouseScrolled(const s_int iDirection);

        const s_bool& HideDecalsWhileDragging() const;

    protected :

        s_bool bHideDecalsWhileDragging_;
    };

    enum ToolType
    {
        TOOL_CAMERA,
        TOOL_HEIGHT
    };

    class ToolCamera : public Tool
    {
    public :

        ToolCamera();
        ~ToolCamera();

        void OnLeftDragged(const s_float& fDX, const s_float& fDY);
        void OnRightDragged(const s_float& fDX, const s_float& fDY);
        void OnBothDragged(const s_float& fDX, const s_float& fDY);
        void OnMouseScrolled(const s_int iDirection);
    };

    class ToolHeight : public Tool
    {
    public :

        ToolHeight();
        ~ToolHeight();

        void OnLeftClicked(const s_float& fX, const s_float& fY);
        void OnRightClicked(const s_float& fX, const s_float& fY);
        void OnBothClicked(const s_float& fX, const s_float& fY);
        void OnMouseScrolled(const s_int iDirection);
    };
}
}

#endif
