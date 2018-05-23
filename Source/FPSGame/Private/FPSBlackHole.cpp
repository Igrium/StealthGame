// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	OuterSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("OuterSphere"));
	OuterSphereComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	OuterSphereComp->SetSphereRadius(1500.0);
	OuterSphereComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();

	DrawDebugSphere(GetWorld(), GetActorLocation(), OuterSphereComp->GetScaledSphereRadius(), 50, FColor::Cyan, true);
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappingActors;
	OuterSphereComp->GetOverlappingActors(OverlappingActors);

	for (AActor* Target : OverlappingActors)
	{
		USceneComponent* RootComp = Target->GetRootComponent();
		UStaticMeshComponent* ActorMesh = Cast<UStaticMeshComponent>(RootComp);
		
		if (ActorMesh)
		{
			ActorMesh->AddRadialForce(GetActorLocation(),OuterSphereComp->GetScaledSphereRadius(),(strength*-1),ERadialImpulseFalloff::RIF_Linear);
		}
	}
}

