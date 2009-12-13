if (GetLocale() == "frFR") then
    AddOns.ColorSelector:SetLocale({
        ["Title"]   = "Selecteur de couleur",
        ["SliderA"] = "Alpha",
        ["SliderR"] = "Rouge",
        ["SliderG"] = "Vert",
        ["SliderB"] = "Bleu",
        ["Copy"]    = "Copier",
        ["Paste"]   = "Coller",
    });
else
    AddOns.ColorSelector:SetLocale({
        ["Title"]   = "Color selector",
        ["SliderA"] = "Alpha",
        ["SliderR"] = "Red",
        ["SliderG"] = "Green",
        ["SliderB"] = "Blue",
        ["Copy"]    = "Copy",
        ["Paste"]   = "Paste",
    });
end
