// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TextRenderComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Resource_M.generated.h"


UCLASS()
class STEPPINGSTONEONE_API AResource_M : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AResource_M();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Name used to identify the type of resource.
	UPROPERTY(EditAnywhere)
	FString resourceName = "Wood";

	// Amount of the resource received each time it is collected.
	UPROPERTY(EditAnywhere)
	int resourceAmount = 5;

	// Total amount available before the resource becomes depleted.
	UPROPERTY(EditAnywhere)
	int totalResource = 100;

	// Text used to display the resource's name in the game world.
	UPROPERTY()
	FText tempText;

	// Component used to display the resource name above the resource.
	UPROPERTY(EditAnywhere)
	UTextRenderComponent* ResourceNameText;

	// Static mesh component representing the physical resource object.
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	// Static mesh asset used by the resource.
	UPROPERTY(EditAnywhere)
	UStaticMesh* resourceMesh;
};