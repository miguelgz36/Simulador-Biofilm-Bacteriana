// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EstablishFactorTemperature.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMULATOR_API UEstablishFactorTemperature : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEstablishFactorTemperature();

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static float ComputeFactorTemperature(float TemperatureEnvironment);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
