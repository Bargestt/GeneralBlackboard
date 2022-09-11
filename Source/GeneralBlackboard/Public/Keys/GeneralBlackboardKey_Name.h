// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Name.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Name"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Name : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (HideInChildren))
	FName DefaultValue;

	UPROPERTY()
	FName CurrentValue;

	typedef FName FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}

	FName GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(const FName& NewValue)
	{
		CurrentValue = NewValue;
	}

	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue = *String;
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = CurrentValue.ToString();
		return true;
	}
};
