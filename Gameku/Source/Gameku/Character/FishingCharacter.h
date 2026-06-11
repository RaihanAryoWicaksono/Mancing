#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FishingCharacter.generated.h"

class UFishingRodComponent;
class UInventoryComponent;
class USkillTreeComponent;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Walking     UMETA(DisplayName = "Walking"),
    OnBoat      UMETA(DisplayName = "On Boat"),
    Casting     UMETA(DisplayName = "Casting"),
    Fishing     UMETA(DisplayName = "Fishing"),
    Reeling     UMETA(DisplayName = "Reeling"),
    Swimming    UMETA(DisplayName = "Swimming"),
    Interacting UMETA(DisplayName = "Interacting")
};

UCLASS()
class GAMEKU_API AFishingCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AFishingCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void StartCasting();

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void ReleaseCast();

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void StartReeling();

    UFUNCTION(BlueprintCallable, Category = "Fishing")
    void StopReeling();

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void TryInteract();

    UFUNCTION(BlueprintPure, Category = "State")
    EPlayerState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "State")
    void SetPlayerState(EPlayerState NewState);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UFishingRodComponent* FishingRodComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UInventoryComponent* InventoryComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkillTreeComponent* SkillTreeComponent;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraComponent* FollowCamera;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* CastAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* ReelAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* InteractAction;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);

private:
    EPlayerState CurrentState = EPlayerState::Idle;

    float CastChargeTime = 0.f;
    bool bIsChargingCast = false;

    UPROPERTY(EditDefaultsOnly, Category = "Fishing")
    float MaxCastChargeTime = 2.f;

    void UpdateCastCharge(float DeltaTime);
};
