--
-- test function 1
--
local cmath = require("lua.cmath")

local function f(base, exp)
	return cmath.pow(base, exp)
end

return f
