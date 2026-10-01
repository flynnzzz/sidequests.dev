local SEED = {
	DENARE = "Denare",
	COPPE = "Coppe",
	SPADE = "Spade",
	BASTONI = "Bastoni",
}

local Seed = {}
Seed.__index = Seed

function Seed:new(seed)
	local newseed = setmetatable({}, self)
	newseed.maxcards = 40

	-- TODO: complete

	return newseed
end

function Seed.is_seed(seed)
	for _, value in pairs(SEED) do
		if value == seed then
			return true
		end
	end
	return false
end

return { Seed = Seed, SEED = SEED }
