local USAGE = [[
Usage:
  lua pazienza.lua <ACTION> <SEEDTYPE> [OPTIONS]

Actions:
  play              Play one game
  simulate          Play games until victory
  average           Calculate the average win rate

Seedtypes:
  italian           Denare, Coppe, Spade, Bastoni
  poker             Hearts, Diamonds, Clubs, Spades
  mahjong           Dots, Bamboo, Characters, Winds, Dragon

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
local Logger = require("logger")
local G = require("game")

local Game = G.Game
local Verbosity = G.Verbosity

local SeedType = require("seedtype")
local seedtype = SeedType.Mahjong

local deck = Deck:new(seedtype)
Game:attachdeck(deck)

local simulation_logger = Logger:new()
local Simulation = {
	game = Game,
	counter = 0,
	logger = simulation_logger,
}

function Simulation:start()
	local won = false
	local winningpiles

	local start = os.clock()
	self.logger:log(Verbosity.MEDIUM, "> starting simulation...\n")
	-- if verbosity == Verbosity.MEDIUM then
	-- 	print("starting simulation...")
	-- end

	repeat
		Game:play()
		won = Game:won()
		if won then
			winningpiles = Game:stacktostring()
		end
		self.logger:log(Verbosity.MEDIUM, string.format("%d. %s\n", self.counter + 1, Game:stackcompactstr()))

		Game:restart()

		self.counter = self.counter + 1

	-- if verbosity == Verbosity.MEDIUM then
	-- 	tracker = tracker .. string.format("%d. %s\n", self.counter, gameresult)
	-- end

	until won

	local elapsed = os.clock() - start

	self.logger:log(
		{ Verbosity.LOW, Verbosity.MEDIUM, Verbosity.HIGH },
		string.format(
			"> achieved victory after %d rounds (%.4f seconds)\n> winning piles: %s\n",
			self.counter,
			elapsed,
			winningpiles
		)
	)
	self.logger:log(Verbosity.LOWEST, "> victory after: " .. self.counter .. " rounds\n")

	-- if verbosity == Verbosity.LOWEST then
	-- 	print("> victory after: " .. count .. " rounds")
	-- end

	-- if verbosity ~= Verbosity.LOWEST then
	-- 	print(
	-- 		string.format(
	-- 			"> achieved victory after %d rounds (%.4f seconds)\n> winning piles: %s",
	-- 			self.counter,
	-- 			elapsed,
	-- 			winningpiles
	-- 		)
	-- 	)
	-- end

	local counter = self.counter
	self.counter = 0

	return counter
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
			Game.logger:print(Verbosity.HIGH)
		else
			Game:play()
			Game.logger:print(verbosity)
		end
	end,

	simulate = function(verbosity)
		if verbosity == nil then
			Simulation:start()
			Simulation.logger:print(Verbosity.MEDIUM)
		else
			Simulation:start()
			if verbosity == Verbosity.HIGH then
				Game.logger:print(Verbosity.HIGH)
			end
			Simulation.logger:print(verbosity)
			-- if verbosity == Verbosity.LOWEST then
			-- 	print("> victory after: " .. count .. " rounds")
			-- end
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
local VERBOSITY_FLAGS = {
	["-z"] = Verbosity.LOWEST,
	["-l"] = Verbosity.LOW,
	["-m"] = Verbosity.MEDIUM,
	["-h"] = Verbosity.HIGH,
}

local SEEDTYPES = {
	italian = SeedType.Italian,
	poker = SeedType.Poker,
	mahjong = SeedType.Mahjong,
}

local function parseargs(argv)
	local positionals = {}
	local verbosity

	for i = 1, #argv do
		local a = argv[i]
		if a:sub(1, 1) == "-" then
			local v = VERBOSITY_FLAGS[a]
			if v == nil then
				return nil, "unrecognized flag: '" .. a .. "'"
			end
			if verbosity ~= nil then
				return nil, "only one flag allowed"
			end
			verbosity = v
		else
			positionals[#positionals + 1] = a
		end
	end

	if #positionals < 1 then
		return nil, "no action specified"
	end
	if #positionals < 2 then
		return nil, "no seed type specified"
	end
	if #positionals > 2 then
		return nil, "unexpected argument: '" .. positionals[3] .. "'"
	end

	local action = actions[positionals[1]]
	if action == nil then
		return nil, "unknown action: '" .. positionals[1] .. "'"
	end

	local seed_t = SEEDTYPES[positionals[2]]
	if seed_t == nil then
		return nil, "unknown seed type: '" .. positionals[2] .. "'"
	end

	return { action = action, seedtype = seed_t, verbosity = verbosity }
end

function Main()
	local opts, err = parseargs(arg)
	if opts == nil then
		print("> " .. err)
		print(USAGE)
		return
	end

	Game:attachdeck(Deck:new(opts.seedtype))
	opts.action(opts.verbosity)
end

Main()
