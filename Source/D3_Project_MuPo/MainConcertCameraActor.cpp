// Fill out your copyright notice in the Description page of Project Settings.


#include "MainConcertCameraActor.h"
#include "NoteBaseClass.h"
#include "NoteSpawner.h"
#include "Kismet/GameplayStatics.h"


AMainConcertCameraActor::AMainConcertCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	TestPlane = CreateDefaultSubobject<UMeshComponent>("Test");
	RootComponent = TestPlane;
	
	NoteChecker = CreateDefaultSubobject<UBoxComponent>(TEXT("NoteChecker"));
	NoteChecker->SetupAttachment(RootComponent);
	NoteChecker->SetGenerateOverlapEvents(true);
	NoteChecker->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	NoteChecker->OnComponentBeginOverlap.AddDynamic(this, &AMainConcertCameraActor::OnNoteBeginOverlap);
	NoteChecker->OnComponentEndOverlap.AddDynamic(this, &AMainConcertCameraActor::OnNoteEndOverlap);

	// Set the box extent to make it wider
	NoteChecker->SetBoxExtent(FVector(10.0f, 2000.0f, 10.0f));
	// Set the location to be in front of the camera
	NoteChecker->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));
}


void AMainConcertCameraActor::Tick(float DeltaTime)
{
	
	Super::Tick(DeltaTime);

	TimeSinceBegin += DeltaTime;
	float SineValue = FMath::Sin(TimeSinceBegin * MovementFrequency) * MovementAmplitude;

	FVector NewLocation = GetActorLocation();
	NewLocation.Y += SineValue; // Assuming X is the side-to-side axis. Adjust if necessary.

	SetActorLocation(NewLocation);
}

void AMainConcertCameraActor::BeginPlay()
{
	Super::BeginPlay();

	/*if(UNoteSpawner* NoteSpawner = NewObject<UNoteSpawner>(this, UNoteSpawner::StaticClass()))
	{
		NoteSpawner->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		UE_LOG(LogTemp, Error, TEXT("NoteSpawner attached to MainConcertCameraActor."));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create NoteSpawner."));
	}
*/
	// Set timer to play music after 4.40 seconds
	GetWorld()->GetTimerManager().SetTimer(MusicStartTimerHandle, this, &AMainConcertCameraActor::PlayMusic, MusicStartTimer, false);
}

void AMainConcertCameraActor::PlayMusic()
{
	if (MusicCue)
	{
		UGameplayStatics::PlaySoundAtLocation(this, MusicCue, GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("Playing music now!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MusicCue is not set!"));
	}
}

void AMainConcertCameraActor::OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ANoteBaseClass::StaticClass()))
	{
		OverlappingNote = OtherActor;
	}
}

void AMainConcertCameraActor::OnNoteEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlappingNote)
	{
		OverlappingNote = nullptr;
	}
}


