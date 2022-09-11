// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_String.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "String"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_String : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	FString DefaultValue;

	UPROPERTY()
	FString	CurrentValue;

	typedef FString FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}

	FString GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(const FString& NewValue)
	{
		CurrentValue = NewValue;
	}

	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue = String;
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = CurrentValue;
		return true;
	}
};
