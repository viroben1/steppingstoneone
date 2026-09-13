// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Resource_M.h"
#include "Kismet/GameplayStatics.h"

#include "PlayerChar.generated.h"


UCLASS()
class STEPPINGSTONEONE_API APlayerChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Moves the player character along the forward axis by axisValue
	UFUNCTION()
	void MoveForward(float axisValue);

    // Moves the player character along the right axis by axisValue
    UFUNCTION()
	void MoveRight(float axisValue);

	// Starts the player character's jump
	UFUNCTION()
	void StartJump();

	// Stops the player character's jump
	UFUNCTION()
	void StopJump();

	// Function used to detect and collect resource objects in front of the player.
	UFUNCTION()
	void FindObject();

	// Pointer to the player character's Camera Component.
	// The camera is also used as the starting point for the resource line trace.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;

	// Stores the player's current health value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Health = 100.0f;

	// Stores the player's current hunger value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Hunger = 100.0f;

	// Stores the player's current stamina value.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Stats")
	float Stamina = 100.0f;

	// Stores the amount of wood collected by the player.
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Wood;

	// Stores the amount of stone collected by the player.
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Stone;

	// Stores the amount of berries collected by the player.
	UPROPERTY(EditAnywhere, Category = "Resources")
	int Berry;

	// Array used to store the amounts of the player's collected resources.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<int> ResourcesArray;

	// Array used to store the names that correspond with each resource.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resources")
	TArray<FString> ResourcesNameArray;

	// Material used to create the decal where the player's line trace hits a resource.
	UPROPERTY(EditAnywhere, Category = "HitMarker")
	UMaterialInterface* hitDecal;

	// Function used to increase or decrease the player's health.
	UFUNCTION(BlueprintCallable)
	void SetHealth(float amount);

	// Function used to increase or decrease the player's hunger.
	UFUNCTION(BlueprintCallable)
	void SetHunger(float amount);

	// Function used to increase or decrease the player's stamina.
	UFUNCTION(BlueprintCallable)
	void SetStamina(float amount);

	// Function used to update the player's survival stats over time.
	UFUNCTION()
	void DecreaseStats();

	// Function used to add the collected resource amount to the correct resource type.
	UFUNCTION()
	void GiveResource(float amount, FString resourceType);
};