# Wirus Mods Manager (WMM) – User Manual

## 1. Introduction
Wirus Mods Manager is a graphical mod manager for games, enabling convenient handling of collections, presets, tuples, and their installation into the game.

## 2. Getting Started

1. **Select the game and its directory**  
   - Go to **Settings → Main**.  
   - Click **Change Game** → choose the desired game.  
   - Click **Change Directory** → specify the path to the game's executable.  
   - Click **Apply**.  

2. **Create a restore point**  
   - In the same window click **Create a recovery point**.  
   - This creates a backup of the game's data for later restoration.  

## 3. Switching the Interface Language

1. Navigate to **Settings → Language → Choose Language**.  
2. Select the desired language and click **Apply**.  
3. Restart the application for the changes to take effect.  

## 4. Core Operations

| Step | Action | What Happens |
|------|--------|--------------|
| 1 | **New object** | Opens a dialog to create a new object. Enter a name and click **Apply**. |
| 2 | **Drag‑and‑drop** | At the bottom of the window there is a “Drag files here” area. Drag a mod archive here – it will be loaded into the manager’s database. |
| 3 | **Add Component** | Switch to the desired collection/preset, click the button, select components, and confirm with **Apply**. |
| 4 | **Assemble** | Select a collection and click **Assemble**. The compilation process starts; the waiting window disappears when finished. |
| 5 | **Link** | After assembly, click **Link** – the collection will be installed into the game. The waiting window disappears upon completion. |

## 5. Notes

- **Presets** are objects that can hold mods and tuples. They cannot be compiled. They are meant to help structure mods into packages.  
- **Collections** represent a set of mods. They may contain presets, mods, and tuples. A collection can be compiled and installed into the game. Cache is preserved between reinstalls.  
- **Tuples** are objects consisting of several versions of the same mod. In the interface they appear as a mod version.  
- To change a mod’s version, right‑click the displayed version in the collection.  
- Switching the **State** toggle of an object to **Off** will exclude that object from the next installation.
