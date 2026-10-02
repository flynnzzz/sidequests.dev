local D = require("deck")
local E = require("seedtype")
local Game = require("game")

local ItalianSeeds = E.Italian
local Deck = D.Deck

local italiane = Deck:new(ItalianSeeds)
Game:attachdeck(italiane)

Game:play()
