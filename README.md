# Project: Apollo (Physics Engine)
### Theory project for Physics II to test our knowledge on fundamental game physics by creating a physics engine from scratch.
![Screenshot](https://github.com/Lladruc37/TheoryProjectPhysicsII/blob/main/Screenshots/Screenshot.png)
## Game description
The year is 2121. Americans want to retake Project Apollo (And this time **for real**). With their flying saucer located in the mighty, wild lands of Florida, their plan is to capitalize the moon once and for all from those pesky reds.

Your job is simple. You've been selected to be the pilot of this new machinery. The first one of its kind (and an unstable one too).

Will you be able to complete the mission?

## How to complete the game
- Get to the moon
- Land safely on the moon (slowly, with the base of the saucer touching the surface, ~~dont bonk~~)
- Return back to Earth

## Controls
- Game:
  - WASD: Pilot saucer
  - Q/E: Rotate saucer **(only near the moon)**
  - Enter: Advance screen
  - R: Retry (When dead)
- Debug:
  - F1: Debug Draw (Colliders)
  - F2: Activate GodMode
  - F10: Toggle Checkpoint
  - F11: Self-Destruct button
  
## Physics Engine Features & Details
  - Velvet integrator
    - How things move
  - Object class
    - Stores all physics-related variables
    - Depending on shape, there may be extra variables (i.e Circle: Contains radius)
    - The Physics Module handles the spawn, updates and despawn of all objects.
  - **Circle** & Rectangle intersection & collision handling
    - The asteroids, the moon and the saucer all have circular working collider boxes
  - Impulsive forces
    - Apply force and reset it every frame.
    - Basic movement handling
    - Converted to acceleration for the Velvet integrator
  - General & **Orbital** gravity
    - General: Vertical general gravity (on Earth)
    - Orbital: Designate a point and create a gravity field around such.
  - Drag force
    - Resistance force **only applied to player** whenever on Earth
  - Buoyancy whenever on water
    - Clearly shown when returning to Earth after applying checkpoint
    
## The God Hand Team
 - Guillem Álava Burgueño ([Github](https://github.com/WillyTrek19))
 - Sergi Colomer Ferrer ([Github](https://github.com/Lladruc37))
 - Sebastian Delgado Vargas ([Github](https://github.com/Vinskky))
 
  ![Logo](https://github.com/Lladruc37/TheoryProjectPhysicsII/blob/main/Screenshots/logo.png)
