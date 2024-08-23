// Fill out your copyright notice in the Description page of Project Settings.

#include "NoteSpawner.h"
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
    

    while (CurrentNoteIndex < CurrentNotesData.Num())
    {
        const FNoteData& Note = CurrentNotesData[CurrentNoteIndex];

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
    FVector SpawnLocation = GetComponentLocation(); // Base location
    FRotator SpawnRotation = GetComponentRotation(); // Rotation of the spawner component
    FActorSpawnParameters SpawnParameters;
    SpawnParameters.Owner = GetOwner();  // Set the owner to the actor that owns this component

    // Adjust spawn location based on note data
    if (Note.NoteNumber >= HighNoteValue && Note.Track == 0)
    {
        SpawnLocation += LocalOffsetHigh;
    }
    else
    {
        SpawnLocation += LocalOffsetLow;
    }

    TSubclassOf<AActor> NoteClass = (Note.NoteNumber >= HighNoteValue && Note.Track == 0) ? BP_DrumNoteHigh : BP_DrumNoteLow;

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
    CurrentNotesData = NewNotesData;  // Ensure this is set
    
    UE_LOG(LogTemp, Log, TEXT("SetNotesData successfully updated NotesData."));

    // Start the note spawning timer here instead
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