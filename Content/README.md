# MathInvaderPRO - Content Directory

This directory contains all game assets for MathInvaderPRO.
Binary assets (.uasset, .umap) are created and managed within the Unreal Engine Editor.

## Directory Structure

```
Content/
├── Maps/                    # Level map files (.umap)
│   ├── MainMenu             # Main menu level
│   ├── Level_01             # Wave 1-2 (Easy difficulty)
│   ├── Level_02             # Wave 3-4 (Medium difficulty)
│   ├── Level_03             # Wave 5 (Hard difficulty / Boss)
│   ├── Level_GameOver       # Game over screen level
│   └── Level_Victory        # Victory screen level
│
├── Blueprints/              # Blueprint classes (visual scripting)
│   ├── Characters/          # BP_MathInvaderPROCharacter (player ship)
│   ├── Enemies/             # BP_Enemy_Basic, BP_Enemy_Fast, BP_Enemy_Tank, BP_Enemy_Boss
│   ├── Projectiles/         # BP_PlayerProjectile, BP_EnemyProjectile
│   └── UI/                  # BP_GameMode, BP_WaveSpawner
│
├── UI/                      # User Interface (UMG Widget Blueprints)
│   └── Widgets/
│       ├── WBP_GameHUD          # Main game HUD (score, lives, wave, question)
│       ├── WBP_PauseMenu        # Pause menu
│       ├── WBP_MainMenu         # Main menu screen
│       ├── WBP_HighScores       # High scores screen
│       ├── WBP_AnswerFeedback   # Correct/Wrong answer feedback widget
│       ├── WBP_WaveBanner       # Wave number announcement
│       └── WBP_GameOver         # Game over screen
│
├── Textures/                # Image assets (.uasset wrapping PNG/TGA source)
│   ├── Characters/          # Player ship sprite sheets and textures
│   ├── Enemies/             # Enemy invader sprites (4 types)
│   ├── UI/                  # UI icons, buttons, backgrounds
│   └── Backgrounds/         # Scrolling starfield and level backgrounds
│
├── Sounds/                  # Audio assets
│   ├── Music/               # Background music tracks per level
│   └── SFX/                 # Sound effects (fire, explosion, correct/wrong answer)
│
├── Animations/              # Flipbook animations for 2D sprites
├── Materials/               # Materials and Material Instances
└── Meshes/                  # Static meshes (ship, enemy, projectile)
```

## Asset Guidelines

- **Textures**: Source files go in `SourceArt/` (gitignored), import into Content/Textures/
- **Audio**: Import .wav files into Content/Sounds/
- **Fonts**: Import .ttf files for UI text rendering
- **Naming Convention**: Prefix assets with type (T_ for Texture, M_ for Material, WBP_ for Widget Blueprint, BP_ for Blueprint, SM_ for Static Mesh, SK_ for Skeletal Mesh)
