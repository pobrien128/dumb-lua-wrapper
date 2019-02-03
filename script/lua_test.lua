local mymath = {}

function mymath.dofact(n)
    print("DO FACT SON!")
    if n == 0 then
        return 1
    else
        return n*mymath.dofact(n-1)
    end
end

function mymath.fact(n)
    local my_table = {}
    my_table['test'] = 3
    my_table[2] = 1
    my_table[2.5] = 3.0
    my_table[3] = {}
    my_table[3][0] = 100
    my_table[3][1] = {}
    my_table[3][1]["meow"] = "woof"

    function my_table.func (self)
        self[10] = 1
        return 1
    end

    return my_table
end

return mymath
