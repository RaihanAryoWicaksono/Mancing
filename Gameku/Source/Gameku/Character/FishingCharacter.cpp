#include "Character/FishingCharacter.h"
#include "Fishing/FishingRodComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Progression/SkillTreeComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AFishingCharacter::AFishingCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.f;
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    FishingRodComponent = CreateDefaultSubobject<UFishingRodComponent>(TEXT("FishingRodComponent"));
    InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
    SkillTreeComponent = CreateDefaultSubobject<USkillTreeComponent>(TEXT("SkillTreeComponent"));

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
    GetCharacterMovement()->MaxWalkSpeed = 400.f;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
}

void AFishingCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AFishingCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateCastCharge(DeltaTime);
}

void AFishingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EIC->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EIC->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFishingCharacter::Move);
        EIC->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFishingCharacter::Look);
        EIC->BindAction(CastAction, ETriggerEvent::Started, this, &AFishingCharacter::StartCasting);
        EIC->BindAction(CastAction, ETriggerEvent::Completed, this, &AFishingCharacter::ReleaseCast);
        EIC->BindAction(ReelAction, ETriggerEvent::Started, this, &AFishingCharacter::StartReeling);
        EIC->BindAction(ReelAction, ETriggerEvent::Completed, this, &AFishingCharacter::StopReeling);
        EIC->BindAction(InteractAction, ETriggerEvent::Started, this, &AFishingCharacter::TryInteract);
    }
}

void AFishingCharacter::Move(const FInputActionValue& Value)
{
    if (CurrentState == EPlayerState::Fishing || CurrentState == EPlayerState::Reeling)
        return;

    FVector2D MovementVector = Value.Get<FVector2D>();
    if (Controller)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AFishingCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    if (Controller)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AFishingCharacter::StartCasting()
{
    if (CurrentState != EPlayerState::Idle && CurrentState != EPlayerState::Walking)
        return;

    bIsChargingCast = true;
    CastChargeTime = 0.f;
    SetPlayerState(EPlayerState::Casting);
}

void AFishingCharacter::ReleaseCast()
{
    if (!bIsChargingCast)
        return;

    bIsChargingCast = false;
    float CastPower = FMath::Clamp(CastChargeTime / MaxCastChargeTime, 0.1f, 1.f);
    FishingRodComponent->Cast(CastPower);
    SetPlayerState(EPlayerState::Fishing);
}

void AFishingCharacter::StartReeling()
{
    if (CurrentState != EPlayerState::Fishing)
        return;

    FishingRodComponent->StartReeling();
    SetPlayerState(EPlayerState::Reeling);
}

void AFishingCharacter::StopReeling()
{
    FishingRodComponent->StopReeling();
    if (CurrentState == EPlayerState::Reeling)
    {
        SetPlayerState(EPlayerState::Fishing);
    }
}

void AFishingCharacter::TryInteract()
{
    // Trace untuk cari interactable di depan pemain
    FVector Start = GetActorLocation();
    FVector End = Start + GetActorForwardVector() * 200.f;

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        if (AActor* HitActor = Hit.GetActor())
        {
            if (HitActor->Implements<UInteractableInterface>())
            {
                IInteractableInterface::Execute_Interact(HitActor, this);
            }
        }
    }
}

void AFishingCharacter::SetPlayerState(EPlayerState NewState)
{
    CurrentState = NewState;
}

void AFishingCharacter::UpdateCastCharge(float DeltaTime)
{
    if (bIsChargingCast)
    {
        CastChargeTime = FMath::Min(CastChargeTime + DeltaTime, MaxCastChargeTime);
    }
}
