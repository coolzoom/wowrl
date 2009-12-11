PanelBar.Config = {
    ["headerColor"] = PackColor(0.75, 0.75, 0.75, 0.8),
    ["panelColor"] = PackColor(0.75, 0.75, 0.75, 0.8),
    ["panelWidth"] = 200,
};

PanelBar.lastPanel = nil;
PanelBar.panelNumber = 0;
PanelBar.panels = {};

function PanelBar:AddPanel(name)
    local panel = CreateFrame("Frame", "$parent"..name, PanelBar, "FrameTemplate_Panel");
    if (panel) then
        panel.rank = PanelBar.panelNumber;
        panel:SetWidth(PanelBar.Config.panelWidth);
        panel.Title:SetText(PanelBar.Locale[name])
        local header = CreateFrame("Button", "$parent"..name.."Header", PanelBar, "ButtonTemplate_PanelHeader");
        if (header) then
            panel.header = header;
            header.panel = panel;
            if (PanelBar.lastPanel) then
                header:SetPoint("TOPRIGHT", PanelBar.lastPanel.header, "BOTTOMRIGHT", 0, -3);
            else
                header:SetPoint("TOPRIGHT", panel, "TOPLEFT");
            end
            header.Background:SetVertexColor(UnpackColor(PanelBar.Config.headerColor));
            local thumb = header:CreateTexture("$parentThumbnail", "ARTWORK");
            if (thumb) then
                thumb:SetWidth(64);
                thumb:SetHeight(64);
                thumb:SetPoint("CENTER", header);
                thumb:SetTexture("Interface/Editor/PanelBar/PanelHeader"..name..".png");
                thumb:SetDesaturated(true);
            end
        end
        PanelBar.lastPanel = panel;
        PanelBar.panels[name] = panel;
        PanelBar.panelNumber = PanelBar.panelNumber + 1;
        return panel;
    end
end

function PanelBar:UpdateVerticalBorder()
    if (PanelBar.currentPanel) then
        if (PanelBar.currentPanel.rank ~= 0) then
            PanelBar.VerticalBorderTop:Show();
            PanelBar.VerticalBorderTop:SetPoint("BOTTOM", PanelBar.currentPanel.header, "TOP");
            PanelBar.VerticalBorderBottom:SetPoint("TOP", PanelBar.currentPanel.header, "BOTTOM");
        else
            PanelBar.VerticalBorderTop:Hide();
            PanelBar.VerticalBorderBottom:SetPoint("TOP", PanelBar.currentPanel.header, "BOTTOM");
        end
    else
        PanelBar.VerticalBorderTop:Hide();
        PanelBar.VerticalBorderBottom:SetPoint("TOP", PanelBar, "TOP");
    end
end
