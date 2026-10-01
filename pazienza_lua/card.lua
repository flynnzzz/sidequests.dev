Seed = {
	DENARE = "Denare",
	COPPE = "Coppe",
	SPADE = "Spade",
	BASTONI = "Bastoni",
}

function Seed.is_seed(seed)
	for _, value in pairs(Seed) do
		if value == seed then
			return true
		end
	end
	return false
end

local function rangeto(n)
	local values = {}
	for i = 1, n do
		values[i] = i
	end
	return values
end

Values = rangeto(10)

local Card = {}
Card.__index = Card

function Card:of(seed, value)
	local card = setmetatable({}, self)

	if not Seed.is_seed(seed) then
		error(tostring(seed) .. " is not an allowed seed")
	end

	local maxvalue = math.max(table.unpack(Values))
	if value > maxvalue then
		error(tostring(value) .. " exceeds the maximum allowed value of " .. tostring(maxvalue))
	end

	if value < 0 then
		error("negative values not allowed: " .. tostring(value))
	end

	card.seed = seed
	card.value = value
	return card
end
