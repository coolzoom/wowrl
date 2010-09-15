AddOns.ColorSelector = AddOn("ColorSelector");

function AddOns.ColorSelector:LockColorUpdate()
    self.lockUpdate = true;
end

function AddOns.ColorSelector:UnlockColorUpdate()
    self.lockUpdate = false;
end

function AddOns.ColorSelector:IsColorUpdateLocked()
    if (self.lockUpdate == nil) then
        self.lockUpdate = false;
    end
    return self.lockUpdate;
end
