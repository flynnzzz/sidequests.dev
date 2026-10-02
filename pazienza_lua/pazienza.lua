local S = require("seed")
local C = require("card")
local D = require("deck")
local E = require("seedtype")

local SeedSet = S.SeedSet
local Eseed = E.Italian
local Card = C.Card
local Deck = D.Deck

-- local denare = SeedSet:new(Eseed.DENARE, Eseed.maxvalue)
-- print(denare)
-- print(Card:of(Eseed.DENARE, 10))

local italiane = Deck:new(Eseed)
-- print("Deck:\n" .. tostring(italiane))
-- print(italiane.maxcards)

local function printstack(s)
	for _, value in pairs(s) do
		print(value)
	end
end

local gamestack = {
	Card:of(Eseed.DENARE, 3),
	Card:of(Eseed.COPPE, 9),
	Card:of(Eseed.SPADE, 1),
	Card:of(Eseed.DENARE, 2),
	Card:of(Eseed.SPADE, 9),
}

printstack(gamestack)

function MATCH(pivot, tail)
	return pivot.seed == tail.seed or pivot.value == tail.value
end

function CHECK(stack, pivot)
	if #stack <= 2 then
		return
	end

	if not MATCH(stack[pivot], stack[pivot - 2]) then
		return
	else
		table.remove(stack, pivot)
		CHECK(stack, pivot - 1)
		CHECK(stack, pivot)
	end
end

CHECK(gamestack, 5)

printstack(gamestack)
