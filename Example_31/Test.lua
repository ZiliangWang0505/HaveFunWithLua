array = require("array")

a = array.new(1000)
for i = 1, 1000 do
    a[i] = (i % 2 == 0)
end

print(a[10])
print(a[11])
print(#a)