# Changelog

<summary>The full history, or so was I told...</summary>

## Beta first minor release

**v0.1.0** first release

- brought back `ctpl.hpp` (thread pool)
- added `ini.hpp` (INI files) and `json.hpp` (JSON files)
- made some header files for game logic

**v0.1.1** fps counter

- added fps with bitmap font
- target fps is now respected (🥳)

**v0.2.x** pacman entity logic

- pacman correctly responds to user input
- stops when hitting a wall
- changes direction (if registered) when hitting a wall
- eats dots

**v0.3.x** some system pause and wait stuff

- added pseudo keypressed event checkers
- `wait_for_ms` w/ internal chrono

**v0.4.x** ghost entity logic

- ghost entity on display
- added chasing algorithms
- added scatter mode and frightened (pure random) mode
- added timed mode switching

**v0.5.x** game logic

- game logic handles pacman / ghosts interractions
- fixed pacman can eat eaten ghosts bug
