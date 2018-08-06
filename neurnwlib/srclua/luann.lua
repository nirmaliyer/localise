--[[
The MIT License (MIT)

Copyright (c) <2013> <Josh Rowe>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

]]--

--[[
Modified by Himangshu Saikia
saikia@kth.se, 2017
]]

-- parameters used
local ELU_alpha = 0.5

--Borrowed table persistence from http://lua-users.org/wiki/TablePersistence, MIT license.
--comments removed, condensed code to oneliners where possible.
local write, writeIndent, writers, refCount;
persistence =
{
	store = function (path, ...)
		local file, e = io.open(path, "w")
		if not file then return error(e)	end
		local n = select("#", ...)
		local objRefCount = {} -- Stores reference that will be exported
		for i = 1, n do refCount(objRefCount, (select(i,...))) end
		local objRefNames = {}
		local objRefIdx = 0;
		file:write("-- Persistent Data\n");
		file:write("local multiRefObjects = {\n");
		for obj, count in pairs(objRefCount) do
			if count > 1 then
				objRefIdx = objRefIdx + 1;
				objRefNames[obj] = objRefIdx;
				file:write("{};"); -- table objRefIdx
			end;
		end;
		file:write("\n} -- multiRefObjects\n");
		for obj, idx in pairs(objRefNames) do
			for k, v in pairs(obj) do
				file:write("multiRefObjects["..idx.."][");
				write(file, k, 0, objRefNames);
				file:write("] = ");
				write(file, v, 0, objRefNames);
				file:write(";\n");
			end;
		end;
		for i = 1, n do
			file:write("local ".."obj"..i.." = ");
			write(file, (select(i,...)), 0, objRefNames);
			file:write("\n");
		end
		if n > 0 then
			file:write("return obj1");
			for i = 2, n do
				file:write(" ,obj"..i);
			end;
			file:write("\n");
		else
			file:write("return\n");
		end;
		file:close();
	end;
	load = function (path)
		local f, e = loadfile(path);
		if f then
			return f();
		else
			return nil, e;
		end;
	end;
}
write = function (file, item, level, objRefNames)
	writers[type(item)](file, item, level, objRefNames);
end;
writeIndent = function (file, level)
	for i = 1, level do
		file:write("\t");
	end;
end;
refCount = function (objRefCount, item)
	if type(item) == "table" then
		if objRefCount[item] then
			objRefCount[item] = objRefCount[item] + 1;
		else
			objRefCount[item] = 1;
			for k, v in pairs(item) do
				refCount(objRefCount, k);
				refCount(objRefCount, v);
			end;
		end;
	end;
end;
writers = {
	["nil"] = function (file, item) file:write("nil") end;
	["number"] = function (file, item)
			file:write(tostring(item));
		end;
	["string"] = function (file, item)
			file:write(string.format("%q", item));
		end;
	["boolean"] = function (file, item)
			if item then
				file:write("true");
			else
				file:write("false");
			end
		end;
	["table"] = function (file, item, level, objRefNames)
			local refIdx = objRefNames[item];
			if refIdx then
				file:write("multiRefObjects["..refIdx.."]");
			else
				file:write("{\n");
				for k, v in pairs(item) do
					writeIndent(file, level+1);
					file:write("[");
					write(file, k, level+1, objRefNames);
					file:write("] = ");
					write(file, v, level+1, objRefNames);
					file:write(";\n");
				end
				writeIndent(file, level);
				file:write("}");
			end;
		end;
	["function"] = function (file, item)
			local dInfo = debug.getinfo(item, "uS");
			if dInfo.nups > 0 then
				file:write("nil --[[functions with upvalue not supported]]");
			elseif dInfo.what ~= "Lua" then
				file:write("nil --[[non-lua function not supported]]");
			else
				local r, s = pcall(string.dump,item);
				if r then
					file:write(string.format("loadstring(%q)", s));
				else
					file:write("nil --[[function could not be dumped]]");
				end
			end
		end;
	["thread"] = function (file, item)
			file:write("nil --[[thread]]\n");
		end;
	["userdata"] = function (file, item)
			file:write("nil --[[userdata]]\n");
		end;
}

local luann = {}
local Layer = {}
local Cell = {}
local exp = math.exp

local function actFun (func, x)
	if func == "SIGMOID" then
		return 1 / (1 + exp(-x))
	elseif func == "TANH" then
		return math.tanh(x)
	elseif func == "RELU" then
		return math.max(0, x)
	elseif func == "LEAKY_RELU" then
		return (x > 0) and x or (0.01 * x)
	elseif func == "ELU" then
		return (x > 0) and x or (ELU_alpha * (exp(x) - 1))
	else
		print("Activation function incorrect!")
		return 0
	end
end

local function actFunGradient (func, x)
	if func == "SIGMOID" then
		return exp(-x) / (1 + exp(-x))^2
	elseif func == "TANH" then
		return 1 - (math.tanh(x))^2
	elseif func == "RELU" then
		return (x > 0) and 1 or 0
	elseif func == "LEAKY_RELU" then
		return (x > 0) and 1 or 0.01
	elseif func == "ELU" then
		return (x > 0) and 1 or (ELU_alpha * exp(x))
	else
		print("Activation function incorrect!")
		return 0
	end
end


--We start by creating the cells.
--The cell has a structure containing weights that modify the input from the previous layer.
--Each cell also has a signal, or output.
function Cell:new(numOutputs)
	local cell = {gradient = 0, weights = {}, deltaWeights = {}, signal = 1}
		for i = 1, numOutputs do
			cell.weights[i] = math.random()
			cell.deltaWeights[i] = 0
		end
		setmetatable(cell, self)
		self.__index = self
	return cell
end

function Cell:activate(selfIndex, prevLayer, actFuncType) -- bias neuron included in inputs
	--print("Activating neuron with " .. #inputs .. " inputs.")
		local signalSum = 0
		local weights = self.weights
		for i = 1, #prevLayer do
			signalSum = signalSum + (prevLayer[i].weights[selfIndex] * prevLayer[i].signal)
		end
	self.signal = actFun(actFuncType, signalSum)
end

--Next we create a Layer of cells. The layer is a table of cells.
function Layer:new(numCells, numOutputs)
	numCells = numCells or 1
	numInputs = numInputs or 1
	local cells = {}
		for i = 1, numCells do cells[i] = Cell:new(numOutputs) end -- including bias
		local layer = {cells = cells}
		setmetatable(layer, self)
		self.__index = self
	return layer
end

--layers = {table of layer sizes from input to output}
function luann:new(layers, learningRate, momentum, actFuncType)
	local network = {netError = 0, learningRate = learningRate, momentum = momentum, actFuncType = actFuncType}
	for i = 1, #layers - 1 do
		network[i] = Layer:new(layers[i] + 1, layers[i + 1]) -- bias neuron points to everyone but doesn't have any inputs
	end

	network[#layers] = Layer:new(layers[#layers], 0)

	setmetatable(network, self)
	self.__index = self
	return network
end

function luann:printnn()
	print("****************")
	for i = 1, #self do
		print("Layer " .. i)
		local layer = self[i]
		for j = 1, #layer.cells do
			local neuron = layer.cells[j]
			print("Gradient Neuron " .. j .. " = " .. neuron.gradient )
			for k = 1, #neuron.weights do
				print("Weight "  .. k .. " = " .. neuron.weights[k] .. " Delta Weight = " .. neuron.deltaWeights[k])
			end
		end
	end
	print("****************")
end

function luann:activate(inputs)
	for i = 1, #inputs do
		self[1].cells[i].signal = inputs[i]
	end

	for i = 2, #self do

		--print("Activating Layer " .. i)

		local cells = self[i].cells
		local prevLayer = self[i-1].cells

		--print("Layer has " .. #cells .. " neurons. Previous layer has " .. #prevCells .. " neurons")

		--for k, v in ipairs(passInputs) do
		--	print("Passing Inputs " .. k, v)
		--end

		local lastIndex = (i == #self) and #cells or (#cells - 1) -- excluding bias neuron in all layers except output layer

		for j = 1, lastIndex do 
			--activate each cell
			--print("Activating neuron " .. j)
			cells[j]:activate(j, prevLayer, self.actFuncType)
		end
	end
end

function luann:getOutputs()
	local numLayers = #self
	local outputCells = self[numLayers].cells
	local outs = {}
	for i = 1, #outputCells do
		outs[i] = outputCells[i].signal
	end
	return outs
end

function luann:getError(outputs)
	return netError
end

function luann:bp(inputs, outputs)
	self:activate(inputs) --update the internal inputs and outputs

	-- get net error

	outs = self:getOutputs()

	--for i, v in ipairs(outs) do
	--	print("  Output Signal " .. i .. " : " .. v)
	--end

	self.netError = 0
	for i = 1, #outs do
		self.netError = self.netError + (outs[i] - outputs[i])^2
	end
	
	self.netError = math.sqrt(self.netError / #outs)

	local numSelf = #self
	local learningRate = self.learningRate
	local momentum = self.momentum
	local currActFunc = self.actFuncType
	for i = numSelf, 2, -1 do --iterate backwards (nothing to calculate for input layer)
		local numCells = #self[i].cells
		local layer = self[i].cells
		for j = 1, numCells do
			
			local signal = layer[j].signal
			local funcGrad = actFunGradient(currActFunc, signal)

			--print("Gradient Activation Function " .. currActFunc .. " returned value " .. funcGrad .. " for signal " .. signal)

			if i ~= numSelf then --special calculations for output layer
				local weightDelta = 0
				local nextLayer = self[i+1].cells
				for k = 1, #self[i+1].cells do
					weightDelta = weightDelta + layer[j].weights[k] * nextLayer[k].gradient
				end
				layer[j].gradient =  weightDelta * funcGrad
			else
				layer[j].gradient = (outputs[j] - signal) * funcGrad
			end
		end
	end

	for i = numSelf, 2, -1 do
		local layer = self[i]
		local prevLayer = self[i - 1]

		local lastIndex = (i == numSelf) and #layer.cells or (#layer.cells - 1) -- excluding bias neuron in all layers except output layer

		for j = 1, lastIndex do
			local gradient = layer.cells[j].gradient
			for k = 1, #prevLayer.cells do
				
				local deltaWeight = prevLayer.cells[k].deltaWeights[j]
				prevLayer.cells[k].deltaWeights[j] = (1 - momentum) * learningRate * prevLayer.cells[k].signal * gradient + momentum * deltaWeight
				prevLayer.cells[k].weights[j] = prevLayer.cells[k].weights[j] + prevLayer.cells[k].deltaWeights[j]
			end
		end
	end
end

function luann:saveNetwork(network, savefile)
	print(savefile)
	persistence.store(savefile, network)
end

function luann:loadNetwork(savefile)
	local ann = persistence.load(savefile)
		ann.bp = luann.bp
		ann.activate = luann.activate
		for i = 1, #ann do
			for j = 1, #ann[i].cells do
				ann[i].cells[j].activate = Cell.activate
			end
		end
	return(ann)
end

function luann:loadTrainingDataFromFile(fileName)
local trainingData = {}
local fileLines = {}
    local f = io.open(fileName, "rb")
		 for line in f:lines() do
			table.insert (fileLines, line);
		 end
	f:close()

	for i = 1, #fileLines do
		if i%2 == 0 then
				local tempInputs = {}
				for input in fileLines[i]:gmatch("%S+") do table.insert(tempInputs, tonumber(input)) end
				local tempOutputs = {}
				for output in fileLines[i+1]:gmatch("%S+") do table.insert(tempOutputs, tonumber(input)) end
			table.insert(trainingData, {tempInputs, tempOutputs})
		end
	end
return(trainingData)
end

return(luann)
