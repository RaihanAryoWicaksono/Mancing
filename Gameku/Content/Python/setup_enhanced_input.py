import unreal

eal = unreal.EditorAssetLibrary
at  = unreal.AssetToolsHelpers.get_asset_tools()

# ── 1. Folders ────────────────────────────────────────────────────────
for folder in ["/Game/Input", "/Game/Input/Actions"]:
    if not eal.does_directory_exist(folder):
        eal.make_directory(folder)
        unreal.log(f"[Input] Folder: {folder}")

# ── 2. Input Actions ──────────────────────────────────────────────────
def make_ia(name):
    path = f"/Game/Input/Actions/{name}"
    if eal.does_asset_exist(path):
        unreal.log(f"[Input] Sudah ada: {name}")
        return unreal.load_asset(path)
    ia = at.create_asset(
        asset_name=name,
        package_path="/Game/Input/Actions",
        asset_class=unreal.InputAction,
        factory=None
    )
    if ia:
        eal.save_asset(path)
        unreal.log(f"[Input] Dibuat: {name}")
    else:
        unreal.log_warning(f"[Input] GAGAL: {name}")
    return ia

ia_move     = make_ia("IA_Move")
ia_look     = make_ia("IA_Look")
ia_jump     = make_ia("IA_Jump")
ia_cast     = make_ia("IA_Cast")
ia_reel     = make_ia("IA_Reel")
ia_interact = make_ia("IA_Interact")

# ── 3. Input Mapping Context ──────────────────────────────────────────
imc_path = "/Game/Input/IMC_FishingCharacter"
if eal.does_asset_exist(imc_path):
    imc = unreal.load_asset(imc_path)
    unreal.log("[Input] Sudah ada: IMC_FishingCharacter")
else:
    imc = at.create_asset(
        asset_name="IMC_FishingCharacter",
        package_path="/Game/Input",
        asset_class=unreal.InputMappingContext,
        factory=None
    )
    unreal.log("[Input] Dibuat: IMC_FishingCharacter")

# ── 4. Key Mappings ───────────────────────────────────────────────────
def make_mapping(action, key_name, modifiers=None):
    m = unreal.EnhancedActionKeyMapping()
    m.set_editor_property("action", action)
    key = unreal.Key()
    key.set_editor_property("key_name", key_name)
    m.set_editor_property("key", key)
    if modifiers:
        m.set_editor_property("modifiers", modifiers)
    return m

negate  = unreal.InputModifierNegate()
swizzle = unreal.InputModifierSwizzleAxis()

mappings = [
    # WASD Movement
    make_mapping(ia_move, "W"),
    make_mapping(ia_move, "S",                 [negate]),
    make_mapping(ia_move, "D",                 [swizzle]),
    make_mapping(ia_move, "A",                 [swizzle, negate]),
    # Mouse Look
    make_mapping(ia_look, "Mouse2D"),
    # Jump
    make_mapping(ia_jump, "SpaceBar"),
    # Cast (hold LMB)
    make_mapping(ia_cast, "LeftMouseButton"),
    # Reel (RMB)
    make_mapping(ia_reel, "RightMouseButton"),
    # Interact (E)
    make_mapping(ia_interact, "E"),
]

imc.set_editor_property("mappings", mappings)
eal.save_asset(imc_path)
unreal.log(f"[Input] IMC: {len(mappings)} mappings ditambahkan")

# ── 5. Assign ke BP_FishingCharacter ─────────────────────────────────
bp_char_class = unreal.load_class(None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C")
if bp_char_class:
    cdo = unreal.get_default_object(bp_char_class)
    cdo.set_editor_property("default_mapping_context", imc)
    cdo.set_editor_property("move_action",     ia_move)
    cdo.set_editor_property("look_action",     ia_look)
    cdo.set_editor_property("jump_action",     ia_jump)
    cdo.set_editor_property("cast_action",     ia_cast)
    cdo.set_editor_property("reel_action",     ia_reel)
    cdo.set_editor_property("interact_action", ia_interact)
    eal.save_asset("/Game/Blueprints/Character/BP_FishingCharacter")
    unreal.log("[Input] BP_FishingCharacter: Input Actions di-assign")

unreal.log("=" * 50)
unreal.log("[Input] SELESAI!")
unreal.log("[Input] LANGKAH MANUAL - buka tiap asset dan ubah Value Type:")
unreal.log("[Input]   IA_Move     -> Axis2D (Vector2D)")
unreal.log("[Input]   IA_Look     -> Axis2D (Vector2D)")
unreal.log("[Input]   IA_Cast     -> Axis1D (float)")
unreal.log("=" * 50)
