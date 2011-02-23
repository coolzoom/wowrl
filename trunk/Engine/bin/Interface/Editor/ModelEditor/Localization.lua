if (GetLocale() == "frFR") then
    AddOns.ModelEditor:SetLocale({
        ["Title"] = "Editeur de modèle",
        ["PickModelFile"] = "Choisir un fichier de modèle...",
        ["PickModel"] = "Choisir un modèle existant...",
        ["ShowAll"] = "Afficher tout";
        ["HideAll"] = "Cacher tout";
        ["SubModelCaption"] = "Sous modèle :",
        ["PickSubModel"] = "--",
        ["SubEntityCaption"] = "Sous entité :",
        ["PickSubEntity"] = "--",
        ["Isolate"] = "Isoler",
        ["Show"] = "Afficher",
        ["Hide"] = "Cacher",
        ["SetColor"] = "Couleur :",
        ["SetTexture"] = "Texture :",
        ["ValidateTexture"] = "Ok",
        ["AlphaReject"] = "Activer l'alpha reject",
        ["Add"] = "Ajouter",
        ["Save"] = "Sauver",
        ["Cancel"] = "Annuler",
        ["ModelAlreadyExists"] = "Un modèle nommé \"[name]\" existe déjà dans cette zone.\nVeuillez choisir un autre nom, ou cliquer sur \"Sauver\" pour valider les changements.",
        ["ModelNoName"] = "Vous devez spécifier un nom pour votre modèle.\nUtilisez la zone de texte juste au dessus du boutton \"Ajouter\" pour entrer un nom.",
    });
else
    AddOns.ModelEditor:SetLocale({
        ["Title"] = "Model editor",
        ["PickModelFile"] = "Pick a model file...",
        ["PickModel"] = "Pick an existing model...",
        ["ShowAll"] = "Show all";
        ["HideAll"] = "Hide all";
        ["SubModelCaption"] = "Sub model :",
        ["PickSubModel"] = "--",
        ["SubEntityCaption"] = "Sub entity :",
        ["PickSubEntity"] = "--",
        ["Isolate"] = "Isolate",
        ["Show"] = "Show",
        ["SetColor"] = "Color :",
        ["SetTexture"] = "Texture :",
        ["ValidateTexture"] = "Ok",
        ["AlphaReject"] = "Enable alpha reject",
        ["Hide"] = "Hide",
        ["Add"] = "Add",
        ["Save"] = "Save",
        ["Cancel"] = "Cancel",
        ["ModelAlreadyExists"] = "A model with the name \"[name]\" already exists in this zone.\nPlease pick another name, or click on \"Save\" to save your changes.",
        ["ModelNoName"] = "You have to specify a name for your model.\nUse the text box right above the \"Add\" button to input a name.",
    });
end
