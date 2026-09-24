# functions.lua

Exploring the Lua C API with a simple math function loader
program.

## 1. requirements

- a C compiler
- lua 5.4+

## 2. notes

- Functions are defined inside lua scripts, these should be able to (hopefully) take advantage of C's raw speed.
- The C part of the codebase loads and stores lua functions, while exposing math operator functions to Lua.
- This is my first time making two different languages interact, finishing this mini-project gave me a better grasp on how lua
can be used for a plugin system.
