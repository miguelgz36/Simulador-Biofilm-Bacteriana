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

	//UFUNCTION(BlueprintCallable, Category = "Analytics", meta = (WorldContext = "ObjectWorld"))
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorldContext))
		static void DoBinaryFission(TSubclassOf<AActor> ActorReference, AActor* Owner, float Length, float Width, UObject* ObjectWorld);

private:

	static void EasyFission(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn, float Length, float Width, UWorld* World);
	static void ComplexFission(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn, float Length, float Width, UWorld* World);
	static void FissionArea(FVector CurrentLocation, FRotator CurrentRotation, TSubclassOf<AActor> ActorToSpawn, UWorld* World, float Length, float Width);

	static float ComputeX1(float B, float Slope, float U, float V, float D);
	static float ComputeY1(float B, float Slope, float U, float V, float D);
	static float ComputeX2(float B, float Slope, float U, float V, float D);
	static float ComputeY2(float B, float Slope, float U, float V, float D);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
