function FrameTemplate_ScrollableDropDown:SetDropDownDefaultText(text)
    self.defaultText = text;
    if (self.selectedItem == nil) then
        self.Text:SetText(text);
    end
end

function FrameTemplate_ScrollableDropDown:SetDropDownMaxHeight(height)
    if (self.maxHeight ~= height) then
        self.maxHeight = height;
        self.maxNum = math.floor(height/18);
    end
end

function FrameTemplate_ScrollableDropDown:ClearDropDownElements()
    for k, v in pairs(self.elements) do
        AddOns.Editor:UnregisterColorFunctions(v);
        DeleteFrame(v);
    end
    
    self.elements = {};
    self.numElem = 0;
    self.lastElem = nil;
    self.selectedItem = nil;
    self.Text:SetText(self.defaultText);
    
    self.DownButton:Disable();
    
    self.DropDownFrame:Hide();
    self.DropDownFrame.Scroll.Up:Hide();
    self.DropDownFrame.Scroll.Down:Hide();
    self.DropDownFrame.Scroll.Slider:Hide();
end

function FrameTemplate_ScrollableDropDown:AddDropDownElement(text, value)
    local elem = CreateFrame("Button", "$parentElem"..(self.numElem+1), self.DropDownFrame.Scroll, "ButtonTemplate_DropDownElement");
    if (elem) then
        if (self.lastElem) then
            elem:SetPoint("TOP", self.lastElem, "BOTTOM");
        else
            elem:SetPoint("TOP", self.DropDownFrame.Scroll, "TOP");
        end
        
        elem.mainParent = self;
    
        elem:SetText(text);
        elem.value = value;
        
        self.numElem = self.numElem + 1;
        elem.id = self.numElem;
        self.elements[self.numElem] = elem;
        self.lastElem = elem;
        
        if (self.numElem > self.maxNum) then
            elem:Hide();
            self.DropDownFrame.Scroll.Up:Show();
            self.DropDownFrame.Scroll.Up:Disable();
            self.DropDownFrame.Scroll.Down:Show();
            self.DropDownFrame.Scroll.Slider:Show();
            self.DropDownFrame.Scroll.Slider:SetMinMaxValues(0, self.numElem - self.maxNum);
        else
            self.DropDownFrame.Scroll:SetHeight(self.numElem*18);
        end
        
        self.DownButton:Enable();
    end
end

function FrameTemplate_ScrollableDropDown:SetDropDownScroll(scroll)
    if (scroll < 0 or scroll > self.numElem - self.maxNum) then
        return
    end

    for i = 1, scroll do
        local elem = self.elements[i];
        elem:Hide();
    end
    self.lastElem = nil;
    for i = scroll+1, self.numElem do
        local elem = self.elements[i];
        if (i - scroll > self.maxNum) then
            elem:Hide();
        else
            elem:Show();
            if (self.lastElem) then
                elem:SetPoint("TOP", self.lastElem, "BOTTOM");
            else
                elem:SetPoint("TOP", self.Scroll, "TOP");
            end
            self.lastElem = elem;
        end
    end
    
    self.scroll = scroll;
end

function FrameTemplate_ScrollableDropDown:SelectElement(id)
    if (id == nil) then
        self.selectedItem = nil;
        self.Text:SetText(self.defaultText);
        
        if (self.onSelect ~= nil) then
            self.onSelect(nil);
        end
        
        self.DropDownFrame:Hide();
        return;
    end

    if (id < 0 or id > self.numElem) then
        return
    end
    
    self.selectedItem = id;
    self.Text:SetText(self.elements[id]:GetText());
    
    if (self.onSelect ~= nil) then
        self.onSelect(self.elements[id].value);
    end
    
    self.DropDownFrame:Hide();
end

FrameTemplate_ScrollableDropDown:MarkForCopy("SetDropDownDefaultText");
FrameTemplate_ScrollableDropDown:MarkForCopy("SetDropDownMaxHeight");
FrameTemplate_ScrollableDropDown:MarkForCopy("ClearDropDownElements");
FrameTemplate_ScrollableDropDown:MarkForCopy("AddDropDownElement");
FrameTemplate_ScrollableDropDown:MarkForCopy("SetDropDownScroll");
FrameTemplate_ScrollableDropDown:MarkForCopy("SelectElement");
