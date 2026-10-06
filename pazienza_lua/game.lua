local Logger = require("logger")

local Verbosity = {
	LOWEST = "z",
	LOW = "l",
	MEDIUM = "m",
	HIGH = "h",
}

local Game = { deck = nil, stack = {} }

function Game:attachdeck(deck)
	self.deck = deck
end

function Game.match(head, tail)
	return head.seed == tail.seed or head.value == tail.value
end

function Game:checkfrom(pivot, last)
	local stack = self.stack

	if #stack < 3 or last < 3 then
		return
	end

	if not self.match(stack[last], stack[last - 2]) then
		return
	else
		table.remove(stack, last - 2)
		pivot.value = pivot.value - 1

		Game:checkfrom(pivot, pivot.value - 1)
		Game:checkfrom(pivot, pivot.value)
	end
end

function Game:check()
	local pivot = { value = #self.stack }
	Game:checkfrom(pivot, pivot.value)
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

function Game:playwith(verbosity)
	if self.deck == nil then
		error("> no deck associated with game, call Game:attachdeck() to attach")
	end
	if #self.stack > 0 then
		error("> restart game with Game:restart() before playing another round")
	end
	Game.drawmul(2)
	local i = 1

	repeat
		Game:draw()

		if verbosity == Verbosity.HIGH then
			Logger:cache(string.format("> drawing: %s\n", self:stackcompactstr()))
		end

		local cardcount = #self.stack
		Game:check()

		if verbosity == Verbosity.HIGH then
			if #self.stack < cardcount then
				Logger:cache(string.format(" > matching: %s\n", self:stackcompactstr()))
			end
			Logger:cache(string.format("> cards left: %d\n\n", self.deck.maxcards))
		end

		i = i + 1
	until self.deck.maxcards == 0
	Logger:flush(verbosity == Verbosity.HIGH)
end

function Game:play()
	self:playwith(Verbosity.HIGH)
end

function Game:restart()
	self.deck:regenerate()
	self.stack = {}
end

return { Game = Game, Verbosity = Verbosity }
