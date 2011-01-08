if (GetLocale() == "frFR") then
    AddOns.Console:SetLocale({
        ["Title"] = "Console",
        ["Run"] = "Executer",
    });
else
    AddOns.Console:SetLocale({
        ["Title"] = "Console",
        ["Run"] = "Run",
    });
end
