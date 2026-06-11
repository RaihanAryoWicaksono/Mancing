import unreal

eal      = unreal.EditorAssetLibrary
registry = unreal.AssetRegistryHelpers.get_asset_registry()

# Cari Animation Blueprint di project
f = unreal.ARFilter(
    class_paths=[unreal.TopLevelAssetPath("/Script/Engine", "AnimBlueprint")],
    recursive_paths=True,
    package_paths=["/Game/"]
)
abp_assets = registry.get_assets(f)

unreal.log_warning(f"=== Animation Blueprints ditemukan: {len(abp_assets)} ===")
for a in abp_assets:
    unreal.log_warning(f"  {a.package_name}")

# Pilih ABP untuk Manny
chosen_abp = None
preferred = ["abp_manny", "abp_mannequin", "abp_quinn"]
for asset in abp_assets:
    name_lower = str(asset.asset_name).lower()
    for kw in preferred:
        if kw in name_lower:
            chosen_abp = unreal.load_asset(str(asset.package_name))
            unreal.log_warning(f"[ABP] Dipilih: {asset.package_name}")
            break
    if chosen_abp:
        break

# Fallback: pakai yang pertama
if not chosen_abp and abp_assets:
    chosen_abp = unreal.load_asset(str(abp_assets[0].package_name))
    unreal.log_warning(f"[ABP] Fallback: {abp_assets[0].package_name}")

# Assign ke BP_FishingCharacter
if chosen_abp:
    bp_char_class = unreal.load_class(
        None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C"
    )
    if bp_char_class:
        cdo = unreal.get_default_object(bp_char_class)
        mesh_comp = cdo.get_editor_property("mesh")
        mesh_comp.set_editor_property("anim_class", chosen_abp.generated_class())
        eal.save_asset("/Game/Blueprints/Character/BP_FishingCharacter")
        unreal.log_warning("[ABP] Animation Blueprint di-assign ke BP_FishingCharacter!")
else:
    unreal.log_warning("[ABP] Tidak ada ABP ditemukan di project!")
