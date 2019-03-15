[CSED451] Computer Graphics Assignment 1

================= Pikachu Volleyball =================

-- Build --

Double Click Pika_volleyball.vcxproj to open VS 2017
project. Build with the setting Release/x64.

-- Control --

↑/↓ : Move menu
Enter : Select menu / Return to menu when game is over
a/d : Move left pikachu
←/→ : Move right pikachu (only on multi player)

-- Files --

- object.h
Header file that contains Object class. This file defines
the position of object as a 2-dimentional vector. It also
sets the getter and settor for the position.

- rectangleObject.h
Header file that contains RectangleObject class. This
file defines the size of rectangle object as a
2-dimensional vector. It also sets the getter and settor
for the size.

- movableRectangleObject.cpp, movableRectangleObject.h
Header and source code files that contain
MovableRectangleObject class. This files define velocity
of rectangle object as a 2-dimensional vector. It also
sets the getter and settor for the velocity. The method
for moving the object by its velocity checks the boundary
of game screen and doesn't let the object be out of screen.

- ballObject.cpp, ballObject.h
Header and source code files that contain BallObject
class. This files define the radius and velocity of the
ball as 2-dimensional vectors. It also sets the getter
and settor for the radius and velocity. The method for
moving the ball by its velocity checks the boundary of
game screen and doesn't let the object be out of screen.
Also, this method changes the velocity of the ball when
the ball is hit by screen boundary.

- render.cpp, render.h
Header and source code files that contain camera setting
functions and various rendering(drawing) functions. They
are called in render method of Game class.

- game.cpp, game.h
Header and source code files that contain Game class.
This files define the game state, scores of each players,
camera mode, and the number of players. It also defines
the methods to update and render the game. Most importantly,
it contains the collision detection algorithms for game
objects.

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