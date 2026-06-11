import unreal

r = unreal.AssetRegistryHelpers.get_asset_registry()
f = unreal.ARFilter(
    class_paths=[unreal.TopLevelAssetPath("/Script/Engine", "SkeletalMesh")],
    recursive_paths=True,
    package_paths=["/Game/"]
)
assets = r.get_assets(f)

unreal.log_warning(f"=== SKELETAL MESH FOUND: {len(assets)} ===")
for a in assets:
    unreal.log_warning(str(a.package_name))

if len(assets) == 0:
    unreal.log_warning("=== TIDAK ADA MESH - Tambah Third Person content dulu! ===")
