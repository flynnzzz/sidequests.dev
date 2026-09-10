--
-- test function 2
--
local cmath = require("lua.cmath")

local function f(x)
	return cmath.exp(2.71828, -cmath.pow(x, 2))
end

return f
