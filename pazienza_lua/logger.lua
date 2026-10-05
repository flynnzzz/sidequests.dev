local Logger = { cachelog = "" }

function Logger.log(condition, msg)
	if condition then
		print(msg)
	end
end

function Logger:cache(condition, str)
	if condition then
		-- print("adding to cache: " .. str)
		self.cachelog = self.cachelog .. str
	end
end

function Logger:flush(condition)
	if condition then
		io.write(self.cachelog)
		self.cachelog = ""
	end
end

return Logger
