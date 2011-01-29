if (GetLocale() == "frFR") then
    AddOns.ObjectList:SetLocale({
        ["Title"] = "Liste des objets",
        ["Delete"] = "Supprimer",
        ["Deselect"] = "Désélectionner",
        ["Select"] = "Sélectionner",
        ["Hide"] = "Cacher",
        ["Show"] = "Montrer",
        ["Properties"] = "Propriétés",
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
