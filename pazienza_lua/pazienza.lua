local Deck = require("deck")
local Game = require("game")

local seedtype = require("seedtype")
local itseedtype = seedtype.Italian

local itdeck = Deck:new(itseedtype)
Game:attachdeck(itdeck)

local Simulation = { counter = 0 }

function Simulation.start()
	print("starting simulation...")
	local win = false
	repeat
		Game:play()
		win = Game:won()
		Game:restart()
		Simulation.counter = Simulation.counter + 1
	until win
	print("achieved victory after " .. Simulation.counter .. " rounds")
end

Simulation.start()
