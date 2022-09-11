// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Bool.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Bool"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Bool : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	bool DefaultValue;

	UPROPERTY()
	bool CurrentValue;


	typedef bool FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}
	
	bool GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(const bool& NewValue)
	{
		CurrentValue = NewValue;
	}


	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue = String.Equals(TEXT("True")) || String.Equals(TEXT("1"));
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = CurrentValue ? TEXT("1") : TEXT("0");
		return true;
	}
};
