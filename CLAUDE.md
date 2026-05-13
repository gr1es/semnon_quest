# semnon_quest

Location-based narrative RPG in C++17. Heavy text focus, ASCII art, strong reactivity to player choices. NOT a tile-based game — scene-to-scene navigation, closer to a visual novel / text adventure.

## Current state

The repo contains an SFML-based Engine skeleton from a previous session with a different AI. This is being discarded — start fresh with the terminal-first approach described below.

## Development plan

**Vertical slice first:** 3 hardcoded locations (tavern → street → church → back), pure terminal (std::cout + numbered menus), no JSON yet. Get the loop working, then layer on systems.

**Phase A:** Pure terminal display
**Phase B:** Migrate to ncurses (arrow key navigation, color)
Migration is cheap because game logic goes through a `Display` abstraction layer — swapping implementations is a one-line change in main().

## Architecture (drafted, not yet fully agreed)

Key classes have been drafted but Chris needs each one explained before committing to implementation. Do not assume agreement on any class design — explain purpose and rationale first.

Classes in scope: `Display` / `TerminalDisplay` / `NcursesDisplay`, `GameState`, `Location` / `LocationManager`, `DialogueNode` / `DialogueResponse` / `Requirement`.

## Naming conventions

- Classes: `UpperCamelCase`
- Methods: `lowerCamelCase`
- Member variables: `_lowerCamelCase`
- State flags: specific names — e.g. `"intimidated_barkeep_at_tavern"`, not `"talked_to_npc"`

## Project structure (target)

```
semnon_quest/
├── CMakeLists.txt
├── data/
│   ├── locations/       # per-location JSON
│   ├── dialogues/       # node-based dialogue trees
│   ├── items.json
│   └── ascii_art/
├── src/
│   ├── main/
│   ├── core/            # game loop, GameState
│   ├── world/           # Location, LocationManager, Connection
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

`GameState` tracks flags (bool), counters (int), skills (int), inventory (int). Every location exit, dialogue option, and NPC interaction can carry `requires` and `effects` fields. Early obscure choices seed consequences that pay off chapters later.

## Working mode

Default: **project mode** — guiding first, Chris writes the code. Explain logic and approach; provide pseudocode if needed. Only provide full code if explicitly asked or if Chris is clearly stuck after genuine attempts. See global CLAUDE.md for full mode rules.
