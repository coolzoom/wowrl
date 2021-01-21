local onColorUpdate = nil;

function ColorSelector:SetOnColorUpdateFunc(func)
    onColorUpdate = func;
end

function ColorSelector:UpdateASlider()
    self.SliderA.Slider:SetValue(self.currentColor.a*255.0, true);
    self.SliderA.ValueString:SetText(self.currentColor.a*255.0);
end

function ColorSelector:UpdateRGBSliders()
    self.SliderR.Slider:SetValue(self.currentColor.r*255.0, true);
    self.SliderG.Slider:SetValue(self.currentColor.g*255.0, true);
    self.SliderB.Slider:SetValue(self.currentColor.b*255.0, true);

    self.SliderR.ValueString:SetText(math.floor(self.currentColor.r*255.0));
    self.SliderG.ValueString:SetText(math.floor(self.currentColor.g*255.0));
    self.SliderB.ValueString:SetText(math.floor(self.currentColor.b*255.0));
end

function ColorSelector:UpdateHLSSliders()
    self.SliderH.Slider:SetValue(self.currentColor.h,       true);
    self.SliderS.Slider:SetValue(self.currentColor.s*100.0, true);
    self.SliderL.Slider:SetValue(self.currentColor.l*100.0, true);
    
    self.SliderH.ValueString:SetText(math.floor(self.currentColor.h));
    self.SliderS.ValueString:SetText(math.floor(self.currentColor.s*100.0));
    self.SliderL.ValueString:SetText(math.floor(self.currentColor.l*100.0));
end

function ColorSelector:SetColor(color)
    self.currentColor = Color(color);

    -- Set the value of classic color sliders
    self:UpdateASlider();
    self:UpdateRGBSliders();
    
    self.Swatch.ColorZone:SetBackdropColor(self.currentColor:Unpack());
    
    -- Calculate HSV values
    self.currentColor:MakeHSLFromRBG();
    self:UpdateHLSSliders();
    
    -- Update the swatch color
    self.Swatch.ColorZone:SetBackdropColor(self.currentColor:Unpack());
end

function ColorSelector:OnColorUpdate(chanel, value)
    -- Update currentColor and make the conversion between RGB and HLS
    if (chanel == "r" or chanel == "g" or chanel == "b") then
        self.currentColor[chanel] = value/255.0;
        self.currentColor:MakeHSLFromRBG();
        self:UpdateHLSSliders();
    elseif (chanel == "a") then
        self.currentColor[chanel] = value/255.0;
    elseif (chanel == "h") then
        self.currentColor[chanel] = value;
        self.currentColor:MakeRGBFromHSL();
        self:UpdateRGBSliders();
    elseif (chanel == "s" or chanel == "l") then
        self.currentColor[chanel] = value/100.0;
        self.currentColor:MakeRGBFromHSL();
        self:UpdateRGBSliders();
    end

    if (not AddOns.ColorSelector:IsColorUpdateLocked()) then
        -- Update the swatch color
        self.Swatch.ColorZone:SetBackdropColor(self.currentColor:Unpack());
        
        -- Notify the client widget that the color has changed
        if (onColorUpdate) then
            onColorUpdate(self.currentColor);
        end
    end
end