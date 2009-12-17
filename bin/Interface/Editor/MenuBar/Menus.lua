
MenuBar:AddMenu("File");
MenuBar:AddMenuItem("File", "New", "Ctrl-N");
MenuBar:AddMenuItem("File", "Open", "Ctrl-O"):SetScript("OnClick", function()
    Frost:LoadZoneFile("Zones/Test/Test.xml");
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuItem("File", "Save", "Ctrl-S");
MenuBar:AddMenuItem("File", "SaveAs");
MenuBar:AddMenuItem("File", "Close", "Ctrl-W"):SetScript("OnClick", function()
    Frost:UnloadZone();
    MenuBar:CloseCurrentDropdown();
end);
MenuBar:AddMenuItem("File", "Quit", "Esc."):SetScript("OnClick", function()
    Exit();
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
