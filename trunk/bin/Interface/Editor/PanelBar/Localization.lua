if (GetLocale() == "frFR") then
    AddOns.PanelBar:SetLocale({
        ["Objects"] = "OBJETS",
        ["ObjectsObjectList"] = "Liste des objets",
        ["ObjectsEditModel"] = "Editer modèle",
        ["ObjectsPickModel"] = "Choisir un modèle...",
        ["ObjectsAddDoodad"] = "Ajouter objet",
        ["Terrain"] = "TERRAIN",
    });
else
    AddOns.PanelBar:SetLocale({
        ["Objects"] = "OBJECTS",
        ["ObjectsObjectList"] = "Object list",
        ["ObjectsEditModel"] = "Edit model",
        ["ObjectsPickModel"] = "Pick a model...",
        ["ObjectsAddDoodad"] = "Add doodad",
        ["Terrain"] = "TERRAIN",
    });
end
