local S = require("seed")
local C = require("cards")
local D = require("deck")

local SeedSet = S.SeedSet
local Eseed = S.Eseed
local Card = C.Card
local Deck = D.Deck

print(SeedSet.maxvalue)
print(SeedSet:new(Eseed.DENARE))
print(tostring(table.concat(SeedSet.values, ", ")))
print(Card:of(Eseed.DENARE, 10))

local deck = Deck:new(Eseed)
print("Deck:\n" .. tostring(deck))
