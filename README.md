# 🚗 Spy Hunter-Inspired Arcade Game 🔫

This is a university project created for the **Basics of Computer Programming** course.  
The game is a fast-paced, top-down driving shooter built in C++ using the **SDL2** graphics library.

## 🎮 Features

- 🛣️ **Scrolling Road Gameplay** — Classic arcade-style racing experience
- 🔫 **Shooting Mechanic** — Fire projectiles to eliminate enemy cars
- 🚗 **AI-Controlled Traffic** — Enemy and neutral vehicles on the road
- ❤️ **Lives, Powerups, and Pickups** — Collect hearts to restore lives and power-ups to enhance gameplay
- 💾 **Save & Load Game State**
- ⚙️ **Game Over & Restart Functionality**
- ⏸️ **Pause, Immortality Timer, and Speed Boosts**

## 🎯 Controls

- `← → ↑ ↓` — Move your car  
- `SPACE` — Shoot  
- `P` — Pause  
- `N` — New Game  
- `F` — End Game  
- `ESC` — Quit  
- `S` — Save game *(function in progress)*  
- `L` — Load game *(function in progress)*  

## 🧱 Installation & Run

Make sure you have **SDL2** installed and set up correctly on Windows.

```bash
git clone https://github.com/yourusername/spy-hunter-game.git
cd spy-hunter-game
g++ Code.cpp -o spyhunter -I./SDL2-2.0.10/include -L./SDL2-2.0.10/lib -lmingw32 -lSDL2main -lSDL2 -std=c++11
spyhunter.exe
