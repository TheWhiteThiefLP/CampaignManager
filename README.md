# D&D Campaign Manager

A local desktop campaign management application built for Dungeon Masters who prefer to have the entire campaign managed in one place.

## Technology Stack & Tools

* **Core Language:** **C++ (C++23)** — Chosen as its the language I have the most experience with and am most comfortable with
* **Build System & IDE:** **CMake & CLion** 
* **Data Serialization:** **`nlohmann/json`** — The data for each campaign is stored and linked using JSON as it was the first thing that came to mind 
* **Graphical User Interface (GUI):** **Qt 6 (Widgets & QGraphicsView)**
---

## Vision & Roadmap: What This Will Eventually Become

This project will hopefully evenually become a full standalone software that allows you to add homebrew content easily.
As copyright can be quite annoying I will not be adding any content to this outright and you will most likely have to create classes and items yourself.

Here is a "roadmap" of what is to come and in what steps I will be working on these. I will not make any promises in regards to release dates.
### Phase 1: Core Entity & Data Management (Current)
* [ ] Structured JSON data modeling for campaign assets (Items, NPCs, Spells, Monsters).
* [ ] Local file-system organization (`data/item/`, `data/npc/`, etc.).
* [ ] Main Menu window for swapping between campaigns
 
### Phase 2: Interactive Desktop UI & Workspace
* [ ] **Dockable Tool Panels:** Freely movable, snap-in, or tear-away floating windows for quick-reference sheets, session notes, and entity inspectors.
* [ ] **Entity Editor Forms:** Native input interfaces to create, modify, and instantly serialize campaign stats directly to disk.

### Phase 3: Tactical Map & World Canvas
* [ ] **Zoomable Map Viewer:** Support for world maps, regional overviews, and multi-level dungeon blueprints.
  * The plan for this is being able to click on a region and seeing all information that is linked to that place
* [ ] **Interactive Markers:** Click-to-place map pins representing cities, points of interest, monsters, and player tokens linked directly to local JSON entity files.

### Phase 4: Advanced DM Tools
* [ ] Initiative trackers and encounter builders integrated directly into the workspace.
  * Encounter builder will work similar to kobold fight club using the enemies you have added to the manager. 
* [ ] Quick-search indexing across all local campaign files for lookups mid-session.
  * This is probably gonna be an optional feature you can enable on an Item, NPC or other to allow use of this item across multiple campaigns
