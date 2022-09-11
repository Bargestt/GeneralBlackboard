// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeneralBlackboardFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GENERALBLACKBOARD_API UGeneralBlackboardFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "General Blackboard")
	static UGeneralBlackboard* CreateGeneralBlackboard(UObject* Outer, bool bAutoInit = true);


	/** Get blackboard using GeneralBlackboardInterface. Defaults to self */
	UFUNCTION(BlueprintPure, Category = "General Blackboard", meta = (DefaultToSelf = Object))
	static class UGeneralBlackboard* GetGeneralBlackboardFromObject(UObject* Object);
};
