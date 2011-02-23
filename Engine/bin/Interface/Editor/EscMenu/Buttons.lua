EscMenu:AddButton("GeneralOptions");
EscMenu:AddButton("EditorOptions"):SetScript("OnClick", function()
    EscMenu:Hide();
    ConfigMenu:Show();
end);
EscMenu:AddSeparator();
EscMenu:AddButton("Exit"):SetScript("OnClick", function()
    if (AddOns.Editor:IsDataSaved()) then
        Exit();
    else
        EscMenuConfirmDialog:Show();
    end
end);