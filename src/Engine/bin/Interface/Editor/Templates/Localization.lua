if (GetLocale() == "frFR") then
    AddOns.Templates:SetLocale({
        ["ConfirmDialogTitle"] = "Etes-vous sur ?",
        ["ConfirmDialogCancel"] = "Annuler",
        ["ConfirmDialogOK"] = "OK",
        ["YesNoDialogTitle"] = "Que faire ?",
        ["YesNoDialogCancel"] = "Annuler",
        ["YesNoDialogNo"] = "Non",
        ["YesNoDialogYes"] = "Oui",
        ["ErrorDialogTitle"] = "Erreur",
        ["ErrorDialogOK"] = "OK",
    });
else
    AddOns.Templates:SetLocale({
        ["ConfirmDialogTitle"] = "Are you sure ?",
        ["ConfirmDialogCancel"] = "Cancel",
        ["ConfirmDialogOK"] = "OK",
        ["YesNoDialogTitle"] = "What should I do ?",
        ["YesNoDialogCancel"] = "Cancel",
        ["YesNoDialogNo"] = "No",
        ["YesNoDialogYes"] = "Yes",
        ["ErrorDialogTitle"] = "Error",
        ["ErrorDialogOK"] = "OK",
    });
end
