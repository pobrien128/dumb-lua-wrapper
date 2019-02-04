local mymath = {}

function mymath.dofact(n)
    print("DO FACT SON!")
    if n == 0 then
        return 1
    else
        return n*mymath.dofact(n-1)
    end
end

function mymath.table_test(test_table)
    print "PLEASE"
    print(tprint(test_table, 0))
    print(test_table["test"])
    print(test_table[3])
    result = test_table["func"]()
    print "WE Got this result: "
    print (result)
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
        print "PROOF OF CONCEPT"
        return 1
    end

    return my_table
end

function tprint (tbl, indent)
  if not indent then indent = 0 end
  local toprint = string.rep(" ", indent) .. "{\r\n"
  indent = indent + 2 
  for k, v in pairs(tbl) do
    toprint = toprint .. string.rep(" ", indent)
    if (type(k) == "number") then
      toprint = toprint .. "[" .. k .. "] = "
    elseif (type(k) == "string") then
      toprint = toprint  .. k ..  "= "   
    end
    if (type(v) == "number") then
      toprint = toprint .. v .. ",\r\n"
    elseif (type(v) == "string") then
      toprint = toprint .. "\"" .. v .. "\",\r\n"
    elseif (type(v) == "table") then
      toprint = toprint .. tprint(v, indent + 2) .. ",\r\n"
    else
      toprint = toprint .. "\"" .. tostring(v) .. "\",\r\n"
    end
  end
  toprint = toprint .. string.rep(" ", indent-2) .. "}"
  return toprint
end

return mymath
