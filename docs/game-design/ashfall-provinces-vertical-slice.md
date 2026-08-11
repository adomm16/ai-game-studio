# Ashfall Provinces Vertical Slice

## Purpose

Prove one playable strategic chain: households produce resources, recruited households stop producing, companies move between provinces, casualties permanently remove households, and the weakened settlement economy remains visible after battle.

## Playable scope

- One small 3D town with 24 households and 8 building slots.
- Four resources: Food, Timber, Metal and Authority.
- Three soldier types: Spear, Ranged and Scout.
- At most four active companies.
- Six provinces, one AI rival, province movement and capture.
- Simple deterministic battle resolution.
- Save/load of simulation state.

## Core loop

1. Assign available households to Food, Timber or Metal production.
2. Advance the fixed simulation clock and collect resources.
3. Spend equipment and Authority to muster available households into a company.
4. Observe that mustered households no longer contribute settlement production.
5. Move the company to an adjacent province over deterministic travel ticks.
6. Resolve combat from company strength, unit counters and a seeded deterministic roll.
7. Apply casualties to the company and the household registry permanently.
8. Capture an undefended or defeated province, then continue with the reduced economy.

## Acceptance checks

- A new game creates exactly 24 living households, one settlement and six provinces.
- Muster rejects unavailable households and a fifth active company.
- Muster immediately reduces the next production result.
- Travel only begins between adjacent provinces and completes after the declared tick count.
- Casualties never exceed company membership and dead households never return to production.
- Province ownership changes after a successful uncontested capture or victory.
- Saving and loading preserves resources, household life/muster state, companies, province ownership and simulation tick.
- Replaying the same commands from the same seed produces the same result.

## Explicitly deferred

MMO backend, multiplayer, online login, microtransactions, large/procedural worlds, MassEntity, World Partition optimization, complex diplomacy, technology trees and advanced graphics/VFX are outside this slice.
