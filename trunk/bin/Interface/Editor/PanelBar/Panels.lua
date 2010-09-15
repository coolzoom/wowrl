local objectsPanel = PanelBar:AddPanel("Objects");
objectsPanel:AddButton("EditModel"):SetScript("OnClick", function()
    ModelEditor:Show();
end);
objectsPanel:AddButton("AddDoodad"):SetScript("OnClick", function()
    Frost:AddDoodad("TestDoodad"..this.doodadNumber, "Crate");
    AddOns.Editor:SetWorldClickFunction(function ()
        Frost:NotifyDoodadPositioned();
    end);
end);

local terrainPanel = PanelBar:AddPanel("Terrain");

