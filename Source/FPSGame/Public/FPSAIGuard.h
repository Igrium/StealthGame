// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted,
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Blueprint

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(EditAnywhere, Category = "AI")
	float DelayBeforeReset;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);

	UPROPERTY(ReplicatedUsing = OnRep_GuardState, BlueprintReadOnly, Category = "AI")
	EAIState GuardState;

	UFUNCTION()
	void OnRep_GuardState();

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<AActor*> PatrolPoints;

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnMoveEnd();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void StartPatrol();

	UPROPERTY(EditAnywhere, Category = "AI")
	bool StartPatrolOnSpawn;

	
	
	//AI

	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);
	
	UFUNCTION()
	void ResetOrientation();

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	bool bIsFollowingPath;

	AActor* Target;

	int NextTargetNum;

	//Internal

	FTimerHandle TimerHandle_ResetOrientation;

	FRotator OriginalRotation;

	void SetGuardState(EAIState NewState);

	void MoveToNextTarget();

	bool bIsMoving;

	void MoveEnd();

	



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
	
};
