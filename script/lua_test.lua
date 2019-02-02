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
    table = {}
    table['test'] = 3
    table[2] = 1
    table[2.5] = 3.0
    table[3] = {}
    table[3][0] = 100
    table[3][1] = {}
    table[3][1]["meow"] = "woof"
    table["func"] = mymath.dofact
    print(table)

    return table
end

return mymath
