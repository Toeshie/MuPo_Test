// Copyright Epic Games, Inc. All Rights Reserved.

#include "D3_Project_MuPoCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "OverworldConcertActor.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "PauseMenuWidget.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AD3_Project_MuPoCharacter

AD3_Project_MuPoCharacter::AD3_Project_MuPoCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuBPClass(TEXT("/Game/Blueprints/UI/PauseMenu"));
	if (PauseMenuBPClass.Class != nullptr)
	{
		PauseMenuClass = PauseMenuBPClass.Class;
	}
}

void AD3_Project_MuPoCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (PauseMenuClass)
	{
		PauseMenuWidgetInstance = CreateWidget<UPauseMenuWidget>(GetWorld(), PauseMenuClass);
	}
}

void AD3_Project_MuPoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AD3_Project_MuPoCharacter::Move);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &AD3_Project_MuPoCharacter::TogglePauseMenuProxy);

		InputComponent->BindKey(EKeys::H, IE_Pressed, this, &AD3_Project_MuPoCharacter::OnKeyInput);
		InputComponent->BindKey(EKeys::E, IE_Pressed, this, &AD3_Project_MuPoCharacter::OnKeyInput);
		InputComponent->BindKey(EKeys::A, IE_Pressed, this, &AD3_Project_MuPoCharacter::OnKeyInput);
		InputComponent->BindKey(EKeys::V, IE_Pressed, this, &AD3_Project_MuPoCharacter::OnKeyInput);
		InputComponent->BindKey(EKeys::Y, IE_Pressed, this, &AD3_Project_MuPoCharacter::OnKeyInput);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AD3_Project_MuPoCharacter::Look);
	}
}

void AD3_Project_MuPoCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AD3_Project_MuPoCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AD3_Project_MuPoCharacter::TogglePauseMenuProxy()
{
	if (PauseMenuWidgetInstance)
	{
		PauseMenuWidgetInstance->TogglePauseMenu();
	}
}

void AD3_Project_MuPoCharacter::OnKeyInput(FKey PressedKey)
{
	FString PressedKeyString = PressedKey.GetDisplayName().ToString();

	CurrentInput += PressedKeyString;

	if (CheatCode.StartsWith(CurrentInput))
	{
		if (CurrentInput == CheatCode)
		{
			SpawnNewConcertLocation();
			CurrentInput.Empty();
		}
	}
	else
	{
		CurrentInput.Empty();
	}
}

void AD3_Project_MuPoCharacter::SpawnNewConcertLocation()
{
	// Check if the location has already been spawned
	if (bHasSpawnedSecretConcertLocation)
	{
		return; // Exit the function if it's already spawned
	}

	FVector SpawnLocation = FVector(-3391.0, 551.0, -92.9999); // Specify your desired spawn location
	FRotator SpawnRotation = FRotator::ZeroRotator;

	// Load the specified blueprint
	UClass* ConcertLocationClass = StaticLoadClass(UObject::StaticClass(), nullptr, TEXT("/Game/Blueprints/Overworld/BP_SecretConcertLocation.BP_SecretConcertLocation_C"));
	if (ConcertLocationClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AActor* SpawnedActor = World->SpawnActor<AActor>(ConcertLocationClass, SpawnLocation, SpawnRotation);
			if (SpawnedActor)
			{
				bHasSpawnedSecretConcertLocation = true; // Set the flag to prevent further spawns
			}
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("Failed to load BP_SecretConcertLocation"));
	}
}