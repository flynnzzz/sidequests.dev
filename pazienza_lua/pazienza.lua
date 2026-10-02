local Deck = require("deck")
local Game = require("game")

local seedtype = require("seedtype")
local itseedtype = seedtype.Italian

local itdeck = Deck:new(itseedtype)
Game:attachdeck(itdeck)

local Simulation = { counter = 0 }

function Simulation:start()
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
		Game:restart()
		if not won then
			self.counter = self.counter + 1
		end
	until won

	local elapsed = os.clock() - start
	print(
		string.format(
			" achieved victory after %d rounds (%.4f seconds)\nwinning piles: %s",
			self.counter,
			elapsed,
			winningpiles
		)
	)
end

function Simulation:startdetailed()
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
		print(string.format("%d. %s", self.counter, Game:stackcompactstr()))
		Game:restart()
		if not won then
			self.counter = self.counter + 1
		end
	until won

	local elapsed = os.clock() - start
	print(
		string.format(
			"\nachieved victory after %d rounds (%.4f seconds)\nwinning piles: %s",
			self.counter,
			elapsed,
			winningpiles
		)
	)
end

Simulation:startdetailed()
