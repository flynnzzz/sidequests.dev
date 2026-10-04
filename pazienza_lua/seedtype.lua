local SeedType = {}
SeedType.__index = SeedType
SeedType.__pairs = function(self)
	local key
	local value

	return function()
		repeat
			key, value = next(self, key)
		until key == nil or (type(value) ~= "function" and type(value) ~= "number")

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
	maxvalue = 10,
}, SeedType)

local Poker = setmetatable({
	HEARTS = "Hearts",
	DIAMONDS = "Diamonds",
	CLUBS = "Clubs",
	SPADES = "Spades",
	maxvalue = 12,
}, SeedType)

--  numbered suits only
local Mahjong = setmetatable({
	DOTS = "Dots",
	BAMBOO = "Bamboo",
	CHARACTERS = "Characters",
	maxvalue = 36,
}, SeedType)

return { Italian = Italian, Poker = Poker, Mahjong = Mahjong }
