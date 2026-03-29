# MathInvaderPRO - Full Pack
A Game that incorporates learning with gaming

## Overview

**MathInvaderPRO** is a math-based space invaders game built with Unreal Engine 5.3. Players must solve math problems to defeat waves of enemy invaders descending from above. The game combines the classic arcade fun of Space Invaders with educational math challenges.

## Gameplay

- **Answer to Destroy**: Each enemy invader displays a math problem. Solve it correctly to destroy the enemy!
- **Wave Progression**: Survive 5 waves of increasingly difficult enemy formations.
- **Multiple Difficulty Levels**: Easy, Medium, Hard, and Expert — matching math complexity to player skill.
- **Math Operations**: Choose from Addition, Subtraction, Multiplication, Division, or Mixed for variety.
- **Lives System**: Players start with 3 lives. Wrong answers and enemy projectiles cost lives.
- **Combo Scoring**: Consecutive correct answers multiply your score.

## Project Structure

```
MathInvaderPRO_FullPack/
├── MathInvaderPRO.uproject          # Unreal Engine project file
│
├── Config/                          # Engine and game configuration
│   ├── DefaultEngine.ini            # Renderer, physics, audio settings
│   ├── DefaultGame.ini              # Game maps, packaging settings
│   ├── DefaultInput.ini             # Input key bindings
│   └── DefaultEditor.ini            # Editor preferences
│
├── Source/                          # C++ source code
│   ├── MathInvaderPRO.Target.cs     # Game build target
│   ├── MathInvaderPROEditor.Target.cs  # Editor build target
│   └── MathInvaderPRO/
│       ├── MathInvaderPRO.Build.cs          # Module build rules
│       ├── MathInvaderPRO.h / .cpp          # Module entry point
│       ├── MathQuestionManager.h / .cpp     # Math question generation & validation
│       ├── MathInvaderPROGameMode.h / .cpp  # Wave management, scoring, lives
│       ├── MathInvaderPROGameState.h / .cpp # Replicated game state data
│       ├── MathInvaderPROPlayerController.h / .cpp  # Input handling, answer submission
│       ├── MathInvaderPROCharacter.h / .cpp # Player ship pawn
│       ├── MathInvaderPROEnemy.h / .cpp     # Enemy invader actor
│       ├── MathInvaderPROProjectile.h / .cpp # Projectile actor
│       ├── MathInvaderPROWaveSpawner.h / .cpp # Enemy formation spawning
│       ├── MathInvaderPROHUD.h / .cpp       # HUD management
│       └── MathInvaderPROSaveGame.h / .cpp  # Save/load game data
│
├── Content/                         # Game assets (see Content/README.md)
│   ├── Maps/                        # Level maps
│   ├── Blueprints/                  # Blueprint classes
│   ├── UI/Widgets/                  # UMG widget blueprints
│   ├── Textures/                    # Sprite and UI textures
│   ├── Sounds/                      # Music and SFX
│   ├── Animations/                  # 2D flipbook animations
│   ├── Materials/                   # Materials and instances
│   └── Meshes/                      # 3D/2D mesh assets
│
└── Build/                           # Platform build configuration
    ├── Win64/PakBlacklist-Shipping.txt
    ├── Linux/PakBlacklist-Shipping.txt
    ├── Mac/PakBlacklist-Shipping.txt
    ├── Android/PakBlacklist-Shipping.txt
    └── IOS/PakBlacklist-Shipping.txt
```

## Getting Started

### Prerequisites
- Unreal Engine 5.3 (download from [Epic Games Launcher](https://www.unrealengine.com/))
- Visual Studio 2022 (Windows) or Xcode 14+ (Mac) with C++ toolchain

### Setup
1. Clone this repository
2. Right-click `MathInvaderPRO.uproject` → **Generate Visual Studio project files**
3. Open `MathInvaderPRO.sln` in Visual Studio
4. Build the project (Development Editor configuration)
5. Launch the Unreal Editor by running the project
6. Create the required Blueprint and UMG widget assets (see `Content/README.md`)

### Building for Distribution
1. Open the project in Unreal Editor
2. Go to **Platforms** → select your target platform
3. Configure settings in **Project Settings → Packaging**
4. Click **Package Project**

## Controls

| Action | Keyboard | Gamepad |
|--------|----------|---------|
| Move Ship Left/Right | A/D or Arrow Keys | Left Stick |
| Fire Projectile | Space | Face Button Bottom (A/X) |
| Select Answer 1 | 1 | - |
| Select Answer 2 | 2 | - |
| Select Answer 3 | 3 | - |
| Select Answer 4 | 4 | - |
| Pause | Escape | Start |

## Core Systems

### MathQuestionManager
Generates and validates math questions. Supports:
- Configurable difficulty (Easy: 1–10, Medium: 1–20, Hard: 1–50, Expert: 1–100)
- Multiple-choice answers with plausible distractors
- Session accuracy tracking

### Wave System
- `AMathInvaderPROWaveSpawner` manages enemy formations
- Enemies move in classic Space Invaders step-down pattern
- Wave difficulty scales with speed multiplier and enemy type

### Scoring
- Base 100 points per enemy × enemy point value
- Combo multiplier (1.5×) for consecutive correct answers
- High scores saved via `UMathInvaderPROSaveGame`

## Target Platforms
- Windows (Win64)
- Linux
- macOS
- Android
- iOS

## License
Copyright 2024 Peteresi Games. All Rights Reserved.

