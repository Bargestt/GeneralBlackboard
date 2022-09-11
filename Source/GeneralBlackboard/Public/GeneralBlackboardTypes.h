// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GeneralBlackboardTypes.generated.h"



/**  */
USTRUCT(BlueprintType)
struct GENERALBLACKBOARD_API FGeneralBlackboardKeySelector
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAlowNone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<class UGeneralBlackboardKey>> AllowedTypes;
};



/**  */
USTRUCT(BlueprintType)
struct GENERALBLACKBOARD_API FGeneralBlackboardInstance
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Instanced)
	class UGeneralBlackboard* Blackboard;	
};
