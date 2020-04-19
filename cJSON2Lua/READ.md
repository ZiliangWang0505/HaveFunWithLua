Lua学习记录-实现Lua扩展cJson

Tips:
```
cd cJSON2Lua
gcc -fPIC -shared JSON.c cJSON.c -lm -o json.so
lua Test.lua
```
