// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerChar.h"

// Sets default values
APlayerChar::APlayerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Setting the player's camera component
	PlayerCamComp = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Cam"));

	// Attaching the player's camera component to the head bone
	PlayerCamComp->SetupAttachment(GetMesh(), "Head");

	// Setting the player's camera component to use the pawn control's rotation
	PlayerCamComp->bUsePawnControlRotation = true;

	ResourcesArray.SetNum(3);
	ResourcesNameArray.Add(TEXT("Wood"));
	ResourcesNameArray.Add(TEXT("Stone"));
	ResourcesNameArray.Add(TEXT("Berry"));

}

// Called when the game starts or when spawned
void APlayerChar::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StatsTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		StatsTimerHandle,
		this,
		&APlayerChar::DecreaseStats,
		2.0f,
		true
	);
}

// Called every frame
void APlayerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    // Binds the MoveForward axis to the MoveForward function
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerChar::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerChar::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &APlayerChar::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &APlayerChar::AddControllerYawInput);
	// Binds the JumpEvent action pressed to the StartJump function
    PlayerInputComponent->BindAction("JumpEvent", IE_Pressed, this, &APlayerChar::StartJump);
	// Binds the JumpEvent action released to the StopJump function
	PlayerInputComponent->BindAction("JumpEvent", IE_Released, this, &APlayerChar::StopJump);
}

// Moves the player character along the forward axis by axisValue
void APlayerChar::MoveForward(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, axisValue);
}

// Moves the player character along the right axis by axisValue
void APlayerChar::MoveRight(float axisValue)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, axisValue);
}

// Starts the player character's jump
void APlayerChar::StartJump()
{
	bPressedJump = true;
}

// Stops the player character's jump
void APlayerChar::StopJump()
{
	bPressedJump = false;
}

void APlayerChar::FindObject()
{
	// Stores information about the object hit by the player's line trace.
	FHitResult HitResult;

	// Start the line trace at the player's camera location.
	FVector StartLocation = PlayerCamComp->GetComponentLocation();

	// Trace 800 units forward in the direction the player's camera is facing.
	FVector Direction = PlayerCamComp->GetForwardVector() * 800.0f;
	FVector EndLocation = StartLocation + Direction;

	// Set the collision parameters used by the line trace.
	FCollisionQueryParams QueryParams;

	// Ignore the player character so the trace does not hit itself.
	QueryParams.AddIgnoredActor(this);
	QueryParams.bTraceComplex = true;
	QueryParams.bReturnFaceIndex = true;

	// Perform a visibility line trace from the camera to detect an object.
	if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
	{
		// Check whether the object hit by the trace is a resource.
		AResource_M* HitResource = Cast<AResource_M>(HitResult.GetActor());

		// Only allow resource collection when the player has enough stamina.
		if (Stamina > 5.0f)
		{
			if (HitResource)
			{
				// Get the resource name and the amount collected per interaction.
				FString hitName = HitResource->resourceName;
				int resourceValue = HitResource->resourceAmount;

				// Subtract the collected amount from the resource's remaining total.
				HitResource->totalResource = HitResource->totalResource - resourceValue;

				// Collect the resource while enough of it remains.
				if (HitResource->totalResource > resourceValue)
				{
					// Add the collected resource to the player's resource array.
					GiveResource(resourceValue, hitName);

					// Display a message confirming that the resource was collected.
					check(GEngine != nullptr);
					GEngine->AddOnScreenDebugMessage(
						-1, 5.0f, FColor::Red, TEXT("Resource Collected"));

					// Place the hit decal at the location where the line trace hit the resource.
					UGameplayStatics::SpawnDecalAtLocation(
						GetWorld(),
						hitDecal,
						FVector(10.0f, 10.0f, 10.0f),
						HitResult.Location,
						FRotator(-90, 0, 0),
						2.0f);

					// Resource collection costs the player five stamina.
					SetStamina(-5.0f);
				}
				else
				{
					// Destroy the resource actor when its available resources are depleted.
					HitResource->Destroy();

					// Display a message indicating that the resource is depleted.
					check(GEngine != nullptr);
					GEngine->AddOnScreenDebugMessage(
						-1, 5.0f, FColor::Red, TEXT("Resource Depleted"));
				}
			}
		}
	}
}


void APlayerChar::SetHealth(float amount)
{
	// Adjust the player's health without allowing it to increase above 100.
	if (Health + amount < 100)
	{
		Health = Health + amount;
	}
}


void APlayerChar::SetHunger(float amount)
{
	// Adjust the player's hunger without allowing it to increase above 100.
	if (Hunger + amount < 100)
	{
		Hunger = Hunger + amount;
	}
}


void APlayerChar::SetStamina(float amount)
{
	// Adjust the player's stamina without allowing it to increase above 100.
	if (Stamina + amount < 100)
	{
		Stamina = Stamina + amount;
	}
}


void APlayerChar::DecreaseStats()
{
	// Reduce hunger over time while the player still has hunger remaining.
	if (Hunger > 0)
	{
		SetHunger(-1.0f);
	}

	// Restore stamina as the player's stats are updated.
	SetStamina(10.0f);

	// Reduce health when the player's hunger reaches zero.
	if (Hunger <= 0)
	{
		SetHealth(-3.0f);
	}
}


void APlayerChar::GiveResource(float amount, FString resourceType)
{
	// Add collected wood to index 0 of the resource array.
	if (resourceType == "Wood *")
	{
		ResourcesArray[0] = ResourcesArray[0] + amount;
	}

	// Add collected stone to index 1 of the resource array.
	if (resourceType == "Stone *")
	{
		ResourcesArray[1] = ResourcesArray[1] + amount;
	}

	// Add collected berries to index 2 of the resource array.
	if (resourceType == "Berry *")
	{
		ResourcesArray[2] = ResourcesArray[2] + amount;
	}
}

