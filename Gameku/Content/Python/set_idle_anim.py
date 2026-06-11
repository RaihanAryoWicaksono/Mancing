import unreal

registry = unreal.AssetRegistryHelpers.get_asset_registry()
eal = unreal.EditorAssetLibrary

# Cari semua AnimSequence
f = unreal.ARFilter(
    class_paths=[unreal.TopLevelAssetPath("/Script/Engine", "AnimSequence")],
    recursive_paths=True,
    package_paths=["/Game/"]
)
anims = registry.get_assets(f)
unreal.log_warning(f"=== AnimSequences ({len(anims)}) ===")
for a in anims:
    unreal.log_warning(f"  {a.asset_name}  →  {a.package_name}")

# Cari idle animation
idle_anim = None
idle_keywords = ["idle", "stand", "mm_idle", "default", "mm_stand"]
for a in anims:
    name_lower = str(a.asset_name).lower()
    for kw in idle_keywords:
        if kw in name_lower:
            idle_anim = unreal.load_asset(str(a.package_name))
            unreal.log_warning(f"[Anim] Dipilih idle: {a.package_name}")
            break
    if idle_anim:
        break

if idle_anim:
    bp_char_class = unreal.load_class(
        None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C"
    )
    cdo = unreal.get_default_object(bp_char_class)
    mesh_comp = cdo.get_editor_property("mesh")

    # Ganti ke mode "Use Animation Asset"
    mesh_comp.set_editor_property("animation_mode", unreal.AnimationMode.ANIMATION_SINGLE_NODE)
    mesh_comp.set_editor_property("anim_to_play", idle_anim)
    mesh_comp.set_editor_property("anim_class", None)

    eal.save_asset("/Game/Blueprints/Character/BP_FishingCharacter")
    unreal.log_warning("[Anim] Idle animation di-set ke mesh!")
else:
    unreal.log_warning("[Anim] Tidak ada idle anim ditemukan — list animasi di atas")
