// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
#include "Engine/TargetPoint.h"
#include "AIController.h"


// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);
	DelayBeforeReset = 3.0f;


	GuardState = EAIState::Idle;

	NextTargetNum = 0;

	bIsMoving = false;
	bIsFollowingPath = false;

	StartPatrolOnSpawn = true;

}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	if (StartPatrolOnSpawn)
	{
		StartPatrol();
	}

}

//Fired when the guard sees a pawn
void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (SeenPawn == nullptr)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);

	SetGuardState(EAIState::Alerted);

	AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}
	
}

//Fired when the guard hears a noise
void AFPSAIGuard::OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	OriginalRotation = GetActorRotation();

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &AFPSAIGuard::ResetOrientation, DelayBeforeReset);
	
	SetGuardState(EAIState::Suspicious);
	
}
//Fired when the guard is finished being alerted
void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		return;
	}
	
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);


	if (bIsFollowingPath)
	{
		AAIController* Controller = Cast<AAIController>(GetController());
		if (Controller  && bIsFollowingPath)
		{
			bIsMoving = true;
			Controller->MoveToActor(Target);
		}
	}
}

//Fired when the guard starts his patrol
void AFPSAIGuard::StartPatrol()
{
	bIsFollowingPath = true;
	MoveToNextTarget();
}

//Sets the guards ai state
void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	
	if (NewState != EAIState::Idle)
	{
		AAIController* Controller = Cast<AAIController>(GetController());
		if (Controller)
		{
			bIsMoving = false;
			Controller->StopMovement();
		}
	}

	OnStateChanged(GuardState);
}

//Moves the guard to the next target
void AFPSAIGuard::MoveToNextTarget()
{
	AAIController* Controller = Cast<AAIController>(GetController());
	if (Controller)
	{
		bIsMoving = true;
		Target = PatrolPoints[NextTargetNum];
		Controller->MoveToActor(Target);
	}
}

//Fired when the guard is finished moving
void AFPSAIGuard::MoveEnd()
{
	//If at the last target move back to the first
	if (NextTargetNum >= (PatrolPoints.Num()-1))
	{
		NextTargetNum = 0;
	}
	else
	{
		NextTargetNum = NextTargetNum + 1;
	}

	OnMoveEnd();

	MoveToNextTarget();
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AAIController* Controller = Cast<AAIController>(GetController());
	if (Controller)
	{
		if ((Controller->GetMoveStatus() == EPathFollowingStatus::Idle) && (bIsMoving))
		{
			bIsMoving = false;
			MoveEnd();
		}
	}

}



