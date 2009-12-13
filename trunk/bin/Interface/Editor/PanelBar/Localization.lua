if (GetLocale() == "frFR") then
    AddOns.PanelBar:SetLocale({
        ["Objects"] = "OBJETS",
        ["Terrain"] = "TERRAIN",
    });
else
    AddOns.PanelBar:SetLocale({
        ["Objects"] = "OBJECTS",
        ["Terrain"] = "TERRAIN",
    });
end
