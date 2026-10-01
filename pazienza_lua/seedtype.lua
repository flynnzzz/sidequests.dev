local SeedType = {}
SeedType.__index = SeedType
SeedType.__pairs = function(self)
	local key
	local value

	return function()
		repeat
			key, value = next(self, key)
		until key == nil or type(value) ~= "function"

		return key, value
	end
end

function SeedType:is_seed(seed)
	for _, v in pairs(self) do
		if v == seed then
			return true
		end
	end
	return false
end

local Italian = setmetatable({
	DENARE = "Denare",
	COPPE = "Coppe",
	SPADE = "Spade",
	BASTONI = "Bastoni",
}, SeedType)

return { Italian = Italian }
