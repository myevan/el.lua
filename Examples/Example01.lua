print(Logger)
print(Logger.Info)
print(Logger:Get())
Logger:Get():Info("11")
Logger:Get():Info("22")

local kChar = 1
local hChar = World:Get():CreateCharacter(kChar)
print(hChar)
Character:Get(hChar):DestroySelf()