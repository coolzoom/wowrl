if (GetLocale() == "frFR") then
    AddOns.EscMenu:SetLocale({
        ["Title"]          = "Menu",
        ["GeneralOptions"] = "Options générales",
        ["EditorOptions"]  = "Options de l'éditeur",
        ["Exit"]           = "Quitter",
        ["ConfirmExit"]    = "Vous êtes sur le point de quitter le programme, mais une partie de votre travail n'a pas étée sauvegardée.\nVoulez-vous sauvegarder maintenant ?",
    });
else
    AddOns.EscMenu:SetLocale({
        ["Title"]          = "Menu",
        ["GeneralOptions"] = "General options",
        ["EditorOptions"]  = "Editor options",
        ["Exit"]           = "Exit",
        ["ConfirmExit"]    = "You are about to exit the program, but some of your work has not been saved.\nDo you want to save it now ?",
    });
end
