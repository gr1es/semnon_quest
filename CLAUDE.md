> This file is maintained by [Claude](https://claude.ai) as a living project reference. Contents may be updated automatically during development sessions.

# semnon_quest

Location-based narrative RPG in C++23 (switched from C++17 on 2026-10-08 — private project, 42's version limit doesn't apply; C++20/23 features like `map::contains`, designated initializers and `std::format` are fair game). Heavy text focus, ASCII art, strong reactivity to player choices. **Hybrid design:** a text-adventure narration mode (scene-to-scene navigation, visual-novel-like) plus short turn-based grid combat sequences, both rendered in a single window via libtcod. See _Design direction_ below.

## Current state

All core classes are implemented and compiling cleanly:
`GameState`, `Display`/`TerminalDisplay`, `Scene`, `SceneVariant`, `Location`, `Connection`, `LocationManager`, `Menu`, `Option`, `Game`, plus the reactivity primitives `Effect` / `Requirement` / `StateType`.

Build system: root `Makefile` delegates to CMake. Executable lands at project root.
- `make` — build
- `make run` — build and launch
- `make re` — full rebuild

**Current:** JSON loading via `LocationLoader` (nlohmann/json, FetchContent). **Effects/requirements system implemented:** `Effect` and `Requirement` structs + `StateType` enum (Flag/Skill/Counter/Feat/Item/Standing), applied by free functions `applyEffects()` and `requirementsMet()`. `Option` and `Connection` each carry `std::vector<Requirement> requirements` + `std::vector<Effect> effects`. `buildOptions()` gates visibility via `requirementsMet()`; `handleInput()` runs `applyEffects()` on any selected option (so **Action options are fully functional**). `Requirement` supports flag/feat `expected` bools and skill/counter/item/standing `min`/`max` ranges (`optional<int>`).

**Variants leg (done — builds & runs):** scene descriptions replaced by requirement-gated `SceneVariant`s — struct `SceneVariant` (world/), JSON array key `variants`, text field `description`. `LocationLoader` has `stringToStateType` + `parseRequirements` helpers parsing a `requirements` array from JSON; `location.schema.json` and VSCode snippets migrated. Verified with `make run` (tavern↔street↔church traversal).

**Options/connections leg (done — builds & runs):** the loader now parses `requirements` AND `effects` arrays for options and connections via `parseRequirements` + `parseEffects` (legacy `required_flag` fully removed). `location.schema.json` refactored to `$defs`+`$ref` for the shared requirement/effect shapes; options drop `required_flag` and their authored `type` is restricted to Dialogue/Action (Move is synthesized at runtime from connections in `buildOptions()`, never authored). Snippets updated (added `effect`, reworked `option`). All three data files validate against the schema. **The whole requirements/effects reactivity data path — variants, options, connections — is now authorable from JSON.**

**Next step — dialogue system (designed, not yet on disk on this machine):**

Data model mirrors the world model 1:1, so the same reactivity plumbing reuses directly:

| Dialogue | mirrors | World |
|---|---|---|
| `Dialogue` (id, `start_node`, `map<string, DialogueNode> nodes`) | ↔ | `Location` |
| `DialogueNode` (id, `text`, `vector<DialogueResponse> responses`) | ↔ | `Scene` |
| `DialogueResponse` (`label`, `vector<Requirement> requirements`, `vector<Effect> effects`, `target_node`; empty `target_node` = end conversation) | ↔ | `Option` |

Named `DialogueResponse`, not `DialogueOption`/`DialogueChoice` — those names collide with the existing `Option` struct and the `Menu`'s `Choice` enum. Gating/effects on `DialogueResponse` are the *only* place the dialogue system touches requirements/effects (no gates at the node or dialogue level) — reuses `Requirement`/`Effect`/`requirementsMet()`/`applyEffects()` as-is.

Build order:
1. Three structs above in `src/narrative/` (`Dialogue.hpp`, `DialogueNode.hpp`, `DialogueResponse.hpp`), each `#include`-ing `Requirement.hpp`/`Effect.hpp` as needed — plain POD, no methods.
2. **Extract shared JSON helpers** — `stringToStateType`/`parseRequirements`/`parseEffects` currently live as `static` functions inside `LocationLoader.cpp`; `DialogueLoader` needs the latter two. Move to a new shared file (e.g. `src/utils/ReactivityParser.hpp`/`.cpp`): header declares `parseRequirements`/`parseEffects` (non-static now), the `.cpp` keeps `stringToStateType` `static`/file-local (only the two parse functions need external linkage). Update `LocationLoader.cpp` to include it instead of defining them; add the new `.cpp` to `CMakeLists.txt`.
3. `DialogueLoader` (mirrors `LocationLoader`) reading `data/dialogues/*.json` into a `DialogueManager` (mirrors `LocationManager`, storage keyed by id).
4. Runtime loop — a `Game::runDialogue(const std::string &id)` method (parallel to `showMenu()`): render current node's `text` via `Display`, show responses filtered by `requirementsMet()`, read choice, `applyEffects()` on the chosen response, follow `target_node` or end if empty.
5. Wire in: load dialogues in `startNewGame()`; the `OptionType::Dialogue` branch in `handleInput()` (currently a `TODO` stub) calls `runDialogue(chosen.target_id)`.

JSON shape (`data/dialogues/*.json`): top-level `id`/`start_node`/`nodes[]`; each node has `id`/`text`/`responses[]`; each response has `label`/`requirements[]`/`effects[]`/`target_node` — same `requirements`/`effects` array shape already used by variants/options/connections (reuses the schema's `$defs`).

**Then:** the pending `Display&`-injection refactor (Game still owns `TerminalDisplay` by value) that unblocks the libtcod backend and the `-t/--terminal` debug toggle.

## Design direction (hybrid: narration + combat)

Major pivot from "pure text adventure." The game is now a **hybrid of two modes sharing one window**:

- **Narration mode** — the existing text-adventure core: scene-to-scene navigation, dialogue, numbered menus. The majority of playtime.
- **Combat mode** — short turn-based grid roguelike sequences (SPD/Stoneshard-inspired: player and world act alternately). Triggered from narration (e.g. provoking a drunkard in the tavern), resolved, then control returns to narration.

**Controls (game design):** the game itself is designed keyboard-focused — keyboard-only where feasible. This is a deliberate UX choice for the *game*, independent of the tooling/workflow reasons below. Do NOT conflate it with Chris's dislike of Godot's mouse-driven editor — that was purely about the game-*creation* process, not game design.

**Rendering + combat backend: libtcod** (The Doryen Library; SDL-based). Chosen over the earlier ncurses/SFML Phase B plan and over a Godot detour (see below). Rationale:
- Native grid-roguelike toolkit: FOV, A*/Dijkstra pathfinding, colored character grid, custom fonts/tilesets, input.
- Hosts BOTH modes in a single window — narration text and combat grid render through the same libtcod console. Avoids the raw-terminal-narration + separate-combat-window trap.
- Keyboard-driven, C++, no GUI editor — matches Chris's VS Code / keyboard *development* workflow. (This concerns the *authoring* experience; Godot's mouse-heavy editor was what ruled it out — see Godot note below.)

**Consequence — the `Display` abstraction is the migration seam.** std::cout narration must move into a `TcodDisplay`: std::cout, ncurses, and libtcod cannot be mixed, so both modes go through one backend. Contained swap, not a rewrite.

**Mode ↔ mode bridge = the reactivity system.** Combat is just another producer of `GameState` flags/effects. A combat outcome writes flags (`"killed_drunkard_at_tavern"`, `"spared_drunkard"`) that narration reads. The planned brutal-vs-pacifist axis falls out of this: lethal vs non-lethal resolutions set different flags, consumed by later `requires`/`effects`.

**ASCII ↔ tile graphics.** In libtcod both are the same mechanism (a character-code → tilesheet-image mapping), so a Cogmind-style toggle is idiomatic. Plan: ship ASCII-only first (zero art), add an optional tile mode later using CC0 tiles (Kenney/itch.io) — same grid, swapped tilesheet, no rearchitecting. Boundary: everything is grid-locked (equal cells, one tile per cell); free-form/large/animated sprites are out of scope for libtcod.

**Far-future (parked — do NOT build yet):** dungeons as longer combat-mode sequences (multi-room maps) with narration sprinkled in (merchant / story-NPC rooms). A dungeon is a content-scale expansion of combat mode, not an architecture change. Possibly a later/"sequel" expansion. Do not build dungeon systems before a single small encounter ships.

**Build note:** libtcod is a compiled lib with an SDL dependency — unlike header-only nlohmann/json. Smoothest install is vcpkg (`vcpkg install libtcod`, pulls SDL); FetchContent is possible but means managing SDL yourself.

**Godot detour (evaluated, parked):** Godot 4.7 was seriously evaluated — a POC lives at `~/projects/semnon_godot`. Set aside because Godot's GUI-heavy, mouse-driven *editor* workflow clashed with Chris's keyboard/VS Code development habits. Scoping note: this objection was strictly about *building the game in Godot* (the creation process) — it is NOT a game-design stance. The keyboard-focused game design stands on its own regardless of engine. The C++ project is the active direction; the Godot POC is kept as a fallback.

## Development plan

**Vertical slice first:** 3 hardcoded locations, pure terminal (std::cout + numbered menus), no JSON yet. Get the loop working, then layer on systems. (Narration prototyping — largely done.)

**Phase A:** Pure terminal display (narration prototyping).
**Phase B (revised):** Migrate the `Display` backend to **libtcod** — supersedes the old ncurses/SFML plan. libtcod hosts both narration and combat in one window (see _Design direction_). Migration stays cheap because game logic goes through the `Display` abstraction — swapping the concrete backend is contained.

## Architecture

Agreed and implemented classes:
- `GameState` — all mutable player/world state: flags, skills, counters, feats, inventory, faction standing, location/scene tracking
- `Display` (abstract) / `TerminalDisplay` — rendering interface; `TcodDisplay` planned for Phase B (libtcod, see _Design direction_); includes `renderMessage()` for prompt/system text
- `Scene` — owns flag-conditional descriptions (tuple: flag, text, art_path), `Option` list, and `Connection` list
- `Location` — owns scenes (map) and `defaultSceneId()`; no connections (moved to Scene)
- `Connection` — struct: `label`, `destination_location`, `destination_scene`, `requirements` (`vector<Requirement>`), `effects` (`vector<Effect>`) — fully implemented, including JSON loader wiring
- `LocationManager` — owns all locations by ID
- `Game` — orchestrator; owns `GameState`, `TerminalDisplay`, `LocationManager`; loop split into `buildOptions()`, `renderScene()`, `handleInput()`, `showMenu()`, `startNewGame()`
- `Menu` — owns entry list and input loop; `showMenu()` in Game constructs it and handles NewGame confirmation and Credits/Settings stubs
- `Option` — struct: `label`, `type` (Dialogue/Action/Move), `target_id`, `destination_location`, `destination_scene`, `requirements` (`vector<Requirement>`), `effects` (`vector<Effect>`) — the old `required_flag` was replaced by the requirements vector
- `Choice` — plain `enum class` in `Menu.hpp`: `Continue`, `NewGame`, `Settings`, `Credits`, `Exit`, `Quit`

Planned but not yet started: `Dialogue` / `DialogueNode` / `DialogueResponse` + `DialogueLoader` / `DialogueManager` (see the detailed plan under _Current state → Next step_); `TcodDisplay` (libtcod backend for both modes — supersedes the old `NcursesDisplay`/`SfmlDisplay` plan); combat-mode classes — a mode state machine (`NarrationMode` / `CombatMode`) and `CombatEncounter` (grid, actors, turn loop) returning an outcome that writes `GameState` flags. `SfmlDisplay` demoted to a possible far-future polish path (real fonts/images) behind the same `Display` seam.

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

`GameState` tracks flags (bool), counters (int), skills (int), inventory (int), faction standing (int), feats (bool). Every location exit, dialogue option, and NPC interaction can carry `requires` and `effects` fields — **now implemented** as `Requirement`/`Effect` structs (keyed by `StateType`) with `requirementsMet()` (visibility gating) and `applyEffects()` (state mutation), the shared channel that combat outcomes will also use. Early obscure choices seed consequences that pay off chapters later.

## Working mode

Default: **project mode** — guiding first, Chris writes the code. Explain logic and approach; provide pseudocode if needed. Only provide full code if explicitly asked or if Chris is clearly stuck after genuine attempts. See global CLAUDE.md for full mode rules.
