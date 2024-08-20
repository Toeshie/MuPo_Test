// Fill out your copyright notice in the Description page of Project Settings.

#include "MarimbaCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

AMarimbaCharacter::AMarimbaCharacter()
{
    // Load new input actions for MarimbaCharacter
    static ConstructorHelpers::FObjectFinder<UInputAction> MarimbaNoteHighAction(TEXT("InputAction'/Game/Blueprints/Inputs/IA_MarimbaNoteHigh.IA_MarimbaNoteHigh'"));
    IA_MarimbaNoteHigh = Cast<UInputAction>(MarimbaNoteHighAction.Object);

    static ConstructorHelpers::FObjectFinder<UInputAction> MarimbaNoteLowAction(TEXT("InputAction'/Game/Blueprints/Inputs/IA_MarimbaNoteLow.IA_MarimbaNoteLow'"));
    IA_MarimbaNoteLow = Cast<UInputAction>(MarimbaNoteLowAction.Object);

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

// Override to handle the Marimba-specific high note input
void AMarimbaCharacter::HandleHighNoteInput(const FInputActionValue& Value)
{
    Super::HandleHighNoteInput(Value);  // Call the base class method
}

// Override to handle the Marimba-specific low note input
void AMarimbaCharacter::HandleLowNoteInput(const FInputActionValue& Value)
{
    Super::HandleLowNoteInput(Value);  // Call the base class method
}

// Override to handle the Marimba-specific pause menu toggle
void AMarimbaCharacter::ToggleProxyMenuPause()
{
    Super::ToggleProxyMenuPause();  // Call the base class method
}
