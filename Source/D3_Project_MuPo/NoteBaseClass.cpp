// Fill out your copyright notice in the Description page of Project Settings.

#include "NoteBaseClass.h"
#include "ConcertGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANoteBaseClass::ANoteBaseClass()
{
	PrimaryActorTick.bCanEverTick = true;
	
	NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Note Static Mesh"));
	RootComponent = NoteMesh;

	NoteSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Note SplineComponent"));
	NoteSpline->SetupAttachment(RootComponent);

	PerfectNoteCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("PerfectCollider"));
	PerfectNoteCollider->SetupAttachment(RootComponent);
	PerfectNoteCollider->SetGenerateOverlapEvents(true);
	PerfectNoteCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	PerfectNoteCollider->ComponentTags.Add("PerfectNoteCollider");

	GoodNoteCollider1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Good Note Collider 1"));
	GoodNoteCollider1->SetupAttachment(RootComponent);
	GoodNoteCollider1->SetGenerateOverlapEvents(true);
	GoodNoteCollider1->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	GoodNoteCollider1->ComponentTags.Add("GoodNoteCollider");

	GoodNoteCollider2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Good Note Collider 2"));
	GoodNoteCollider2->SetupAttachment(RootComponent);
	GoodNoteCollider2->SetGenerateOverlapEvents(true);
	GoodNoteCollider2->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	GoodNoteCollider2->ComponentTags.Add("GoodNoteCollider");

	MoveSpeed = 20;
	
	const FVector Location2(-200, 0, 0);
	const FVector Location3(-300, 0, 0);
	const FVector Location4(-400, 0, 0);
	const FVector Location5(-500, 0, 0);
	const FVector Location6(-600, 0, 0);
	const FVector Location7(-700, 0, 0);
	
	
	NoteSpline->AddSplinePoint(Location2, ESplineCoordinateSpace::Local, true);
	NoteSpline->AddSplinePoint(Location3, ESplineCoordinateSpace::Local, true);
	NoteSpline->AddSplinePoint(Location4, ESplineCoordinateSpace::Local, true);
	NoteSpline->AddSplinePoint(Location5, ESplineCoordinateSpace::Local, true);
	NoteSpline->AddSplinePoint(Location6, ESplineCoordinateSpace::Local, true);
	NoteSpline->AddSplinePoint(Location7, ESplineCoordinateSpace::Local, true);

}

// Called when the game starts or when spawned
void ANoteBaseClass::BeginPlay()
{
	Super::BeginPlay();

	MaterialInstance = NoteMesh->CreateDynamicMaterialInstance(0);
}

void ANoteBaseClass::MoveAlongSpline()
{
	auto TotalDistance = NoteSpline->GetSplineLength();
	auto Distance = MoveSpeed * GetWorld()->GetDeltaSeconds();
	
	TotalDistanceMoved += Distance;

	if (TotalDistanceMoved >= TotalDistance)
	{
	    AConcertGameMode* GameMode = Cast<AConcertGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	    if (GameMode)
	    {
	        GameMode->NoteHit(false, false); // Reset streak and multiplier
	    }
		Destroy();
		return;
	}

	auto Location = NoteSpline->GetLocationAtDistanceAlongSpline(TotalDistanceMoved, ESplineCoordinateSpace::World);
	NoteMesh->SetWorldLocation(Location);

	auto Opacity = TotalDistanceMoved / TotalDistance;

	if (MaterialInstance != nullptr)
	{
		//UE_LOG(LogTemp, Log, TEXT("Opacity set to %f"), Opacity);
		MaterialInstance->SetScalarParameterValue(FName("Opacity"), Opacity);
	}
	else
	{
		UE_LOG(LogTemp,Log, TEXT("Failed to create dynamic material instance"));
	}
}

// Called every frame
void ANoteBaseClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveAlongSpline();
}

