MenuBar:AddMenu("Edit");

-- EDIT / UNDO
local item = MenuBar:AddMenuItem("Edit", "Undo", "Ctrl-Z");
item:RegisterEvent("ZONE_MODIFIED");
item:SetScript("OnClick", function()
    if (Editor:Undo()) then
        MenuBar.Edit.Redo:Enable();
        if (not Editor:CanUndo()) then
            this:Disable();
        end
    end
end);
item:SetScript("OnEvent", function()
    if (event == "ZONE_MODIFIED") then
        this:Enable();
    end
end);
if (not Editor:CanUndo()) then
    item:Disable();
end

-- EDIT / REDO
item = MenuBar:AddMenuItem("Edit", "Redo", "Ctrl-Shift-Z");
item:RegisterEvent("ZONE_MODIFIED");
item:SetScript("OnClick", function()
    if (Editor:Redo()) then
        MenuBar.Edit.Undo:Enable();
        if (not Editor:CanRedo()) then
            this:Disable();
        end
    end
end);
item:SetScript("OnEvent", function()
    if (event == "ZONE_MODIFIED") then
        this:Disable();
    end
end);
if (not Editor:CanRedo()) then
    item:Disable();
end
MenuBar:AddMenuItem("Edit", "Clone", "Ctrl-C");
MenuBar:AddMenuItem("Edit", "Delete", "Del.");
