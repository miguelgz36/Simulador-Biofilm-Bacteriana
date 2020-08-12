// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "BacteriaAgent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMULATOR_API UBacteriaAgent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBacteriaAgent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bacteria", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bacteria", meta = (AllowPrivateAccess = "true"))
		float Length;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bacteria", meta = (AllowPrivateAccess = "true"))
		float Width;

	float GrowthRate = 0.5;
	float EnergyThresholdToBinaryFission = 3.0;
	float EnergyLevel = 0.0;
	bool MyVariable = true;

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static bool TestFunction(bool A);

private:

	void DoBinaryFission();

	void EasyFission(FVector CurrentLocation, FRotator CurrentRotation);
	void ComplexFission(FVector CurrentLocation, FRotator CurrentRotation);
	void FissionArea();

	float ComputeX1(float B, float Slope, float U, float V, float D);
	float ComputeY1(float B, float Slope, float U, float V, float D);
	float ComputeX2(float B, float Slope, float U, float V, float D);
	float ComputeY2(float B, float Slope, float U, float V, float D);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
