local Deck = require("deck")
local Game = require("game")

local seedtype = require("seedtype")
local itseedtype = seedtype.Italian

local itdeck = Deck:new(itseedtype)
Game:attachdeck(itdeck)

local Simulation = { counter = 0 }

function Simulation.start()
	local start = os.clock()

	print("starting simulation...")
	local win = false
	repeat
		Game:playnoprint()
		win = Game:won()
		Game:restart()
		Simulation.counter = Simulation.counter + 1
	until win
	local elapsed = os.clock() - start
	print(string.format("achieved victory after %d rounds (%.4f seconds)", Simulation.counter, elapsed))
end

Simulation.start()
