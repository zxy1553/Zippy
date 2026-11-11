// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZippyCharacterMovementComponent.generated.h"


class AZippyCharacter;

UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_None	UMETA(Hidden),
	CMOVE_Slide	UMETA(DisplayName="Slide"),
	CMOVE_Prone UMETA(DisplayName="Prone"),
	CMOVE_MAX	UMETA(Hidden),
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ZIPPY_API UZippyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	class FSavedMove_Zippy : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;
		
		// Flag
		uint8 Saved_bWantsToSprint:1;
		
		uint8 Saved_bPrevWantsToCrouch:1;
		uint8 Saved_bWantsToProne:1;

		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};
	
	class FNetworkPredictionData_Client_Zippy : public FNetworkPredictionData_Client_Character
	{
	public:
		FNetworkPredictionData_Client_Zippy(const UCharacterMovementComponent& ClientMovement);
		
		typedef FNetworkPredictionData_Client_Character Super;
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
	// Slide
	void EnterSlide(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitSlide();
	void PhysSlide(float deltaTime, int32 Iterations);
	bool GetSlideSurface(FHitResult& Hit) const;
	
	// Prone
	void EnterProne(EMovementMode PrevMode, ECustomMovementMode PrevCustomMode);
	void ExitProne();
	bool CanProne() const;
	void PhysProne(float deltaTime, int32 Iterations);
	void TryEnterProne() { Safe_bWantsToProne = true; }
	UFUNCTION(Server, Reliable) void Server_EnterProne();

	
	UPROPERTY(EditDefaultsOnly) float Sprint_MaxWalkSpeed;
	UPROPERTY(EditDefaultsOnly) float Walk_MaxWalkSpeed;
	
	// Slide Properties
	UPROPERTY(EditDefaultsOnly) float Slide_MinSpeed=350;
	UPROPERTY(EditDefaultsOnly) float Slide_EnterImpulse=500;
	UPROPERTY(EditDefaultsOnly) float Slide_GravityForce=5000;
	UPROPERTY(EditDefaultsOnly) float Slide_Friction=1.3;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationSliding=1000.f;
	
	// Prone Properties
	UPROPERTY(EditDefaultsOnly) float Prone_EnterHoldDuration=.2f;
	UPROPERTY(EditDefaultsOnly) float ProneSlideEnterImpulse=300.f;
	UPROPERTY(EditDefaultsOnly) float ProneMaxSpeed=300.f;
	UPROPERTY(EditDefaultsOnly) float BrakingDecelerationProning=2500.f;
	
	
	
	UPROPERTY(Transient) AZippyCharacter* ZippyCharacterOwner;
	
	bool Safe_bWantsToSprint;
	bool Safe_bPrevWantsToCrouch;
	bool Safe_bWantsToProne;

	FTimerHandle TimerHandle_EnterProne;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;

	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

public:
	virtual void InitializeComponent() override;

	virtual void UpdateCharacterStateBeforeMovement(float DeltaSeconds) override;

	virtual bool IsMovingOnGround() const override;
	virtual bool CanCrouchInCurrentState() const override;
	virtual float GetMaxSpeed() const override;
	virtual float GetMaxBrakingDeceleration() const override;

	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;
	
	UFUNCTION(BlueprintCallable) void SprintPressed();
	UFUNCTION(BlueprintCallable) void SprintReleased();
	
	UFUNCTION(BlueprintCallable) void CrouchPressed();
	UFUNCTION(BlueprintCallable) void CrouchReleased();

	UZippyCharacterMovementComponent();

	UFUNCTION(BlueprintPure) bool IsCustomMovementMode(ECustomMovementMode InCustomMovementMode) const;
	
	UFUNCTION(BlueprintPure) bool IsMovementMode(EMovementMode InMovementMode) const;
};
