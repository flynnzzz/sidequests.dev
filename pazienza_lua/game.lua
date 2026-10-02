local Game = { deck = nil, stack = {} }

local function stackcompactstr(stack)
	local values = {}

	for _, value in ipairs(stack) do
		values[#values + 1] = value:compactstr()
	end

	return "{ " .. table.concat(values, ", ") .. " }"
end

local function stacktostring(stack)
	local values = {}

	for _, value in ipairs(stack) do
		values[#values + 1] = tostring(value)
	end

	return "{ " .. table.concat(values, ", ") .. " }"
end

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
	return stacktostring(self.stack)
end

function Game:stackcompactstr()
	return stackcompactstr(self.stack)
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
		print(string.format("drawing: %s", stackcompactstr(Game.stack)))
		Game:check()
		print(string.format(" matching: %s\ncards left: %d\n", stackcompactstr(Game.stack), self.deck.maxcards))
		i = i + 1

	until self.deck.maxcards == 0

	print("result: " .. stackcompactstr(self.stack))

	if Game:won() then
		print("WIN")
	else
		print("LOSE")
	end
end

function Game:playnoprint()
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
		Game:check()
		i = i + 1

	until self.deck.maxcards == 0
end

function Game:restart()
	self.deck:regenerate()
	self.stack = {}
end

return Game
