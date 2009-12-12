-- Vector class definition
Vector = CreateClass(
    function(vec, x, y, z)
        if (x ~= nil) then vec.x = x; else vec.x = 0; end
        if (y ~= nil) then vec.y = y; else vec.y = 0; end
        if (z ~= nil) then vec.z = z; else vec.z = 0; end
    end
)

-- Operator overloadings
function Vector.__eq(vec1, vec2)
    return vec1.x == vec2.x and vec1.y == vec2.y and vec1.z == vec2.z;
end

function Vector.__add(vec1, vec2)
    return Vector(vec1.x+vec2.x, vec1.y+vec2.y, vec1.z+vec2.z);
end

function Vector.__sub(vec1, vec2)
    return Vector(vec1.x-vec2.x, vec1.y+vec2.y, vec1.z+vec2.z);
end

function Vector.__unm(vec)
    return Vector(-vec.x, -vec.y, -vec.z);
end

function Vector.__mul(a, vec)
    return Vector(a*vec.x, a*vec.y, a*vec.z);
end

function Vector.__div(vec, a)
    return Vector(vec.x/a, vec.y/a, vec.z/a);
end

function Vector.__pow(vec1, vec2)
    return Vector(
        vec1.y*vec2.z - vec1.z*vec2.y,
        vec1.z*vec2.x - vec1.x*vec2.z,
        vec1.x*vec2.y - vec1.y*vec2.x
    );
end

function Vector.__tostring(p)
    return "("..p.x..", "..p.y..", "..p.z..")";
end

-- Member functions
function Vector:GetNorm()
    return math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z);
end

function Vector:Normalize()
    local length = math.sqrt(self.x*self.x + self.y*self.y + self.z*self.z);
    self.x = self.x / length;
    self.y = self.y / length;
    self.z = self.z / length;
end

function Vector:Rotate(angle, vec)
    local halfAngle = 0.5*angle;
    local sinus = math.sin(halfAngle*2*math.pi);
    local w = math.cos(halfAngle*2*math.pi);
    vec = sinus*vec;

    local uv = vec^self;
    local uuv = vec^uv;
    uv = 2*w*uv;
    uuv = 2*uuv;

    self.x = self.x + uv.x + uuv.x;
    self.y = self.y + uv.y + uuv.y;
    self.z = self.z + uv.z + uuv.z;
end

function Vector:IsNull()
    return (self.x == 0.0 and self.y == 0.0 and self.z == 0.0);
end

function Vector:Log()
    Log("("..self.x..", "..self.y..", "..self.z..")");
end

Vector.UNIT_X = Vector(1, 0, 0);
Vector.UNIT_Y = Vector(0, 1, 0);
Vector.UNIT_Z = Vector(0, 0, 1);
