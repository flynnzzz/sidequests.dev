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

local function generate(deck, times)
	local stack = {}
	local n = times or 1

	for _ = 1, n do
		for _, s in pairs(deck.seedtype) do
			for _, v in pairs(deck[s].values) do
				table.insert(stack, v)
			end
		end
		for _, entry in pairs(deck.seedtype.unnumbered or {}) do
			for _, v in ipairs(deck[entry.name].values) do
				table.insert(stack, v)
			end
		end
	end

	return stack
end

function Deck:printcards()
	for _ = 1, self.seedtype.times or 1 do
		for _, s in pairs(self.seedtype) do
			for _, v in pairs(self[s].values) do
				print(tostring(v))
			end
		end
		for _, entry in pairs(self.seedtype.unnumbered or {}) do
			for _, v in ipairs(self[entry.name].values) do
				print(tostring(v))
			end
		end
	end
end

function Deck:new(seedtype, multiplicity)
	local deck = setmetatable({}, self)
	local mul = multiplicity or 1
	local times = seedtype.times or 1

	deck.seedtype = seedtype
	deck.multiplicity = mul * times

	for _, value in pairs(seedtype) do
		deck[value] = SeedSet:new(value, seedtype.maxvalue)
	end

	local unnumbered = seedtype.unnumbered or {}
	for _, entry in pairs(unnumbered) do
		local namelist = entry.set
		deck[entry.name] = SeedSet:new(entry.name, #namelist, namelist)
	end

	deck.cards = generate(deck, deck.multiplicity)
	deck.maxcards = #deck.cards

	return deck
end

function Deck:regenerate()
	self.cards = generate(self, self.multiplicity)
	self.maxcards = #self.cards
	return self.cards
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
