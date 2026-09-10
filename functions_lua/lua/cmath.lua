-- cmath.lua
--
-- Wrapper module for "libcmath"
-- Provides a clean Lua interface to optimized C mathematical functions
--

local libcmath = require("libcmath")

local M = {}

-- ============================================================================
-- Arithmetic Operations
-- ============================================================================

--- Add two numbers
-- @param a number First operand
-- @param b number Second operand
-- @return number Sum of a and b
function M.add(a, b)
	return libcmath.add(a, b)
end

--- Subtract two numbers
-- @param a number First operand
-- @param b number Second operand
-- @return number Difference (a - b)
function M.sub(a, b)
	return libcmath.sub(a, b)
end

--- Multiply two numbers
-- @param a number First operand
-- @param b number Second operand
-- @return number Product of a and b
function M.mul(a, b)
	return libcmath.mul(a, b)
end

--- Divide two numbers
-- @param a number Dividend
-- @param b number Divisor (must not be zero)
-- @return number Quotient (a / b)
-- @error Raises error if divisor is zero
function M.div(a, b)
	return libcmath.div(a, b)
end

--- Raise a number to a power
-- @param base number Base value
-- @param exp number Exponent
-- @return number base raised to the power of exp
function M.pow(base, exp)
	return libcmath.pow(base, exp)
end

-- ============================================================================
-- Root and Absolute Value
-- ============================================================================

--- Calculate the square root
-- @param x number Non-negative value
-- @return number Square root of x
function M.sqrt(x)
	return libcmath.sqrt(x)
end

--- Calculate absolute value
-- @param x number Input value
-- @return number Absolute value of x
function M.abs(x)
	return libcmath.abs(x)
end

-- ============================================================================
-- Trigonometric Functions
-- ============================================================================

--- Calculate sine (input in radians)
-- @param x number Angle in radians
-- @return number Sine of x
function M.sin(x)
	return libcmath.sin(x)
end

--- Calculate cosine (input in radians)
-- @param x number Angle in radians
-- @return number Cosine of x
function M.cos(x)
	return libcmath.cos(x)
end

--- Calculate tangent (input in radians)
-- @param x number Angle in radians
-- @return number Tangent of x
function M.tan(x)
	return libcmath.tan(x)
end

--- Calculate arcsine
-- @param x number Value in range [-1, 1]
-- @return number Arcsine of x in radians
function M.asin(x)
	return libcmath.asin(x)
end

--- Calculate arccosine
-- @param x number Value in range [-1, 1]
-- @return number Arccosine of x in radians
function M.acos(x)
	return libcmath.acos(x)
end

--- Calculate arctangent
-- @param x number Input value
-- @return number Arctangent of x in radians
function M.atan(x)
	return libcmath.atan(x)
end

--- Calculate arctangent of y/x with proper quadrant handling
-- @param y number Y coordinate
-- @param x number X coordinate
-- @return number Arctangent of y/x in radians (range [-π, π])
function M.atan2(y, x)
	return libcmath.atan2(y, x)
end

-- ============================================================================
-- Exponential and Logarithmic Functions
-- ============================================================================

--- Calculate e raised to the power x
-- @param x number Exponent
-- @return number e^x
function M.exp(x)
	return libcmath.exp(x)
end

--- Calculate natural logarithm (base e)
-- @param x number Positive value
-- @return number Natural logarithm of x
function M.log(x)
	return libcmath.log(x)
end

--- Calculate base-10 logarithm
-- @param x number Positive value
-- @return number Base-10 logarithm of x
function M.log10(x)
	return libcmath.log10(x)
end

-- ============================================================================
-- Rounding Functions
-- ============================================================================

--- Round up to nearest integer
-- @param x number Input value
-- @return number Smallest integer greater than or equal to x
function M.ceil(x)
	return libcmath.ceil(x)
end

--- Round down to nearest integer
-- @param x number Input value
-- @return number Largest integer less than or equal to x
function M.floor(x)
	return libcmath.floor(x)
end

--- Calculate remainder of division
-- @param x number Dividend
-- @param y number Divisor
-- @return number Floating-point remainder of x/y
function M.fmod(x, y)
	return libcmath.fmod(x, y)
end

-- ============================================================================
-- Comparison Functions
-- ============================================================================

--- Return the minimum of two values
-- @param a number First value
-- @param b number Second value
-- @return number Smaller of a and b
function M.min(a, b)
	return libcmath.min(a, b)
end

--- Return the maximum of two values
-- @param a number First value
-- @param b number Second value
-- @return number Larger of a and b
function M.max(a, b)
	return libcmath.max(a, b)
end

-- ============================================================================
-- Angle Conversion
-- ============================================================================

--- Convert radians to degrees
-- @param x number Angle in radians
-- @return number Angle in degrees
function M.deg(x)
	return libcmath.deg(x)
end

--- Convert degrees to radians
-- @param x number Angle in degrees
-- @return number Angle in radians
function M.rad(x)
	return libcmath.rad(x)
end

-- ============================================================================
-- Module Metadata
-- ============================================================================

M._VERSION = "1.0"
M._DESCRIPTION = "C math library wrapper with optimized function bindings"

return M
