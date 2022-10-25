# Rainfall

Rainfall is the compiler for the Thrice language.

Transpiles the Thrice source to C.

This is the version of the Rainfall written in C++. Eventually, it will be
rewritten in Thrice.

---

## Usage

Loads all the Thrice packages in the workspace directory. Transpile executable
packages to a single C file with an entry point.

`rainfall (-(-<option>|<option_shortcut>) <value>?)* <command>|<command_shortcut> <argument>*`

After the executable name, there should be zero or more options, which come
after two dashes and might expect a value after them. The shortcut for an option
can only be used with a single dash before it.

Then, there should be the command. The shortcut for a command can be directly
used in its place.

After the command, there could be zero or more arguments passed to the command.

### Commands

| Command | Shortcut | Description                                           | Arguments        |
| ------- | -------- | ----------------------------------------------------- | ---------------- |
| new     | n        | Creates a new package.                                | name             |
| check   | c        | Checks the validity of given or all the packages.     | checked packages |
| test    | t        | Runs the tests of given or all the packages.          | tested packages  |
| build   | b        | Generates the C file of the given executable package. | built package    |
| run     | r        | Runs the given executable package.                    | run package      |

### Options

| Option    | Shortcut | Description                                       | Arguments           |
| --------- | -------- | ------------------------------------------------- | ------------------- |
| directory | d        | Change the workspace directory to the given path. | workspace directory |

## Setup

Project is written using VS Code and clangd. It is compiled using CMake and
clang.

VS Code launching settings exist for Windows and Linux.

## License

Licensed under GPL 3.0 or later.

---

Copyright (C) 2022 Cem Geçgel <gecgelcem@outlook.com>
