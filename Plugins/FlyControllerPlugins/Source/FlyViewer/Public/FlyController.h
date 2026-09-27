

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "FlyController.generated.h"

class USlider;
class UTextBlock;

UCLASS(Blueprintable, BlueprintType)
class FLYVIEWER_API AFlyController : public APawn
{
    GENERATED_BODY()
    
public: 
    // Sets default values for this pawn's properties
    AFlyController();
    
protected: 
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
public: 
    // Camera component for the fly viewer
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fly Viewer")
    UCameraComponent* CameraComponent;
    
    // Base flying speed
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer", meta = (ClampMin = "1.0", ClampMax = "1000.0"))
    float FlySpeed;
    
    // Mouse sensitivity for camera rotation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer", meta = (ClampMin = "0.01", ClampMax = "1.0"))
    float MouseSensitivity;
    
    // How fast the movement reaches its maximum speed (higher is snappier)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float Acceleration;

    // How fast the movement stops when no input is provided (higher is snappier, lower is more "sliding")
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer", meta = (ClampMin = "1.0", ClampMax = "100.0"))
    float Deceleration;
    
    // Whether right mouse button is pressed
    UPROPERTY(BlueprintReadOnly, Category = "Fly Viewer")
    bool bIsRightMousePressed;
    
    // Input Mapping Context (must be set in editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer|Input")
    UInputMappingContext* InputMappingContext;
    
    // Input Actions (must be set in editor)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer|Input")
    UInputAction* MoveAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer|Input")
    UInputAction* LookAction;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fly Viewer|Input")
    UInputAction* RightMouseAction;
    
    // Set flying speed from blueprint
    UFUNCTION(BlueprintCallable, Category = "Fly Viewer")
    void SetFlySpeed(float NewSpeed);
    
    // Get current flying speed
    UFUNCTION(BlueprintPure, Category = "Fly Viewer")
    float GetFlySpeed() const;

private:
    UFUNCTION()
    void HandleCameraSpeedChanged(float Multiplier);

    void ConnectCameraSpeedSlider();

    UPROPERTY(Transient)
    TObjectPtr<USlider> CameraSpeedSlider;

    UPROPERTY(Transient)
    TObjectPtr<UTextBlock> CameraSpeedReadout;

    float InitialFlySpeed = 100.0f;

    FTimerHandle CameraSpeedBindTimer;
    
protected: 
    // Enhanced Input handlers
    void HandleMove(const FInputActionValue& Value);
    void HandleLook(const FInputActionValue& Value);
    void HandleRightMousePressed(const FInputActionValue& Value);
    void HandleRightMouseReleased(const FInputActionValue& Value);
    
    // Movement input vector
    FVector MovementInput;
    
    // Current camera rotation
    FRotator CameraRotation;
    
    // Current smoothed velocity
    FVector CurrentVelocity;
    
    // Player controller reference
    APlayerController* PlayerController;
    
    // Enhanced Input subsystem reference
    UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem;
};
