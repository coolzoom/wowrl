
function PackColor(r, g, b, a)
    return {["a"] = a, ["r"] = r, ["g"] = g, ["b"] = b};
end

function UnpackColor(color)
    return color.r, color.g, color.b, color.a;
end