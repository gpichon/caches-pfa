
--[[Read the file test.txt]]
function reading(file)
c = 0
trace = io.open(file, "r")      
for c in trace:lines(0) do
    line = trace:read("*l")
    print(line)
    end
trace:close()

end

--[[Call the function reading on 1st argument]]