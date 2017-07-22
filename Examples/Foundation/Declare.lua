function Declare(name, args, baseCls)
    local cls = args or {}
    setmetatable(cls, baseCls)

    cls.__name = name
    cls.__index = cls 
    cls.__call = function(cls, args)
        local inst = args or {}
        setmetatable(inst, cls)
        return inst
    end
    return cls
end
