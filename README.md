# Evalexpr

A C program that evaluates arithmetic expressions using either infix notation or Reverse Polish Notation (RPN).

## Description

The program reads an expression from standard input and prints the result to standard output.

Example:

```bash
echo "1 + 1" | ./evalexpr
2
```

The program supports:
- standard (infix) notation
- Reverse Polish Notation with the `-rpn` option

## Features

### Supported operations

- `+` : addition
- `-` : subtraction
- `*` : multiplication
- `/` : division
- `%` : modulo
- `^` : exponentiation

### Infix notation

- operator precedence handling
- parentheses `(` `)`
- unary operators (`+` and `-`)

### RPN notation

Example:

```bash
echo "5 2 2 ^ 3 + *" | ./evalexpr -rpn
35
```

## Build

```bash
make
```

This generates the `evalexpr` binary at the root of the project.

## Usage

### Infix

```bash
echo "5*(2^2+3)" | ./evalexpr
```

### RPN

```bash
echo "1 1 +" | ./evalexpr -rpn
```

## Tests

```bash
make check
```

Runs the test suite (functional shell tests).

## Clean

```bash
make clean
```

## Exit codes

- `0` : success
- `1` : lexical error
- `2` : syntax error
- `3` : arithmetic error (e.g. division by zero)
- `4` : other errors (e.g. invalid arguments)

## Project structure

```
evalexpr/
├── Makefile
├── src/
│   └── ...
└── tests/
    └── ...
```

## Constraints

- Compilation flags:
  `-std=c99 -pedantic -Werror -Wall -Wextra -Wvla`
- A `main` function is required
- Input is read from standard input
