// Fill out your copyright notice in the Description page of Project Settings.

#include "MarimbaCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"


AMarimbaCharacter::AMarimbaCharacter()
{
    // Load new input actions for MarimbaCharacter
    static ConstructorHelpers::FObjectFinder<UInputAction> MarimbaNoteHighAction(TEXT("InputAction'/Game/Blueprints/Inputs/IA_MarimbaNoteHigh.IA_MarimbaNoteHigh'"));
    IA_MarimbaNoteHigh = Cast<UInputAction>(MarimbaNoteHighAction.Object);

    static ConstructorHelpers::FObjectFinder<UInputAction> MarimbaNoteLowAction(TEXT("InputAction'/Game/Blueprints/Inputs/IA_MarimbaNoteLow.IA_MarimbaNoteLow'"));
    IA_MarimbaNoteLow = Cast<UInputAction>(MarimbaNoteLowAction.Object);

    static ConstructorHelpers::FObjectFinder<UInputAction> Pause(TEXT("InputAction'/Game/Blueprints/Inputs/IA_Pause.IA_Pause'"));
    IA_Pause = Cast<UInputAction>(Pause.Object);

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> MarimbaContext(TEXT("InputMappingContext'/Game/Blueprints/Inputs/IMC_Marimba.IMC_Marimba'"));
    IMC_Marimba = Cast<UInputMappingContext>(MarimbaContext.Object);
    
}

void AMarimbaCharacter::BeginPlay()
{
    Super::BeginPlay();  // Call the base class's BeginPlay

    // Initialize Marimba-specific input mappings
    InitializeInputMappings();
}

void AMarimbaCharacter::InitializeInputMappings()
{
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();

            // Add the new input context for MarimbaCharacter
            Subsystem->AddMappingContext(IMC_Marimba, 1);
            UE_LOG(LogTemp, Warning, TEXT("Input context for marimba reset and added"));
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

// Ensure that the correct input actions are bound
void AMarimbaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_MarimbaNoteHigh.Get(), ETriggerEvent::Triggered, this, &AConcertCharacter::HandleHighNoteInput);
        EnhancedInputComponent->BindAction(IA_MarimbaNoteLow.Get(), ETriggerEvent::Triggered, this, &AConcertCharacter::HandleLowNoteInput);

        // Bind the pause action using the base class action (IA_Pause)
        EnhancedInputComponent->BindAction(IA_Pause.Get(), ETriggerEvent::Triggered, this, &AConcertCharacter::ToggleProxyMenuPause);
    }
}

