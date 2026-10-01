local S = require("seed")
local C = require("card")
local D = require("deck")
local E = require("seedtype")

local SeedSet = S.SeedSet
local Eseed = E.Italian
local Card = C.Card
local Deck = D.Deck

print(SeedSet.maxvalue)

local denare = SeedSet:new(Eseed.DENARE, Eseed.maxvalue)
print(denare)
print(Card:of(Eseed.DENARE, 10))

local deck = Deck:new(Eseed)
print("Deck:\n" .. tostring(deck))
print(deck.maxcards)
