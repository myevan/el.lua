require("Foundation/Declare")
require("Foundation/Event")

print(Logger)
print(Logger.Info)
print(Logger:Get())
Logger:Get():Info("11")
Logger:Get():Info("22")

local kChar = 1
local hChar = World:Get():CreateCharacter(kChar)
print(hChar)
Character:Get(hChar):DestroySelf()

Event:Bind("OnTick", function(msg)
    Logger:Get():Info(msg)
end)

Event:Call("OnTick", "hello")

function Test(msg)
    Logger:Get():Info(msg)
end