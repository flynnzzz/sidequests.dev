local Deck = require("deck")
local Game = require("game")

local seedtype = require("seedtype")
local itseedtype = seedtype.Italian

local itdeck = Deck:new(itseedtype)
Game:attachdeck(itdeck)

local Simulation = { counter = 0 }

function Simulation.start()
	local win = false

	local start = os.clock()
	print("starting simulation...")
	repeat
		Game:playnoprint()
		win = Game:won()
		Game:restart()
		Simulation.counter = Simulation.counter + 1
	until win

	local elapsed = os.clock() - start
	print(
		string.format(
			" achieved victory after %d rounds (%.4f seconds)\nwinning piles: %s",
			Simulation.counter,
			elapsed,
			Game:stacktostring()
		)
	)
end

function Simulation.startdetailed()
	local won = false
	local winningpiles
	local start = os.clock()

	print("starting simulation...")
	repeat
		Game:playnoprint()
		won = Game:won()
		if won then
			winningpiles = Game:stacktostring()
		end
		print(string.format("%d. %s", Simulation.counter, Game:stackcompactstr()))
		Game:restart()
		if not won then
			Simulation.counter = Simulation.counter + 1
		end
	until won

	local elapsed = os.clock() - start
	print(
		string.format(
			"\nachieved victory after %d rounds (%.4f seconds)\nwinning piles: %s",
			Simulation.counter,
			elapsed,
			winningpiles
		)
	)
end

Simulation.start()
