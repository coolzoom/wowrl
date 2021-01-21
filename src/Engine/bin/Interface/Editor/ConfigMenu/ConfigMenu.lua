function ConfigMenu:AddPanel(caption)
    if (not self.panels) then
        self.lastPanel = nil;
        self.currentPanel = nil;
        self.panelNumber = 0;
        self.panels = {};
    end

    local panel = CreateFrame("Frame", "$parent"..caption.."Panel", self, "FrameTemplate_ConfigMenuPanel");
    if (panel) then
        panel.rank = self.panelNumber;
        panel.caption = caption;
        panel.lastItem = nil;
        panel.itemNumber = 0;
        panel.items = {};

        local header = CreateFrame("Button", "$parent"..caption.."Header", self, "ButtonTemplate_ConfigMenuPanelHeader");
        if (header) then
            panel.header = header;
            header.panel = panel;
            header:SetText(AddOns.ConfigMenu:GetLocalizedString("Panel"..caption));
            header:SetWidth(math.max(header:GetTextWidth()+10, 18));

            if (self.lastPanel) then
                header:SetPoint("LEFT", self.lastPanel.header, "RIGHT", -2, 0);
            else
                header:SetPoint("BOTTOMLEFT", panel, "TOPLEFT", 0, 2);
            end

            AddOns.Editor:CallColorFunctions(header);
        end

        self.lastPanel = panel;
        self.panels[caption] = panel;
        self.panelNumber = self.panelNumber + 1;

        return panel;
    end
end

function ConfigMenu:AddColorItem(panelName, caption, color)
    local panel = self.panels[panelName];
    if (panel) then
        local item = CreateFrame("Frame", "$parent"..caption.."Item", panel, "FrameTemplate_ConfigMenuColorItem");
        if (item) then
            item.caption = caption;
            item.Swatch.color = color;

            item.Caption:SetText(AddOns.ConfigMenu:GetLocalizedString("Panel"..panelName..caption));
            item:SetWidth(item.Caption:GetStringWidth() + 20);

            if (not color) then
                color = Color(1,1,1);
            end
            item.Swatch.ColorZone:SetBackdropColor(color:Unpack());

            if (panel.lastItem) then
                item:SetPoint("TOP", panel.lastItem, "BOTTOM");
            else
                item:SetPoint("TOP", panel, "TOP", 0, 5);
            end

            AddOns.Editor:CallColorFunctions(item);

            panel.lastItem = item;
            panel.items[caption] = item;
            panel.itemNumber = panel.itemNumber + 1;
            return item;
        end
    end
end

function ConfigMenu:SetCurrentPanel(caption)
    if (self.currentPanel) then
        self.currentPanel:Hide();
        self.currentPanel.header:SetBackdropColor(0.3, 0.3, 0.3, 0.2);
        self.currentPanel.header.normalTextColor = Color(0.8, 0.8, 0.8);
        local newColor = self.currentPanel.header.normalTextColor*AddOns.Editor.Config.UITextColor;
        self.currentPanel.header:GetNormalFontObject():SetTextColor(newColor:Unpack());
    end

    self.currentPanel = self.panels[caption];

    if (self.currentPanel) then
        self.currentPanel:Show();
        self.currentPanel.header:SetBackdropColor(1, 1, 1, 0.2);
        self.currentPanel.header.normalTextColor = Color(1, 1, 1);
        local newColor = self.currentPanel.header.normalTextColor*AddOns.Editor.Config.UITextColor;
        self.currentPanel.header:GetNormalFontObject():SetTextColor(newColor:Unpack());
        self.Borders.HorizontalLeft:SetPoint("RIGHT", self.currentPanel.header, "BOTTOMLEFT", 2);
        self.Borders.HorizontalRight:SetPoint("LEFT", self.currentPanel.header, "BOTTOMRIGHT", -2);
    end
end

ConfigMenu:AddPanel("General");
ConfigMenu:AddPanel("Interface");
ConfigMenu:AddColorItem("Interface", "TextColor", AddOns.Editor.Config.UITextColor).Swatch.onColorChange = function (color)
    for obj, f in pairs(AddOns.Editor.textColorFuncList) do
        f(obj, color);
    end
end;
ConfigMenu:AddColorItem("Interface", "SecondaryTextColor", AddOns.Editor.Config.UISecondaryTextColor).Swatch.onColorChange = function (color)
    for obj, f in pairs(AddOns.Editor.sndTextColorFuncList) do
        f(obj, color);
    end
end;
ConfigMenu:AddColorItem("Interface", "BackgroundColor", AddOns.Editor.Config.UIBackgroundColor).Swatch.onColorChange = function (color)
    for obj, f in pairs(AddOns.Editor.bgColorFuncList) do
        f(obj, color);
    end
end;

ConfigMenu:SetCurrentPanel("General");
