> This file is maintained by [Claude](https://claude.ai) as a living project reference. Contents may be updated automatically during development sessions.

# semnon_quest

Location-based narrative RPG in C++23 (switched from C++17 on 2026-10-08 — private project, 42's version limit doesn't apply; C++20/23 features like `map::contains`, designated initializers and `std::format` are fair game). Heavy text focus, ASCII art, strong reactivity to player choices. **Hybrid design:** a text-adventure narration mode (scene-to-scene navigation, visual-novel-like) plus short turn-based grid combat sequences, both rendered in a single window via libtcod. See _Design direction_ below.

## Current state

All core classes are implemented and compiling cleanly:
`GameState`, `Display`/`TerminalDisplay`, `Scene`, `SceneVariant`, `Location`, `Connection`, `LocationManager`, `Menu`, `Option`, `Game`, plus the reactivity primitives `Effect` / `Requirement` / `StateType`, the dialogue types `Dialogue` / `DialogueNode` / `DialogueResponse` + `DialogueManager`, and the shared JSON helper `ReactivityParser`.

Build system: root `Makefile` delegates to CMake. Executable lands at project root.
- `make` — build
- `make run` — build and launch
- `make re` — full rebuild

Compiler flags (explained in `CMakeLists.txt` comments): strict warning set `-Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast -Wnon-virtual-dtor -Woverloaded-virtual` plus `-Werror` (switchable via option `SEMNON_WARNINGS_AS_ERRORS`, e.g. if another machine's compiler version reports new warnings); standard C++ only (`CMAKE_CXX_EXTENSIONS OFF`); nlohmann/json included as `SYSTEM`. AddressSanitizer + UBSan available via option `SEMNON_SANITIZERS`, off until `libasan`/`libubsan` are installed (`sudo dnf install libasan libubsan`). `make re` wipes `build/`, so change an option's default in `CMakeLists.txt` rather than passing `-D`.

**Current:** JSON loading via `LocationLoader` (nlohmann/json, FetchContent). **Effects/requirements system implemented:** `Effect` and `Requirement` structs + `StateType` enum (Flag/Skill/Counter/Feat/Item/Standing), applied by free functions `applyEffects()` and `requirementsMet()`. `Option` and `Connection` each carry `std::vector<Requirement> requirements` + `std::vector<Effect> effects`. `buildOptions()` gates visibility via `requirementsMet()`; `handleInput()` runs `applyEffects()` on any selected option (so **Action options are fully functional**). `Requirement` supports flag/feat `expected` bools and skill/counter/item/standing `min`/`max` ranges (`optional<int>`).

**Variants leg (done — builds & runs):** scene descriptions replaced by requirement-gated `SceneVariant`s — struct `SceneVariant` (world/), JSON array key `variants`, text field `description`. a `requirements` array is parsed from JSON by `parseRequirements` (now in `ReactivityParser`, see below); `location.schema.json` and VSCode snippets migrated. Verified with `make run` (tavern↔street↔church traversal).

**Options/connections leg (done — builds & runs):** the loader now parses `requirements` AND `effects` arrays for options and connections via `parseRequirements` + `parseEffects` (legacy `required_flag` fully removed). `location.schema.json` refactored to `$defs`+`$ref` for the shared requirement/effect shapes; options drop `required_flag` and their authored `type` is restricted to Dialogue/Action (Move is synthesized at runtime from connections in `buildOptions()`, never authored). Snippets updated (added `effect`, reworked `option`). All three data files validate against the schema. **The whole requirements/effects reactivity data path — variants, options, connections — is now authorable from JSON.**

**Next step — dialogue system (in progress, status per step below):**

Data model mirrors the world model 1:1, so the same reactivity plumbing reuses directly:

| Dialogue | mirrors | World |
|---|---|---|
| `Dialogue` class (`const` members; `id()`, `startNode()`, `nodes()`, `getNode()`; constructor validates the node chain) | ↔ | `Location` class |
| `DialogueNode` (id, `text`, `vector<DialogueResponse> responses`) | ↔ | `Scene` |
| `DialogueResponse` (`label`, `vector<Requirement> requirements`, `vector<Effect> effects`, `target_node`; empty `target_node` = end conversation) | ↔ | `Option` |

Named `DialogueResponse`, not `DialogueOption`/`DialogueChoice` — those names collide with the existing `Option` struct and the `Menu`'s `Choice` enum. Gating/effects on `DialogueResponse` are the *only* place the dialogue system touches requirements/effects (no gates at the node or dialogue level) — reuses `Requirement`/`Effect`/`requirementsMet()`/`applyEffects()` as-is.

Build order (status as of 2026-10-09):
1. **[done]** Data types in `src/narrative/`. `DialogueNode`/`DialogueResponse` are plain structs. `Dialogue` is a class (all members `const`): its constructor throws unless `start_node` is non-empty and exists and every non-empty `target_node` exists; `getNode(id)` throws on unknown ids. Mirror on the world side: the `Location` constructor throws if `default_scene` is empty or doesn't exist.
2. **[done]** Shared JSON helpers in `src/utils/ReactivityParser.{hpp,cpp}`: `parseRequirements`/`parseEffects` (external linkage), `stringToStateType` file-local. Used by `LocationLoader`; `DialogueLoader` will use it too.
3. **[done]** `DialogueManager` (`src/world/`), an exact mirror of `LocationManager`: `getDialogue` (throws), `addDialogue` (keyed by `dialogue.id()`; a repeated id is ignored by design, see `LocationManager`), `hasDialogue`.
   **[todo]** `DialogueLoader`: header declares `static DialogueManager load(dir)`; `DialogueLoader.cpp` still to write (mirror `LocationLoader`, reading `data/dialogues/*.json`; build each `Dialogue` through its constructor; never use `map::operator[]` on dialogue maps, since `Dialogue` has no default constructor).
4. **[todo]** Cross-reference pass at the end of `startNewGame()`, after both loads: connection destinations must name an existing location and scene; Dialogue-type options' `target_id` must name an existing dialogue. Can't live in either manager (neither sees the other, and `directory_iterator` order means a target file may not be loaded yet). Needs read-only accessors (a manager's full map, `Location::scenes()`).
5. **[todo]** Runtime loop — a `Game::runDialogue(const std::string &id)` method (parallel to `showMenu()`): render current node's `text` via `Display`, show responses filtered by `requirementsMet()`, read choice, `applyEffects()` on the chosen response, follow `target_node` or end if empty.
6. **[todo]** Wire in: load dialogues in `startNewGame()`; the `OptionType::Dialogue` branch in `handleInput()` (currently a `TODO` stub) calls `runDialogue(chosen.target_id)`.

JSON shape (`data/dialogues/*.json`): top-level `id`/`start_node`/`nodes[]`; each node has `id`/`text`/`responses[]`; each response has `label`/`requirements[]`/`effects[]`/`target_node` — same `requirements`/`effects` array shape already used by variants/options/connections (reuses the schema's `$defs`).

**Then:** the pending `Display&`-injection refactor (Game still owns `TerminalDisplay` by value) that unblocks the libtcod backend and the `-t/--terminal` debug toggle.

## Next session plan (decided 2026-10-09 — bring this up at the start of the next session)

**Main focus: the `Move` rework (item 5).** Also close the load-error gaps (items 1–4). The dialogue system (`DialogueLoader.cpp` onward) continues after that.

1. **Missing `data/locations` directory** gives an unformatted `std::filesystem::filesystem_error`: the `directory_iterator` line in `LocationLoader::load()` sits outside the per-file `try`. Fix: check `std::filesystem::is_directory(directory)` first and throw `ERROR: LocationLoader: couldn't open directory <dir>.`
2. **`Scene`'s constructor validates nothing.** A scene without a default variant (one with no requirements) loads fine and only throws when the player enters it (`Scene::getDescription()`). Add a constructor check, mirroring `Location`/`Dialogue`, so it fails at load with the file name.
3. **Duplicate scene ids inside one location file** are silently dropped by `scenes.insert`. Decide: error (most likely a copy-paste mistake) or ignore, as for locations/dialogues.
4. **Wording mismatch:** the loader's Move message says `..., as movement belongs in connections.`; the example in `docs/message_conventions.md` has no "as". Align them.
5. **Design: `Move` is the odd one out in `OptionType`.** It's never authored, only synthesized from connections, so `stringToOptionType` accepts a value content must never use, the loader needs a special check, and `Option` carries two destination fields that authored options always leave empty. Candidate fix: `OptionType` = `Dialogue`/`Action` only; `buildOptions()` returns its own runtime menu-entry type (label + kind `Dialogue`/`Action`/`Move` + reference to the source `Option`/`Connection`). Decided: this is the **main focus of the next session**, done before continuing the dialogue system rather than waiting for the dialogue wiring.
6. Optional: `sudo dnf install libasan libubsan` on each machine, then switch `SEMNON_SANITIZERS` on.

Things that don't throw but arguably should: the `Scene` constructor (item 2) and `scenes.insert` (item 3). Deliberately left as is: `std::ifstream` (an unreadable file surfaces as a parse error) and `addLocation`/`addDialogue` ignoring duplicate ids (by design).

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
- `Scene` — owns requirement-gated `SceneVariant`s (`getDescription()`/`getArtPath()` return the first variant whose requirements are met, falling back to the one with no requirements), an `Option` list, and a `Connection` list
- `Location` — owns scenes (map) and `defaultSceneId()`; no connections (moved to Scene); constructor throws if the default scene is empty or missing
- `Connection` — struct: `label`, `destination_location`, `destination_scene`, `requirements` (`vector<Requirement>`), `effects` (`vector<Effect>`) — fully implemented, including JSON loader wiring
- `LocationManager` — owns all locations by ID. `addLocation` silently ignores an id that's already present — **by design, never an error**: content may be added again when it's unlocked on different occasions. Same for `DialogueManager::addDialogue`. Don't "fix" this into a throw.
- `Game` — orchestrator; owns `GameState`, `TerminalDisplay`, `LocationManager`; loop split into `buildOptions()`, `renderScene()`, `handleInput()`, `showMenu()`, `startNewGame()`
- `Menu` — owns entry list and input loop; `showMenu()` in Game constructs it and handles NewGame confirmation and Credits/Settings stubs
- `Option` — struct: `label`, `type` (Dialogue/Action/Move), `target_id`, `destination_location`, `destination_scene`, `requirements` (`vector<Requirement>`), `effects` (`vector<Effect>`) — the old `required_flag` was replaced by the requirements vector
- `Choice` — plain `enum class` in `Menu.hpp`: `Continue`, `NewGame`, `Settings`, `Credits`, `Exit`, `Quit`

In progress: the dialogue system (see _Current state → Next step_ for per-step status). Planned but not yet started: `TcodDisplay` (libtcod backend for both modes — supersedes the old `NcursesDisplay`/`SfmlDisplay` plan); combat-mode classes — a mode state machine (`NarrationMode` / `CombatMode`) and `CombatEncounter` (grid, actors, turn loop) returning an outcome that writes `GameState` flags. `SfmlDisplay` demoted to a possible far-future polish path (real fonts/images) behind the same `Display` seam.

## Naming conventions

- Classes: `UpperCamelCase`
- Methods: `lowerCamelCase`
- Member variables: `_lowerCamelCase`
- Method parameters: `snake_case`
- State flags: specific names — e.g. `"intimidated_barkeep_at_tavern"`, not `"talked_to_npc"`
- Git commits: Conventional Commits style — `type(scope): description`, lowercase, imperative mood
- Error/warning messages: follow [docs/message_conventions.md](docs/message_conventions.md). In short: `SEVERITY: Where: message.` (`ERROR` = throw, `WARNING` = program continues), `has no X` / `unknown X`, the offending value always named, ids unquoted, long labels quoted, one line, trailing period, string concatenation.

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
