// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Simulator.generated.h"

USTRUCT(BlueprintType)
struct FS_SimulationDataPerTimeAux
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int NumberTicks;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		int NumberBacteria;
	UPROPERTY(BlueprintReadWrite, Category = Variables)
		float AreaBiofilm;
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

	friend FArchive& operator<<(FArchive& Archive, FS_SimulationConfiguration& Configuration)
	{
		Archive << Configuration.SurfaceType;
		Archive << Configuration.BacteriaType;
		Archive << Configuration.FactorNutrientUptakePerSize;
		Archive << Configuration.Temperature;

		return Archive;
	}
};

