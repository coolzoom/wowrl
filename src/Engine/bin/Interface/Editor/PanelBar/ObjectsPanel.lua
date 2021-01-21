AddOns.PanelBar.doodadCount = {};

function AddOns.PanelBar:AddDoodad()
    if (self.currentModel == "") then return end

    if (self.doodadCount[self.currentModel] == nil) then
        self.doodadCount[self.currentModel] = 1;
    else
        self.doodadCount[self.currentModel] = self.doodadCount[self.currentModel] + 1;
    end
    
    Editor:AddDoodad(self.currentModel .. self.doodadCount[self.currentModel], self.currentModel);
    
    AddOns.Editor:SetWorldClickFunction(function ()
        Editor:NotifyDoodadPositioned();
    end);
end

local panel = PanelBar:AddPanel("Objects");
ObjectPanelContent:SetParent(panel);
ObjectPanelContent:SetPoint("LEFT",   panel,       "LEFT",   5,  0);
ObjectPanelContent:SetPoint("RIGHT",  panel,       "RIGHT", -5,  0);
ObjectPanelContent:SetPoint("TOP",    panel.Title, "BOTTOM", 0,  3);
ObjectPanelContent:SetPoint("BOTTOM", panel,       "BOTTOM", 0, -5);