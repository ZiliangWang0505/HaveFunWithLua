json = require("json")

a = json.new("{\"string\": \"string\",\"int\": 1,\"float\": 1.5, \"bool\": true}")

print(a["string"])
print(a["float"])
print(a["int"])
print(a["bool"])

a["string"] = "string2"
a["float"] = 2.5
a["int"] = 3
a["bool"] = false

print(a["string"])
print(a["float"])
print(a["int"])
print(a["bool"])