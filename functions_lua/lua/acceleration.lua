local cmath = require("lua.cmath")

local function y(x0, v0, a, t)
	return cmath.add(x0, cmath.add(cmath.mul(v0, t), cmath.mul(cmath.div(1, 2), cmath.mul(a, cmath.mul(t, t)))))
end

return y
