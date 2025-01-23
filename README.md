# Maze Generator/Solver Project
This project is a both a maze generator and solver than I created to showcase my custom built OpenGL batch renderer. When the program is ran, you can input the desired size for your maze, and the maze will be generated and displayed.
You can then press space, and the maze will be automatically solved and the solution will be displayed in green. You can also move the maze around using the wasd keys and zoom in and out using the up and down arrow keys in order to
get a better look at the maze.

## Compiling/building
To build the project, simply open the visual studio solution and compile in x86 either release or debug.

## Technical Details
My custom built batch renderer is able to render artibtiraily large numbers of textured quads in a single draw call. The number of quads can be set via the MAX_QUAD_COUNT constant defined in batch_renderer.h. In order to make my renderer easy to use, 
I wrote functions and structs to easily deal with common OpenGL concepts such as textures, shaders, and buffers. In addition, i also made implementations of the disjoint set and priority queue data structures for use by the maze generation and solving
algorithms respectively.

## Libraries used
glfw - window creation/input handling  
glew - expose OpenGL functions for use  
cglm - linear algebra functions  
stbimage - for texture loading  
