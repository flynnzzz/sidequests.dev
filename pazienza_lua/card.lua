local S = require("seed")
local SeedSet = S.SeedSet

local Card = {}
Card.__index = Card

function Card:of(seed, value)
	local card = setmetatable({}, self)

	local maxvalue = SeedSet.maxvalue
	if value > maxvalue then
		error(tostring(value) .. " exceeds the maximum allowed value of " .. tostring(maxvalue))
	end

	if value <= 0 then
		error("card value not allowed: " .. tostring(value))
	end

	card.seed = seed
	card.value = value
	return card
end

function Card:__tostring()
	return self.value .. " of " .. self.seed
end

return { Card = Card }
