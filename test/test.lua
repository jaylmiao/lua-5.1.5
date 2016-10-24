function Func1(param1)
    print("Func2 begin.")
    Func2(param1 + 10)
    print("Func1 end.")
    return 30
end

function Func2(value)
    print("Func1 begin.")
    --coroutine.yield(10, value)
    print("Func2 end.")
end

function add(a, b)
    return a + b
end

test = {
    a='123',
    b='456'
}
