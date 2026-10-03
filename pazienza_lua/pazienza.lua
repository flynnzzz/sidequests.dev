local USAGE = [[
Usage:
  lua pazienza.lua <ACTION> [OPTIONS]

Actions:
  play              Play one game
  simulate          Play games until victory
  average           Calculate the average win rate

Options:
  -z                Set verbosity to LOWEST
  -l                Set verbosity to LOW
  -m                Set verbosity to MEDIUM
  -h                Set verbosity to HIGH
]]

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

	local tracker = ""
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
			tracker = tracker .. string.format("%d. %s\n", self.counter, gameresult)
		end

	until won

	if self.verbosity == Verbosity.MEDIUM then
		print(tracker)
	end

	local elapsed = os.clock() - start

	if self.verbosity ~= Verbosity.LOWEST then
		print(
			string.format(
				"> achieved victory after %d rounds (%.4f seconds)\n> winning piles: %s",
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
	print("> running " .. maxiteration .. " simulation loops...")

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
			"\n> time elapsed: %.4f\n> average winchance: %.8f %%\n> average # of games to win: %.4f",
			elapsed,
			1 / average * 100,
			average
		)
	)

	return average
end

local actions = {
	play = function()
		Game:play()
		if Game.verbosity == Verbosity.MEDIUM or Game.verbosity == Verbosity.HIGH then
			print("> final round: " .. Game:stacktostring())
		elseif Game.verbosity == Verbosity.LOW then
			print("> " .. Game:stackcompactstr())
		end
		if Game:won() then
			print("> You won!")
		else
			print("> You lost...")
		end
	end,

	simulate = function()
		local count = Simulation:start()
		if Simulation.verbosity == Verbosity.LOWEST then
			print("> victory after: " .. count .. " rounds")
		end
	end,

	average = function()
		Simulation:calcaverage(20)
	end,
}

local flags = "-zlmh"

local function isallowed(flag)
	if flag == nil or string.len(flag) ~= 2 then
		return false
	end

	local prefix = flag:sub(1, 1)
	local content = flag:sub(2, 2)

	return prefix == "-" and string.find(flags, prefix, 1, true) ~= nil and string.find(flags, content, 1, true) ~= nil
end

function Main()
	local action = arg[1]
	local flag = arg[2]

	if isallowed(flag) then
		local verbosity = flag:sub(2, 2)
		Simulation:setverbosity(verbosity)
	end

	local runnable = actions[action]
	if runnable ~= nil then
		runnable()
	else
		print(USAGE)
		return
	end
end

Main()
