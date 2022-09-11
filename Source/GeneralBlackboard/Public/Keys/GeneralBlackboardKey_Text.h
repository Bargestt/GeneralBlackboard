// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Keys/GeneralBlackboardKey.h"
#include "GeneralBlackboardKey_Text.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, meta = (DisplayName = "Text"), collapseCategories)
class GENERALBLACKBOARD_API UGeneralBlackboardKey_Text : public UGeneralBlackboardKey
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	FText DefaultValue;
	
	UPROPERTY()
	FText CurrentValue;

	typedef FText FValueType;
	static const FValueType InvalidValue;

	virtual void Reset() override
	{
		CurrentValue = DefaultValue;
	}

	FText GetValue() const
	{
		return CurrentValue;
	}

	void SetValue(const FText& NewValue)
	{
		CurrentValue = NewValue;
	}

	virtual bool ImportFromString(const FString& String) override
	{
		CurrentValue = FText::FromString(String);
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = CurrentValue.ToString();
		return true;
	}
};
