// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;

class FGeneralBlackboardKeySelectorCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef< IPropertyTypeCustomization > MakeInstance()
	{
		return MakeShareable(new FGeneralBlackboardKeySelectorCustomization);
	}
	
	//~ Begin IPropertyTypeCustomization Interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	//~ End IPropertyTypeCustomization Interface

private:
	void GenerateOptions();	

	TSharedRef<SWidget> GenerateOption(TSharedPtr<FString> OptionString) const;
	void OnSelectionChanged(TSharedPtr<FString> Option, ESelectInfo::Type Type);
	TSharedPtr<FString> GetCurrentOption() const;


	TSharedRef<SWidget> GenerateKeyWidget(FName KeyName) const;


	const FSlateBrush* GetCurrentKeyBrush() const;
	FText GetCurrentKeyName() const;
	FSlateColor GetCurrentKeyColor() const;
	FText GetTooltipText() const;

private:
	TSharedPtr<IPropertyHandle> StructHandle;
	TSharedPtr<IPropertyHandle> NameHandle;

	bool bHasError; 
	bool bNoSourceError;


	TSharedPtr<class SSearchableComboBox> ComboBox;
	TArray<TSharedPtr<FString>> Options;
	TMap<FName, TSubclassOf<UGeneralBlackboardKey>> KeyTypes;
};

