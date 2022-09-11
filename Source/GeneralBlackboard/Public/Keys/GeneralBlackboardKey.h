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
// bool SetValue(<CppTypeName>);
// 
// Use meta 'HideInChildren' to hide Value from expanded view 
	
	
	/** Set using another key */
	virtual void SetFrom(const UGeneralBlackboardKey* Other)
	{

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
