// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryFileManager.h"

<<<<<<< HEAD
//Saves the specified SimulationConfiguration to a binary file. The boolean result of operation is returned.
=======
//Saves the specified SimulationConfiguration to a binary file
>>>>>>> origin/develop
bool UBinaryFileManager::SaveConfigurationFile(FString SaveDirectory, FString FileName,
	FS_SimulationConfigurationAux Configuration, bool AllowOverWriting = false)
{

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
			return false;
		}

	}

	FArchive MyArchive;

	//Save the data to binary
	FBufferArchive BinaryData;

	BinaryData << Configuration;

	//Save binaries to disk
	bool result = FFileHelper::SaveArrayToFile(BinaryData, *SaveDirectory);

	//Empty the buffer's contents
	BinaryData.FlushCache();
	BinaryData.Empty();

	return result;
}

<<<<<<< HEAD
//Retrieves a SimulationConfiguration from a specified binary file. The boolean result of operation is returned.
=======
//Retrieves a SimulationConfiguration from a specified binary file
>>>>>>> origin/develop
bool UBinaryFileManager::LoadConfigurationFile(FString SaveDirectory, FString FileName,
	FS_SimulationConfigurationAux& Configuration) {

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	TArray<uint8> BinaryArray;

	//load disk data to binary array
	if (!FFileHelper::LoadFileToArray(BinaryArray, *SaveDirectory)) return false;

	if (BinaryArray.Num() <= 0) return false;

	//Memory reader is the archive that we're going to use in order to read the loaded data
	FMemoryReader BinaryData = FMemoryReader(BinaryArray, true);
	BinaryData.Seek(0);

	BinaryData << Configuration;

	//Empty the buffer's contents
	BinaryData.FlushCache();
	BinaryArray.Empty();
	//Close the stream
	BinaryData.Close();

	return true;
}