import unreal

eal  = unreal.EditorAssetLibrary
at   = unreal.AssetToolsHelpers.get_asset_tools()

# ── 1. Buat folders ───────────────────────────────────────────────────
for folder in ["/Game/Input", "/Game/Input/Actions"]:
    if not eal.does_directory_exist(folder):
        eal.make_directory(folder)
        unreal.log(f"[Input] Folder dibuat: {folder}")

# ── 2. Helper buat Input Action ───────────────────────────────────────
def make_ia(name, value_type):
    path = f"/Game/Input/Actions/{name}"
    if eal.does_asset_exist(path):
        unreal.log(f"[Input] Sudah ada: {name}")
        return unreal.load_asset(path)

    ia = at.create_asset(
        asset_name=name,
        package_path="/Game/Input/Actions",
        asset_class=unreal.InputAction,
        factory=unreal.InputActionFactory()
    )
    if ia:
        ia.set_editor_property("value_type", value_type)
        eal.save_asset(path)
        unreal.log(f"[Input] Dibuat: {name} ({value_type})")
    else:
        unreal.log_warning(f"[Input] GAGAL: {name}")
    return ia

BOOL   = unreal.InputActionValueType.BOOLEAN
AXIS1  = unreal.InputActionValueType.AXIS1_D
AXIS2  = unreal.InputActionValueType.AXIS2_D

ia_move     = make_ia("IA_Move",     AXIS2)
ia_look     = make_ia("IA_Look",     AXIS2)
ia_jump     = make_ia("IA_Jump",     BOOL)
ia_cast     = make_ia("IA_Cast",     AXIS1)
ia_reel     = make_ia("IA_Reel",     BOOL)
ia_interact = make_ia("IA_Interact", BOOL)

# ── 3. Buat Input Mapping Context ─────────────────────────────────────
imc_path = "/Game/Input/IMC_FishingCharacter"
if eal.does_asset_exist(imc_path):
    imc = unreal.load_asset(imc_path)
    unreal.log("[Input] Sudah ada: IMC_FishingCharacter")
else:
    imc = at.create_asset(
        asset_name="IMC_FishingCharacter",
        package_path="/Game/Input",
        asset_class=unreal.InputMappingContext,
        factory=unreal.InputMappingContextFactory()
    )
    unreal.log("[Input] Dibuat: IMC_FishingCharacter")

# ── 4. Tambah key mappings ────────────────────────────────────────────
def make_mapping(action, key_name, modifiers=None):
    m = unreal.EnhancedActionKeyMapping()
    m.set_editor_property("action", action)
    m.set_editor_property("key", unreal.Key(key_name))
    if modifiers:
        m.set_editor_property("modifiers", modifiers)
    return m

# Modifier instances
negate  = unreal.InputModifierNegate()
swizzle = unreal.InputModifierSwizzleAxis()

mappings = []

# Move: W=forward(+X), S=back(-X), D=right(+Y, swizzle), A=left(-Y, swizzle+negate)
mappings.append(make_mapping(ia_move, "W"))
mappings.append(make_mapping(ia_move, "S",              [negate]))
mappings.append(make_mapping(ia_move, "D",              [swizzle]))
mappings.append(make_mapping(ia_move, "A",              [swizzle, negate]))

# Look: mouse 2D
mappings.append(make_mapping(ia_look, "Mouse2D"))

# Jump
mappings.append(make_mapping(ia_jump, "SpaceBar"))

# Cast: hold LMB to charge, release to cast
mappings.append(make_mapping(ia_cast, "LeftMouseButton"))

# Reel: hold RMB
mappings.append(make_mapping(ia_reel, "RightMouseButton"))

# Interact: E
mappings.append(make_mapping(ia_interact, "E"))

imc.set_editor_property("mappings", mappings)
eal.save_asset(imc_path)
unreal.log(f"[Input] IMC selesai: {len(mappings)} mappings")

# ── 5. Assign ke BP_FishingCharacter CDO ─────────────────────────────
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
    unreal.log("[Input] BP_FishingCharacter: semua Input Actions di-assign")
else:
    unreal.log_warning("[Input] Gagal load BP_FishingCharacter class")

unreal.log("=" * 50)
unreal.log("[Input] SELESAI! Enhanced Input sudah dikonfigurasi.")
unreal.log("=" * 50)
