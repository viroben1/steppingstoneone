// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
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

	UFUNCTION()
	void FindObject();

	// Pointer to the player character's Camera Component
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* PlayerCamComp;
};
