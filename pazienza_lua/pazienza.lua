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

	local counter = self.counter
	self.counter = 0

	return counter
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

	local counter = self.counter
	self.counter = 0

	return counter
end

function Simulation:startnoprint()
	local won = false
	repeat
		Game:playnoprint()
		won = Game:won()
		Game:restart()
		if not won then
			self.counter = self.counter + 1
		end
	until won

	local counter = self.counter
	self.counter = 0

	return counter
end

function Simulation:calcaverage(maxiteration)
	local average
	local totalgames = 0
	local start = os.clock()
	for _ = 1, maxiteration do
		local gamesplayed = self:startnoprint()
		totalgames = totalgames + gamesplayed
	end
	local elapsed = os.clock() - start

	average = totalgames / maxiteration
	print(
		string.format(
			"time elapsed: %.4f\naverage winchance: %.8f %%\naverage # of games to win: %.4f",
			elapsed,
			1 / average * 100,
			average
		)
	)

	return average
end

print("single simulation:\n")
Simulation:start()

print("\naverage:\n")
Simulation:calcaverage(999)
