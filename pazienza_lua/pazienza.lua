local Deck = require("deck")
local G = require("game")

local Game = G.Game
local Verbosity = G.Verbosity

local seedtype = require("seedtype")
local itseedtype = seedtype.Italian

local itdeck = Deck:new(itseedtype)
Game:attachdeck(itdeck)

local Simulation = {
	game = Game,
	counter = 0,
	verbosity = Verbosity.MEDIUM,
}

function Simulation:setverbosity(verbosity)
	self.verbosity = verbosity
	self.game.verbosity = verbosity
end

function Simulation:start()
	local won = false
	local winningpiles
	local gameresult

	local start = os.clock()
	if self.verbosity == Verbosity.MEDIUM then
		print("starting simulation...")
	end

	local s = ""
	repeat
		Game:play()
		won = Game:won()
		if won then
			winningpiles = Game:stacktostring()
		end

		if self.verbosity == Verbosity.MEDIUM then
			gameresult = Game:stackcompactstr()
		end
		Game:restart()

		self.counter = self.counter + 1

		if self.verbosity == Verbosity.MEDIUM then
			s = s .. string.format("%d. %s\n", self.counter, gameresult)
		end

	until won

	if self.verbosity == Verbosity.MEDIUM then
		print(s)
	end

	local elapsed = os.clock() - start
	if self.verbosity ~= Verbosity.ZERO then
		print(
			string.format(
				"achieved victory after %d rounds (%.4f seconds)\nwinning piles: %s",
				self.counter,
				elapsed,
				winningpiles
			)
		)
	end

	local counter = self.counter
	self.counter = 0

	return counter
end

function Simulation:calcaverage(maxiteration)
	local origverb = self.verbosity
	self:setverbosity(Verbosity.ZERO)

	local average
	local totalgames = 0

	local start = os.clock()
	for _ = 1, maxiteration do
		local gamesplayed = self:start()
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

	self:setverbosity(origverb)

	return average
end

Simulation:setverbosity(Verbosity.MEDIUM)
Simulation:start()
