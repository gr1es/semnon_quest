# semnon_quest — Design Brainstorm

Living document. Updated as decisions are made or revised. Includes both settled decisions and open questions.

---

## Concept

Location-based narrative RPG set in Chris's original PnP campaign world. Heavy text focus, ASCII art, strong reactivity to player choices. Closer to a visual novel / text adventure than a tile-based game. The core promise: early obscure choices seed consequences that pay off chapters later.

---

## Navigation

**Node map** (not pure scene-to-scene). Locations are nodes; connections between them are edges. The player moves via a simple node map — similar to roguelike maps — showing only discovered locations. Movement between nodes can take artificial time and can trigger scripted events mid-transit.

- Undiscovered locations are hidden until visited (or possibly until adjacent to a visited node — TBD).
- Phase A: numbered menu to select destination from connected nodes.
- Phase B: arrow key navigation on a rendered node map.

**Traversal events:** Connections (edges) can carry their own `requires` and `effects`, not just locations. Example: crossing a district node controlled by the thieves guild triggers an encounter if the player is on bad terms with them. Conflict is not purely negative — it creates drama and opportunities for the character's strengths to matter.

---

## Display / Rendering

Two-phase approach:

- **Phase A:** Pure terminal — `std::cout` + numbered menus. Build and stress-test all core systems here. Do not migrate until the feature set is satisfying.
- **Phase B:** ncurses — arrow key navigation, color, rendered node map. Migration is cheap because all rendering goes through a `Display` abstraction layer (one-line swap in `main()`).

Note: ncurses changes input handling (character-by-character vs. line-buffered). Numbered menus migrate cleanly; typed commands would need more care.

---

## Skill / Check System

**Flat checks** — no RNG. A check passes if the relevant skill meets or exceeds the required threshold. Rationale: RNG in text-heavy narrative games feels arbitrary without either a GM to improvise around bad rolls or massive written content for every outcome tier. Flat checks reward player investment and are easier to balance and design around.

**Potential future addition:** A fortune/misfortune mechanic to add flavor to the flat check system without full RNG — specifics TBD, needs narrative framing first.

---

## Character Creation

### Race
Gives small bonuses to certain skills, some world interactivity, and access to specific subclasses. Planned races include humans, wood elves, orcs, and others TBD.

### Background (= Class + Subclass)
Three generic class types, three subclasses each — nine backgrounds total. Background defines starting skills, starting gear, starting background feats, and ongoing narrative identity (the world reacts differently to a northern barbarian than a western mercenary).

**Martial type:**
- Paladin (standard / Hellknight)
- Fighter (soldier / mercenary)
- Northern Barbarian (human exclusive)

**Scoundrel type:**
- Criminal (thieves guild member / pirate)
- Specialist (brawler monk / gunslinger)
- Ranger (wood elf exclusive)

**Caster type:**
- Cleric (good-aligned / evil-aligned deity)
- Arcane magic user (wizard / sorcerer)
- Animist (orc exclusive)

Development plan: implement one race/background combo first, build outward from there.

### Gender
Player-selected at character creation. Not mechanically significant but offered for player expression. May influence some NPC dialogue.

---

## GameState

Single object tracking all mutable and immutable player/world state. The save file is a serialized snapshot of this object.

| Category | Type | Mutability | Notes |
|---|---|---|---|
| `playerName` | string | immutable | Set at character creation |
| `gender` | string | immutable | Set at character creation |
| `race` | string | immutable | Set at character creation |
| `background` | string | immutable | Set at character creation |
| `currentLocation` | string | mutable | Node ID of current location |
| `level` | int | mutable | |
| `experience` | int | mutable | |
| `discoveredLocations` | set\<string\> | mutable | Controls node map visibility |
| `flags` | map\<string, bool\> | mutable | World state facts |
| `skills` | map\<string, int\> | mutable | Flat-checked against thresholds |
| `counters` | map\<string, int\> | mutable | Time, rep, tracked quantities |
| `feats` | map\<string, bool\> | mutable | All feat categories (see below) |
| `inventory` | map\<string, int\> | mutable | Item ID → quantity |
| `factionStanding` | map\<string, int\> | mutable | Low priority, added when needed |

---

## Feats

All feats are stored as named booleans in `GameState` (`feats` map). Category is a data-layer property, not a structural one. Four categories:

- **General feats** — learnable anytime a feat slot is available, some with level or background requirements.
- **Skill feats** — tied to a specific skill; extend or modify how that skill works. Example: "Disable Lock" (lockpicking can make a door unopenable); "Arcane Bypass" (lockpicking works on magically enhanced locks without triggering alarms).
- **World feats** — earned through gameplay only: NPC teachers, quest rewards, hidden achievements/easter eggs. The reactivity payoff.
- **Background feats** — derived from race + background at character creation. Define cultural/social lens. World reacts to these (e.g. a northern barbarian is perceived differently in foreign territory than a local mercenary).

---

## Save System

- **Format:** JSON. Chris is learning JSON; treat implementation as a learning opportunity.
- **Slots:** Single save file for now. Multiple named slots added later via main menu.
- **Location:** `~/.local/share/semnon_quest/save.json` (XDG standard on Linux).
- **Timing:** Manual save + autosave on location transition. Implementation deferred until core loop is working.

---

## Progression (TBD)

- Experience + level as the baseline. Specific thresholds and level-up mechanics not yet decided.
- Level gates some general feats.
- Skill point distribution on level-up likely, but not confirmed.

---

## Open Questions

- Feat acquisition: when and how does the player select feats? Level-up screen? Found in world? Both?
- Progression: what does leveling up actually unlock beyond feat slots?
- Fortune/misfortune mechanic: how is it narratively framed? What triggers it?
- Faction standing: how central is it to the narrative? Full system or light touch?
- Node map: are adjacent-but-unvisited nodes visible (grayed out) or fully hidden?
- Races: full list not yet decided beyond humans, wood elves, orcs.
