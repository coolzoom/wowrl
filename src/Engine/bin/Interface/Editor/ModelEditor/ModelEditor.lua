function ModelEditor:Error(message, width, height)
    if (width) then
        ModelEditorErrorDialog:SetWidth(width);
    end
    if (height) then
        ModelEditorErrorDialog:SetHeight(height);
    end
    
    message = string.gsub(message, "%[name%]", self.modelName);
    
    ModelEditorErrorDialogContent:SetText(message);
    ModelEditorErrorDialog:Show();
end

function ModelEditor:Init()
    self.optionsEnabled = false;
    self.selectColor = true;
    self.alphaReject = false;
    self.texture = "";

    self.Title.TitleString:SetText(AddOns.ModelEditor:GetLocalizedString("Title"));
    
    self.Options.ShowAll:Disable();
    self.Options.HideAll:Disable();
    
    self.Options.SubModelCaption:SetText(AddOns.ModelEditor:GetLocalizedString("SubModelCaption"));
    self.Options.SubEntityCaption:SetText(AddOns.ModelEditor:GetLocalizedString("SubEntityCaption"));
    
    self.Options.IsolateSelected:Disable();
    self.Options.ShowSelected:Disable();
    self.Options.HideSelected:Disable();
    
    self.Options.ColorTextureSwitch.Color:SetChecked(true);
    self.Options.ColorTextureSwitch.Color:Disable();
    self.Options.ColorTextureSwitch.Texture:Disable();
    
    self.Options.SetColorCaption:SetText(AddOns.ModelEditor:GetLocalizedString("SetColor"));
    self.Options.SetColorCaption:SetWidth(self.Options.SetColorCaption:GetStringWidth()+2);
    
    self.Options.ColorPicker.color = Color(1, 1, 1);
    self.Options.ColorPicker.ColorZone:SetBackdropColor(1, 1, 1);
    self.Options.ColorPicker:Disable();
    
    self.Options.SetTextureCaption:SetText(AddOns.ModelEditor:GetLocalizedString("SetTexture"));
    self.Options.SetTextureCaption:SetWidth(self.Options.SetTextureCaption:GetStringWidth()+2);
    
    self.Options.TextureEditBox:EnableMouse(false);
    self.Options.ValidateTextureButton:Disable();
    self.Options.PickTextureButton:Disable();
    
    self.Options.AlphaReject:Disable();
    
    self.Options.NameEditBox:EnableMouse(false);
    
    self.Options.Add:Disable();
    self.Options.Save:Disable();
end

function ModelEditor:OnModelSelected()
    self.optionsEnabled = true;
    AddOns.Editor:CallColorFunctions(self);
    
    ModelEditor.Options.ShowAll:Enable();
    ModelEditor.Options.HideAll:Enable();
    
    ModelEditor.Options.ColorTextureSwitch.Color:Enable();
    ModelEditor.Options.ColorTextureSwitch.Texture:Enable();
    
    ModelEditor.Options.ColorPicker.color = Color(1, 1, 1);
    ModelEditor.Options.ColorPicker.ColorZone:SetBackdropColor(1, 1, 1);
    
    ModelEditor.Options.TextureEditBox:SetText("");
    
    if (self.selectColor) then
        ModelEditor.Options.ColorPicker:Enable();
    else
        ModelEditor.Options.TextureEditBox:EnableMouse(true);
        ModelEditor.Options.ValidateTextureButton:Enable();
        ModelEditor.Options.PickTextureButton:Enable();
    end
    
    ModelEditor.Options.AlphaReject:Enable();
    ModelEditor.Options.AlphaReject:SetChecked(false);
    
    ModelEditor.Options.NameEditBox:EnableMouse(true);

    local subModels = {ModelEditor.Model:GetAvailableSubMeshes()};
    if (#subModels == 0) then
        return;
    end
    
    ModelEditor.Options.PickSubModel:ClearDropDownElements();
    ModelEditor.Options.PickSubEntity:ClearDropDownElements();
    
    ModelEditor.Options.PickSubModel:AddDropDownElement("--", nil);

    for k, v in pairs(subModels) do
        ModelEditor.Options.PickSubModel:AddDropDownElement(""..v, v);
    end
    
    self.subModel = nil;
    self.subEntity = nil;
    self.texture = "";
    self.alphaReject = false;
    
    ModelEditor.Options.IsolateSelected:Disable();
    ModelEditor.Options.ShowSelected:Disable();
    ModelEditor.Options.HideSelected:Disable();
end

function ModelEditor:OnSubModelSelected()
    ModelEditor.Options.PickSubEntity:ClearDropDownElements();
    
    if (self.subModel == nil) then
        ModelEditor.Options.IsolateSelected:Disable();
        ModelEditor.Options.ShowSelected:Disable();
        ModelEditor.Options.HideSelected:Disable();
        return;
    end
    
    ModelEditor.Options.IsolateSelected:Enable();
    ModelEditor.Options.ShowSelected:Enable();
    ModelEditor.Options.HideSelected:Enable();
    
    ModelEditor.Options.PickSubEntity:AddDropDownElement("--", nil);
    
    local subEntityNbr = ModelEditor.Model:GetSubEntityNumber(self.subModel);
    if (subEntityNbr == 0) then
        return;
    end

    for i=0, subEntityNbr do
        ModelEditor.Options.PickSubEntity:AddDropDownElement(""..i, i);
    end
end
