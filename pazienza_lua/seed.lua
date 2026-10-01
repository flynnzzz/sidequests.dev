local Eseed = {
	DENARE = "Denare",
	COPPE = "Coppe",
	SPADE = "Spade",
	BASTONI = "Bastoni",
}
local function is_seed(seed)
	for _, value in pairs(Eseed) do
		if value == seed then
			return true
		end
	end
	return false
end

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
	if not is_seed(seed) then
		error(tostring(seed) .. " is not an allowed seed")
	end
	local newseed = setmetatable({}, self)
	newseed.seed = seed
	return newseed
end

function SeedSet:__tostring()
	return self.seed .. "\n values: " .. table.concat(self.values, " - ")
end

return { SeedSet = SeedSet, Eseed = Eseed, is_seed = is_seed }
