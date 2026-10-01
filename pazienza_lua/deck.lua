local seed = require("seed")
local SeedSet = seed.SeedSet

function table.len(table)
	local count = 0
	for _ in pairs(table) do
		count = count + 1
	end
	return count
end

local Deck = {}
Deck.__index = Deck

function Deck:new(seedtype)
	local deck = setmetatable({}, self)
	deck.seeds = seedtype
	deck.maxcards = table.len(seedtype) * seedtype.maxvalue
	for _, value in pairs(seedtype) do
		deck[value] = SeedSet:new(value, seedtype.maxvalue)
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
