
function ButtonTemplate_MenuItem.UpdateWidth()
    local width = this:GetTextWidth();
    if (this.Key) then
        width = width + 38 + this.Key:GetStringWidth();
    end
    width = math.max(width, 100);
    this:SetWidth(width);
end

ButtonTemplate_MenuCheckItem.UpdateWidth = ButtonTemplate_MenuItem.UpdateWidth;
