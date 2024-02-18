function AddStuff(a,b)
	print("[LUA] AddStuff("..a..", "..b..") called \n")
	
	if (a >= b) then
		return a+b
	else
		return a-b
	end
	
	return 0
end


function DoAThing(a, b)
	print("[LUA] DoAThing("..a..", "..b..") called \n")

	c = HostFunction(a + 10, b * 3) -- вызов функции в хост программе С++

	return c
end