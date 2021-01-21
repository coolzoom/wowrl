local panel = PanelBar:AddPanel("Terrain");
TerrainPanelContent:SetParent(panel);
TerrainPanelContent:SetPoint("LEFT",   panel,       "LEFT",   5,  0);
TerrainPanelContent:SetPoint("RIGHT",  panel,       "RIGHT", -5,  0);
TerrainPanelContent:SetPoint("TOP",    panel.Title, "BOTTOM", 0,  3);
TerrainPanelContent:SetPoint("BOTTOM", panel,       "BOTTOM", 0, -5);