// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterSelectionWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "ConcertGameInstance.h"
#include "GameFramework/Character.h"

void UCharacterSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Bind buttons to their respective handlers
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

    // Load character meshes if not already done
    CharacterMeshes[0] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/TestMeshed/Characters_Character1.Characters_Character1"));
    CharacterMeshes[1] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/TestMeshed/Characters_Character2.Characters_Character2"));
    CharacterMeshes[2] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/TestMeshed/Characters_Character3.Characters_Character3"));
    CharacterMeshes[3] = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/TestMeshed/Characters_Character4.Characters_Character4"));

    // Set character images (optional, depending on your implementation)
    CharacterImage1->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/cartoon_style_smley_face_star.cartoon_style_smley_face_star")));
    CharacterImage2->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/13595.13595")));
    CharacterImage3->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/Images/Character.Character")));
    CharacterImage4->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Game/Blueprints/UI/Images/Character.Character")));
}

// Button click handlers
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

// Main function to handle character button clicks
void UCharacterSelectionWidget::OnCharacterButtonClicked(int32 CharacterIndex)
{
    SetCharacterMesh(CharacterIndex);

    // Get the corresponding image for the selected character
    UTexture2D* SelectedCharacterImage = nullptr;
    switch (CharacterIndex)
    {
    case 0:
        SelectedCharacterImage = Cast<UTexture2D>(CharacterImage1->GetBrush().GetResourceObject());
        break;
    case 1:
        SelectedCharacterImage = Cast<UTexture2D>(CharacterImage2->GetBrush().GetResourceObject());
        break;
    case 2:
        SelectedCharacterImage = Cast<UTexture2D>(CharacterImage3->GetBrush().GetResourceObject());
        break;
    case 3:
        SelectedCharacterImage = Cast<UTexture2D>(CharacterImage4->GetBrush().GetResourceObject());
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Invalid Character Index"));
        return;
    }

    // Broadcast the character selection along with the selected image
    OnCharacterSelected.Broadcast(CharacterIndex, SelectedCharacterImage);

    // Optionally, remove this widget if you're done with it
    RemoveFromParent();
}

// Sets the character mesh in the game instance
void UCharacterSelectionWidget::SetCharacterMesh(int32 CharacterIndex)
{
    UConcertGameInstance* GameInstance = Cast<UConcertGameInstance>(UGameplayStatics::GetGameInstance(this));
    if (GameInstance && CharacterIndex >= 0 && CharacterIndex < 4)
    {
        GameInstance->SetSelectedCharacterMesh(CharacterMeshes[CharacterIndex]);
        UE_LOG(LogTemp, Log, TEXT("Selected character mesh: %d"), CharacterIndex);
    }
}