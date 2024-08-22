// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectionWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"

void UCharacterSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind each button to its respective click event
    if (CharacterButton1)
    {
        CharacterButton1->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnCharacter1ButtonClicked);
    }
    if (CharacterButton2)
    {
        CharacterButton2->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnCharacter2ButtonClicked);
    }
    if (CharacterButton3)
    {
        CharacterButton3->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnCharacter3ButtonClicked);
    }
    if (CharacterButton4)
    {
        CharacterButton4->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::OnCharacter4ButtonClicked);
    }

    // Load character meshes (ensure these paths are correct)
    CharacterMeshes[0] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Path/To/Character1Mesh.Character1Mesh"));
    CharacterMeshes[1] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Path/To/Character2Mesh.Character2Mesh"));
    CharacterMeshes[2] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Path/To/Character3Mesh.Character3Mesh"));
    CharacterMeshes[3] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Path/To/Character4Mesh.Character4Mesh"));

    // Optionally, set images to buttons (ensure these paths are correct)
    CharacterImage1->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star.cartoon_style_smley_face_star")));
    CharacterImage2->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star.cartoon_style_smley_face_star")));
    CharacterImage3->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star.cartoon_style_smley_face_star")));
    CharacterImage4->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star.cartoon_style_smley_face_star")));
    
   
}

// Separate functions for each button click event
void UCharacterSelectionWidget::OnCharacter1ButtonClicked()
{
    OnCharacterButtonClicked(0);
}

void UCharacterSelectionWidget::OnCharacter2ButtonClicked()
{
    OnCharacterButtonClicked(1);
}

void UCharacterSelectionWidget::OnCharacter3ButtonClicked()
{
    OnCharacterButtonClicked(2);
}

void UCharacterSelectionWidget::OnCharacter4ButtonClicked()
{
    OnCharacterButtonClicked(3);
}

void UCharacterSelectionWidget::OnCharacterButtonClicked(int32 CharacterIndex)
{
    SetCharacterMesh(CharacterIndex);
    OnCharacterSelected.Broadcast(CharacterIndex);
    RemoveFromParent();
}

void UCharacterSelectionWidget::SetCharacterMesh(int32 CharacterIndex)
{
    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance && CharacterIndex >= 0 && CharacterIndex < 4)
    {
        GameInstance->SetSelectedCharacterMesh(CharacterMeshes[CharacterIndex]);
        UE_LOG(LogTemp, Log, TEXT("Selected character mesh: %d"), CharacterIndex);
    }
}