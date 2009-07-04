-- Simple class creation.
-- Link : http://lua-users.org/wiki/SimpleLuaClasses
-------
function class(base, ctor)
    local c = {};     -- A new class instance
    if (not(ctor) and (type(base) == 'function')) then
        ctor = base;
        base = nil;
    elseif (type(base) == 'table') then
        -- Our new class is a shallow copy of the base class!
        for i,v in pairs(base) do
            c[i] = v;
        end
        c._base = base;
    end
    -- The class will be the metatable for all its objects,
    -- And they will look up their methods in it.
    c.__index = c;

    -- Expose a ctor which can be called by <classname>(<args>)
    local mt = {};
    mt.__call = function (class_tbl, ...)
        local obj = {};
        setmetatable(obj, c);
        if (ctor) then
            ctor(obj, ...);
        else
            -- Make sure that any stuff from the base class is initialized!
            if (base and base.init) then
                base.init(obj, ...);
            end
        end
        return obj;
    end

    c.init = ctor;
    c.is_a = function(self, klass)
        local m = getmetatable(self);
        while (m) do
            if (m == klass) then
                return true;
            end
            m = m._base;
        end
        return false;
    end
    setmetatable(c,mt);
    return c;
end
-------
-- End of quoted code

-- Vector class definition
Vector = class(
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
  return "("..self.x..", "..self.y..", "..self.z..")";
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

function Vector:Log()
    Log("("..self.x..", "..self.y..", "..self.z..")");
end

Vector.UNIT_X = Vector(1, 0, 0);
Vector.UNIT_Y = Vector(0, 1, 0);
Vector.UNIT_Z = Vector(0, 0, 1);
