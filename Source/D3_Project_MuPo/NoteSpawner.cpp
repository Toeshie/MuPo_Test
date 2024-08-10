// Fill out your copyright notice in the Description page of Project Settings.

#include "NoteSpawner.h"
#include "NoteBaseClass.h"
#include "Kismet/GameplayStatics.h"

UNoteSpawner::UNoteSpawner()
{
    PrimaryComponentTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<ANoteBaseClass> BP_DrumNoteHighClassFinder(TEXT("/Game/Blueprints/NoteStuff/C++Stuff/Notes/BP_DrumNoteHigh"));
    static ConstructorHelpers::FClassFinder<ANoteBaseClass> BP_DrumNoteLowClassFinder(TEXT("/Game/Blueprints/NoteStuff/C++Stuff/Notes/BP_DrumNoteLow"));

    BP_DrumNoteHigh = BP_DrumNoteHighClassFinder.Class;
    BP_DrumNoteLow = BP_DrumNoteLowClassFinder.Class;
}

void UNoteSpawner::BeginPlay()
{
    Super::BeginPlay();

    if (CurrentNotesData.Num() > 0)
    {
        // Start a single timer to handle all note spawns
        GetWorld()->GetTimerManager().SetTimer(NoteSpawnTimerHandle, this, &UNoteSpawner::HandleNoteSpawning, 0.01f, true);
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

    // Check if there are notes to spawn
    while (CurrentNoteIndex < CurrentNotesData.Num())
    {
        const FNoteData& Note = CurrentNotesData[CurrentNoteIndex];

        // Convert note time to seconds
        float NoteTime = Note.TimeMs / 1000.0f;

        if (CurrentTime >= NoteTime)
        {
            SpawnNoteBasedOnNoteData(Note);
            CurrentNoteIndex++;
        }
        else
        {
            break; // Break if it's not yet time for the next note
        }
    }
}

void UNoteSpawner::SpawnNoteBasedOnNoteData(const FNoteData& Note)
{
    FVector SpawnLocation;
    FRotator SpawnRotation = GetComponentRotation(); // Use the rotation of the spawner component
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Owner = GetOwner();  // Set the owner to the actor that owns this component

    bool isHighNote = Note.NoteNumber >= HighNoteValue && Note.Track == 0;
    bool isSpawnAction = Note.Action.Equals(TEXT("spawn"), ESearchCase::IgnoreCase);

    if (isSpawnAction)
    {
        SpawnLocation = isHighNote ? GetComponentLocation() + LocalOffsetHigh : GetComponentLocation() + LocalOffsetLow;
        TSubclassOf<AActor> NoteClass = isHighNote ? BP_DrumNoteHigh : BP_DrumNoteLow;

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
}

void UNoteSpawner::SetNotesData(const TArray<FNoteData>& NotesData)
{
    CurrentNotesData = NotesData;
    CurrentNoteIndex = 0;

    if (CurrentNotesData.Num() > 0 && HasBegunPlay())
    {
        GetWorld()->GetTimerManager().ClearTimer(NoteSpawnTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(NoteSpawnTimerHandle, this, &UNoteSpawner::HandleNoteSpawning, 0.01f, true);
    }
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