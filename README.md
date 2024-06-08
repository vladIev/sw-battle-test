# battle-simulator

## Objective
The goal is to demonstrate knowledge of design principles and ensure code simplicity and extensibility for the team. The code should be understandable to a Junior (closer to Middle) developer who is well-versed in language fundamentals and patterns. This requires an understanding of KISS, YAGNI, DRY, SOLID principles, low coupling, high cohesion, and a desire to write simple yet flexible code.

What Not to Do:
Overcomplicate the solution, making it difficult to understand.
Focus on performance optimization, as it is not evaluated in this task.
Use threads or include external libraries.
Spend too much time on the task (it's recommended not to get bogged down in details).
Time is not measured or restricted.
You can ask questions to clarify the goal and task during the process.

## Task
Create a simulation of unit battles on a map to experiment with mechanics. The map is H*W cells, with N units placed on it. Multiple units cannot occupy the same cell. Units of different types have various actions and characteristics. Each game tick, each unit can perform one action independently of others. The program receives a list of commands at startup and outputs events as they occur. The simulation ends when all commands are executed and no unit has an action for the next tick.

### For All Units:
* Parameter: Id - unique unit identifier, an integer.
* Characteristic: HP (Health Points)
If HP is 0, the unit dies and will not be on the map in the next tick.
The unit completes its action for the tick even if it dies during that tick.
* Action: Move

A unit can be sent to a point on the map.
Each tick, it can move to any of the 8 surrounding cells toward the target.
If the target cell is occupied, the unit does not move.
Pathfinding is unnecessary as there are no static obstacles on the map.

## Unit Types:
### Warrior

Characteristic: Strength
Action: Melee Attack
Takes precedence over Move.
If other units are within one cell radius, selects and attacks one unit.
Chooses the weakest by HP or the lowest by Id and fights until the enemy (or itself) dies.
Reduces the enemy's HP by its Strength per tick.

### Archer

Characteristics: Strength, Range, and Agility
Action: Melee Attack - same as Warrior.
Action: Range Attack
Takes precedence over Move and Melee Attack.
If other units are within 2 to Range cells radius, selects and attacks one unit.
Chooses the nearest, weakest by HP, or the youngest by Id and fights until the enemy (or itself) dies.
Reduces the enemy's HP by its Agility per tick.

## Future Development Plans
These do not need to be implemented now but should be considered as extension points:

New actions for units, such as Healing, Invisibility, Buffing.
New unit types, such as Healer, Mage, Mage-Warrior, etc.

## Input-Output
Input-output code is partially written; See IO folder.

The parser's value is not significant as it is expected to be replaced soon.
You may change anything in the implementation, but not the input-output format as scenario files are pre-prepared.

## Commands
At startup, the application receives the path to the scenario file via argv, containing commands. The following commands exist: 'CREATE_MAP', 'SPAWN_WARRIOR', 'SPAWN_ARCHER', 'MARCH', and 'WAIT'.

Scenario Example:

```cpp

// Format: COMMAND_NAME [...int args]
// Each command has specific properties at given argument positions

// create a map width=30 height=30 (can only be one and at the very beginning)
CREATE_MAP 30 30

// place a warrior id=1 at x=2 y=3 with hp=15 and strength=5
SPAWN_WARRIOR 1 2 3 15 5

// place an archer id=3 at x=19 y=20 with hp=10, strength=2, range=2, agility=3
SPAWN_ARCHER 3 19 20 10 2 2 3

// place an archer id=2 at x=13 y=23 with hp=10, strength=2, range=2, agility=3
SPAWN_ARCHER 2 13 23 10 2 2 3

// send unit id=1 to point 11 21
MARCH 1 11 21

// wait for 10 ticks before the next command
WAIT 10

// send unit id=3 to point 14 24
MARCH 3 14 24

// wait for 10 ticks before the next command
WAIT 10

// place a warrior id=4
SPAWN_WARRIOR 4 30 30 15 5

// send unit id=4 to point 16 20
MARCH 4 16 20

## Events
The program should output all occurring events to stdout. The following events exist: 'MAP_CREATED', 'UNIT_SPAWNED', 'MARCH_STARTED', 'MARCH_FINISHED', 'RECEIVED_DAMAGE', 'UNIT_DIED'.

```

Example Output:

```cpp
// TICK_NUMBER is the tick count when the event occurred
[TICK_NUMBER] MAP_CREATED 30 30 // map created
[TICK_NUMBER] UNIT_SPAWNED ID 1 TYPE Warrior ON 2 2 // warrior spawned at coordinates 2 2
[TICK_NUMBER] UNIT_SPAWNED ID 2 TYPE Archer ON 19 20 // archer spawned at coordinates 19 20 ...
// unit 1 movement started to coordinates 11 21 from 2 2
[TICK_NUMBER] MARCH_STARTED ID 1 FROM 2 2 TO 11 21
// unit 2 received 10 damage from unit 1, 10 HP remaining for unit 2
[TICK_NUMBER] RECEIVED_DAMAGE ID 2 DAMAGE 10 FROM_ID 1 REMAINING_HP 10 ...
// Unit 2 died
[TICK_NUMBER] UNIT_DIED ID 2
// unit 1 movement finished at coordinates 11 21
[TICK_NUMBER] MARCH_FINISHED ID 1 AT 11 21
```

Technical Requirements:
OS: Ubuntu
Compiler: clang 15+
Standard: C++17 and above
Build system: cmake (project must include CMakeLists.txt)
Publish the assignment in any git repository (GitHub, Bitbucket, etc.)
Ensure access via link without authentication
