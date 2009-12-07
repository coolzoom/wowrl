MenuBar.Config = {
    ["barHeight"] = 16,
    ["barColor"] = {["a"] = 0.8, ["r"] = 0.75, ["g"] = 0.75, ["b"] = 0.75},
}

MenuBar.lastMenu = nil;
MenuBar.menus = {};

function MenuBar:AddMenu(caption)
    local menu = CreateFrame("Button", "$parent"..caption.."Button", MenuBar, "ButtonTemplate_Menu");
    if (menu) then
        menu.caption = caption;
        menu.lastItem = nil;
       
        menu:SetHeight(MenuBar.Config.barHeight);
        if (MenuBar.lastMenu) then
            menu:SetPoint("TOPLEFT", MenuBar.lastMenu, "TOPRIGHT", 3);
        else
            menu:SetPoint("TOPLEFT", MenuBar, "TOPLEFT", 3);
        end
        
        menu:SetScript("OnLoad",
            function()
                this:SetText(MenuBar.Locale[this.caption]);
                base:OnLoad();
            end
        );
        
        local border = CreateFrame("Frame", "$parentBorder", menu);
        if (border) then
            border:SetPoint("TOP", MenuBar, "TOP");
            border:SetPoint("BOTTOM", MenuBar, "BOTTOM");
            border:SetPoint("LEFT", menu, "RIGHT");
            border:SetWidth(3);
            border:SetBackdrop({});
            border:SetBackdropColor(1, 1, 1);
        end
        
        MenuBar.lastMenu = menu;
        MenuBar.menus[caption] = menu;
        return menu;
    end
end

function MenuBar:CreateMenuDropdown(caption)
    local menu = MenuBar.menus[caption];
    if (menu) then
        local dropdown = CreateFrame("Frame", "$parentDropdown", menu, "FrameTemplate_Dropdown");
        if (dropdown) then
            dropdown:SetPoint("TOPLEFT", menu, "BOTTOMLEFT", -8, -5);
            local backdrop = dropdown:GetBackdrop();
            backdrop.edgeFile = "Interface/Editor/MenuBar/DropdownBorder.png";
            dropdown:SetBackdrop(backdrop);
            dropdown:SetBackdropColor(MenuBar.Config.barColor.r, MenuBar.Config.barColor.g, MenuBar.Config.barColor.b, MenuBar.Config.barColor.a);
            
            dropdown:SetScript("OnShow",
                function()
                    base:OnShow(); 
                    MenuBar:UpdateHorizontalBorder();
                end
            );
            
            dropdown:SetScript("OnHide",
                function()
                    MenuBar:UpdateHorizontalBorder();
                end
            );
        end
    end
end

function MenuBar:AddMenuItem(menuCaption, itemCaption, keycode)
    local menu = MenuBar.menus[menuCaption];
    if (menu) then
        if (not menu.Dropdown) then
            MenuBar:CreateMenuDropdown(menuCaption);
        end
        if (menu.Dropdown) then
            local item = CreateFrame("Button", menu:GetName()..itemCaption, menu.Dropdown, "ButtonTemplate_MenuItem");
            if (item) then
                item.caption = menuCaption..itemCaption;
            
                item:SetHeight(MenuBar.Config.barHeight);
                
                if (menu.lastItem) then
                    item:SetPoint("TOPLEFT", menu.lastItem, "BOTTOMLEFT");
                else
                    item:SetPoint("TOPLEFT", menu.Dropdown, "TOPLEFT", 8, 9);
                end
                
                item:SetScript("OnLoad",
                    function()
                        this:SetText(MenuBar.Locale[this.caption]);
                        base:UpdateWidth();
                    end
                );
                
                if (keycode) then
                    local key = item:CreateFontString("$parentKey");
                    if (key) then
                        key:SetFont("Fonts/Vera.ttf", 8);
                        key:SetJustifyH("RIGHT");
                        key:SetText(keycode);
                        key:SetPoint("RIGHT", item, "RIGHT", -8, 0);
                    end
                end
                
                menu.lastItem = item;
                return item;
            end
        end
    end
end

function MenuBar:AddMenuCheckItem(menuCaption, itemCaption, keycode, defaultState)
    local menu = MenuBar.menus[menuCaption];
    if (menu) then
        if (not menu.Dropdown) then
            MenuBar:CreateMenuDropdown(menuCaption);
        end
        if (menu.Dropdown) then
            local item = CreateFrame("Button", menu:GetName()..itemCaption, menu.Dropdown, "ButtonTemplate_MenuCheckItem");
            if (item) then
                item.caption = menuCaption..itemCaption;
            
                item:SetHeight(MenuBar.Config.barHeight);
                
                if (menu.lastItem) then
                    item:SetPoint("TOPLEFT", menu.lastItem, "BOTTOMLEFT");
                else
                    item:SetPoint("TOPLEFT", menu.Dropdown, "TOPLEFT", 8, 8);
                end
                
                if (defaultState) then
                    item:SetScript("OnLoad",
                        function()
                            this:SetText(MenuBar.Locale[this.caption]);
                            base:UpdateWidth();
                            this.enabled = true;
                        end
                    );
                else
                    item:SetScript("OnLoad",
                        function()
                            this:SetText(MenuBar.Locale[this.caption]);
                            base:UpdateWidth();
                            this.enabled = false;
                        end
                    );
                end
                
                if (keycode) then
                    local key = item:CreateFontString("$parentKey");
                    if (key) then
                        key:SetFont("Fonts/Vera.ttf", 8);
                        key:SetJustifyH("RIGHT");
                        key:SetText(keycode);
                        key:SetPoint("RIGHT", item, "RIGHT", -8, 0);
                    end
                end
                
                menu.lastItem = item;
                return item;
            end
        end
    end
end

function MenuBar:UpdateHorizontalBorder()
    if (UIParent.activeDropdown) then
        --MenuBar.HorizontalBorderRight:Show();
        MenuBar.HorizontalBorderRight:SetPoint("TOPLEFT", UIParent.activeDropdown:GetParent(), "BOTTOMRIGHT");
        MenuBar.HorizontalBorderLeft:SetPoint("TOPRIGHT", UIParent.activeDropdown:GetParent(), "BOTTOMLEFT");
    else
        --MenuBar.HorizontalBorderRight:Hide();
        MenuBar.HorizontalBorderLeft:SetPoint("TOPRIGHT", MenuBar, "BOTTOMRIGHT");
    end
end
