
EscMenu.lastItem = nil;
EscMenu.buttons = {};
EscMenu.buttonNbr = 0;
EscMenu.separatorNbr = 0;
EscMenu.height = 35;

function EscMenu:AddButton(caption)
    local button = CreateFrame("Button", "$parent"..caption.."Button", self, "ButtonTemplate_Button");
    if (button) then
        button.caption = caption;
        
        button:SetText(AddOns.EscMenu:GetLocalizedString(caption));       
        if (self.lastItem) then
            button:SetPoint("TOP", self.lastItem, "BOTTOM", 0, 3);
        else
            button:SetPoint("TOP", self, "TOP", 0, 27);
        end
        
        button:SetPoint("LEFT",  self, "LEFT",   10, 0);
        button:SetPoint("RIGHT", self, "RIGHT", -10, 0);
        
        -- Prevent the button to automatically resize itself on show
        button:SetScript("OnShow", nil);
        
        self.lastItem = button;
        self.buttons[caption] = button;
        self.buttonNbr = self.buttonNbr + 1;
        
        self.height = self.height + button:GetHeight()+3;
        self:SetHeight(self.height);
        return button;
    end
end

function EscMenu:AddSeparator()
    local sep = CreateFrame("Frame", "$parentSeparator"..(self.separatorNbr+1), self, "FrameTemplate_EscMenuSeparator");
    if (sep) then
        if (self.lastItem) then
            sep:SetPoint("TOP", self.lastItem, "BOTTOM", 0, 3);
        else
            sep:SetPoint("TOP", self, "TOP", 0, 27);
        end
        
        self.lastItem = sep;
        self.separatorNbr = self.separatorNbr + 1;
        
        self.height = self.height + sep:GetHeight()+3;
        self:SetHeight(self.height);
        return sep;
    end
end

SetKeyBinding(KEY_ESCAPE, "EscMenu:OnEscape();");
function EscMenu:OnEscape()
    if (not self:IsShown()) then
        self:Show();
    else
        self.buttons["Exit"]:OnClick();
    end
end
