if (GetLocale() == "frFR") then
    AddOns.FileSelector:SetLocale({
        ["Title"] = "Sélecteur de fichier",
        ["NotFoundError"] = "Impossible d\'ouvrir le dossier :\n",
    });
else
    AddOns.FileSelector:SetLocale({
        ["Title"] = "File selector",
        ["NotFoundError"] = "Unable to open folder :\n",
    });
end
