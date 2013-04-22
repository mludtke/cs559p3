# Moshball

## CS559 Project 3 Spring 2013

### Game space
The arena is a square with 5280 units per side (think of this as feet, so a one square mile arena). 
Your player’s radius is 50 (feet). 
The walls of the arena are double the height of the player’s radius. 
Your player must be able to move swiftly across the arena.

### Shader requirements
Walls and targets must both implement Phong (per pixel) shading.
Targets must implement at least one dynamically created (static or animated) texture or effect.

### Target placement
Seed their random number generators to 0 (or argv[2]) so that for the same
number of targets, the layout is made. This will allow players to compare their performance with others
(for bragging rights). Those implementing billiards-like action or true 3D movement will not be able to
compare strictly apples to apples since every target strike will be calculated uniquely).

### Frame Buffer Object
Render the main screen camera’s view in a frame buffer object 
as a texture to apply to “Jumbotrons” located on each side of the arena.

### Floor effects
In my reference implementation I created a polished obsidian surface. You do not have to duplicate this
effect. You do have to something cool on the floor.

### Skybox or skydome
You must correctly implement a skybox or skydome. To receive full credit no seams can be visible and in
the case of a skydome, no pinching can be visible.

### Keys
The ‘p’ key must completely pause the program and all **_related_** timers (!!!)
The ‘w’ key must convert the rendering to wireframe. Remember, wireframe rendering can be
accomplished in a single global state. Do not create separate objects for solid rendering and wireframe
rendering. Function keys must toggle the use of each of your shaders in turn. The ‘x’ key and the escape
key ESC must exit the game prematurely.

### Required second view
As indicated in the reference implementation, there must be an overhead view provided (god-view).
Your implementation specifics do not have to match mine.

### Indicating that a target is struck
In addition to having a countdown floating above a struck target, the target itself must be rendered in a
manner distinguishing it from targets that are not struck.
