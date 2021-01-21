
MenuBar.lastMenu = nil;
MenuBar.menus = {};

function MenuBar:AddMenu(caption)
    local menu = CreateFrame("Button", "$parent"..caption, MenuBar, "ButtonTemplate_Menu");
    if (menu) then
        menu.caption = caption;
        menu.items = {};
        menu.itemNbr = 0;
        menu.lastItem = nil;

        menu:SetHeight(AddOns.MenuBar.Config.barHeight);
        if (MenuBar.lastMenu) then
            menu:SetPoint("TOPLEFT", MenuBar.lastMenu, "TOPRIGHT", 3);
        else
            menu:SetPoint("TOPLEFT", MenuBar, "TOPLEFT", 3);
        end

        menu:SetText(AddOns.MenuBar:GetLocalizedString(menu.caption));
        menu:SetWidth(math.max(menu:GetTextWidth(), 10)+16);

        local border = CreateFrame("Frame", "$parentBorder", menu);
        if (border) then
            border:SetPoint("TOP", MenuBar, "TOP");
            border:SetPoint("BOTTOM", MenuBar, "BOTTOM");
            border:SetPoint("LEFT", menu, "RIGHT");
            border:SetWidth(3);
            border:SetBackdropColor(1, 1, 1);
        end

        AddOns.Editor:CallColorFunctions(menu);

        MenuBar.lastMenu = menu;
        MenuBar.menus[caption] = menu;
        return menu;
    end
end

function MenuBar:CreateMenuDropDown(caption)
    local menu = MenuBar.menus[caption];
    if (menu) then
        local dropdown = CreateFrame("Frame", "$parentDropDown", menu, "FrameTemplate_DropDown");
        if (dropdown) then
            dropdown:SetPoint("TOPLEFT", menu, "BOTTOMLEFT", -8, -5);
            local backdrop = dropdown:GetBackdrop();
            backdrop.edgeFile = "Interface/Editor/MenuBar/DropDownBorder.png";
            dropdown:SetBackdrop(backdrop);
			dropdown:EnableMouse(true);
			dropdown:SetHitRectInsets(5, 5, 0, 5);
            dropdown.menu = menu;
            dropdown.children = {};

            dropdown:SetScript("OnShow",
                function()
                    this:GetBase():OnShow();
                    MenuBar:UpdateHorizontalBorder();

                    for k, item in pairs(this.menu.items) do
                        item:Show();
                    end
                end
            );

            dropdown:SetScript("OnHide",
                function()
                    MenuBar:UpdateHorizontalBorder();

                    for k, item in pairs(this.menu.items) do
                        item:Hide();
                    end
                end
            );

            AddOns.Editor:CallColorFunctions(dropdown);
        end
    end
end

function MenuBar:AddMenuItem(menuCaption, itemCaption, keycode)
    local menu = MenuBar.menus[menuCaption];
    if (menu) then
        if (not menu.DropDown) then
            MenuBar:CreateMenuDropDown(menuCaption);
        end
        if (menu.DropDown) then
            local item = CreateFrame("Button", "$parent"..itemCaption, menu.DropDown, "ButtonTemplate_MenuItem");
            if (item) then
                item.caption = menuCaption..itemCaption;

                item:SetHeight(AddOns.MenuBar.Config.barHeight);
                item:Hide();

                if (menu.lastItem) then
                    item:SetPoint("TOPLEFT", menu.lastItem, "BOTTOMLEFT");
                else
                    item:SetPoint("TOPLEFT", menu.DropDown, "TOPLEFT", 8, 9);
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

                item:SetText(AddOns.MenuBar:GetLocalizedString(item.caption));

                local width = item:GetTextWidth();
                if (item.Key) then
                    width = width + 38 + item.Key:GetStringWidth();
                end
                width = math.max(width, 100);
                item:SetWidth(width);

                AddOns.Editor:CallColorFunctions(item);

                menu.itemNbr = menu.itemNbr + 1;
                menu.items[menu.itemNbr] = item;
                menu.DropDown.children[menu.itemNbr] = item;
                menu.lastItem = item;
                return item;
            end
        end
    end
end

function MenuBar:AddMenuCheckItem(menuCaption, itemCaption, keycode, defaultState)
    local menu = MenuBar.menus[menuCaption];
    if (menu) then
        if (not menu.DropDown) then
            MenuBar:CreateMenuDropDown(menuCaption);
        end
        if (menu.DropDown) then
            local item = CreateFrame("CheckButton", "$parent"..itemCaption, menu.DropDown, "CheckButtonTemplate_MenuCheckItem");
            if (item) then
                item.caption = menuCaption..itemCaption;

                item:SetChecked(defaultState);
                item:SetHeight(AddOns.MenuBar.Config.barHeight);
                item:Hide();

                if (menu.lastItem) then
                    item:SetPoint("TOPLEFT", menu.lastItem, "BOTTOMLEFT");
                else
                    item:SetPoint("TOPLEFT", menu.DropDown, "TOPLEFT", 8, 8);
                end

                item:SetText(AddOns.MenuBar:GetLocalizedString(item.caption));

                if (keycode) then
                    local key = item:CreateFontString("$parentKey");
                    if (key) then
                        key:SetFont("Fonts/Vera.ttf", 8);
                        key:SetJustifyH("RIGHT");
                        key:SetText(keycode);
                        key:SetPoint("RIGHT", item, "RIGHT", -8, 0);
                        key:SetTextColor(0.3,0.3,0.3);
                    end
                end

                local width = item:GetTextWidth();
                if (item.Key) then
                    width = width + 38 + item.Key:GetStringWidth();
                end
                width = math.max(width, 100);
                item:SetWidth(width);

                AddOns.Editor:CallColorFunctions(item);

                menu.itemNbr = menu.itemNbr + 1;
                menu.items[menu.itemNbr] = item;
                menu.DropDown.children[menu.itemNbr] = item;
                menu.lastItem = item;
                return item;
            end
        end
    end
end

function MenuBar:SetCurrentDropDown(dropdown)
	AddOns.Editor:SetCurrentDropDown(dropdown, self.CloseCurrentDropDown, self);
    if (self.activeDropDown) then
        if (self.activeDropDown ~= dropdown) then
            self.activeDropDown = dropdown;
        else
            self.activeDropDown = nil;
        end
    else
        self.activeDropDown = dropdown;
    end
end

function MenuBar:CloseCurrentDropDown()
	if (self.activeDropDown) then
		AddOns.Editor:CloseCurrentDropDown();
		self.activeDropDown = nil;
	end
end

function MenuBar:UpdateHorizontalBorder()
    if (self.activeDropDown) then
        self.HorizontalBorderRight:SetPoint("TOPLEFT", self.activeDropDown:GetParent(), "BOTTOMRIGHT");
        self.HorizontalBorderLeft:SetPoint("TOPRIGHT", self.activeDropDown:GetParent(), "BOTTOMLEFT");
    else
        self.HorizontalBorderLeft:SetPoint("TOPRIGHT", self, "BOTTOMRIGHT");
    end
end
