Mobs
====

Mobs are objects that move around scenes and/or between scenes. Each mob needs a unique ID, beginning with `MOB`, and at most 13 characters after that (e.g. `MOBHELLO`) to distinguish it from other mobs and to store its state. The state of a mob is preserved when the game is saved, and restored when the game is loaded.

All mob IDs must be listed in the `mobNames` array defined in `Mob.cpp` in order for the engine to see them. In addition, some information about mobs must be provided within the `InitMob()` function - `mobSprites` maps a mob ID to its SpriteSheet (see further on for details), `mobRealNames` maps mob IDs to their display name (a C++ string), and `mobElements` maps mob IDs to their Element.

The SpriteSheet for a mob must be a 3x4 grid of 48x48 sprites, and a single SpriteSheet may be used for multiple mobs. See `data/images/todd.png` for a template.

If a mob ID is listed in the `autoMobs` array, then the mob will randomly walk around the scene. This is used for villagers, for example.

The mob `MOBTODD` gets special treatment in the engine - keyboard input moves that mob around, and its position is used to set the camera.

In `GameState.cpp`, in the `NewGame()` function, you must initialise the state of all mobs; there are plenty of examples of how it is done in there already. You may also want to check out the full MobState structure, in Mob.h.
