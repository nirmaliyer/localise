local luann = require("luann")
math.randomseed(89890)

local learningRate = 0.15 -- set between 0, 1
local momentum = 0.1 -- set between 0 and 1
local attempts = 10000 -- number of times to do backpropagation
local actFun = "TANH"

--create a network with 2 inputs, 3 hidden cells, and 1 output
myNetwork = luann:new({2, 4, 1}, learningRate, momentum, actFun)

xorI = {{0, 0}, {0, 1}, {1, 0}, {1, 1}}
xorO = {{0}, {1}, {1}, {0}}
--run backpropagation (bp)
for i = 1,attempts do

	--myNetwork:printnn()

	R = math.fmod(i - 1, 4) + 1
	--print("IDX " .. R)
	--R = math.random(4)

	--for i, v in ipairs(xorI[R]) do
	--	print("Input " .. i .. " : " .. v)
	--end

	--for i, v in ipairs(xorO[R]) do
	--	print("Expected Output " .. i .. " : " .. v)
	--end

	myNetwork:bp(xorI[R], xorO[R])

	print("Error " .. myNetwork.netError)

end

--print the signal of the single output cell when :activated with different inputs
print("Results:")
myNetwork:activate({0,0})
print("0 0 | " .. myNetwork[3].cells[1].signal)
myNetwork:activate({0,1})
print("0 1 | " .. myNetwork[3].cells[1].signal)
myNetwork:activate({1,0})
print("1 0 | " .. myNetwork[3].cells[1].signal)
myNetwork:activate({1,1})
print("1 1 | " .. myNetwork[3].cells[1].signal)

--[[
--Save the network to a file
luann:saveNetwork(myNetwork, "myTestNetwork.network")

--Load the network from a file
newNetwork = luann:loadNetwork("myTestNetwork.network")

--run the loaded network
print("Results:")
newNetwork:activate({0,0})
print("Output of 0,0: " .. newNetwork[3].cells[1].signal)
newNetwork:activate({0,1})
print("Output of 0,1: " .. newNetwork[3].cells[1].signal)
newNetwork:activate({1,0})
print("Output of 1,0: " .. newNetwork[3].cells[1].signal)
newNetwork:activate({1,1})
print("Output of 1,1: " .. newNetwork[3].cells[1].signal)
]]