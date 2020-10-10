// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Templates/SharedPointer.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Containers/Array.h"

#include "../Simulator.h"

#include "TextFileManager.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATOR_API UTextFileManager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static bool SaveArrayStringsToFile(FString SaveDirectory, FString FileName, TArray<FString> ArrayStrings,
			bool AllowOverWriting);

	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static FString ArrayBacteriaPerTimeToJsonString(FS_SimulationConfiguration Configuration, TArray<FS_BacteriaAreaPerTime> StructBacteriaPerTime);

	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static bool SaveStringToFile(FString SaveDirectory, FString FileName, FString StringToSave,
			bool AllowOverWriting);

	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static bool SaveConfigurationFile(FString SaveDirectory, FString FileName, FS_SimulationConfiguration Configuration,
			bool AllowOverWriting);

	UFUNCTION(BlueprintCallable, Category = "TextFile", meta = (Keywords = "Save"))
		static FS_SimulationConfiguration LoadConfigurationFile(FString SaveDirectory, FString FileName);

};
