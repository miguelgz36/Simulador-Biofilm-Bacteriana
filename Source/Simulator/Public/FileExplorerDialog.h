// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DesktopPlatform/Public/IDesktopPlatform.h"
#include "DesktopPlatform/Public/DesktopPlatformModule.h"

#include "FileExplorerDialog.generated.h"

/**
 * 
 */
UCLASS()
class SIMULATOR_API UFileExplorerDialog : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "FileExplorerDialog")
		static bool OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath,
			const FString& FileTypes, TArray<FString>& OutFileNames);

	UFUNCTION(BlueprintCallable, Category = "FileExplorerDialog")
		static bool OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath,
			FString& OutFolderName);

	UFUNCTION(BlueprintCallable, Category = "FileExplorerDialog")
		static bool SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath,
			const FString& DefaultFile, const FString& FileTypes, TArray < FString >& OutFilenames);
	
};
