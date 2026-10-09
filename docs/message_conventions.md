> This file is maintained by [Claude](https://claude.ai) as a living project reference. Contents may be updated automatically during development sessions.

# Error and Warning Message Conventions

Agreed on 2026-10-09. Applies to every message the program puts into an exception or prints to `std::cerr`.

Format: `SEVERITY: Where: message.`

## Severity

| Severity | When | How |
|---|---|---|
| `ERROR` | the program can't continue | `throw std::runtime_error(...)` |
| `WARNING` | the program carries on | `std::cerr << ... << "\n";` |

No other severities (no `DEBUG`), no `!!!` banners.

## Where

The class or module name of the source file the message comes from: `Dialogue`, `LocationManager`, `Game`, `ReactivityParser`, `StateType`, ... In this project that's always the file name without its extension, so you know at a glance which file to open.

## Message shapes

| Situation | Shape | Example |
|---|---|---|
| something is missing | `<id> has no <thing>.` | `ERROR: Location: askas_rest has no default scene.` |
| a named thing is missing | `<id> has no <thing> <value>.` | `ERROR: Dialogue: barkeep_chat has no start node greting.` |
| missing, owner has no id (managers) | `has no <thing> <value>.` | `ERROR: LocationManager: has no location stret.` |
| unrecognised type value | `unknown <Type> <value>.` | `ERROR: StateType: unknown StateType Flg.` |
| an action failed | `couldn't <verb> <thing> <value>.` | `WARNING: TerminalDisplay: couldn't open art_path ./data/ascii/pint.txt.` |
| something isn't allowed there | `<thing> can't have <X>, <reason>.` | `ERROR: LocationLoader: option "Walk to the church." in scene common_room can't have OptionType Move, movement belongs in connections.` |

- Short form: no "a", no "defined".
- Always name the offending value.
- If the same message can come from several places, say which: `WARNING: GameState: has no skill strength, requested by getSkill.`
- Extra context goes after a comma: `ERROR: Dialogue: barkeep_chat has no node about_chruch, targeted by response "Ask about the church." in node greeting.`
- Errors raised while a location file is loaded get the file appended by `LocationLoader::load`, following the same comma rule: `ERROR: OptionType: unknown OptionType Actoin, in file askas_rest.json.` The original `Where` stays. Foreign messages (e.g. nlohmann's) get the `ERROR: LocationLoader:` prefix there, so they follow the format too.

## Spelling and quoting

- If a matching variable or type exists in the code at that spot, use its exact spelling (`art_path`, `OptionType`, `StateType`, `Move`). Otherwise use natural language (`art path`, `default scene`).
- **Ids are never quoted.** They're single words.
- **Player-facing text that can be long is quoted** with `"…"`: response and option labels.

## Rules

- One line per message, no `\n` inside.
- Every message ends with a period.
- Built with string concatenation, not `std::format` (decided after comparing both side by side).
- `std::cerr` lines end with `"\n"`, never `std::endl`. `std::cerr` is unbuffered, so the extra flush does nothing.
- Throw `std::runtime_error`, not `std::invalid_argument`. A typo in a JSON file is bad input met at runtime, which is what `runtime_error` is for; `invalid_argument` belongs to the `logic_error` family, meant for bugs in the code itself.
- A JSON value streamed with `<<` keeps its JSON quotes (it prints `""Trade""`). Extract the plain string first with `.get<std::string>()`.
- Enum values: print the name with `enumToString(value)` where the value is known to be valid. In a fallback for invalid values (e.g. after the `switch` in `enumToString` itself), print `static_cast<int>(value)`, since `enumToString` would throw there.

## Examples

```
ERROR: Scene: common_room has no default description.
ERROR: DialogueManager: has no dialogue barkeep_chat.
ERROR: OptionType: unknown OptionType Trade.
WARNING: Game: scene common_room has more than 10 options, excess options are unreachable.
WARNING: Game: Move option "Leave the tavern." has no destination.
WARNING: LocationLoader: unknown OptionType Trade.
```
