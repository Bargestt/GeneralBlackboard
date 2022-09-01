// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GeneralBlackboard.generated.h"




/**  */
USTRUCT()
struct GENERALBLACKBOARD_API FBlackboardKeySetup
{
	GENERATED_BODY()
	
	FName KeyName;

	uint32 KeyFlags;

	UObject* KeyType;
};


/**
 * 
 */
UCLASS()
class GENERALBLACKBOARD_API UGeneralBlackboard : public UObject
{
	GENERATED_BODY()
	
};
