// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

#include "DrawDebugHelpers.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComponent"));
	ForceComp->Falloff = ERadialImpulseFalloff::RIF_Linear;
	ForceComp->Radius = 1000.0f;
	ForceComp->ImpulseStrength = (strength*-1);
	ForceComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	if (ShowDebugLines)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), ForceComp->Radius, 50, FColor::Cyan, true);
	}
	
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ForceComp->FireImpulse();
}

