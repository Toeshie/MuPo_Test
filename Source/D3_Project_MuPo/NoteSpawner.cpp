// Fill out your copyright notice in the Description page of Project Settings.

#include "NoteSpawner.h"
#include "NoteBaseClass.h"
#include "SongDataParserSubsystem.h"



// Sets default values for this component's properties
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
	SetNotesData(CurrentNotesData);
}

void UNoteSpawner::InitializeComponent()
{
	Super::InitializeComponent();
	CurrentNotesData.Empty();
	ClearScheduledNotes();
}

void UNoteSpawner::SetNotesData(const TArray<FNoteData>& NotesData)
{
	CurrentNotesData = NotesData;
	UE_LOG(LogTemp, Warning, TEXT("SetNotesData called. Notes count: %d"), NotesData.Num());

	// Schedule notes if already begun play
	if (HasBegunPlay())
	{
		ClearScheduledNotes();
		ScheduleNotes(CurrentNotesData);
	}
}

void UNoteSpawner::SpawnNoteBasedOnNoteData(const FNoteData& Note)
{
   FVector SpawnLocation;
	FRotator SpawnRotation = GetOwner()->GetActorRotation();
	FActorSpawnParameters SpawnParameters;

	bool isHighNote = Note.NoteNumber >= HighNoteValue && Note.Track == 0;
	bool isSpawnAction = Note.Action.Equals(TEXT("spawn"), ESearchCase::IgnoreCase);
	bool isDespawnAction = Note.Action.Equals(TEXT("despawn"), ESearchCase::IgnoreCase);
	
	if (isSpawnAction)
	{
		if (isHighNote)
		{
			SpawnLocation = GetOwner()->GetActorTransform().TransformPosition(LocalOffsetHigh);
			if (BP_DrumNoteHigh)
			{
				AActor* SpawnedActor = GetWorld()->SpawnActor<ANoteBaseClass>(BP_DrumNoteHigh, SpawnLocation, SpawnRotation, SpawnParameters);
				//UE_LOG(LogTemp, Warning, TEXT("Spawned High Note: %s"), (SpawnedActor != nullptr) ? TEXT("Yes") : TEXT("No"));
				if (SpawnedActor)
				{
					SpawnedActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
					OnNoteSpawned.Broadcast(); // Notify that a note has been spawned
				}
			}
		}
		else if (Note.NoteNumber <= LowNoteValue && Note.Track == 0)
		{
			SpawnLocation = GetOwner()->GetActorTransform().TransformPosition(LocalOffsetLow);
			if (BP_DrumNoteLow)
			{
				AActor* SpawnedActor = GetWorld()->SpawnActor<ANoteBaseClass>(BP_DrumNoteLow, SpawnLocation, SpawnRotation, SpawnParameters);
				//UE_LOG(LogTemp, Warning, TEXT("Spawned Low Note: %s"), (SpawnedActor != nullptr) ? TEXT("Yes") : TEXT("No"));
				if (SpawnedActor)
				{
					SpawnedActor->AttachToActor(GetOwner(), FAttachmentTransformRules::KeepRelativeTransform);
					OnNoteSpawned.Broadcast(); // Notify that a note has been spawned
				}
			}
		}
	}
	else if (isDespawnAction)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Despawn action detected. Handling despawn logic here."));
	}
}

void UNoteSpawner::ScheduleNotes(const TArray<FNoteData>& InNotesData)
{
	// Clear any existing timers before scheduling new ones
	ClearScheduledNotes();
	

	for (const FNoteData& Note : InNotesData)
	{
		if (!Note.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Invalid Note: %s"), *Note.ToString());
			continue;
		}

		float Delay = Note.TimeMs / 1000.0f;
		UE_LOG(LogTemp, Warning, TEXT("Setting timer for Note: %s with Delay: %f seconds"), *Note.ToString(), Delay);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Note]() {
			UE_LOG(LogTemp, Warning, TEXT("Timer Triggered for Note: %s"), *Note.ToString());
			this->SpawnNoteBasedOnNoteData(Note);
		}, Delay, false);

		// Store the TimerHandle so it can be cleared later
		NoteTimerHandles.Add(TimerHandle);
	}	
}
void UNoteSpawner::ClearScheduledNotes()
{
	// Iterate over all stored timer handles and clear them
	for (FTimerHandle& TimerHandle : NoteTimerHandles)
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	// Clear the array after all timers are cleared
	NoteTimerHandles.Empty();

	UE_LOG(LogTemp, Warning, TEXT("All scheduled notes have been cleared"));
}


void UNoteSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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