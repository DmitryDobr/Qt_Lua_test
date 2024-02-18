
players = {}
players[0] = {Name = "Ciaran", Class = "Warrior", Level = 20}
players[1] = {Name = "Diego", Class = "Mag", Level = 50}
-- Это таблица!! ключи -> значения 

function tablelength(T)
	local count = 0
	for _ in pairs(T) do count = count + 1 end
	return count
end

function getPlayer(n)
	-- print(tablelength(players))
	
	if (tablelength(players) >= n) then
		return players[n]
	else
		return 0
	end
end
