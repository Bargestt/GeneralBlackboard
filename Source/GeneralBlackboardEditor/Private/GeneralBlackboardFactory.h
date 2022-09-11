// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "GeneralBlackboardFactory.generated.h"

/**
 * 
 */
UCLASS()
class UGeneralBlackboardFactory : public UFactory
{
	GENERATED_BODY()

public:
	UGeneralBlackboardFactory();

	// UFactory interface
	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
	// End of UFactory interface
};
