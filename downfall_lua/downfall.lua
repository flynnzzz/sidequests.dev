MAXINT = 2 ^ 50
USAGE =
	"usage:\n lua downfall.lua <A> [B]\n\n  A = first player's starting points\n  B = second player's starting points\n\n if only A is given then B will be set to the same value"

local Side = {
	HEADS = 0,
	TAILS = 1,
}

local function winchance(k, n)
	return (k / n) * 100
end

local function play(A, B)
	local total = A + B
	local cointoss
	local percentage
	local over = A == 0 or B == 0
	local result = "NA"
	local winner = "no one"

	local i = 0
	while i < MAXINT and not over do
		cointoss = math.random(0, 1)
		percentage = winchance(A, total)

		if cointoss == Side.HEADS then
			A = A + 1
			B = B - 1
			result = "Heads"
			winner = "A"
		else
			B = B + 1
			A = A - 1
			result = "Tails"
			winner = "B"
		end
		i = i + 1

		over = A == 0 or B == 0

		print(
			string.format(
				"round %d: %s, %s takes\n winchance: (A) %.1f%%  vs  (B) %.1f%%\n",
				i,
				result,
				winner,
				percentage,
				100 - percentage
			)
		)
	end

	if over and i ~= 0 then
		if A > B then
			winner = "A"
		elseif A < B then
			winner = "B"
		end

		print("Player " .. winner .. " wins after " .. i .. " rounds")
	elseif i > 0 then
		print("A winner could not be determined")
	else
		print("Rounds couldn't be started, there is no winner")
	end
end

function Main()
	local n

	if arg[1] == nil then
		print(USAGE)
		return
	end

	n = tonumber(arg[1])

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
