if (GetLocale() == "frFR") then
    AddOns.ObjectList:SetLocale({
        ["Title"] = "Liste des objets",
        ["Delete"] = "Supprimer",
        ["Deselect"] = "D�s�lectionner",
        ["Select"] = "S�lectionner",
        ["Hide"] = "Cacher",
        ["Show"] = "Montrer",
        ["Properties"] = "Propri�t�s",
    });
else
    AddOns.ObjectList:SetLocale({
        ["Title"] = "Object list",
        ["Delete"] = "Delete",
        ["Deselect"] = "Deselect",
        ["Select"] = "Select",
        ["Hide"] = "Hide",
        ["Show"] = "Show",
        ["Properties"] = "Properties",
    });
end
