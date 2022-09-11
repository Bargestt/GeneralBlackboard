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
	FText Value;
	

	typedef FText FValueType;
	static const FValueType InvalidValue;


	FText GetValue() const
	{
		return Value;
	}

	bool SetValue(const FText& NewValue)
	{
		if (!Value.EqualTo(NewValue))
		{
			Value = NewValue;
			return true;
		}
		return false;
	}

	virtual void SetFrom(const UGeneralBlackboardKey* Other) override
	{
		if (const ThisClass* Casted = Cast<ThisClass>(Other))
		{
			SetValue(Casted->GetValue());
		}
	}

	virtual bool ImportFromString(const FString& String) override
	{
		Value = FText::FromString(String);
		return true;
	}

	virtual bool ExportToString(FString& String) const override
	{
		String = Value.ToString();
		return true;
	}
};
