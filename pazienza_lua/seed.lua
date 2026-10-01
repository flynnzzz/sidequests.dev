local function rangeto(n)
	local values = {}
	for i = 1, n do
		values[i] = i
	end
	return values
end
local values = rangeto(10)
local maxvalue = math.max(table.unpack(values))

local SeedSet = { values = values, maxvalue = maxvalue }
SeedSet.__index = SeedSet

function SeedSet:new(seed)
	local newseed = setmetatable({}, self)
	newseed.seed = seed
	return newseed
end

function SeedSet:__tostring()
	return self.seed .. "\n values: " .. table.concat(self.values, " - ")
end

return { SeedSet = SeedSet }
