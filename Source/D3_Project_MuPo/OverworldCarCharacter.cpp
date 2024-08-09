// Fill out your copyright notice in the Description page of Project Settings.


#include "OverworldCarCharacter.h"
#include "Camera/CameraComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AOverworldCarCharacter::AOverworldCarCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    CarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarMesh"));
    CarMesh->SetupAttachment(RootComponent);

    MoveSpeed = 600.0f;  // Character movement speed
    TurnSpeed = 50.0f;   // Character turn speed

    static ConstructorHelpers::FObjectFinder<UInputAction> MoveAction(TEXT("/Game/Blueprints/Inputs/CarInputs/IA_MoveForward.IA_MoveForward"));
    if (MoveAction.Succeeded())
    {
        IA_MoveForward = MoveAction.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputAction> TurnAction(TEXT("/Game/Blueprints/Inputs/CarInputs/IA_Turn.IA_Turn"));
    if (TurnAction.Succeeded())
    {
        IA_Turn = TurnAction.Object;
    }

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> CarContext(TEXT("/Game/Blueprints/Inputs/CarInputs/IMC_Car.IMC_Car"));
    if (CarContext.Succeeded())
    {
        IMC_Car = CarContext.Object;
    }
    
    UCameraComponent* TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
    TopDownCameraComponent->SetupAttachment(RootComponent);
    TopDownCameraComponent->SetRelativeLocation(FVector(-500.f, 0.f, 500.f)); // Adjust as needed
    TopDownCameraComponent->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f)); // Adjust as needed
    TopDownCameraComponent->bUsePawnControlRotation = false; // We don't want the camera to rotate with the car
}

void AOverworldCarCharacter::BeginPlay()
{
    Super::BeginPlay();

    InitializeInputMappings();

    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(nullptr);
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }    
}


void AOverworldCarCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!CurrentVelocity.IsZero())
    {
        AddMovementInput(GetActorForwardVector(), CurrentVelocity.X * DeltaTime);
    }

    if (CurrentTurnRate != 0.0f)
    {
        AddControllerYawInput(CurrentTurnRate * DeltaTime);
    }
}

void AOverworldCarCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_MoveForward, ETriggerEvent::Triggered, this, &AOverworldCarCharacter::Move);
        EnhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AOverworldCarCharacter::Turn);
    }
}

void AOverworldCarCharacter::Move(const FInputActionValue& Value)
{
    CurrentVelocity.X = Value.Get<float>() * MoveSpeed;
    UE_LOG(LogTemp, Error, TEXT("Move called with value: %f"), Value.Get<float>());
}

void AOverworldCarCharacter::Turn(const FInputActionValue& Value)
{
    CurrentTurnRate = Value.Get<float>() * TurnSpeed;
    UE_LOG(LogTemp, Error, TEXT("Turn called with value: %f"), Value.Get<float>());
}

void AOverworldCarCharacter::InitializeInputMappings()
{
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(IMC_Car, 1);
            UE_LOG(LogTemp, Warning, TEXT("Input context for car reset and added"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get UEnhancedInputLocalPlayerSubsystem"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get PlayerController"));
    }
}

