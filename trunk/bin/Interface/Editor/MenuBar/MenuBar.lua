MenuBar.Config = {
    ["barHeight"] = 16
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
            menu:SetPoint("TOPLEFT", MenuBar.lastMenu, "TOPRIGHT");
        else
            menu:SetPoint("TOPLEFT");
        end
        
        menu:SetScript("OnLoad",
            function()
                this:SetText(MenuBar.Locale[this.caption]);
                base:OnLoad();
            end
        );
        
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
            dropdown:SetPoint("TOPLEFT", menu, "BOTTOMLEFT");
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
                    item:SetPoint("TOPLEFT");
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
                        key:SetPoint("RIGHT", item, "RIGHT", -16, 0);
                        key:SetTextColor(0.5, 0.5, 0.5);
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
                    item:SetPoint("TOPLEFT");
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
                        key:SetPoint("RIGHT", item, "RIGHT", -16, 0);
                        key:SetTextColor(0.5, 0.5, 0.5);
                    end
                end
                
                menu.lastItem = item;
                return item;
            end
        end
    end
end
