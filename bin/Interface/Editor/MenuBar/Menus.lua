
MenuBar:AddMenu("File");
MenuBar:AddMenuItem("File", "New", "Ctrl-N");
MenuBar:AddMenuItem("File", "Open", "Ctrl-O"):SetScript("OnClick", function()
    FileSelector:Show();
    FileSelector:SelectFolder("Zones");
    FileSelector:SetOnOkFunc(function()
        local result = Frost:LoadZoneFile(FileSelector:GetSelectedFile());
        if (not result) then
            FileSelector:Error(AddOns.MenuBar:GetLocalizedString("ZoneLoadingError"), 256, 100);
            return false;
        else
            return true;
        end
    end
    );
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuItem("File", "Save", "Ctrl-S"):SetScript("OnClick", function()
    Frost:SaveZone("Zones/Test/Test.xml");
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuItem("File", "SaveAs");
MenuBar:AddMenuItem("File", "Close", "Ctrl-W"):SetScript("OnClick", function()
    Frost:UnloadZone();
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuItem("File", "Exit", "Esc."):SetScript("OnClick", function()
    if (AddOns.Editor:IsDataSaved()) then
        Exit();
    else
        EscMenuConfirmDialog:Show();
    end
    MenuBar:CloseCurrentDropdown();
end);

MenuBar:AddMenu("Edit");
MenuBar:AddMenuItem("Edit", "Undo", "Ctrl-Z");
MenuBar:AddMenuItem("Edit", "Redo", "Ctrl-Shift-Z");
MenuBar:AddMenuItem("Edit", "Clone", "Ctrl-C");
MenuBar:AddMenuItem("Edit", "Delete", "Del.");

MenuBar:AddMenu("View");
MenuBar:AddMenuCheckItem("View", "Wireframe", "F2", false):SetScript("OnClick", function ()
    Frost:ToggleWireframeView();
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuCheckItem("View", "Shading", "F3", true):SetScript("OnClick", function ()
    Frost:ToggleShading();
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuCheckItem("View", "Decal", "F4", true):SetScript("OnClick", function ()
    if (AddOns.Editor:IsMouseDecalEnabled()) then
        AddOns.Editor:EnableMouseDecal(false);
    else
        AddOns.Editor:EnableMouseDecal(true);
    end
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuItem("View", "DecalColor"):SetScript("OnClick", function ()
    ColorSelector:SetColor(Color(Frost:GetMouseDecalColor()));
    ColorSelector:SetOnColorUpdateFunc(function (color)
        AddOns.Editor.Config.mouseDecalColor = Color(color);
        Frost:SetMouseDecalColor(color:Unpack());
    end);
    ColorSelector:Show();
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuItem("View", "BGColor"):SetScript("OnClick", function ()
    ColorSelector:SetColor(Color(Frost:GetBackgroundColor()));
    ColorSelector:SetOnColorUpdateFunc(function (color)
        AddOns.Editor.Config.worldBackgroundColor = Color(color);
        Frost:SetBackgroundColor(color:Unpack());
    end);
    ColorSelector:Show();
    MenuBar:CloseCurrentDropdown();
end);

MenuBar:AddMenu("Settings");
MenuBar:AddMenuItem("Settings", "Options"):SetScript("OnClick", function ()
    ConfigMenu:Show();
    MenuBar:CloseCurrentDropdown();
end);

MenuBar:AddMenu("Help");
MenuBar:AddMenuItem("Help", "Show", "F1");
MenuBar:AddMenuItem("Help", "About");
