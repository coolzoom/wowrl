-- Color class definition
Color = CreateClass(function (color, r, g, b, a)
    if (not g and not b and not a) then
        g = r.g; b = r.b; a = r.a; r = r.r;
    end
    color.r = math.min(math.max(r, 0), 1);
    color.g = math.min(math.max(g, 0), 1);
    color.b = math.min(math.max(b, 0), 1);
    if (a) then color.a = math.min(math.max(a, 0), 1); else color.a = 1; end
end);

-- Operators overloadings
function Color.__add(c1, c2)
    if (type(c2) == "number") then
        return Color(c1.r+c2, c1.g+c2, c1.b+c2, c1.a);
    else
        return Color(c1.r+c2.r, c1.g+c2.g, c1.b+c2.b, c1.a);
    end
end

function Color.__sub(c1, c2)
    if (type(c2) == "number") then
        return Color(c1.r-c2, c1.g-c2, c1.b-c2, c1.a);
    else
        return Color(c1.r-c2.r, c1.g-c2.g, c1.b-c2.b, c1.a);
    end
end

function Color.__mul(c1, c2)
    if (type(c2) == "number") then
        return Color(c1.r*c2, c1.g*c2, c1.b*c2, c1.a);
    else
        return Color(c1.r*c2.r, c1.g*c2.g, c1.b*c2.b, c1.a*c2.a);
    end
end

function Color.__div(c1, c2)
    if (type(c2) == "number") then
        return Color(c1.r/c2, c1.g/c2, c1.b/c2, c1.a);
    else
        return Color(c1.r/c2.r, c1.g/c2.g, c1.b/c2.b, c1.a/c2.a);
    end
end

function Color.__concat(str, c)
    return str.."a : "..c.a..", r : "..c.r..", g : "..c.g..", b : "..c.b;
end

-- Member functions
function Color:Assign(r, g, b, a)
    if (not g and not b and not a) then
        g = r.g; b = r.b; a = r.a; r = r.r;
    end
    self.r = math.min(math.max(r, 0), 1);
    self.g = math.min(math.max(g, 0), 1);
    self.b = math.min(math.max(b, 0), 1);
    if (a) then self.a = math.min(math.max(a, 0), 1); else self.a = 1; end
end

function Color:Unpack()
    return self.r, self.g, self.b, self.a;
end

function Color:Serialize(tab)
    return "Color("..self.r..", "..self.g..", "..self.b..", "..self.a..")";
end
