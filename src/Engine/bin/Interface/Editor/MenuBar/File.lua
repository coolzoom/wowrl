MenuBar:AddMenu("File");

-- FILE / NEW
function FileNew()
    NewZoneDialog:Show();
end

local item = MenuBar:AddMenuItem("File", "New", "Ctrl-N");
item:SetScript("OnClick", function()
    if (not Editor:IsZoneLoaded() or Editor:IsZoneSaved()) then
        FileNew();
    else
        CloseConfirmDialog.Action = FileNew;
        CloseConfirmDialog:Show();
    end
    MenuBar:CloseCurrentDropDown();
end);

SetKeyBinding(KEY_N, KEY_LCONTROL, "MenuBar.File.DropDown.New:Click();");
SetKeyBinding(KEY_N, KEY_RCONTROL, "MenuBar.File.DropDown.New:Click();");

-- FILE / OPEN
function FileOpen()
    FileSelector:Show();
    FileSelector:Raise();
    FileSelector:SelectFolder("Zones");
    FileSelector:SetOnOkFunc(function()
        local error = Editor:LoadZoneFile(FileSelector:GetSelectedFile());
        if (error) then
            FileSelector:Error(AddOns.MenuBar:GetLocalizedString("ZoneLoadingError"), 256, 100);
            return false;
        else
            return true;
        end
    end);
end

local item = MenuBar:AddMenuItem("File", "Open", "Ctrl-O");
item:SetScript("OnClick", function()
    if (not Editor:IsZoneLoaded() or Editor:IsZoneSaved()) then
        FileOpen();
    else
        CloseConfirmDialog.Action = FileOpen;
        CloseConfirmDialog:Show();
    end
    MenuBar:CloseCurrentDropDown();
end);

SetKeyBinding(KEY_O, KEY_LCONTROL, "MenuBar.File.DropDown.Open:Click();");
SetKeyBinding(KEY_O, KEY_RCONTROL, "MenuBar.File.DropDown.Open:Click();");

-- FILE / SAVE
function FileSave()
    Editor:SaveZone();
    AddOns.Editor:NotifyDataSaved();
end

local item = MenuBar:AddMenuItem("File", "Save", "Ctrl-S");
item:RegisterEvent("ZONE_SAVED");
item:RegisterEvent("NEW_ZONE");
item:RegisterEvent("ZONE_MODIFIED");
item:SetScript("OnClick", function()
    if (Editor:GetCurrentZoneFile() ~= nil) then
        FileSave();
    else
        FileSaveAs();
    end
    MenuBar:CloseCurrentDropDown();
end);
item:SetScript("OnEvent", function()
    if (event == "ZONE_SAVED") then
        this:Disable();
    elseif (event == "ZONE_MODIFIED" or event == "NEW_ZONE") then
        this:Enable();
    end
end);
if (not Editor:IsZoneLoaded() or Editor:IsZoneSaved()) then
    item:Disable();
end

SetKeyBinding(KEY_S, KEY_LCONTROL, "MenuBar.File.DropDown.Save:Click();");
SetKeyBinding(KEY_S, KEY_RCONTROL, "MenuBar.File.DropDown.Save:Click();");

-- FILE / SAVE AS
function FileSaveAs()
    FileSelector:Show();
    FileSelector:Raise();
    FileSelector:SelectFolder("Zones");
    FileSelector:SetOnOkFunc(function()
        Editor:SaveZone(FileSelector:GetSelectedFile());
        AddOns.Editor:NotifyDataSaved();
    end);
end

local item = MenuBar:AddMenuItem("File", "SaveAs", nil);
item:RegisterEvent("NEW_ZONE");
item:RegisterEvent("ZONE_LOADED");
item:RegisterEvent("ZONE_CLOSED");
item:SetScript("OnClick", function()
    FileSaveAs();
    MenuBar:CloseCurrentDropDown();
end);
item:SetScript("OnEvent", function()
    if (event == "NEW_ZONE" or event == "ZONE_LOADED") then
        this:Enable();
    elseif (event == "ZONE_CLOSED") then
        this:Disable();
    end
end);
if (not Editor:IsZoneLoaded()) then
    item:Disable();
end

-- FILE / CLOSE
function FileClose()
    Editor:UnloadZone();
end

local item = MenuBar:AddMenuItem("File", "Close", "Ctrl-W");
item:RegisterEvent("NEW_ZONE");
item:RegisterEvent("ZONE_LOADED");
item:RegisterEvent("ZONE_CLOSED");
item:SetScript("OnClick", function()
    if (Editor:IsZoneSaved()) then
        FileClose();
    else
        CloseConfirmDialog.Action = FileClose;
        CloseConfirmDialog:Show();
    end
    MenuBar:CloseCurrentDropDown();
end);
item:SetScript("OnEvent", function()
    if (event == "NEW_ZONE" or event == "ZONE_LOADED") then
        this:Enable();
    elseif (event == "ZONE_CLOSED") then
        this:Disable();
    end
end);
if (not Editor:IsZoneLoaded()) then
    item:Disable();
end

-- FILE / EXIT
local item = MenuBar:AddMenuItem("File", "Exit", "Esc.");
item:SetScript("OnClick", function()
    if (not EscMenu:IsShown()) then
        EscMenu:Show();
    else
        EscMenu.buttons["Exit"]:OnClick();
    end
    MenuBar:CloseCurrentDropDown();
end);

SetKeyBinding(KEY_ESCAPE, "MenuBar.File.DropDown.Exit:Click();");
