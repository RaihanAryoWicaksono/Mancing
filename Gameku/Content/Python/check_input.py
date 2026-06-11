import unreal

# Cek BP_FishingCharacter properties
bp_char_class = unreal.load_class(
    None, "/Game/Blueprints/Character/BP_FishingCharacter.BP_FishingCharacter_C"
)
cdo = unreal.get_default_object(bp_char_class)

imc  = cdo.get_editor_property("default_mapping_context")
move = cdo.get_editor_property("move_action")
look = cdo.get_editor_property("look_action")
jump = cdo.get_editor_property("jump_action")

unreal.log_warning(f"DefaultMappingContext : {imc}")
unreal.log_warning(f"MoveAction            : {move}")
unreal.log_warning(f"LookAction            : {look}")
unreal.log_warning(f"JumpAction            : {jump}")

# Cek Input Settings (Enhanced Input Component)
input_settings = unreal.InputSettings.get_default_object()
default_class = input_settings.get_editor_property("default_player_input_class")
default_comp  = input_settings.get_editor_property("default_input_component_class")
unreal.log_warning(f"DefaultInputComponentClass : {default_comp}")
unreal.log_warning(f"DefaultPlayerInputClass    : {default_class}")

# Cek IA_Move value type
ia_move = unreal.load_asset("/Game/Input/Actions/IA_Move")
if ia_move:
    vtype = ia_move.get_editor_property("value_type")
    unreal.log_warning(f"IA_Move ValueType : {vtype}")
