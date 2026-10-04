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

Default verbosity values:
  play: HIGH
  simulate: MEDIUM
  average: LOWEST
]]

local Deck = require("deck")
local G = require("game")

local Game = G.Game
local Verbosity = G.Verbosity

local SeedType = require("seedtype")
local seedtype = SeedType.Mahjong

local deck = Deck:new(seedtype)
Game:attachdeck(deck)

local Simulation = {
	game = Game,
	counter = 0,
}

function Simulation:startwith(verbosity)
	local won = false
	local winningpiles
	local gameresult

	local start = os.clock()
	if verbosity == Verbosity.MEDIUM then
		print("starting simulation...")
	end

	local tracker = ""
	repeat
		Game:playwith(verbosity)
		won = Game:won()
		if won then
			winningpiles = Game:stacktostring()
		end

		if verbosity == Verbosity.MEDIUM then
			gameresult = Game:stackcompactstr()
		end
		Game:restart()

		self.counter = self.counter + 1

		if verbosity == Verbosity.MEDIUM then
			tracker = tracker .. string.format("%d. %s\n", self.counter, gameresult)
		end

	until won

	if verbosity == Verbosity.MEDIUM then
		print(tracker)
	end

	local elapsed = os.clock() - start

	if verbosity ~= Verbosity.LOWEST then
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

function Simulation:start()
	return self:startwith(Verbosity.MEDIUM)
end

function Simulation:calcaverage(maxiteration, verbosity)
	print("> running " .. maxiteration .. " simulation loops...")

	local average
	local totalgames = 0

	local start = os.clock()
	for _ = 1, maxiteration do
		local gamesplayed = self:startwith(verbosity)
		totalgames = totalgames + gamesplayed
	end
	local elapsed = os.clock() - start

	average = totalgames / maxiteration
	print(
		string.format(
			"\n> time elapsed: %.4f\n> average winchance: %.8f %%\n> average # of games to win: %.1f",
			elapsed,
			1 / average * 100,
			average
		)
	)

	return average
end

local actions = {
	play = function(verbosity)
		if verbosity == nil then
			Game:play()
		else
			Game:playwith(verbosity)
			if verbosity == Verbosity.MEDIUM or verbosity == Verbosity.HIGH then
				print("> final round: " .. Game:stacktostring())
			elseif verbosity == Verbosity.LOW then
				print("> " .. Game:stackcompactstr())
			end
		end
		if Game:won() then
			print("> You won!")
		else
			print("> You lost...")
		end
	end,

	simulate = function(verbosity)
		if verbosity == nil then
			Simulation:start()
		else
			local count = Simulation:startwith(verbosity)
			if verbosity == Verbosity.LOWEST then
				print("> victory after: " .. count .. " rounds")
			end
		end
	end,

	average = function(verbosity)
		if verbosity == nil then
			Simulation:calcaverage(256, Verbosity.LOWEST)
		else
			Simulation:calcaverage(256, verbosity)
		end
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
	local verbosity

	if isallowed(flag) then
		verbosity = flag:sub(2, 2)
	elseif flag ~= nil then
		print("> unrecognized flag: '" .. flag .. "'")
		return
	end

	local runnable = actions[action]
	if runnable ~= nil then
		runnable(verbosity)
	else
		print(USAGE)
		return
	end
end

Main()
