--
-- test function 1
--
local cmath = require("lua.cmath")

local function f(a, b)
	return cmath.sqrt(cmath.add(cmath.pow(a, 2), cmath.pow(b, 2)))
end

return f
