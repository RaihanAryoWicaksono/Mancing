import unreal

eal = unreal.EditorAssetLibrary

# Load Blueprint asset (bukan class)
bp_asset = unreal.load_asset("/Game/Blueprints/Character/BP_FishingCharacter")
bp_char_class = unreal.load_class(
    None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C"
)
cdo = unreal.get_default_object(bp_char_class)
mesh_comp = cdo.get_editor_property("mesh")

# Hapus ABP
mesh_comp.set_editor_property("anim_class", None)
unreal.log_warning("[Anim] anim_class set to None")

# Cek hasilnya
anim_class_now = mesh_comp.get_editor_property("anim_class")
unreal.log_warning(f"[Anim] anim_class sekarang: {anim_class_now}")

eal.save_asset("/Game/Blueprints/Character/BP_FishingCharacter")
unreal.log_warning("[Anim] Saved!")
unreal.log_warning("[Anim] SEKARANG: Buka BP_FishingCharacter → klik Compile → Play lagi")
