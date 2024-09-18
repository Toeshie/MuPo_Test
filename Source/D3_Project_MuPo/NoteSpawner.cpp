// Fill out your copyright notice in the Description page of Project Settings.

#include "NoteSpawner.h"

#include "ConcertCharacter.h"
#include "MarimbaCharacter.h"
#include "NoteBaseClass.h"
#include "Kismet/GameplayStatics.h"

UNoteSpawner::UNoteSpawner()
{
    PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<ANoteBaseClass> BP_DrumNoteHighClassFinder(TEXT("/Game/Blueprints/NoteStuff/C++Stuff/Notes/BP_DrumNoteHigh"));
    static ConstructorHelpers::FClassFinder<ANoteBaseClass> BP_DrumNoteLowClassFinder(TEXT("/Game/Blueprints/NoteStuff/C++Stuff/Notes/BP_DrumNoteLow"));

    if (BP_DrumNoteHighClassFinder.Succeeded() && BP_DrumNoteLowClassFinder.Succeeded())
    {
        BP_DrumNoteHigh = BP_DrumNoteHighClassFinder.Class;
        BP_DrumNoteLow = BP_DrumNoteLowClassFinder.Class;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find note classes."));
    }
}

void UNoteSpawner::BeginPlay()
{
    Super::BeginPlay();

    TrackToSpawn = -1; // Initialize with an invalid track value

    TArray<AActor*> FoundMarimbaCharacters;
    TArray<AActor*> FoundConcertCharacters;

    // Check if there's a MarimbaCharacter in the level first
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMarimbaCharacter::StaticClass(), FoundMarimbaCharacters);

    if (FoundMarimbaCharacters.Num() > 0)
    {
        TrackToSpawn = 1; // Set to spawn notes from Track 1
        UE_LOG(LogTemp, Warning, TEXT("MarimbaCharacter detected. Spawning Track 1 notes."));
    }
    else
    {
        // Now check if there's a ConcertCharacter in the level
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), AConcertCharacter::StaticClass(), FoundConcertCharacters);

        if (FoundConcertCharacters.Num() > 0)
        {
            TrackToSpawn = 0; // Set to spawn notes from Track 0
            UE_LOG(LogTemp, Warning, TEXT("ConcertCharacter detected. Spawning Track 0 notes."));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("No ConcertCharacter or MarimbaCharacter found in the level."));
        }
    }
}

void UNoteSpawner::InitializeComponent()
{
    Super::InitializeComponent();
    CurrentNotesData.Empty();
    ClearScheduledNotes();
}

void UNoteSpawner::HandleNoteSpawning()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();

    // Loop through the notes and spawn only those that match the correct track
    while (CurrentNoteIndex < CurrentNotesData.Num())
    {
        const FNoteData& Note = CurrentNotesData[CurrentNoteIndex];

        // Skip notes that are not on the correct track
        if (Note.Track != TrackToSpawn)
        {
            CurrentNoteIndex++;
            continue;
        }

        float NoteTime = Note.TimeMs / 1000.0f;

        if (CurrentTime >= NoteTime)
        {
            SpawnNoteBasedOnNoteData(Note);
            CurrentNoteIndex++;
        }
        else
        {
            break;
        }
    }
}


void UNoteSpawner::SpawnNoteBasedOnNoteData(const FNoteData& Note)
{
    // Only spawn the note if it matches the determined track
    if (Note.Track != TrackToSpawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Skipping note on Track %d. Current TrackToSpawn is %d."), Note.Track, TrackToSpawn);
        return; // Skip spawning if it's not the correct track
    }

    UE_LOG(LogTemp, Warning, TEXT("Spawning note on Track %d."), Note.Track);

    FVector SpawnLocation = GetComponentLocation(); // Base location
    FRotator SpawnRotation = GetComponentRotation(); // Rotation of the spawner component
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Owner = GetOwner();  // Set the owner to the actor that owns this component

    // Adjust spawn location based on note data
    if (Note.NoteNumber >= HighNoteValue)
    {
        SpawnLocation += LocalOffsetHigh;
    }
    else
    {
        SpawnLocation += LocalOffsetLow;
    }

    TSubclassOf<AActor> NoteClass = (Note.NoteNumber >= HighNoteValue) ? BP_DrumNoteHigh : BP_DrumNoteLow;

    if (NoteClass)
    {
        AActor* SpawnedActor = GetWorld()->SpawnActor<ANoteBaseClass>(NoteClass, SpawnLocation, SpawnRotation, SpawnParameters);
        if (SpawnedActor)
        {
            SpawnedActor->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);
            OnNoteSpawned.Broadcast(); // Notify that a note has been spawned
        }
    }
}

void UNoteSpawner::SetNotesData(const TArray<FNoteData>& NewNotesData)
{
    if (NewNotesData.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("SetNotesData received an empty array"));
        return;
    }

    this->NotesData = NewNotesData;
    CurrentNotesData = NewNotesData; 
    
    UE_LOG(LogTemp, Log, TEXT("SetNotesData successfully updated NotesData."));
    
    GetWorld()->GetTimerManager().SetTimer(NoteSpawnTimerHandle, this, &UNoteSpawner::HandleNoteSpawning, 0.005f, true);
}
void UNoteSpawner::ClearScheduledNotes()
{
    GetWorld()->GetTimerManager().ClearTimer(NoteSpawnTimerHandle);
    CurrentNoteIndex = 0;
}

/*
void UNoteSpawner::SpawnNote()
{
	FVector WorldOffsetHigh = GetOwner()->GetActorTransform().TransformPosition(LocalOffsetHigh);
	FVector WorldOffsetLow = GetOwner()->GetActorTransform().TransformPosition(LocalOffsetLow);

	if (BP_DrumNoteHigh && BP_DrumNoteLow != nullptr)
	{
		FActorSpawnParameters SpawnParameters;
		
		AActor* SpawnedNoteHigh = GetWorld()->SpawnActor<ANoteBaseClass>(BP_DrumNoteHigh ,WorldOffsetHigh, GetOwner()->GetActorRotation(),SpawnParameters);
		SpawnedNoteHigh-> AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
		
		AActor* SpawnedNoteLow = GetWorld()->SpawnActor<ANoteBaseClass>(BP_DrumNoteLow ,WorldOffsetLow, GetOwner()->GetActorRotation(), SpawnParameters);
		SpawnedNoteLow-> AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
	}
}
*/