local mymath = {}

function mymath.fact(n)
    if n == 0 then
        return 1
    else
        return n*mymath.fact(n-1)
    end
end

return mymath
