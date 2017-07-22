Event = Declare("Event", {
    name_func_dict = {}
})

function Event:Bind(name, func)
    local func_list = self.name_func_dict[name]
    if func_list == nil then
        self.name_func_dict[name] = {func}
    else
        table.insert(func_list, func)
    end
end

function Event:Call(name, ...)
    local func_list = self.name_func_dict[name] or {}
    for index, func in ipairs(func_list) do
        func(...)
    end
end