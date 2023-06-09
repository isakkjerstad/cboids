# cboids:
Boids implementation in C using SDL2.

## Installation:
On Linux, follow these steps to install SDL2:
- Run: ```git clone https://github.com/libsdl-org/SDL.git```.
- Then: ```cd SDL```, ```git checkout e6cb082``` and ```./configure```.
- Finally do: ```make``` and ```sudo make install```.


Download the code and compile the program:
- In another directory, do: ```git clone https://github.com/isakkjerstad/cboids```.
- Then run: ```make``` to compile.
- Run: ```make clean``` to delete the executable.

## Running the program:
Run the program with default arguments: ```./boids```.
Selecting numbers of birds and hoiks: ```./boids <birds> <hoiks>```.

## Settings:
You may change behaviour in ```config.h``` and ```boids.h```.

## Prerequisites:
You need the following to install and run the program:
- GCC
- Make
- Git
