local Logger = require("logger")

local Verbosity = {
	LOWEST = "z",
	LOW = "l",
	MEDIUM = "m",
	HIGH = "h",
}
local gamelogger = Logger:new()

local Game = { deck = nil, stack = {}, logger = gamelogger }

function Game:attachdeck(deck)
	self.deck = deck
end

function Game.match(head, tail)
	return head.seed == tail.seed or head.value == tail.value
end

function Game:checkfrom(pivot)
	local stack = self.stack
	if #stack < 3 or pivot < 3 then
		return
	end

	if not Game.match(stack[pivot], stack[pivot - 2]) then
		return
	else
		table.remove(stack, pivot - 2)
		pivot = pivot - 1

		Game:checkfrom(pivot - 1)
		Game:check()
	end
end

function Game:check()
	Game:checkfrom(#self.stack)
end

function Game:draw()
	if self.deck == nil then
		error("no deck associated with game, call Game:attachdeck() to attach")
	end
	table.insert(self.stack, self.deck:draw())
end

function Game.drawmul(n)
	for _ = 1, n do
		Game:draw()
	end
end

function Game:won()
	return #self.stack == 2
end

function Game:stacktostring()
	local values = {}

	for _, value in ipairs(self.stack) do
		values[#values + 1] = tostring(value)
	end

	return "{ " .. table.concat(values, ", ") .. " }"
end

function Game:stackcompactstr()
	local values = {}

	for _, value in ipairs(self.stack) do
		values[#values + 1] = value:compactstr()
	end

	return "{ " .. table.concat(values, ", ") .. " }"
end

function Game:play()
	if self.deck == nil then
		error("no deck associated with game, call Game:attachdeck() to attach")
	end
	if #self.stack > 0 then
		error("restart game with Game:restart() before playing another round")
	end
	Game.drawmul(2)
	local i = 1

	repeat
		Game:draw()

		self.logger:log(Verbosity.HIGH, string.format("> drawing: %s\n", self:stackcompactstr()))

		local cardcount = #self.stack
		Game:check()

		if #self.stack < cardcount then
			self.logger:log(Verbosity.HIGH, string.format("> matching: %s\n", self:stackcompactstr()))
		end
		self.logger:log(Verbosity.HIGH, string.format("> cards left: %d\n\n", self.deck.maxcards))

		i = i + 1
	until self.deck.maxcards == 0

	self.logger:log({ Verbosity.MEDIUM, Verbosity.HIGH }, "> final round: " .. Game:stacktostring() .. "\n")
	self.logger:log(Verbosity.LOW, "> " .. Game:stackcompactstr() .. "\n")

	if Game:won() then
		self.logger:log({}, "> You won!\n")
	else
		self.logger:log({}, "> You lost...\n")
	end
end

function Game:restart()
	self.deck:regenerate()
	self.stack = {}
end

return { Game = Game, Verbosity = Verbosity }
