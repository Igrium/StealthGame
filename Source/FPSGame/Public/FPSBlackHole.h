// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "FPSBlackHole.generated.h"



UCLASS()
class FPSGAME_API AFPSBlackHole : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSBlackHole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Variables
	UPROPERTY(EditAnywhere, Category = "Gameplay")
		float strength = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Rendering")
		bool ShowDebugLines = false;


	//Components
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		URadialForceComponent* ForceComp;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
};
