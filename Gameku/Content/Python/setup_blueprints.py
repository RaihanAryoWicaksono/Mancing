"""
Jalankan script ini di UE Editor:
  Tools > Execute Python Script > pilih file ini
  ATAU via console: py "D:/Game Tubes/Gameku/Content/Python/setup_blueprints.py"
"""

import unreal

asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
editor_asset = unreal.EditorAssetLibrary
asset_registry = unreal.AssetRegistryHelpers.get_asset_registry()

# ── 1. Buat folder structure ──────────────────────────────────────────
folders = [
    "/Game/Blueprints",
    "/Game/Blueprints/Character",
    "/Game/Blueprints/Fishing",
    "/Game/Blueprints/GameMode",
    "/Game/Blueprints/Environment",
    "/Game/Blueprints/UI",
    "/Game/DataAssets",
    "/Game/DataAssets/Fish",
    "/Game/Maps",
]

for folder in folders:
    if not editor_asset.does_directory_exist(folder):
        editor_asset.make_directory(folder)
        unreal.log(f"[Setup] Folder dibuat: {folder}")
    else:
        unreal.log(f"[Setup] Folder sudah ada: {folder}")

# ── 2. Helper buat Blueprint ──────────────────────────────────────────
def create_blueprint(bp_name, parent_class, save_path):
    full_path = f"{save_path}/{bp_name}"

    if editor_asset.does_asset_exist(full_path):
        unreal.log(f"[Setup] Sudah ada: {full_path}")
        return unreal.load_asset(full_path)

    factory = unreal.BlueprintFactory()
    factory.set_editor_property("parent_class", parent_class)

    bp = asset_tools.create_asset(
        asset_name=bp_name,
        package_path=save_path,
        asset_class=unreal.Blueprint,
        factory=factory
    )

    if bp:
        editor_asset.save_asset(full_path)
        unreal.log(f"[Setup] Blueprint dibuat: {full_path}")
    else:
        unreal.log_warning(f"[Setup] GAGAL membuat: {full_path}")

    return bp

# ── 3. Buat Blueprint classes ─────────────────────────────────────────

# GameMode
create_blueprint(
    "BP_GamekuGameMode",
    unreal.load_class(None, "/Script/Gameku.GamekuGameMode"),
    "/Game/Blueprints/GameMode"
)

# Player Character
create_blueprint(
    "BP_FishingCharacter",
    unreal.load_class(None, "/Script/Gameku.FishingCharacter"),
    "/Game/Blueprints/Character"
)

# Player Controller
create_blueprint(
    "BP_GamekuPlayerController",
    unreal.load_class(None, "/Script/Gameku.GamekuPlayerController"),
    "/Game/Blueprints/GameMode"
)

# Fishing Spot
create_blueprint(
    "BP_FishingSpot",
    unreal.load_class(None, "/Script/Gameku.FishingSpot"),
    "/Game/Blueprints/Fishing"
)

# Weather System
create_blueprint(
    "BP_WeatherSystem",
    unreal.load_class(None, "/Script/Gameku.WeatherSystem"),
    "/Game/Blueprints/Environment"
)

# Economy Manager
create_blueprint(
    "BP_EconomyManager",
    unreal.load_class(None, "/Script/Gameku.EconomyManager"),
    "/Game/Blueprints/GameMode"
)

unreal.log("=" * 50)
unreal.log("[Setup] SELESAI! Semua Blueprint sudah dibuat.")
unreal.log("[Setup] Buka Edit > Project Settings > Maps & Modes")
unreal.log("[Setup] Set Default GameMode ke BP_GamekuGameMode")
unreal.log("=" * 50)
