import unreal
import os

# ── 1. Load Blueprint classes ─────────────────────────────────────────
bp_gm   = unreal.load_class(None, "/Game/Blueprints/GameMode/BP_GamekuGameMode.BP_GamekuGameMode_C")
bp_char = unreal.load_class(None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C")
bp_pc   = unreal.load_class(None, "/Game/Blueprints/GameMode/BP_GamekuPlayerController.BP_GamekuPlayerController_C")

for name, cls in [("BP_GamekuGameMode", bp_gm), ("BP_FishingCharacter", bp_char), ("BP_GamekuPlayerController", bp_pc)]:
    unreal.log(f"[Config] {'OK' if cls else 'GAGAL'} load class: {name}")

# ── 2. Edit DefaultGame.ini langsung ─────────────────────────────────
config_path = os.path.join(unreal.SystemLibrary.get_project_directory(), "Config", "DefaultGame.ini")
unreal.log(f"[Config] Editing: {config_path}")

# Baca existing content
with open(config_path, "r", encoding="utf-8") as f:
    content = f.read()

section = "/Script/EngineSettings.GameMapsSettings"

# Cari map Main
main_map_path = "/Game/Main"
for candidate in ["/Game/Maps/Main", "/Game/Main", "/Game/Levels/Main"]:
    if unreal.EditorAssetLibrary.does_asset_exist(candidate):
        main_map_path = candidate
        break
unreal.log(f"[Config] Map path: {main_map_path}")

new_entries = {
    "GlobalDefaultGameMode": "/Game/Blueprints/GameMode/BP_GamekuGameMode.BP_GamekuGameMode_C",
    "GameDefaultMap": main_map_path,
}

# Parse dan update section
lines = content.splitlines()
in_section = False
keys_written = set()
new_lines = []

for line in lines:
    stripped = line.strip()
    if stripped.startswith("["):
        # Sebelum pindah section, tulis key yang belum ada
        if in_section:
            for key, val in new_entries.items():
                if key not in keys_written:
                    new_lines.append(f"{key}={val}")
                    keys_written.add(key)
        in_section = stripped == f"[{section}]"
        new_lines.append(line)
        continue

    if in_section:
        for key in new_entries:
            if stripped.startswith(f"{key}="):
                new_lines.append(f"{key}={new_entries[key]}")
                keys_written.add(key)
                break
        else:
            new_lines.append(line)
    else:
        new_lines.append(line)

# Kalau section tidak ditemukan, tambahkan di akhir
if not in_section and len(keys_written) < len(new_entries):
    # Section mungkin sudah diproses, cek
    pass

if len(keys_written) < len(new_entries):
    new_lines.append(f"\n[{section}]")
    for key, val in new_entries.items():
        if key not in keys_written:
            new_lines.append(f"{key}={val}")

with open(config_path, "w", encoding="utf-8") as f:
    f.write("\n".join(new_lines))

unreal.log("[Config] DefaultGame.ini updated")

# ── 3. Set Class Defaults di BP_GamekuGameMode ────────────────────────
if bp_gm:
    gm_cdo = unreal.get_default_object(bp_gm)

    if bp_char:
        gm_cdo.set_editor_property("default_pawn_class", bp_char)
        unreal.log("[Config] Default Pawn Class → BP_FishingCharacter")

    if bp_pc:
        gm_cdo.set_editor_property("player_controller_class", bp_pc)
        unreal.log("[Config] Player Controller Class → BP_GamekuPlayerController")

    unreal.EditorAssetLibrary.save_asset("/Game/Blueprints/GameMode/BP_GamekuGameMode")
    unreal.log("[Config] BP_GamekuGameMode saved")

unreal.log("=" * 50)
unreal.log("[Config] SELESAI! Restart UE editor agar DefaultGame.ini terbaca.")
unreal.log("=" * 50)
