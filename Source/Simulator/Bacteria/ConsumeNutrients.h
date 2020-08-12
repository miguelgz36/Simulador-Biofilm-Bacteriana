// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ConsumeNutrients.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMULATOR_API UConsumeNutrients : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UConsumeNutrients();

	UFUNCTION(BlueprintCallable, Category = "Analytics")
		static float GetNutrients(float AvailableNutrients, float EnergyLevel);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
