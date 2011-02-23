UIParent:SetFrameStrata("BACKGROUND");
UIParent:EnableMouse(true, true);

UIParent:SetScript("OnMouseUp", function ()
    if (AddOns.Editor.worldClickFunction ~= nil) then
        AddOns.Editor.worldClickFunction(arg1);
    end

    if (arg1 == "LeftButton") then
        if (AddOns.Editor.activeDropDown) then
			if (AddOns.Editor.dropDownCloseFunction) then
				if (AddOns.Editor.dropDownCloseFunctionObject) then
					AddOns.Editor.dropDownCloseFunction(AddOns.Editor.dropDownCloseFunctionObject);
				else
					AddOns.Editor.dropDownCloseFunction();
				end
			else
				AddOns.Editor.activeDropDown:Hide();
			end
			
            AddOns.Editor.activeDropDown = nil;
        end
    elseif (arg1 == "RightButton") then
        -- Handle right clicking the 3D view
    end
end);

function AddOns.Editor:SetWorldClickFunction(func)
    self.worldClickFunction = func;
end

function AddOns.Editor:NotifyDataChanged()
    MenuBar.File.DropDown.Save:Enable();
    self.dataSaved = false;
end

function AddOns.Editor:NotifyDataSaved()
    MenuBar.File.DropDown.Save:Disable();
    self.dataSaved = true;
end

function AddOns.Editor:IsDataSaved()
    if (self.dataSaved == nil) then
        self.dataSaved = true;
    end
    return self.dataSaved;
end

function AddOns.Editor:EnableMouseDecal(enabled)
    if (self.mouseDecalEnabled == nil) then
        self.mouseDecalEnabled = false;
    end

    if (self.mouseDecalEnabled ~= enabled) then
        if (enabled) then
            Frost:EnableMouseDecal(true,
                "Textures/EditorCrosshair.png", 5, self.Config.mouseDecalColor:Unpack()
            );
        else
            Frost:EnableMouseDecal(false);
        end

        self.mouseDecalEnabled = enabled;
    end
end

function AddOns.Editor:IsMouseDecalEnabled()
    if (self.mouseDecalEnabled == nil) then
        self.mouseDecalEnabled = true;
    end
    return self.mouseDecalEnabled;
end

function AddOns.Editor:SetCurrentDropDown(dropdown, closeFunction, functionObject)
    if (self.activeDropDown) then
        if (self.activeDropDown ~= dropdown) then
            self.activeDropDown:Hide();
            self.activeDropDown = dropdown;
			self.dropDownCloseFunction = closeFunction;
			self.dropDownCloseFunctionObject = functionObject;
            self.activeDropDown:Show();
        else
            self.activeDropDown:Hide();
            self.activeDropDown = nil;
			self.dropDownCloseFunction = nil;
			self.dropDownCloseFunctionObject = nil;
        end
    else
        self.activeDropDown = dropdown;
		self.dropDownCloseFunction = closeFunction;
			self.dropDownCloseFunctionObject = functionObject;
        self.activeDropDown:Show();
    end
end

function AddOns.Editor:CloseCurrentDropDown()
    if (self.activeDropDown) then
        self.activeDropDown:Hide();
        self.activeDropDown = nil;
    end
end

function AddOns.Editor:CallColorFunctions(object)
    if (self.textColorFuncList) then
        local f = self.textColorFuncList[object];
        if (f) then
            f(object, AddOns.Editor.Config.UITextColor);
        end
    end
    if (self.sndTextColorFuncList) then
        local f = self.sndTextColorFuncList[object];
        if (f) then
            f(object, AddOns.Editor.Config.UISecondaryTextColor);
        end
    end
    if (self.bgColorFuncList) then
        local f = self.bgColorFuncList[object];
        if (f) then
            f(object, AddOns.Editor.Config.UIBackgroundColor);
        end
    end
end

function AddOns.Editor:UnregisterColorFunctions(object)
    if (self.textColorFuncList) then
        self.textColorFuncList[object] = nil;
    end
    if (self.sndTextColorFuncList) then
        self.sndTextColorFuncList[object] = nil;
    end
    if (self.bgColorFuncList) then
        self.bgColorFuncList[object] = nil;
    end
end

function AddOns.Editor:RegisterTextColorFunction(object, func)
    if (self.textColorFuncList == nil) then
        self.textColorFuncList = {};
    end

    self.textColorFuncList[object] = func;
end

function AddOns.Editor:RegisterSecondaryTextColorFunction(object, func)
    if (self.sndTextColorFuncList == nil) then
        self.sndTextColorFuncList = {};
    end

    self.sndTextColorFuncList[object] = func;
end

function AddOns.Editor:RegisterBackgroundColorFunction(object, func)
    if (self.bgColorFuncList == nil) then
        self.bgColorFuncList = {};
    end

    self.bgColorFuncList[object] = func;
end
