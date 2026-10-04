local Logger = {}
Logger.__index = Logger

function Logger:new()
	return setmetatable({ logs = {} }, Logger)
end

function Logger:log(verbosity, msg)
	local log
	local v
	if type(verbosity) == "table" then
		v = verbosity
	else
		v = { verbosity }
	end
	log = { verbosity = v, msg = msg }
	table.insert(self.logs, log)
end

-- by convention, an empty table means 'all'
local function contains(verbosities, element)
	if #verbosities == 0 then
		return true
	end
	for _, value in ipairs(verbosities) do
		if value == element then
			return true
		end
	end

	return false
end

function Logger:print(verbosity)
	local str = ""
	for _, entry in pairs(self.logs) do
		if contains(entry.verbosity, verbosity) then
			str = str .. entry.msg
		end
	end
	io.write(str)
end

function Logger:printlast(n, verbosity)
	local matching = {}

	for _, entry in ipairs(self.logs) do
		if contains(entry.verbosity, verbosity) then
			matching[#matching + 1] = entry.msg
		end
	end

	local first = math.max(1, #matching - n + 1)
	local str = ""

	for i = first, #matching do
		str = str .. matching[i]
	end

	io.write(str)
end

return Logger
