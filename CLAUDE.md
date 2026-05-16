> This file is maintained by [Claude](https://claude.ai) as a living project reference. Contents may be updated automatically during development sessions.

# semnon_quest

Location-based narrative RPG in C++17. Heavy text focus, ASCII art, strong reactivity to player choices. NOT a tile-based game — scene-to-scene navigation, closer to a visual novel / text adventure.

## Current state

All core classes are implemented and compiling cleanly:
`GameState`, `Display`/`TerminalDisplay`, `Scene`, `Location`, `Connection`, `LocationManager`, `Game`.

Build system: root `Makefile` delegates to CMake. Executable lands at project root.
- `make` — build
- `make run` — build and launch
- `make re` — full rebuild

**Current:** vertical slice is complete and playable — 3 locations (tavern → street → church → back), ASCII art loading from file, full game loop working (render → input → move). `getInput()` reprompts on multi-character input via `peek()`.

**Next step:** JSON loading system to replace hardcoded `buildLocations()`.

## Development plan

**Vertical slice first:** 3 hardcoded locations, pure terminal (std::cout + numbered menus), no JSON yet. Get the loop working, then layer on systems.

**Phase A:** Pure terminal display
**Phase B:** Migrate to ncurses (arrow key navigation, color)
Migration is cheap because game logic goes through a `Display` abstraction layer — swapping implementations is a one-line change in main().

## Architecture

Agreed and implemented classes:
- `GameState` — all mutable player/world state: flags, skills, counters, feats, inventory, faction standing, location/scene tracking
- `Display` (abstract) / `TerminalDisplay` — rendering interface; `NcursesDisplay` planned for Phase B
- `Scene` — owns flag-conditional descriptions (tuple: flag, text, art_path), `Option` list, and `Connection` list
- `Location` — owns scenes (map) and `defaultSceneId()`; no connections (moved to Scene)
- `Connection` — struct: `destination_location`, `destination_scene`, `label`; `requires`/`effects` planned
- `LocationManager` — owns all locations by ID
- `Game` — orchestrator; owns `GameState`, `TerminalDisplay`, `LocationManager`; loop split into `buildOptions()`, `renderScene()`, `handleInput()`
- `Option` — struct: `label`, `type` (Dialogue/Action/Move), `target_id`, `destination_location`, `destination_scene`, `required_flag`

Planned but not yet started: `DialogueNode` / `DialogueResponse` / `Requirement`, `NcursesDisplay`.

## Naming conventions

- Classes: `UpperCamelCase`
- Methods: `lowerCamelCase`
- Member variables: `_lowerCamelCase`
- Method parameters: `snake_case`
- State flags: specific names — e.g. `"intimidated_barkeep_at_tavern"`, not `"talked_to_npc"`
- Git commits: Conventional Commits style — `type(scope): description`, lowercase, imperative mood

## Include paths

All subdirectories are registered in CMake's `target_include_directories`, so headers are included by filename only everywhere — no `../` prefixes.

## Project structure (target)

```
semnon_quest/
├── CMakeLists.txt
├── Makefile
├── data/
│   ├── locations/       # per-location JSON
│   ├── dialogues/       # node-based dialogue trees
│   ├── items.json
│   └── ascii_art/
├── src/
│   ├── main.cpp         # entry point
│   ├── core/            # Game, GameState
│   ├── world/           # Location, LocationManager, Connection, Scene
│   ├── entities/        # Player, NPC, ACharacter
│   ├── items/           # Item, Inventory
│   ├── ui/              # Display interface + implementations
│   ├── narrative/       # DialogueNode, DialogueManager
│   ├── systems/         # Combat, DialogueSystem
│   └── utils/           # FileLoader, etc.
└── build/
```

.hpp files live next to .cpp files, organised by domain. All narrative content in JSON — no hardcoded strings in C++.

## Reactivity system (core feature)

`GameState` tracks flags (bool), counters (int), skills (int), inventory (int), faction standing (int), feats (bool). Every location exit, dialogue option, and NPC interaction can carry `requires` and `effects` fields. Early obscure choices seed consequences that pay off chapters later.

## Working mode

Default: **project mode** — guiding first, Chris writes the code. Explain logic and approach; provide pseudocode if needed. Only provide full code if explicitly asked or if Chris is clearly stuck after genuine attempts. See global CLAUDE.md for full mode rules.
