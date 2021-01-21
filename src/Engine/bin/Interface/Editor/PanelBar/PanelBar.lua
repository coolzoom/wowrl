function PanelBar:AddPanel(name)
    if (not self.panels) then
        self.lastPanel = nil;
        self.panelNumber = 0;
        self.panels = {};
    end

    local panel = CreateFrame("Frame", "$parent"..name, self, "FrameTemplate_Panel");
    if (panel) then
        panel.name = name;
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

function AddFontString(panel, name, baseString, lineNbr)
    if (not panel) then return nil end

    local text = panel.Content:CreateFontString("$parent"..name, "ARTWORK");
    text:SetPoint("LEFT", panel.Content, "LEFT", 5, 0);
    text:SetPoint("RIGHT", panel.Content, "RIGHT", -5, 0);
    if (panel.lastObject) then
        text:SetPoint("TOP", panel.lastObject, "BOTTOM", 0, 3);
    else
        text:SetPoint("TOP", panel.Content, "TOP", 0, 5);
    end
    text:SetFont("Fonts/Vera.ttf", 8);
    text:SetHeight(18*lineNbr);
    text:SetText(AddOns.PanelBar:GetLocalizedString(baseString));

    panel.lastObject = text;

    return text;
end

function AddButton(panel, name)
    if (not panel) then return nil end

    local button = CreateFrame("Button", "$parent"..name, panel.Content, "ButtonTemplate_Button");
    button:SetPoint("LEFT", panel.Content, "LEFT", 5, 0);
    button:SetPoint("RIGHT", panel.Content, "RIGHT", -5, 0);
    if (panel.lastObject) then
        button:SetPoint("TOP", panel.lastObject, "BOTTOM", 0, 3);
    else
        button:SetPoint("TOP", panel.Content, "TOP", 0, 5);
    end
    button:SetText(AddOns.PanelBar:GetLocalizedString(panel.name..name));

    panel.lastObject = button;

    return button;
end

function PanelBar:UpdateVerticalBorder()
    if (self.currentPanel) then
        if (self.currentPanel.rank ~= 0) then
            self.Borders.VerticalTop:Show();
            self.Borders.VerticalTop:SetPoint("BOTTOM", self.currentPanel.header, "TOP");
            self.Borders.VerticalBottom:SetPoint("TOP", self.currentPanel.header, "BOTTOM");
        else
            self.Borders.VerticalTop:Hide();
            self.Borders.VerticalBottom:SetPoint("TOP", self.currentPanel.header, "BOTTOM");
        end
    else
        self.Borders.VerticalTop:Hide();
        self.Borders.VerticalBottom:SetPoint("TOP", self, "TOP");
    end
end
