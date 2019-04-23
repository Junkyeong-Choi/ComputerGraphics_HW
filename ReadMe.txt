[CSED451] Computer Graphics Assignment

================= Pikachu Volleyball =================

-- Build --

Double Click Pika_volleyball.vcxproj to open VS 2017
project. Build with the setting Release/x64.

-- Control --

↑/↓ : Move menu
Enter : Select menu / Return to menu when game is over
a/d : Change moving direction of Pikachu
w/s : Move Pikachu forward/backward
r: Restart game
h: Toggle hidden line removal
Spacebar : Change View mode of camera
j/l/i/k : Move camera on celling view of camera

-- Files --

- settings.h
Header file that contains several global variables such as
player size, ball velocity, and map size.

- object.h
Header file that contains Object class. This file defines
the position of object as a 3-dimentional vector. It also
sets the getter and settor for the position.

- cubeObject.h
Header file that contains RectangleObject class. This
file defines the size of rectangle object as a
3-dimensional vector. It also sets the getter and settor
for the size.

- movableCubeObject.h
Header files that containMovableRectangleObject class. 
This file defines velocity of rectangle object as a speed 
value and a 2-dimensional direction angle vector. The 
elements on the vector consist of the rotation angle about 
z-axis (on xy plane), the angle between z-axis and direction 
vector. It also sets the getter and settor for the speed, 
direction angles and velocity. The method for moving the 
object by its velocity checks the boundary of game screen 
and doesn't let the object be out of screen.

- ballObject.h
Header  files that contain BallObject class. This files
defines the radius and velocity of the ball as 2-dimensional 
vectors. It also sets the getter and settor for the radius 
and velocity. The method for moving the ball by its velocity
checks the boundary of game screen and doesn't let the 
object be out of screen. Also, this method changes the 
velocity of the ball when the ball is hit by screen boundary.

- shader.h
Header files that contain Shader class. This file defines
some interface to loading, compiling, and linking shader 
program to our program.

- mesh.h
Header files that contain Mesh class. This file construct
VAO, VBO and EBO for shader program and also proceed settings 
about vertex attribute array.

- model.cpp, model.h
Header and source code files that contains Model class. These
files load obj files that contain information about model and 
save as a structure of struct aiScene, which is defined by assimp
library. As it loads obj file and construct aiScene, it changes
data structure in forms of predefined Mesh, Texture class, enabling
drawing for model.

- text_renderer.cpp, text_renderer.h
Header and source code files that contains TextRenderer class.
These files load font file and text shader, and actually renders
texts by using font textures.

- renderer.cpp, renderer.h
Header and source code files that contains Renderer class. These 
files load actual model(Pikachu, Pokeball, Map) and shader and
actually renders a scene by defining transform matrix(model-view-
projection).

- sceneGraphNode.h
Header file that contains SceneGraphNode class. SceneGraphNode
contains informations that are needed for rendering such as
color, model information, and transformation matrix. Traversing
scene graph results rendering the whole scene it contains.

- shader.vert, shader.frag
Vertex/fragment shader files that are used in Renderer class.

- text.vert, text.frag
Vertex/fragment shader files that are used in TextRenderer class.

- ui.cpp, ui.h
Header and source code files that contains rendering functions of
main screen. These are developed in older version so that they don't
use shaders but use immediate mode.

- collision.h
Header and source code files that contains a collision 
detection algorithms for game objects. The are called for
detecting collision of each game objects in update method
of Game class

- game.cpp, game.h
Header and source code files that contain Game class.
This files define the game state, scores of each players,
camera mode, and the number of players. It also defines
the methods to update and render the game. 

- main.cpp
Source code file that contains  main function for program.
It initializes Game object and attach event handlers to it.
The main function has while loop which calculates the
ellapsed time and use it to update and render the game.

                                             ,-.
                                          _.|  '
                                        .'  | /
                                      ,'    |'
                                     /      /
                       _..----""---.'      /
 _.....---------...,-""                  ,'
 `-._  \                                /
     `-.+_            __           ,--. .
          `-.._     .:  ).        (`--"| \
               7    | `" |         `...'  \
               |     `--'     '+"        ,". ,""-
               |   _...        .____     | |/    '
          _.   |  .    `.  '--"   /      `./     j
         \' `-.|  '     |   `.   /        /     /
         '     `-. `---"      `-"        /     /
          \       `.                  _,'     /
           \        `                        .
            \                                j
             \                              /
              `.                           .
                +                          \
                |                           L
                |                           |
                |  _ /,                     |
                | | L)'..                   |
                | .    | `                  |
                '  \'   L                   '
                 \  \   |                  j
                  `. `__'                 /
                _,.--.---........__      /
               ---.,'---`         |   -j"
                .-'  '....__      L    |
              ""--..    _,-'       \ l||
                  ,-'  .....------. `||'
               _,'                /
             ,'                  /
            '---------+-        /
                     /         /
                   .'         /
                 .'          /
               ,'           /
             _'....----"""""