
# /* KTerminal */

# UNIX-Based Terminal Emulator and Shell

[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/) 

> [!NOTE]
> This Piece of software is intended for myself and is still in active 
> development. Certain Pieces all tools may not work but we'll get there!

A Unix Terminal Emulator and Text Editor project with custom built-ins and bash-derived commands. Built for my own workflow on my Linux workspace. Originally, and still is, intended for my Toy Operating system "Coffee OS" where the command set may differ slightly but will have the same functionality.

This is a pet project meant for myself. In order to understand the theory behind system calls and Low-level hardware manipulation from the command-line. My Goal with this is to create a sustainable, modular design that can be updated in a live environment, i.e changing the source code will affect live changes in the terminal (for the debug build) emulating that of a JIT workflow;

the terminal acts as a TTY emulator built using the QT Framework with QWidgets. The inbuilt text editor "keditor" is based of two source-code projects which I have found impactful: Kilo - a lesser-known but nicely implemented, lightweight editor and Neovim an editor which I grew to love over the last couple of years.


## Acknowledgements

- Huge Thanks to these projects for making the learning much easier!
    - [Neovim Repository](https://github.com/neovim/neovim)
    - [Kilo Editor Respository](https://github.com/antirez/kilo)
    - [QT Framework](https://github.com/qt/qt5)


## Authors

- [@TomAndJerrys2](https://github.com/TomAndJerrys2)


## Roadmap

- Working text editor integration
    - In-built commands that are system independent
    - Memory optimizations for a lightweight build
    - Syntax Hightlighting and Intellisense behaviour
    
- Beautified Interface for easier usage

- Shell (based off bash behaviour) will be independent
    - Low-Level Kernel interaction for Process Spawning
    - Possible Multi-Threaded behaviour for working Multiple Spawns
    - Easy to use for already existing tools: i.e GDB, Valgrind, etc

- Current Focus: Basic working Debug build with proper code-quality


## License

[MIT](https://choosealicense.com/licenses/mit/)

