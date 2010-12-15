if (GetLocale() == "frFR") then
    AddOns.PanelBar:SetLocale({
        ["Objects"] = "OBJETS",
        ["ObjectsEditModel"] = "Editer modèle",
        ["ObjectsPickModel"] = "Choisir un modèle...",
        ["ObjectsAddDoodad"] = "Ajouter objet",
        ["Terrain"] = "TERRAIN",
    });
else
    AddOns.PanelBar:SetLocale({
        ["Objects"] = "OBJECTS",
        ["ObjectsEditModel"] = "Edit model",
        ["ObjectsPickModel"] = "Pick a model...",
        ["ObjectsAddDoodad"] = "Add doodad",
        ["Terrain"] = "TERRAIN",
    });
end
