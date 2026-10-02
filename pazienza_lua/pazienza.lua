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

function RemoveMatch(stack, pivot)
	table.remove(stack, pivot - 2)

	return pivot - 1
end

function MATCH(head, tail)
	print("matching: " .. tostring(head) .. " - " .. tostring(tail))
	return head.seed == tail.seed or head.value == tail.value
end

function CHECK(stack, pivot)
	if #stack < 3 or pivot < 3 then
		return
	end

	if not MATCH(stack[pivot], stack[pivot - 2]) then
		return
	else
		pivot = RemoveMatch(stack, pivot)

		CHECK(stack, pivot - 1)
		CHECK(stack, #stack)
	end
end

CHECK(gamestack, 5)

print()

printstack(gamestack)
