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

local function contains(list, element)
	for _, value in ipairs(list) do
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

return Logger
