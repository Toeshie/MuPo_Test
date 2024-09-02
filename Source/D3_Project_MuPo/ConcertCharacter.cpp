// Fill out your copyright notice in the Description page of Project Settings.

#include "ConcertCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "ConcertGameMode.h"
#include "NiagaraComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Kismet/GameplayStatics.h"
#include "MainConcertCameraActor.h"
#include "NoteBaseClass.h"
#include "PauseMenuWidget.h"
#include "Components/BoxComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
AConcertCharacter::AConcertCharacter()
{
    // Set this character to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CharacterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CharacterMesh"));
    CharacterMesh->SetupAttachment(RootComponent);
    CharacterMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); 
    CharacterMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    InstrumentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InstrumentMesh"));
    InstrumentMesh->SetupAttachment(CharacterMesh); 
    InstrumentMesh->SetRelativeLocation(FVector(0.0f, 50.0f, 15.0f));
    InstrumentMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    
    
    // Initialize input actions
    static ConstructorHelpers::FObjectFinder<UInputAction> NoteHighAction(TEXT("InputAction'/Game/Blueprints/Inputs/IA_NoteHigh.IA_NoteHigh'"));
    IA_NoteHigh = Cast<UInputAction>(NoteHighAction.Object);

    static ConstructorHelpers::FObjectFinder<UInputAction> NoteLowAction(TEXT("InputAction'/Game/Blueprints/Inputs/IA_NoteLow.IA_NoteLow'"));
    IA_NoteLow = Cast<UInputAction>(NoteLowAction.Object);

    static ConstructorHelpers::FObjectFinder<UInputAction> Pause(TEXT("InputAction'/Game/Blueprints/Inputs/IA_Pause.IA_Pause'"));
    IA_Pause = Cast<UInputAction>(Pause.Object);

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> DrumsContext(TEXT("InputMappingContext'/Game/Blueprints/Inputs/IMC_Drums.IMC_Drums'"));
    IMC_Drums = Cast<UInputMappingContext>(DrumsContext.Object);

    //static ConstructorHelpers::FObjectFinder<USoundCue> HighNoteHitCue(TEXT("SoundCue'/Game/Sounds/HighNoteHitCue.HighNoteHitCue'"));
    //HighNoteHitSound = HighNoteHitCue.Object;

   // static ConstructorHelpers::FObjectFinder<USoundCue> LowNoteHitCue(TEXT("SoundCue'/Game/Sounds/LowNoteHitCue.LowNoteHitCue'"));
   // LowNoteHitSound = LowNoteHitCue.Object;

    static ConstructorHelpers::FObjectFinder<USoundCue> MissNoteCue(TEXT("SoundCue'/Game/Sounds/MissNoteCue.MissNoteCue'"));
    MissNoteSound = MissNoteCue.Object;

    static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/Blueprints/UI/PauseMenu"));
    if (PauseMenuBPClass.Class != nullptr)
    {
        PauseMenuClass = PauseMenuBPClass.Class;
    }
}

// Called when the game starts or when spawned
void AConcertCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    InitializeInputMappings();
    
    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance && GameInstance->GetSelectedCharacterMesh())
    {
        SetCharacterMesh(GameInstance->GetSelectedCharacterMesh());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Character mesh not set after level load."));
    }

    UStaticMesh* DrumMesh = LoadObject<UStaticMesh>(nullptr, TEXT("StaticMesh'/Game/Models/Base_All_Levels/Instruments_Drum.Instruments_Drum'"));
    if (DrumMesh)
    {
        SetInstrumentMesh(DrumMesh);
    }
    
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        FInputModeGameAndUI InputMode;
        InputMode.SetWidgetToFocus(nullptr);
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);
        PlayerController->bShowMouseCursor = false;
    }
   
    
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMainConcertCameraActor::StaticClass(), FoundActors);
    if (FoundActors.Num() > 0)
    {
        ConcertCameraActor = Cast<AMainConcertCameraActor>(FoundActors[0]);
    }

    if (PauseMenuClass)
    {
        PauseMenuWidgetInstance = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuClass);
    }
}



void AConcertCharacter::ToggleProxyMenuPause()
{
    if (PauseMenuWidgetInstance)
    {
        PauseMenuWidgetInstance->TogglePauseMenu();
    }
}

void AConcertCharacter::InitializeInputMappings()
{
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->ClearAllMappings();
            Subsystem->AddMappingContext(IMC_Drums, 1);
            UE_LOG(LogTemp, Warning, TEXT("Input context for drums reset and added"));
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

// Called every frame
void AConcertCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AConcertCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_NoteHigh.Get(), ETriggerEvent::Triggered, this, &AConcertCharacter::HandleHighNoteInput);
        EnhancedInputComponent->BindAction(IA_NoteLow.Get(), ETriggerEvent::Triggered, this, &AConcertCharacter::HandleLowNoteInput);
        EnhancedInputComponent->BindAction(IA_Pause.Get(), ETriggerEvent::Triggered, this, &AConcertCharacter::ToggleProxyMenuPause);
    }
}

void AConcertCharacter::HandleHighNoteInput(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Error, TEXT("High Note Input Pressed"));
    ValidateNoteHit(Value, true);
}

void AConcertCharacter::HandleLowNoteInput(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Error, TEXT("Low Note Input Pressed"));
    ValidateNoteHit(Value, false);
    
}

void AConcertCharacter::ValidateNoteHit(const FInputActionValue& Value, bool bIsHighNote)
{
    bool bNoteHit = false; // Track if a note was hit

    if (ConcertCameraActor && ConcertCameraActor->OverlappingNote)
    {
        ANoteBaseClass* Note = Cast<ANoteBaseClass>(ConcertCameraActor->OverlappingNote);
        if (Note)
        {
            bool bIsCorrectNoteType = (bIsHighNote && Note->ActorHasTag("HighNote")) || (!bIsHighNote && Note->ActorHasTag("LowNote"));
            UE_LOG(LogTemp, Error, TEXT("Correct note type: %s"), bIsCorrectNoteType ? TEXT("True") : TEXT("False"));

            if (bIsCorrectNoteType)
            {
                bNoteHit = true; // Note was hit
                AConcertGameMode* GameMode = Cast<AConcertGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
                if (GameMode)
                {
                    bool perfectHit = false;
                    bool goodHit = false;

                    // Activate Hit Niagara System
                    if (Hit_NGS)
                    {
                        Hit_NGS->ActivateSystem();
                    }

                    for (UActorComponent* Component : Note->GetComponents())
                    {
                        UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(Component);
                        if (PrimitiveComponent && PrimitiveComponent->IsOverlappingComponent(ConcertCameraActor->NoteChecker))
                        {
                            if (PrimitiveComponent->ComponentHasTag("PerfectNoteCollider"))
                            {
                                perfectHit = true;
                                break;
                            }
                            else if (PrimitiveComponent->ComponentHasTag("GoodNoteCollider"))
                            {
                                goodHit = true;
                            }
                        }
                    }

                    if (perfectHit)
                    {
                        GameMode->UpdatePlayer1Score(50); 
                        GameMode->NoteHit(true, true); 
                        PlaySound(bIsHighNote ? HighNoteHitSound : LowNoteHitSound); 
                    }
                    else if (goodHit)
                    {
                        GameMode->UpdatePlayer1Score(10); 
                        GameMode->NoteHit(true, false); 
                        PlaySound(bIsHighNote ? HighNoteHitSound : LowNoteHitSound);
                    }

                    Note->Destroy();
                }
            }
        }
    }

    if (!bNoteHit)
    {
        AConcertGameMode* GameMode = Cast<AConcertGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        if (GameMode)
        {
            GameMode->NoteHit(false, false); // Reset streak and multiplier
            PlaySound(MissNoteSound);

            // Activate Miss Niagara System
            if (Miss_NGS)
            {
                Miss_NGS->ActivateSystem();
            }
        }
    }
}

void AConcertCharacter::PlaySound(USoundCue* SoundCue)
{
    if (SoundCue)
    {
        UGameplayStatics::PlaySoundAtLocation(this, SoundCue, GetActorLocation());
    }
}

void AConcertCharacter::SetCharacterMesh(UStaticMesh* NewMesh)
{
    if (CharacterMesh && NewMesh)
    {
        CharacterMesh->SetStaticMesh(NewMesh);
        UE_LOG(LogTemp, Log, TEXT("Character mesh updated to %s"), *NewMesh->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to update character mesh: CharacterMesh or NewMesh is null."));
    }
}

void AConcertCharacter::SetInstrumentMesh(UStaticMesh* NewMesh)
{
    if (InstrumentMesh && NewMesh)
    {
        InstrumentMesh->SetStaticMesh(NewMesh);
        UE_LOG(LogTemp, Log, TEXT("Instrument mesh set to %s"), *NewMesh->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to set instrument mesh."));
    }
}
