if (GetLocale() == "frFR") then
    AddOns.ColorSelector:SetLocale({
        ["Title"]   = "Sélecteur de couleur",
        ["SliderA"] = "Alpha",
        ["SliderR"] = "Rouge",
        ["SliderG"] = "Vert",
        ["SliderB"] = "Bleu",
        ["SliderH"] = "Teinte",
        ["SliderS"] = "Sat.",
        ["SliderL"] = "Lum.",
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
        ["SliderH"] = "Hue",
        ["SliderS"] = "Sat.",
        ["SliderL"] = "Light",
        ["Copy"]    = "Copy",
        ["Paste"]   = "Paste",
    });
end
