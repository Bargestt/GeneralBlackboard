// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GeneralBlackboardInterface.generated.h"


UINTERFACE(MinimalAPI)
class UGeneralBlackboardInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Provides access to blackboard and it's key info
 */
class GENERALBLACKBOARD_API IGeneralBlackboardInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintNativeEvent)
	UGeneralBlackboard* GetGeneralBlackboard() const;	
	virtual UGeneralBlackboard* GetGeneralBlackboard_Implementation() const
	{
		return nullptr;
	}

};
