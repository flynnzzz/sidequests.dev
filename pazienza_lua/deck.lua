local seed = require("seed")
local SeedSet = seed.SeedSet

local Deck = {}
Deck.__index = Deck

function Deck:new(seeds)
	local deck = setmetatable({}, self)
	deck.seeds = seeds
	for _, value in pairs(seeds) do
		deck[value] = SeedSet:new(value)
	end

	return deck
end

function Deck:__tostring()
	local s = ""
	for _, v in pairs(self.seeds) do
		s = s .. tostring(self[v]) .. "\n"
	end
	return s
end

return { Deck = Deck }
