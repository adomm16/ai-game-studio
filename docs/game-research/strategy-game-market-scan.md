# Strategy Game Market Scan

**Scope:** 21 decision-relevant games across PC/console 4X, RTS, city-building and mobile persistent-world strategy.

**Access date:** 2026-08-11. **Confidence:** High for advertised systems and business models; medium for friction themes, which are qualitative patterns from store/community feedback rather than a statistical player survey.

**Decision boundary:** This scan recommends design directions only. It does not select a final concept or authorize production.

## Executive read

The strongest opportunity is not “a smaller MMO.” It is a single-player-first 3D strategy slice where a compact settlement produces a personally meaningful army, that army moves on a readable regional map, and one decisive battle changes the settlement. The architecture can keep authoritative state and deterministic commands ready for later co-op/PvP, but accounts, alliances, live operations and monetized timers should wait.

## 1. Systems players repeatedly value

- **Visible causality:** farms, workshops and logistics visibly become equipment, soldiers and battlefield capacity. *Anno 1800*, *Manor Lords* and *Farthest Frontier* make the economy feel physical.
- **Meaningful geography:** terrain shapes city specialization, routes and fights rather than serving as decoration. District placement, island trade, zones and region resources make expansion a strategic choice.
- **Readable army identity:** a small number of unit roles, counters, formations and persistent veterans produces ownership without requiring hundreds of units.
- **Medium-term goals and pressure:** seasons, orders, crises, settlement objectives and telegraphed assaults prevent passive optimization from becoming endless waiting.
- **Asymmetry and replay:** different starting constraints, procedural regions, technology offers or policies invite adaptation more cheaply than huge content volume.
- **Social interdependence:** guild help, rallies and co-op objectives are sticky when they create coordination, not merely compulsory attendance.

## 2. Systems that commonly create friction

- Long construction/research timers, stacked currencies, stamina, daily checklists and paid acceleration turn planning into calendar management.
- Late-game micromanagement grows faster than decision value: repeated building queues, route repairs, unit shuffling and many low-impact notifications.
- Opaque combat resolution, hidden modifiers and unreliable autoresolve break the economy-to-war promise.
- Harsh snowballing makes one mistake or an early power gap invalidate hours of play; total-loss cascades need recovery options.
- Too many parallel progression tracks fragment mastery. A player should understand whether the next upgrade improves the city, army or campaign goal.
- Multiplayer waiting, desync and offline vulnerability make persistent worlds expensive both technically and emotionally.

## 3. Overused clichés

- Generic medieval kingdom with interchangeable wood/stone/food/gold and a linear “Town Hall level” ladder.
- Collectible rarity-colored heroes whose main purpose is numerical power and monetization.
- World-map resource nodes, marching lines and alliance territory copied without terrain-level decisions.
- Zombie shelter reskins whose activity structure is unchanged from kingdom builders.
- Technology trees made mostly of repeated percentage bonuses.
- “MMO-scale” marketing while battles are reports or largely automatic comparisons of power scores.

## 4. Differentiation spaces

1. **The army is the city:** soldiers are named households/work crews; equipment and casualties alter production, morale and local stories.
2. **Mobile terrain, not resource nodes:** roads, weather fronts, river crossings and supply range change routes and battle readiness.
3. **Command bandwidth:** limited orders make a small force deep, reduce APM pressure and translate naturally to turns/ticks for networking.
4. **Reversible specialization:** districts and doctrine can be repurposed at a cost, avoiding solved build orders and permanent beginner traps.
5. **Cooperative threats before PvP:** shared bosses, sieges or seasonal hazards validate multiplayer architecture without pay-to-win power races.
6. **Short campaigns with persistent doctrine:** 45–90 minute regions can deliver a complete strategic arc while meta-progression unlocks options rather than raw power.

## 5. PC and mobile approach

| Dimension | PC-first expectation | Mobile-first expectation | Recommended slice position |
|---|---|---|---|
| Session | 30–120 minute focused play | Frequent 2–10 minute checks | 30–60 minute mission, safe pause/save |
| Input | Precision, hotkeys, multi-select | Touch targets, fewer simultaneous actions | Low-APM command groups and contextual orders |
| Depth | More systems and transparent numbers | Fewer visible systems, layered retention | Few systems with strong cross-effects |
| World | Solo sandbox or bounded campaign | Persistent shared map/events | Bounded single-player region using server-friendly state |
| Business | Premium/DLC accepted | F2P/IAP/pass common | Premium prototype; no monetized timers or power |
| Failure | Reload/retry is acceptable | Offline loss is highly sensitive | Recoverable defeat and explicit risk preview |

## 6. Systems that can grow from single-player to multiplayer

- **Simulation clock:** fixed ticks, pause/speed controls in solo; lockstep or authoritative ticks online.
- **Commands:** orders expressed as data (`move`, `gather`, `fortify`, `engage`) rather than direct animation calls.
- **World ownership:** region IDs, routes, garrisons and objectives work with AI now and player owners later.
- **Economy ledger:** explicit inputs/outputs and transaction events support sync, replay and anti-cheat later.
- **Army snapshots:** composition, equipment, morale and supply are serializable; battle instances can be hosted separately.
- **Scenario rules:** the slice's AI rival or world threat can later become co-op, asynchronous contest or seasonal server rule.
- **Deterministic seeds and replay logs:** useful now for debugging and balance; later for reconciliation and spectators.

The prototype should avoid assuming every future multiplayer feature is synchronous. Co-op regions and asynchronous trade/war orders are cheaper stepping stones than a seamless MMO.

## 7. Unnecessary for the first vertical slice

- Accounts, guilds, chat, matchmaking, live services, cross-server migration or a persistent backend.
- Multiple civilizations/factions, large hero rosters, cosmetics, battle passes, shops or monetization.
- Diplomacy webs, dynasty simulation, espionage, religion, naval warfare or global trade markets.
- Hundreds of buildings/resources, multi-city automation, procedural continents or a full campaign generator.
- Massive battles, complex siege destruction, day/night plus seasons plus full weather simulation.
- Fully featured tech trees; six to ten mutually meaningful upgrades are enough.
- Final art volume, voice acting and lore encyclopedia. One coherent region and one enemy identity are enough.

## Recommended vertical-slice proof

A 45–60 minute scenario should prove this chain: **scout region → choose settlement specialization → establish two production chains → recruit/equip 3–4 unit types → move under supply constraints → fight one field encounter → respond to consequences → win or lose one regional objective**. Success means players can explain why their economic choices changed the battle and want to try a different plan.

## Evidence notes

The full per-game synthesis and source URL are in [`competitor-matrix.csv`](competitor-matrix.csv). Representative primary/authorized sources include:

- 2K, “Civilization VI,” publication date not stated, accessed 2026-08-11, official product page, supports districts/research/victory framing, high confidence: https://civilization.2k.com/en-GB/civ-vi/
- World's Edge, “Quickstart Guide to Age of Empires IV,” 2021-10-28, accessed 2026-08-11, official guide, supports campaign/multiplayer and core RTS framing, high confidence: https://www.ageofempires.com/news/quickstart-guide-age-of-empires-iv/
- Ubisoft, “Anno 1800,” publication date not stated, accessed 2026-08-11, official product page, supports city, trade, multi-session and multiplayer features, high confidence: https://www.ubisoft.com/en-us/game/anno/1800
- Hooded Horse/Steam, “Manor Lords,” 2024 early-access description, accessed 2026-08-11, publisher/store page, supports gridless building, goods and tactical battles, high confidence: https://store.steampowered.com/app/1363080/_Manor_Lords/
- Crate Entertainment, “Farthest Frontier Major Features,” publication date not stated, accessed 2026-08-11, official guide, supports production, logistics, defense and technology scale, high confidence: https://www.farthestfrontier.com/guide/about/major-features/
- Apple App Store publisher listings for *Clash of Clans*, *Call of Dragons*, *Lords Mobile*, *State of Survival* and *Evony*, publication dates vary/not stated, accessed 2026-08-11, authorized storefront descriptions and disclosed IAP, medium-high confidence for current advertised features/business model.

**Conflict/limitation note:** Publishers have an incentive to emphasize strengths. Store reviews are self-selected and were used only to identify possible friction, not prevalence or causality. No revenue, audience-size or market-share claim is made. Final concept selection remains with Founder.
