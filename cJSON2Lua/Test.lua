json = require("json")

a = json.new("{\"string\": \"string\",\"int\": 1,\"float\": 1.5, \"bool\": true}")

print(json.get(a, "string"))
print(json.get(a, "float"))
print(json.get(a, "int"))
print(json.get(a, "bool"))

json.set(a, "string", "string2")
json.set(a, "float", 2.5)
json.set(a, "int", 3)
json.set(a, "bool", false)

print(json.get(a, "string"))
print(json.get(a, "float"))
print(json.get(a, "int"))
print(json.get(a, "bool"))