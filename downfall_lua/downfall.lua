MAXINT = 2 ^ 50

local Side = {
	HEADS = 0,
	TAILS = 1,
}

local function winchance(k, n)
	return (k / n) * 100
end

local function play(A, B)
	local total = A + B
	local over = A == 0 or B == 0
	local result = "None"

	local i = 0
	while i < MAXINT and not over do
		local cointoss = math.random(0, 1)

		if cointoss == Side.HEADS then
			B = B + 1
			A = A - 1
			result = "Heads"
		else
			A = A + 1
			B = B - 1
			result = "Tails"
		end
		i = i + 1

		over = A == 0 or B == 0

		print(string.format("round %d: %s - A's winchance: %.1f%%", i, result, winchance(A, total)))
	end

	if over and i ~= 0 then
		local winner
		if A > B then
			winner = "A"
		elseif A < B then
			winner = "B"
		end

		print("Player " .. winner .. " wins")
	elseif i == MAXINT then
		print("A winner could not be determined")
	else
		print("There was no winner")
	end
end

function Main()
	local n = tonumber(0)

	if arg[1] ~= nil then
		n = tonumber(arg[1])
	end

	local m
	if arg[2] ~= nil then
		m = tonumber(arg[2])
	else
		m = n
	end

	print("Starting rounds\n A: " .. n .. " pts\n B: " .. m .. " pts")

	play(n, m)
end

Main()
