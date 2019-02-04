local func_test = {}

function func_test.test(hello_world)
    print(hello_world)
    hello_world(1.234)
    return hello_world
end

return func_test
