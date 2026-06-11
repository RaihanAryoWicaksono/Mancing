import unreal

eal      = unreal.EditorAssetLibrary
registry = unreal.AssetRegistryHelpers.get_asset_registry()

# ── 1. Cari Skeletal Mesh di project ─────────────────────────────────
unreal.log("[Mesh] Mencari Skeletal Mesh di /Game/ ...")

ar_filter = unreal.ARFilter(
    class_names=["SkeletalMesh"],
    recursive_paths=True,
    package_paths=["/Game/"]
)
found_meshes = registry.get_assets(ar_filter)

if found_meshes:
    unreal.log(f"[Mesh] Ditemukan {len(found_meshes)} Skeletal Mesh:")
    for asset in found_meshes:
        unreal.log(f"[Mesh]   {asset.package_name}")
else:
    unreal.log_warning("[Mesh] Tidak ada Skeletal Mesh di /Game/")

# Prioritas: cari Mannequin
preferred_keywords = ["manny", "mannequin", "character", "hero", "player"]
chosen_mesh = None
chosen_path = None

for asset in found_meshes:
    name_lower = str(asset.asset_name).lower()
    for kw in preferred_keywords:
        if kw in name_lower:
            chosen_path = str(asset.package_name)
            chosen_mesh = unreal.load_asset(chosen_path)
            unreal.log(f"[Mesh] Dipilih: {chosen_path}")
            break
    if chosen_mesh:
        break

# Fallback: pakai mesh pertama yang ditemukan
if not chosen_mesh and found_meshes:
    chosen_path = str(found_meshes[0].package_name)
    chosen_mesh = unreal.load_asset(chosen_path)
    unreal.log(f"[Mesh] Fallback ke: {chosen_path}")

# ── 2. Assign mesh ke BP_FishingCharacter ─────────────────────────────
bp_char_class = unreal.load_class(
    None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C"
)

if bp_char_class and chosen_mesh:
    cdo = unreal.get_default_object(bp_char_class)

    mesh_comp = cdo.get_editor_property("mesh")
    mesh_comp.set_editor_property("skeletal_mesh_asset", chosen_mesh)

    # Offset standar UE: turun 90 unit, rotasi -90 yaw supaya hadap depan
    mesh_comp.set_editor_property(
        "relative_location", unreal.Vector(0.0, 0.0, -90.0)
    )
    mesh_comp.set_editor_property(
        "relative_rotation", unreal.Rotator(0.0, -90.0, 0.0)
    )

    eal.save_asset("/Game/Blueprints/Character/BP_FishingCharacter")
    unreal.log(f"[Mesh] Skeletal Mesh di-assign ke BP_FishingCharacter")
elif not chosen_mesh:
    unreal.log_warning("[Mesh] Tidak ada mesh ditemukan!")
    unreal.log_warning("[Mesh] Tambahkan Mannequin dulu:")
    unreal.log_warning("[Mesh]   Content Browser → Add → Add Feature or Content Pack")
    unreal.log_warning("[Mesh]   Pilih 'Third Person' → Add to Project")

# ── 3. Fix IMC - WASD mapping (W/D tadi kebalik) ─────────────────────
imc = unreal.load_asset("/Game/Input/IMC_FishingCharacter")
ia_move = unreal.load_asset("/Game/Input/Actions/IA_Move")

if imc and ia_move:
    def make_key(key_name):
        k = unreal.Key()
        k.set_editor_property("key_name", key_name)
        return k

    negate  = unreal.InputModifierNegate()
    swizzle = unreal.InputModifierSwizzleAxis()

    def make_mapping(action, key_name, modifiers=None):
        m = unreal.EnhancedActionKeyMapping()
        m.set_editor_property("action", action)
        m.set_editor_property("key", make_key(key_name))
        if modifiers:
            m.set_editor_property("modifiers", modifiers)
        return m

    ia_look     = unreal.load_asset("/Game/Input/Actions/IA_Look")
    ia_jump     = unreal.load_asset("/Game/Input/Actions/IA_Jump")
    ia_cast     = unreal.load_asset("/Game/Input/Actions/IA_Cast")
    ia_reel     = unreal.load_asset("/Game/Input/Actions/IA_Reel")
    ia_interact = unreal.load_asset("/Game/Input/Actions/IA_Interact")

    # Move code: X=Right, Y=Forward
    # W (forward → Y=1): butuh Swizzle supaya (1,0)→(0,1)
    # S (backward → Y=-1): Swizzle + Negate
    # D (right → X=1): no modifier
    # A (left → X=-1): Negate
    mappings = [
        make_mapping(ia_move, "W",                 [swizzle]),
        make_mapping(ia_move, "S",                 [swizzle, negate]),
        make_mapping(ia_move, "D"),
        make_mapping(ia_move, "A",                 [negate]),
        make_mapping(ia_look, "Mouse2D"),
        make_mapping(ia_jump, "SpaceBar"),
        make_mapping(ia_cast, "LeftMouseButton"),
        make_mapping(ia_reel, "RightMouseButton"),
        make_mapping(ia_interact, "E"),
    ]

    imc.set_editor_property("mappings", mappings)
    eal.save_asset("/Game/Input/IMC_FishingCharacter")
    unreal.log("[Mesh] IMC WASD mapping diperbaiki (W/D sudah benar)")

# ── 4. Pastikan Player Start ada di level ─────────────────────────────
world = unreal.EditorLevelLibrary.get_editor_world()
player_starts = unreal.GameplayStatics.get_all_actors_of_class(
    world, unreal.PlayerStart
)
if not player_starts:
    loc = unreal.Vector(0.0, 0.0, 100.0)
    rot = unreal.Rotator(0.0, 0.0, 0.0)
    unreal.EditorLevelLibrary.spawn_actor_from_class(unreal.PlayerStart, loc, rot)
    unreal.EditorLevelLibrary.save_current_level()
    unreal.log("[Mesh] Player Start ditambahkan ke level")
else:
    unreal.log(f"[Mesh] Player Start sudah ada: {len(player_starts)} buah")

unreal.log("=" * 50)
unreal.log("[Mesh] SELESAI! Coba Play (Alt+P) untuk test karakter.")
unreal.log("=" * 50)
