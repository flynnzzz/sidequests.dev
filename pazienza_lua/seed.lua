local function rangeto(n)
	local values = {}
	for i = 1, n do
		values[i] = i
	end
	return values
end

local SeedSet = {}
SeedSet.__index = SeedSet

function SeedSet:new(seed, maxvalue)
	local newseed = setmetatable({}, self)
	newseed.seed = seed
	newseed.maxvalue = maxvalue
	newseed.values = rangeto(maxvalue)
	return newseed
end

function SeedSet:__tostring()
	return self.seed .. "\n values: " .. table.concat(self.values, " - ")
end

return { SeedSet = SeedSet }
