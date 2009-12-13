function PanelBar:AddPanel(name)
    if (not self.panels) then
        self.lastPanel = nil;
        self.panelNumber = 0;
        self.panels = {};
    end
    
    local panel = CreateFrame("Frame", "$parent"..name, self, "FrameTemplate_Panel");
    if (panel) then
        panel.rank = self.panelNumber;
        panel:SetWidth(AddOns.PanelBar.Config.panelWidth);
        panel.Title:SetText(AddOns.PanelBar:GetLocalizedString(name));
        local header = CreateFrame("Button", "$parent"..name.."Header", self, "ButtonTemplate_PanelHeader");
        if (header) then
            panel.header = header;
            header.panel = panel;
            if (self.lastPanel) then
                header:SetPoint("TOPRIGHT", self.lastPanel.header, "BOTTOMRIGHT", 0, -3);
            else
                header:SetPoint("TOPRIGHT", panel, "TOPLEFT");
            end
            local thumb = header:CreateTexture("$parentThumbnail", "ARTWORK");
            if (thumb) then
                thumb:SetWidth(64);
                thumb:SetHeight(64);
                thumb:SetPoint("CENTER", header);
                thumb:SetTexture("Interface/Editor/PanelBar/PanelHeader"..name..".png");
                thumb:SetDesaturated(true);
            end
        end
        self.lastPanel = panel;
        self.panels[name] = panel;
        self.panelNumber = self.panelNumber + 1;
        return panel;
    end
end

function PanelBar:UpdateVerticalBorder()
    if (self.currentPanel) then
        if (self.currentPanel.rank ~= 0) then
            self.VerticalBorderTop:Show();
            self.VerticalBorderTop:SetPoint("BOTTOM", self.currentPanel.header, "TOP");
            self.VerticalBorderBottom:SetPoint("TOP", self.currentPanel.header, "BOTTOM");
        else
            self.VerticalBorderTop:Hide();
            self.VerticalBorderBottom:SetPoint("TOP", self.currentPanel.header, "BOTTOM");
        end
    else
        self.VerticalBorderTop:Hide();
        self.VerticalBorderBottom:SetPoint("TOP", self, "TOP");
    end
end
