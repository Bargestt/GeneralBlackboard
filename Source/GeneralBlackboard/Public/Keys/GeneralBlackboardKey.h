// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include <UObject/WeakFieldPtr.h>
#include "GeneralBlackboardKey.generated.h"

/**
 * 
 */
UCLASS(Abstract, editinlinenew, collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey : public UObject
{
	GENERATED_BODY()	
public:

// Each key must include:
// typedef <CppTypeName> FValueType;
// static const FValueType InvalidValue;
// 
// <CppTypeName> GetValue() const;
// void SetValue(<CppTypeName>) const;
// 
// Use meta 'HideInChildren' to hide DefaultValue from expanded view 
	
	

	/** Create key state after creating blackboard */
	virtual void Initialize()
	{
		Reset();
	}

	/** Reset key value to default */
	virtual void Reset()
	{
		unimplemented();
	}


	/** Import current value from string and return true when succeeded */
	virtual bool ImportFromString(const FString& String)
	{		
		unimplemented();
		return false;
	}
	
	/** Export current value as string and return true when succeeded */
	virtual bool ExportToString(FString& String) const
	{
		unimplemented();
		return false;
	}
};
