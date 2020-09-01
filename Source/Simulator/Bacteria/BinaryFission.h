// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BinaryFission.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMULATOR_API UBinaryFission : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBinaryFission();

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static int SelectBinaryFissionMethod(FRotator CurrentRotation);

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static int SelectSimpleFissionWay(FRotator CurrentRotation);

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static FVector GetCoordinatesSimpleFission(FVector CurrentLocation, float Length, bool choice, int option);

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static FVector GetVectorFissionArea(FVector CurrentLocation, float Length, float Width);

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static void GetEquationLine(FVector CurrentLocation, FRotator CurrentRotation, float& Slope, 
			float& Intercept, float Pi);

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static FVector GetIntersectionSolution(FVector CurrentLocation, float Intercept, float Slope,
			float Length, bool choice);

private:

	static float ComputeX1(float B, float Slope, float U, float V, float R);
	static float ComputeY1(float B, float Slope, float U, float V, float R);
	static float ComputeX2(float B, float Slope, float U, float V, float R);
	static float ComputeY2(float B, float Slope, float U, float V, float R);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
