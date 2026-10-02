local Card = {}
Card.__index = Card

function Card:of(seed, value)
	local card = setmetatable({}, self)

	if value <= 0 then
		error("card value not allowed: " .. tostring(value))
	end

	card.seed = seed
	card.value = value
	return card
end

function Card:__tostring()
	return self.value .. " of " .. self.seed
end

function Card:compactstr()
	local pref = math.min(3, #self.seed)
	return self.value .. " " .. string.sub(self.seed, 1, pref)
end

return Card
