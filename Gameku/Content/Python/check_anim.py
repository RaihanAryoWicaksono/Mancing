import unreal

registry = unreal.AssetRegistryHelpers.get_asset_registry()

# List semua ABP
f = unreal.ARFilter(
    class_paths=[unreal.TopLevelAssetPath("/Script/Engine", "AnimBlueprint")],
    recursive_paths=True,
    package_paths=["/Game/"]
)
abps = registry.get_assets(f)
unreal.log_warning(f"=== ALL ABPs ({len(abps)}) ===")
for a in abps:
    unreal.log_warning(f"  {a.package_name}")

# Cek current anim_class di BP_FishingCharacter
bp_char_class = unreal.load_class(
    None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C"
)
cdo = unreal.get_default_object(bp_char_class)
mesh_comp = cdo.get_editor_property("mesh")
anim_class = mesh_comp.get_editor_property("anim_class")
unreal.log_warning(f"Current anim_class: {anim_class}")

# Cek skeleton SKM_Manny_Simple
sk_mesh = unreal.load_asset("/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple")
if sk_mesh:
    skeleton = sk_mesh.get_editor_property("skeleton")
    unreal.log_warning(f"SKM_Manny_Simple skeleton: {skeleton}")
