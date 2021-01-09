# roblox_playerbot

uh, it launches roblox instances, yes

# Configuration

the program searches for config in "files/conf", all paths are relative to the executable

ex. config
```
cookie_file=files/cookies
place_ids=11111,22222
debug=true
```
cookie_file and place_ids are self explanatory, debug enables debug output

place_ids are split by `,`
# Compiling

to compile you need CPR

to get easily CPR on windows, get yourself vcpkg then
```
vcpkg integrate install
vcpkg install cpr[*]
```
and pray that zlib will be nice, idk how i fixed it anymore

# License
GNU GPLv3 