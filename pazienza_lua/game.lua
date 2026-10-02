local Game = { deck = nil, stack = {} }

local function printstack(stack)
	local values = {}

	for _, value in ipairs(stack) do
		values[#values + 1] = value:compactstr()
	end

	print("[ " .. table.concat(values, ", ") .. " ]")
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

function Game:play()
	if self.deck == nil then
		error("no deck associated with game, call Game:attachdeck() to attach")
	end
	Game.drawmul(2)
	local i = 1

	repeat
		Game:draw()
		-- printstack(Game.stack)
		Game:check()
		-- printstack(Game.stack)
		i = i + 1

	until self.deck.maxcards == 0

	print("result: ")
	printstack(self.stack)

	if Game:won() then
		print("WIN")
	else
		print("LOSE")
	end
end

function Game:restart()
	self.deck:regenerate()
	self.stack = {}
end

return Game
