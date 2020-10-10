// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Simulator.generated.h"

USTRUCT(BlueprintType)
struct FS_BacteriaAreaPerTime
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int NumberTicks;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int NumberBacteria;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float TotalArea;
};

USTRUCT(BlueprintType)
struct FS_SimulationConfiguration
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, Category = Variables)
		FString SurfaceType;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		FString BacteriaType;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float FactorNutrientUptakePerSize;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float Temperature;
};

