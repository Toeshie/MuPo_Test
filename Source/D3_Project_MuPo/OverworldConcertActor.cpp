// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldConcertActor.h"
#include "ConcertSelectionWidget.h"
#include "D3_Project_MuPoCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "ConcertSelectionSongChoiceWidget.h"

// Sets default values
AOverworldConcertActor::AOverworldConcertActor()
{
    PrimaryActorTick.bCanEverTick = true;

    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
    RootComponent = BuildingMesh;  // Set the mesh as the root

    // Initialize the collider
    ConcertCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Concert Collider"));
    ConcertCollider->SetupAttachment(BuildingMesh);

    // Set default values for the collider, such as its extent
    ConcertCollider->InitBoxExtent(FVector(50.f, 50.f, 50.f));  // Example size
    ConcertCollider->SetCollisionProfileName(TEXT("Trigger"));  // Set the collision profile to Trigger (adjust as needed)

    // Attach overlap events (if necessary)
    ConcertCollider->OnComponentBeginOverlap.AddDynamic(this, &AOverworldConcertActor::OnBeginOverlap);
    ConcertCollider->OnComponentEndOverlap.AddDynamic(this, &AOverworldConcertActor::OnEndOverlap);
}

void AOverworldConcertActor::BeginPlay()
{
    Super::BeginPlay();
}

void AOverworldConcertActor::ShowWidget()
{
    if (!WidgetClass || WidgetInstance)
    {
        return;
    }

    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get GameInstance."));
        return;
    }

    if (LevelToLoad == "ConcertLocation_CustomSongs")
    {
        WidgetInstance = CreateWidget<UConcertSelectionSongChoiceWidget>(GetWorld(), WidgetClass);
    }
    else
    {
        WidgetInstance = CreateWidget<UConcertSelectionWidget>(GetWorld(), WidgetClass);
    }

    if (WidgetInstance)
    {
        WidgetInstance->AddToViewport();
        EnablePlayerInteraction();

        // If it's a custom song selection widget, initialize it accordingly
        if (UConcertSelectionSongChoiceWidget* CustomWidget = Cast<UConcertSelectionSongChoiceWidget>(WidgetInstance))
        {
            TArray<FString> AvailableSongs = GameInstance->GetAvailableCustomSongs();
            CustomWidget->InitializeWidgetWithSongs(GameInstance->GetSongNameForLevel(LevelToLoad), { TEXT("Concert Character") }, AvailableSongs);
            CustomWidget->OnSongChosen.AddDynamic(this, &AOverworldConcertActor::OnSongChosen);
        }
        else
        {
            WidgetInstance->InitializeWidget(GameInstance->GetSongNameForLevel(LevelToLoad), { TEXT("Concert Character") });
            WidgetInstance->OnConfirm.AddDynamic(this, &AOverworldConcertActor::LoadLevel);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create widget instance."));
    }
}
 


void AOverworldConcertActor::LoadLevel()
{
    if (WidgetInstance)
    {
        UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
        if (GameInstance)
        {
            GameInstance->SetSelectedCharacter(WidgetInstance->GetSelectedCharacter());
        }

        WidgetInstance->RemoveFromParent();
        WidgetInstance = nullptr;

        if (LevelToLoad != NAME_None)
        {
            UGameplayStatics::OpenLevel(this, LevelToLoad);
        }
    }
}

void AOverworldConcertActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Cast<AD3_Project_MuPoCharacter>(OtherActor))
    {
        ShowWidget();
    }
}

void AOverworldConcertActor::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (Cast<AD3_Project_MuPoCharacter>(OtherActor))
    {
        if (WidgetInstance)
        {
            WidgetInstance->RemoveFromParent();
            WidgetInstance = nullptr;

            APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
            if (PlayerController)
            {
                PlayerController->bShowMouseCursor = false;
                PlayerController->bEnableClickEvents = false;
                PlayerController->bEnableMouseOverEvents = false;
            }
        }
    }
}

void AOverworldConcertActor::OnSongChosen(const FString& SelectedSongName, const FString& SelectedCharacter)
{
    if (SelectedSongName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No song selected."));
        return;
    }

    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance)
    {
        GameInstance->SetSelectedSong(SelectedSongName);
        GameInstance->SetSelectedCharacter(SelectedCharacter);
    }

    UGameplayStatics::OpenLevel(this, FName("ConcertLocation_CustomSongs"));
}

void AOverworldConcertActor::ShowStandardWidget(const FString& SongName)
{
    WidgetInstance = CreateWidget<UConcertSelectionWidget>(GetWorld(), WidgetClass);
    if (!WidgetInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create standard widget instance."));
        return;
    }

    WidgetInstance->InitializeWidget(SongName, TArray<FString>({ TEXT("Concert Character") }));
    WidgetInstance->OnConfirm.AddDynamic(this, &AOverworldConcertActor::LoadLevel);
    WidgetInstance->AddToViewport();

    EnablePlayerInteraction();
}

void AOverworldConcertActor::ShowCustomSongSelectionWidget(UConcertGameInstance* GameInstance, const FString& SongName)
{
    WidgetInstance = CreateWidget<UConcertSelectionSongChoiceWidget>(GetWorld(), WidgetClass);
    if (!WidgetInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create custom song selection widget instance."));
        return;
    }

    TArray<FString> AvailableSongs;
    IFileManager& FileManager = IFileManager::Get();
    FString Path = FPaths::Combine(FPaths::ProjectContentDir(), TEXT("CustomSongs/"));
    FileManager.FindFiles(AvailableSongs, *Path, TEXT("*.csv"));

    if (AvailableSongs.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No songs found in directory: %s"), *Path);
    }
    else
    {
        for (const FString& Song : AvailableSongs)
        {
            UE_LOG(LogTemp, Log, TEXT("Found song: %s"), *Song);
        }
    }

    Cast<UConcertSelectionSongChoiceWidget>(WidgetInstance)->InitializeWidgetWithSongs(SongName, TArray<FString>({ TEXT("Concert Character") }), AvailableSongs);
    Cast<UConcertSelectionSongChoiceWidget>(WidgetInstance)->OnSongChosen.AddDynamic(this, &AOverworldConcertActor::OnSongChosen);
    WidgetInstance->AddToViewport();

    EnablePlayerInteraction();
}

void AOverworldConcertActor::EnablePlayerInteraction()
{
    if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
    {
        PlayerController->bShowMouseCursor = true;
        PlayerController->bEnableClickEvents = true;
        PlayerController->bEnableMouseOverEvents = true;
    }
}