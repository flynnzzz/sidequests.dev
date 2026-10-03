local Card = require("card")

local function cardset(seed, maxvalue)
	local values = {}
	for i = 1, maxvalue do
		values[i] = Card:of(seed, i)
	end
	return values
end

local SeedSet = {}
SeedSet.__index = SeedSet

function SeedSet:new(seed, maxvalue)
	local newseed = setmetatable({}, self)
	newseed.seed = seed
	newseed.maxvalue = maxvalue
	newseed.values = cardset(seed, maxvalue)
	return newseed
end

function SeedSet:__tostring()
	local ivalues = {}
	for i, _ in ipairs(self.values) do
		ivalues[i] = i
	end

	return self.seed .. "\n values: " .. table.concat(ivalues, " - ")
end

return SeedSet
