if (GetLocale() == "frFR") then
    AddOns.MenuBar:SetLocale({
        ["File"] = "Fichier",
        ["FileNew"] = "Nouveau",
        ["FileOpen"] = "Ouvrir...",
        ["FileSave"] = "Enregistrer",
        ["FileSaveAs"] = "Enregistrer sous...",
        ["FileClose"] = "Fermer",
        ["FileQuit"] = "Quiter",
        ["Edit"] = "Edition",
        ["EditUndo"] = "Annuler",
        ["EditRedo"] = "Retablir",
        ["EditClone"] = "Dupliquer",
        ["EditDelete"] = "Supprimer",
        ["View"] = "Affichage",
        ["ViewWireframe"] = "Activer/desactiver le mode fil de fer",
        ["ViewShading"] = "Activer/desactiver l'eclairage",
        ["ViewBGColor"] = "Couleur d'arriere plan...",
        ["Settings"] = "Parametres",
        ["SettingsOptions"] = "Options...",
        ["Help"] = "Aide",
        ["HelpShow"] = "Montrer l'aide",
        ["HelpAbout"] = "A propos"
    });
else
    AddOns.MenuBar:SetLocale({
        ["File"] = "File",
        ["FileNew"] = "New",
        ["FileOpen"] = "Open...",
        ["FileSave"] = "Save",
        ["FileSaveAs"] = "Save as...",
        ["FileClose"] = "Close",
        ["FileQuit"] = "Quit",
        ["Edit"] = "Edit",
        ["EditUndo"] = "Undo",
        ["EditRedo"] = "Redo",
        ["EditClone"] = "Clone",
        ["EditDelete"] = "Delete",
        ["View"] = "View",
        ["ViewWireframe"] = "Toggle wireframe",
        ["ViewShading"] = "Toggle shading",
        ["ViewBGColor"] = "Background color...",
        ["Settings"] = "Settings",
        ["SettingsOptions"] = "Options...",
        ["Help"] = "Help",
        ["HelpShow"] = "Show help",
        ["HelpAbout"] = "About"
    });
end
