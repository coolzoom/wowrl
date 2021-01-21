MenuBar:AddMenu("Settings");

MenuBar:AddMenuItem("Settings", "Options"):SetScript("OnClick", function ()
    ConfigMenu:Show();
    MenuBar:CloseCurrentDropDown();
end);
