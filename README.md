# granglecast

This is an experimental C++ SDL2 game with many features to come 

## Features
The game incorporates several notable features:

- **ECS Type Architecture:** Utilizing an Entity-Component-System (ECS) architecture to efficiently manage game entities and their behaviors.

- **Impulse-Based Physics:** Employing an impulse-based physics system for dynamic and responsive in-game interactions.

- **Event System:** Implementing an event system to facilitate communication and coordination between different game components.


## Roadmap
- **OpenGL Integration:** Integrate OpenGL functionality into the game for enhanced graphics and visual effects.

- **Emscripten Compatibility:** Web deployment using Emscripten

- **Online Multiplayer (SDL_net):** Planning to implement online multiplayer functionality using SDL_net

## Dependencies
Make sure you have the following dependencies installed and linked in your development environment:

- SDL2
- SDL_image
- SDL_mixer
- GLEW

## Build Instructions

To build the project, follow these steps:

1. Open a terminal and navigate to the project directory.

2. Execute the following command based on your platform:

    - For Linux:
      ```
      make -f Makefile.lin
      ```

    - For Windows:
      ```
      make -f Makefile.win
      ```

3. Ensure that SDL2, SDL_image, SDL_mixer, and GLEW are properly installed and linked in your development environment.

