--
-- test function 2
--
local cmath = require("lua.cmath")

local function f(x)
	return cmath.exp(-cmath.pow(x, 2))
end

return f
