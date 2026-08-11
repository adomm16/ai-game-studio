# Game Concept Options

These are three original system combinations derived from competitor lessons, not copies of names, lore, UI or exact mechanics. They are decision options only; **no final concept is selected**.

## CONCEPT-A — The Moving Citadel

- **Short pitch:** Build a city on the back of a colossal walking machine, then choose when to anchor, harvest and march through a collapsing world.
- **World/theme:** Original post-ecological fantasy where habitable ground shifts beneath migrating storms; settlements survive by moving between temporary safe basins.
- **Player role:** Marshal-engineer responsible for a mobile civilian citadel and its expeditionary guard.
- **Core gameplay loop:** Scout route → anchor → deploy modular districts → harvest/refine → recruit/equip → choose departure timing → protect the march → reach the next basin.
- **City/kingdom development:** A compact 3D citadel has limited attachment sockets. Production, housing, defense and propulsion modules compete for space and power; modules can be moved between journeys at a cost.
- **Resource system:** Biomass sustains people, ore makes equipment, coolant enables movement, and morale supplies command capacity. Local resources are rich but storms impose a departure clock.
- **Units:** Four initial roles: pathfinders, shield infantry, ranged crews and repair teams. Civilians occupy jobs until mobilized, so army size directly slows production.
- **World map:** A branching route map of basins, chokepoints and moving weather fronts. Citadel weight, supply and discovered hazards change reachable routes.
- **Battle system:** Real-time with tactical pause and a limited command budget. The citadel is a moving objective; units screen flanks, secure salvage and repair modules.
- **Technology/progression:** Choose mutually exclusive engineering doctrines and unlock new module behaviors, not repeated percentage bonuses. Between regions, preserve blueprints and veteran traits rather than raw stockpiles.
- **Unique feature:** The base itself physically marches and becomes terrain during combat; every city-layout choice affects travel and defense.
- **Why it could be fun:** It unifies city, map and battle into one visible object, creates strong departure tension and makes reconfiguration expressive.
- **Why it could fail:** A moving modular city can become visually confusing; pathfinding and base/battle integration may consume the project.
- **Technical difficulty:** HIGH
- **First vertical slice:** One citadel deck; 6 modules; 4 resources; 4 unit roles; 3 connected basins; one storm clock; one ambush and one final crossing defense; 45–60 minutes.
- **Multiplayer expansion:** Two citadels can share routes, exchange modules and coordinate convoy defense; later, asynchronous route competition or 2v2 convoy interception.
- **Estimated development complexity:** High—custom moving-base navigation, modular placement, multi-layer AI and combat-state persistence. Small team estimate: 12–18 months to a polished slice after pre-production, highly engine-dependent.

## CONCEPT-B — Ashfall Provinces

- **Short pitch:** Rebuild a volcanic frontier where every household can become a military company, and winning a war may cripple the economy that funded it.
- **World/theme:** Original low-fantasy archipelago periodically reshaped by ashfall; fertile and mineral-rich provinces emerge, disappear or become unsafe.
- **Player role:** Elected warden balancing household promises, regional survival and defense against a rival expedition.
- **Core gameplay loop:** Survey province → assign household charters → produce food/materials/equipment → muster companies → issue map orders → fight → absorb casualties and political consequences → secure evacuation beacon.
- **City/kingdom development:** One organic 3D town plus two satellite outposts. Households specialize as growers, haulers, smiths or wardens; roads and storage determine actual throughput.
- **Resource system:** Food, timber, metal and authority. Equipment is physical inventory. Authority funds orders and emergency policies, then regenerates through fulfilled public promises.
- **Units:** Spear wardens, slingers, scouts and sappers, each formed from named households. Veterans gain one readable trait; casualties remove workers and can create dependents.
- **World map:** Six province nodes linked by roads with travel time, ash risk, supply radius and control. Player and AI orders resolve on a fixed campaign tick.
- **Battle system:** Small real-time formation battles with pause: facing, cohesion, elevation, fatigue and retreat. No hero abilities or individual action combat.
- **Technology/progression:** Six doctrine cards across civic logistics, metallurgy and fieldcraft; each unlock requires a demonstrated action and forces one tradeoff.
- **Unique feature:** The household ledger closes the loop between city labor, army composition, casualties, morale and political authority.
- **Why it could be fun:** Every soldier matters, victories have stories, and economy/war tradeoffs are immediately understandable.
- **Why it could fail:** Human consequences may feel punitive; household simulation could create UI overload or discourage combat.
- **Technical difficulty:** MEDIUM
- **First vertical slice:** One town; 24 households; 8 buildings; 4 resources; 4 companies; 6-node map; one rival; two skirmishes and one beacon defense; 50–70 minutes.
- **Multiplayer expansion:** Deterministic province ticks support co-op wardens or 1v1 campaigns; players can command separate companies in shared battles and trade explicit ledger contracts.
- **Estimated development complexity:** Medium—standard city logistics plus bounded formations and serializable world orders. Small team estimate: 8–12 months for a polished slice.

## CONCEPT-C — Signal Kingdoms

- **Short pitch:** Build a network of beacon-cities whose armies can only act where your living signal reaches; reshape the network to outmaneuver an adaptive wilderness.
- **World/theme:** Original luminous science-fantasy world covered by a perception-distorting fog. Communities survive inside synchronized beacon fields.
- **Player role:** Cartographer-regent rebuilding a broken signal network and deciding which settlements stay connected.
- **Core gameplay loop:** Triangulate fog → place/upgrade beacon → route energy and supplies → grow node settlement → assemble detachments → spend signal pulses on simultaneous orders → cleanse or capture anomaly → extend network.
- **City/kingdom development:** Instead of one large city, the player develops 3–5 small specialized nodes: foundry, granary, archive or barracks. Links provide bonuses but consume energy and can be disrupted.
- **Resource system:** Sustenance, alloy and signal. Local stocks travel as visible packets; signal is both network upkeep and the global command budget.
- **Units:** Survey drones, line guards, arc crews and relay engineers. Formations gain bonuses from nearby nodes and can deploy temporary relays.
- **World map:** A 3D tabletop-like continuous region. Visibility, supply and orders propagate through a player-built graph rather than a conventional territory color fill.
- **Battle system:** Command-pulse tactics: the player queues orders during a short planning phase, then both sides execute for several seconds. Terrain and signal latency change when orders arrive.
- **Technology/progression:** Recover knowledge fragments and choose one of three network protocols per run: reach, resilience or burst command. Unlocks add options, not permanent combat power.
- **Unique feature:** The same signal graph is fog of war, supply line, command resource and territorial identity, making infrastructure tactically active.
- **Why it could be fun:** Planning produces readable “I outsmarted it” moments; building and battle share one elegant rule; low APM suits PC and later touch input.
- **Why it could fail:** Pulsed execution may feel indirect, and network-state clarity demands excellent visualization and tutorial design.
- **Technical difficulty:** MEDIUM
- **First vertical slice:** Four beacon nodes; 3 resources; 4 unit roles; one 3D region; 3 protocol upgrades; one adaptive AI nest; 35–50 minutes.
- **Multiplayer expansion:** Fixed pulse windows naturally support deterministic co-op or competitive lockstep; players can own different network branches, share signal or jam rival links.
- **Estimated development complexity:** Medium—graph simulation and command playback are tractable, while clarity/VFX and deterministic execution are the main risks. Small team estimate: 7–11 months for a polished slice.

## A/B/C decision matrix

Scores are directional hypotheses (10 = strongest for the criterion), not approval. They should be validated with paper/greybox prototypes.

| Criterion | CONCEPT-A | CONCEPT-B | CONCEPT-C |
|---|---:|---:|---:|
| Fun potential | 9 | 8 | 8 |
| Differentiation | 10 | 7 | 9 |
| Technical feasibility | 5 | 8 | 8 |
| Small-team fit | 4 | 8 | 8 |
| Vertical-slice speed | 4 | 8 | 9 |
| Long-term expansion | 9 | 8 | 8 |
| Multiplayer potential | 8 | 8 | 9 |
| Commercial potential | 8 | 7 | 8 |
| **Total / 80** | **57** | **62** | **67** |

### Decision notes

- **CONCEPT-A** has the clearest trailer hook and highest differentiation, but its technical integration risk is materially above the others.
- **CONCEPT-B** most directly fulfills the requested city–army–world-map fantasy with moderate technical risk; its distinctiveness depends on making household consequences readable rather than punitive.
- **CONCEPT-C** is fastest to greybox and most network-ready; its main validation question is whether command pulses feel satisfying rather than detached.

**Final concept:** Not selected. Founder owns the final choice.
