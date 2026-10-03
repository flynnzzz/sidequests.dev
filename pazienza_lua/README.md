# pazienza.lua
 Simulation of a popular italian card game that goes by many names, one of which is "Solitario della pazienza" (Solitaire of patience).


## 1. requirements

- Lua 5.4+

## 2. usage
``` bash
Usage:
  lua pazienza.lua <ACTION> [OPTIONS]

Actions:
  play              Play one game (verbosity is fixed and set to HIGH)
  simulate          Play games until victory
  average           Calculate the average win rate

Options:
  -z                Show no print output
  -l                Set verbosity to LOW
  -m                Set verbosity to MEDIUM
  -h                Set verbosity to HIGH
```

## 3. notes
- My first multiple files lua project, this helped me familiarize with the language
- Implementing the game solver using recursion was very stimulating
- Digitalizing this card game and "achieving victory" this way was quite satisfying (considering the unlikeliness of winning
with actual physical cards).
