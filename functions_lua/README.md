# functions.lua

Exploring the Lua C API with a simple math function loader 
program.

## 1. requirements

- a C compiler
- lua 5.4+

## 2. notes

- Functions are defined inside lua scripts, these should be able to (hopefully) take advantage of C's raw speed.
- The C part of the codebase loads and stores lua functions, while exposing math operator functions to Lua.
- Each .lua file should contain at least one function definition that matches names with its source file, that's exactly the function that will be loaded.
- WIP...
