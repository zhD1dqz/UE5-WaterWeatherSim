

#include "FlyController.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "TimerManager.h"

// Sets default values
AFlyController::AFlyController()
{
    // Set this pawn to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;
    
    // Create camera component
    CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    RootComponent = CameraComponent;
    
    // Set default values
    FlySpeed = 100.0f;
    MouseSensitivity = 0.1f;
    Acceleration = 10.0f;
    Deceleration = 5.0f;
    bIsRightMousePressed = false;
    CameraRotation = FRotator::ZeroRotator;
    MovementInput = FVector::ZeroVector;
    CurrentVelocity = FVector::ZeroVector;
    PlayerController = nullptr;
    EnhancedInputSubsystem = nullptr;
    
    // Initialize input references to nullptr (must be set in editor)
    InputMappingContext = nullptr;
    MoveAction = nullptr;
    LookAction = nullptr;
    RightMouseAction = nullptr;
    
    // Set auto possess player to player 0
    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AFlyController::BeginPlay()
{
    Super::BeginPlay();

    InitialFlySpeed = FlySpeed;
    GetWorldTimerManager().SetTimer(CameraSpeedBindTimer, this,
        &AFlyController::ConnectCameraSpeedSlider, 0.25f, true);
    
    // Get player controller reference
    PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        // Get Enhanced Input subsystem
        EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        
        // Add input mapping context if set
        if (EnhancedInputSubsystem && InputMappingContext)
        {
            EnhancedInputSubsystem->AddMappingContext(InputMappingContext, 0);
        }
        
        // Set initial input mode to Game and UI
        FInputModeGameAndUI InputModeData;
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputModeData.SetHideCursorDuringCapture(false);
        PlayerController->SetInputMode(InputModeData);
        PlayerController->SetShowMouseCursor(true);
    }
}

void AFlyController::ConnectCameraSpeedSlider()
{
    TArray<UUserWidget*> Widgets;
    UWidgetBlueprintLibrary::GetAllWidgetsOfClass(this, Widgets, UUserWidget::StaticClass(), false);
    for (UUserWidget* Widget : Widgets)
    {
        if (!Widget || !Widget->WidgetTree)
        {
            continue;
        }

        UVerticalBox* Content = Cast<UVerticalBox>(Widget->WidgetTree->FindWidget(FName(TEXT("VerticalBox_109"))));
        UWidget* WaterRow = Widget->WidgetTree->FindWidget(FName(TEXT("HorizontalBox_176")));
        if (!Content || !WaterRow)
        {
            continue;
        }

        USlider* Slider = Cast<USlider>(Widget->WidgetTree->FindWidget(FName(TEXT("CameraSpeedSlider"))));
        if (!Slider)
        {
            UHorizontalBox* Row = Widget->WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(TEXT("CameraSpeedRow")));
            UTextBlock* Label = Widget->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(TEXT("CameraSpeedLabel")));
            Slider = Widget->WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), FName(TEXT("CameraSpeedSlider")));
            UTextBlock* Readout = Widget->WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(TEXT("CameraSpeedReadout")));

            Label->SetText(FText::FromString(TEXT("Camera Speed")));
            Readout->SetText(FText::FromString(TEXT("1.0x")));
            if (UTextBlock* WaterLabel = Cast<UTextBlock>(Widget->WidgetTree->FindWidget(FName(TEXT("TextBlock_123")))))
            {
                Label->SetFont(WaterLabel->GetFont());
                Readout->SetFont(WaterLabel->GetFont());
                Label->SetColorAndOpacity(WaterLabel->GetColorAndOpacity());
                Readout->SetColorAndOpacity(WaterLabel->GetColorAndOpacity());
            }
            if (USlider* WaterSlider = Cast<USlider>(Widget->WidgetTree->FindWidget(FName(TEXT("WaterHeight")))))
            {
                Slider->SetWidgetStyle(WaterSlider->GetWidgetStyle());
            }
            Slider->SetToolTipText(FText::FromString(TEXT("Camera movement speed: 1x to 10x")));
            Slider->SetStepSize(0.1f);

            UHorizontalBoxSlot* LabelSlot = Row->AddChildToHorizontalBox(Label);
            LabelSlot->SetPadding(FMargin(0.0f, 0.0f, 12.0f, 0.0f));
            LabelSlot->SetVerticalAlignment(VAlign_Center);
            UHorizontalBoxSlot* SliderSlot = Row->AddChildToHorizontalBox(Slider);
            FSlateChildSize FillSize;
            FillSize.SizeRule = ESlateSizeRule::Fill;
            FillSize.Value = 1.0f;
            SliderSlot->SetSize(FillSize);
            SliderSlot->SetVerticalAlignment(VAlign_Center);
            UHorizontalBoxSlot* ReadoutSlot = Row->AddChildToHorizontalBox(Readout);
            ReadoutSlot->SetPadding(FMargin(10.0f, 0.0f, 0.0f, 0.0f));
            ReadoutSlot->SetVerticalAlignment(VAlign_Center);

            struct FExistingSlot
            {
                UWidget* Child;
                FSlateChildSize Size;
                FMargin Padding;
                EHorizontalAlignment HorizontalAlignment;
                EVerticalAlignment VerticalAlignment;
            };
            TArray<FExistingSlot> Tail;
            // Place the control after the existing wave sliders. This only moves
            // the action buttons and leaves the live weather widget untouched.
            UWidget* LastWaveRow = Widget->WidgetTree->FindWidget(FName(TEXT("HorizontalBox_3")));
            const int32 InsertAt = LastWaveRow ? Content->GetChildIndex(LastWaveRow) + 1
                                               : Content->GetChildIndex(WaterRow) + 1;
            while (Content->GetChildrenCount() > InsertAt)
            {
                UWidget* Child = Content->GetChildAt(InsertAt);
                UVerticalBoxSlot* OldSlot = CastChecked<UVerticalBoxSlot>(Child->Slot);
                Tail.Add({Child, OldSlot->GetSize(), OldSlot->GetPadding(),
                          OldSlot->GetHorizontalAlignment(), OldSlot->GetVerticalAlignment()});
                Content->RemoveChild(Child);
            }
            UVerticalBoxSlot* RowSlot = Content->AddChildToVerticalBox(Row);
            RowSlot->SetPadding(FMargin(0.0f, 3.0f, 0.0f, 3.0f));
            for (const FExistingSlot& Existing : Tail)
            {
                UVerticalBoxSlot* NewSlot = Content->AddChildToVerticalBox(Existing.Child);
                NewSlot->SetSize(Existing.Size);
                NewSlot->SetPadding(Existing.Padding);
                NewSlot->SetHorizontalAlignment(Existing.HorizontalAlignment);
                NewSlot->SetVerticalAlignment(Existing.VerticalAlignment);
            }
        }

        CameraSpeedSlider = Slider;
        CameraSpeedReadout = Cast<UTextBlock>(Widget->WidgetTree->FindWidget(FName(TEXT("CameraSpeedReadout"))));
        Slider->SetMinValue(1.0f);
        Slider->SetMaxValue(10.0f);
        Slider->SetValue(1.0f);
        Slider->OnValueChanged.AddDynamic(this, &AFlyController::HandleCameraSpeedChanged);
        HandleCameraSpeedChanged(1.0f);
        UE_LOG(LogTemp, Display, TEXT("Camera speed slider bound: base %.1f, range 1x-10x"), InitialFlySpeed);
        GetWorldTimerManager().ClearTimer(CameraSpeedBindTimer);
        return;
    }
}

void AFlyController::HandleCameraSpeedChanged(float Multiplier)
{
    const float ClampedMultiplier = FMath::Clamp(Multiplier, 1.0f, 10.0f);
    SetFlySpeed(InitialFlySpeed * ClampedMultiplier);
    if (CameraSpeedReadout)
    {
        CameraSpeedReadout->SetText(FText::FromString(FString::Printf(TEXT("%.1fx"), ClampedMultiplier)));
    }
}

// Called every frame
void AFlyController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // Calculate desired movement direction
    FVector ForwardDirection = CameraComponent->GetForwardVector();
    FVector RightDirection = CameraComponent->GetRightVector();
    FVector UpDirection = FVector::UpVector;
    
    // Target velocity based on input: X=Right, Y=Forward, Z=Up
    FVector TargetVelocity = (ForwardDirection * MovementInput.Y + RightDirection * MovementInput.X + UpDirection * MovementInput.Z) * FlySpeed;
    
    // Use Deceleration if input is zero (braking), Acceleration if input is active (speeding up)
    float InterpSpeed = MovementInput.IsNearlyZero() ? Deceleration : Acceleration;
    
    // Smoothly interpolate current velocity towards target velocity
    CurrentVelocity = FMath::VInterpTo(CurrentVelocity, TargetVelocity, DeltaTime, InterpSpeed);
    
    // Update actor position based on smoothed velocity
    AddActorWorldOffset(CurrentVelocity * DeltaTime * 100.0f);
    
    // Reset movement input for next frame
    MovementInput = FVector::ZeroVector;
}

// Set flying speed from blueprint
void AFlyController::SetFlySpeed(float NewSpeed)
{
    FlySpeed = FMath::Clamp(NewSpeed, 1.0f, 1000.0f);
}

// Get current flying speed
float AFlyController::GetFlySpeed() const
{
    return FlySpeed;
}

// Enhanced Input handlers
void AFlyController::HandleMove(const FInputActionValue& Value)
{
    MovementInput = Value.Get<FVector>();
}

void AFlyController::HandleLook(const FInputActionValue& Value)
{
    if (bIsRightMousePressed)
    {
        FVector2D LookVector = Value.Get<FVector2D>();
        
        // Update camera rotation
        CameraRotation.Yaw += LookVector.X * MouseSensitivity;
        CameraRotation.Pitch = FMath::Clamp(CameraRotation.Pitch + LookVector.Y * MouseSensitivity, -89.0f, 89.0f);
        
        // Apply rotation to camera
        CameraComponent->SetWorldRotation(CameraRotation);
    }
}

void AFlyController::HandleRightMousePressed(const FInputActionValue& Value)
{
    bIsRightMousePressed = true;
    
    if (PlayerController)
    {
        // Set input mode to game only when right mouse is pressed to prevent UI interaction
        FInputModeGameOnly InputModeData;
        PlayerController->SetInputMode(InputModeData);
        PlayerController->SetShowMouseCursor(false);
        PlayerController->SetIgnoreLookInput(false);
    }
}

void AFlyController::HandleRightMouseReleased(const FInputActionValue& Value)
{
    bIsRightMousePressed = false;
    
    if (PlayerController)
    {
        // Restore input mode to game and UI when right mouse is released
        FInputModeGameAndUI InputModeData;
        InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        InputModeData.SetHideCursorDuringCapture(false);
        PlayerController->SetInputMode(InputModeData);
        PlayerController->SetShowMouseCursor(true);
        PlayerController->SetIgnoreLookInput(true);
    }
}

// Called to bind functionality to input
void AFlyController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // Check if using Enhanced Input
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // Bind movement input (Vector3) - use Triggered for axis input
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AFlyController::HandleMove);
        }
        
        // Bind look input
        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AFlyController::HandleLook);
        }
        
        // Bind right mouse button
        if (RightMouseAction)
        {
            EnhancedInputComponent->BindAction(RightMouseAction, ETriggerEvent::Started, this, &AFlyController::HandleRightMousePressed);
            EnhancedInputComponent->BindAction(RightMouseAction, ETriggerEvent::Completed, this, &AFlyController::HandleRightMouseReleased);
        }
    }
}
