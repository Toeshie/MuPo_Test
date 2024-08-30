// Fill out your copyright notice in the Description page of Project Settings.

#include "MarimbaCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"

AMarimbaCharacter::AMarimbaCharacter()
{
    if (CharacterMesh)
    {
        CharacterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f)); // Example of modifying properties
    }
    
    static ConstructorHelpers::FObjectFinder<UInputAction> MarimbaNoteHighAction(TEXT("InputAction'/Game/Blueprints/Inputs/MarimbaInputs/IA_MarimbaHighNote.IA_MarimbaHighNote'"));
    IA_MarimbaNoteHigh = Cast<UInputAction>(MarimbaNoteHighAction.Object);

    static ConstructorHelpers::FObjectFinder<UInputAction> MarimbaNoteLowAction(TEXT("InputAction'/Game/Blueprints/Inputs/MarimbaInputs/IA_MarimbalowNote.IA_MarimbaLowNote'"));
    IA_MarimbaNoteLow = Cast<UInputAction>(MarimbaNoteLowAction.Object);
    
    static ConstructorHelpers::FObjectFinder<UInputAction> Pause(TEXT("InputAction'/Game/Blueprints/Inputs/IA_Pause.IA_Pause'"));
    IA_Pause = Cast<UInputAction>(Pause.Object);

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> MarimbaContext(TEXT("InputMappingContext'/Game/Blueprints/Inputs/MarimbaInputs/IMC_Marimba.IMC_Marimba'"));
    IMC_Marimba = Cast<UInputMappingContext>(MarimbaContext.Object);

    // Load specific sounds for MarimbaCharacter
    static ConstructorHelpers::FObjectFinder<USoundCue> MarimbaHighNoteHitCue(TEXT("SoundCue'/Game/Sounds/MarimbaHighNoteHitCue.MarimbaHighNoteHitCue'"));
    MarimbaHighNoteHitSound = MarimbaHighNoteHitCue.Object;

    static ConstructorHelpers::FObjectFinder<USoundCue> MarimbaLowNoteHitCue(TEXT("SoundCue'/Game/Sounds/MarimbaLowNoteHitCue.MarimbaLowNoteHitCue'"));
    MarimbaLowNoteHitSound = MarimbaLowNoteHitCue.Object;

    static ConstructorHelpers::FObjectFinder<USoundCue> MarimbaMissNoteCue(TEXT("SoundCue'/Game/Sounds/MarimbaMissNoteCue.MarimbaMissNoteCue'"));
    MarimbaMissNoteSound = MarimbaMissNoteCue.Object;
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
    // Call the base class method
    Super::ValidateNoteHit(Value, true);  
}

// Override to handle the Marimba-specific low note input
void AMarimbaCharacter::HandleLowNoteInput(const FInputActionValue& Value)
{
    // Call the base class method
    Super::ValidateNoteHit(Value, false);  
}

// Override to handle the Marimba-specific pause menu toggle
void AMarimbaCharacter::ToggleProxyMenuPause()
{
    Super::ToggleProxyMenuPause();  // Call the base class method
}

void AMarimbaCharacter::PlaySound(USoundCue* SoundCue)
{
    if (SoundCue == nullptr) return;

    // Use Marimba-specific sound cues
    if (SoundCue == HighNoteHitSound)
    {
        Super::PlaySound(MarimbaHighNoteHitSound);
    }
    else if (SoundCue == LowNoteHitSound)
    {
        Super::PlaySound(MarimbaLowNoteHitSound);
    }
    else if (SoundCue == MissNoteSound)
    {
        Super::PlaySound(MarimbaMissNoteSound);
    }
    else
    {
        Super::PlaySound(SoundCue);  // Fallback to the provided sound cue
    }
}

void AMarimbaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_MarimbaNoteHigh.Get(), ETriggerEvent::Triggered, this, &AMarimbaCharacter::HandleHighNoteInput);
        EnhancedInputComponent->BindAction(IA_MarimbaNoteLow.Get(), ETriggerEvent::Triggered, this, &AMarimbaCharacter::HandleLowNoteInput);
        EnhancedInputComponent->BindAction(IA_Pause.Get(), ETriggerEvent::Triggered, this, &AMarimbaCharacter::ToggleProxyMenuPause);
    }
}

void AMarimbaCharacter::SetCharacterMesh(UStaticMesh* NewMesh)
{
    Super::SetCharacterMesh(NewMesh);  // Call the base class method
    // Additional logic specific to MarimbaCharacter, if needed
}