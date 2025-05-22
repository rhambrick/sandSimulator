# Sand Game - written in C from scratch using the RayLib library.
Fun particle simulator game with pretty basic rules rendering complex interactions of materials (sand, water, lava, stone).
Features physics for fluids, gravity, burning, and various brush sizes.
## Controls:
 - Left click: place material
 - Right click: remove material
 - '1': sand
 - '2': water
 - '3': lava
 - '4': stone
 - 'q': brush size 1x1
 - 'w': brush 3x3
 - 'e': brush 5x5
 - 'c': clear all
 - 'esc': close

 ## How to run (Mac with Apple Silicon):
Since I built this on Mac, the "sand" executable is all you need. Download that, put it in a folder, open a terminal window in that folder and run:

```./sand```

If you're cautious about running random machine code from the internet (which you should be, lol), you'll want to compile the code (main.c) and then run it, you'll need to have RayLib installed (homebrew is easiest).

If you don't have homebrew install it with this terminal command:

```/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"```

Then install the gcc compiler and the raylib library

```brew install gcc```

```brew install raylib```

Finally, compile the program by running this command from a terminal opened in the same folder as "main.c":

```gcc main.c -o sand -I/opt/homebrew/opt/raylib/include -L/opt/homebrew/opt/raylib/lib -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo```

The output will be the "sand" executable you run with the ./sand.

## How to run (Windows)
Install MSYS2 (recommended for easy package management): (https://www.msys2.org/)

Open the MSYS2 MSYS terminal and run:

```pacman -Syu```

Then close and reopen the terminal and run again:

```pacman -Su```

Install required packages:

```pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-raylib```

Compile with:

```gcc main.c -o sand.exe -lraylib -lopengl32 -lgdi32 -lwinmm```

Run

```./sand.exe```


## How to run (Linux Ubuntu/Debian-based distros):
```sudo apt update```

```sudo apt install build-essential libraylib-dev libgl1 libglu1-mesa-dev```

If "libraylib-dev" isn't available in your distro's repositories, you can try building raylib from source:
https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux

Compile with

```gcc main.c -o sand -lraylib -lGL -lm -lpthread -ldl -lrt -lX11```

```./sand```
