MenuBar:AddMenu("Edit");

-- EDIT / UNDO
local item = MenuBar:AddMenuItem("Edit", "Undo", "Ctrl-Z");
item:RegisterEvent("ZONE_MODIFIED");
item:SetScript("OnClick", function()
    if (Editor:Undo()) then
        MenuBar.Edit.DropDown.Redo:Enable();
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

SetKeyBinding(KEY_Z, KEY_LCONTROL, "MenuBar.Edit.DropDown.Undo:Click();");
SetKeyBinding(KEY_Z, KEY_RCONTROL, "MenuBar.Edit.DropDown.Undo:Click();");

-- EDIT / REDO
item = MenuBar:AddMenuItem("Edit", "Redo", "Ctrl-Shift-Z");
item:RegisterEvent("ZONE_MODIFIED");
item:SetScript("OnClick", function()
    if (Editor:Redo()) then
        MenuBar.Edit.DropDown.Undo:Enable();
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

SetKeyBinding(KEY_Z, KEY_LCONTROL, KEY_LSHIFT, "MenuBar.Edit.DropDown.Redo:Click();");
SetKeyBinding(KEY_Z, KEY_LCONTROL, KEY_RSHIFT, "MenuBar.Edit.DropDown.Redo:Click();");
SetKeyBinding(KEY_Z, KEY_RCONTROL, KEY_LSHIFT, "MenuBar.Edit.DropDown.Redo:Click();");
SetKeyBinding(KEY_Z, KEY_RCONTROL, KEY_RSHIFT, "MenuBar.Edit.DropDown.Redo:Click();");

-- EDIT / CLONE
item = MenuBar:AddMenuItem("Edit", "Clone", "Ctrl-C");
if (Editor:GetSelectedObjectCount() == 0) then
	item:Disable();
end

-- EDIT / DELETE
item = MenuBar:AddMenuItem("Edit", "Delete", "Del.");
if (Editor:GetSelectedObjectCount() == 0) then
	item:Disable();
end
item:RegisterEvent("OBJECT_SELECTED");
item:RegisterEvent("NO_OBJECT_SELECTED");
item:SetScript("OnEvent", function()
    if (event == "OBJECT_SELECTED") then
		this:Enable();
	elseif (event == "NO_OBJECT_SELECTED") then
		this:Disable();
	end
end);
item:SetScript("OnClick", function()
    local objects = {Editor:GetSelectedObjects()}
	for i, obj in pairs(objects) do
		Editor:DeleteObject(obj);
	end
end);

SetKeyBinding(KEY_DELETE, "MenuBar.Edit.DropDown.Delete:Click();");
