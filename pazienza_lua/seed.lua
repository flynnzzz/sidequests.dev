local C = require("card")
local Card = C.card

local function cardset(seed, maxvalue)
	local values = {}
	for i = 1, maxvalue do
		values[i] = Card:new(seed, i)
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
	return self.seed .. "\n values: " .. table.concat(self.values, " - ")
end

return { SeedSet = SeedSet }
