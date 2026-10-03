local SeedSet = require("seedset")

function table.len(table)
	local count = 0
	for _ in pairs(table) do
		count = count + 1
	end
	return count
end

local Deck = {}
Deck.__index = Deck

local function generate(deck)
	local stack = {}

	for _, s in pairs(deck.seedtype) do
		for _, v in pairs(deck[s].values) do
			table.insert(stack, v)
		end
	end

	return stack
end

function Deck:new(seedtype)
	local deck = setmetatable({}, self)
	deck.seedtype = seedtype
	deck.maxcards = table.len(seedtype) * seedtype.maxvalue
	for _, value in pairs(seedtype) do
		deck[value] = SeedSet:new(value, seedtype.maxvalue)
	end
	deck.cards = generate(deck)

	return deck
end

function Deck:regenerate()
	local new = generate(self)
	self.maxcards = table.len(self.seedtype) * self.seedtype.maxvalue
	self.cards = new
	return new
end

function Deck:draw()
	local i = math.random(1, self.maxcards)
	self.maxcards = self.maxcards - 1
	local drawn = table.remove(self.cards, i)
	return drawn
end

function Deck:select(i)
	if i > self.maxcards then
		error("maxcards exceeded: " .. i)
	end
	if i <= 0 then
		error("card value not allowed: " .. tostring(i))
	end

	self.maxcards = self.maxcards - 1
	local drawn = table.remove(self.cards, i)
	return drawn
end

function Deck:__tostring()
	local s = ""
	for _, v in pairs(self.seedtype) do
		s = s .. tostring(self[v]) .. "\n"
	end
	return s
end

return Deck
