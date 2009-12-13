if (GetLocale() == "frFR") then
    AddOns.ConfigMenu:SetLocale({
        ["Title"]                            = "Configuration",
        ["PanelGeneral"]                     = "General",
        ["PanelInterface"]                   = "Interface",
        ["PanelInterfaceTextColor"]          = "Couleur du texte :",
        ["PanelInterfaceSecondaryTextColor"] = "Couleur du texte secondaire :",
        ["PanelInterfaceBackgroundColor"]    = "Couleur de l'arriere plan :",
    });
else
    AddOns.ConfigMenu:SetLocale({
        ["Title"]                            = "Configuration",
        ["PanelGeneral"]                     = "General",
        ["PanelInterface"]                   = "Interface",
        ["PanelInterfaceTextColor"]          = "Text color :",
        ["PanelInterfaceSecondaryTextColor"] = "Secondary text color :",
        ["PanelInterfaceBackgroundColor"]    = "Background color :",
    });
end
