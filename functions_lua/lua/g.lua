--
-- test function 2
--
local cmath = require("lua.cmath")

local function g(a)
	return cmath.pow(a, a)
end

return g
