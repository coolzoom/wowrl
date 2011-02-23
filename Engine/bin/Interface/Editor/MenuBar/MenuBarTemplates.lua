
function ButtonTemplate_MenuItem.UpdateWidth(item)
    local width = item:GetTextWidth();
    if (item.Key) then
        width = width + 38 + item.Key:GetStringWidth();
    end
    width = math.max(width, 100);
    item:SetWidth(width);
end

CheckButtonTemplate_MenuCheckItem.UpdateWidth = ButtonTemplate_MenuItem.UpdateWidth;
