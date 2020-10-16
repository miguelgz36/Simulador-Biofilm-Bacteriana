// Fill out your copyright notice in the Description page of Project Settings.


#include "FileExplorerDialog.h"

bool UFileExplorerDialog::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath,
	const FString& FileTypes, TArray<FString>& OutFileNames)
{
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 
											//represents multiple file selection
				return DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), 
					FileTypes, SelectionFlag, OutFileNames);
			}
		}
	}
	return false;
}

bool UFileExplorerDialog::OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath,
	FString& OutFolderName)
{
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 
											//represents multiple file selection
				return DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, DialogTitle, DefaultPath, 
					OutFolderName);
			}
		}
	}
	return false;
}

bool UFileExplorerDialog::SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath,
	const FString& DefaultFile, const FString& FileTypes, TArray < FString >& OutFilenames)
{
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				//Opening the file picker!
				uint32 SelectionFlag = 0; //A value of 0 represents single file selection while a value of 1 
											//represents multiple file selection
				return DesktopPlatform->SaveFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, DefaultFile, 
					FileTypes, SelectionFlag, OutFilenames);
			}
		}
	}
	return false;
}