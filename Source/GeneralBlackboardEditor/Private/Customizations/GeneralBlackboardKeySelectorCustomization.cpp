// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#include "Customizations/GeneralBlackboardKeySelectorCustomization.h"
#include "PropertyEditing.h"
#include "GeneralBlackboard.h"
#include "GeneralBlackboardStyle.h"
#include "GeneralBlackboardInterface.h"
#include <SSearchableComboBox.h>
#include "GeneralBlackboardTypes.h"
#include <Algo/Copy.h>



#define LOCTEXT_NAMESPACE "FGeneralBlackboardKeyNameCustomization"




void FGeneralBlackboardKeySelectorCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	StructHandle = PropertyHandle;
	NameHandle = PropertyHandle->GetChildHandle(0);	
	
	GenerateOptions();

	HeaderRow
	.NameContent()
	[
		PropertyHandle->CreatePropertyNameWidget()
	]
	.ValueContent()
	.MinDesiredWidth(150.0f)
	.MaxDesiredWidth(2048.0f)
	[
		SAssignNew(ComboBox, SSearchableComboBox)
		.ButtonStyle(&FEditorStyle::Get().GetWidgetStyle< FButtonStyle >("ToggleButton"))
		.ForegroundColor(FCoreStyle::Get().GetSlateColor("DefaultForeground"))
		.ContentPadding(FMargin(0, 1, 5, 1))
		.SearchVisibility(EVisibility::Collapsed)
		.OptionsSource(&Options)
		.OnSelectionChanged(this, &FGeneralBlackboardKeySelectorCustomization::OnSelectionChanged)
		.OnGenerateWidget(this, &FGeneralBlackboardKeySelectorCustomization::GenerateOption)
		.OnComboBoxOpening(this, &FGeneralBlackboardKeySelectorCustomization::GenerateOptions)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 10, 0)
			[
				SNew(SImage)
				.Image(this, &FGeneralBlackboardKeySelectorCustomization::GetCurrentKeyBrush)
			]
			+ SHorizontalBox::Slot().Padding(0, 0, 10, 0)
			[
				SNew(STextBlock)
				.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
				.Text(this, &FGeneralBlackboardKeySelectorCustomization::GetCurrentKeyName)
				.ColorAndOpacity(this, &FGeneralBlackboardKeySelectorCustomization::GetCurrentKeyColor)
				.ToolTipText(this, &FGeneralBlackboardKeySelectorCustomization::GetTooltipText)
			]
		]
	];
}

void FGeneralBlackboardKeySelectorCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	bool bIsCDO = false;
	{
		TArray<UObject*> Outers;
		PropertyHandle->GetOuterObjects(Outers);

		bIsCDO = Outers.Num() == 1 && Outers[0]->HasAllFlags(RF_ClassDefaultObject);
	}

	if (bIsCDO)
	{
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(1).ToSharedRef());
		ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(2).ToSharedRef());
	}
}


void FGeneralBlackboardKeySelectorCustomization::GenerateOptions()
{
	Options.Empty();
	KeyTypes.Empty();

	bHasError = false;
	bNoSourceError = false;


	TArray<UGeneralBlackboard*> SourceBlackboards;
	{
		TArray<UObject*> Outers;
		StructHandle->GetOuterObjects(Outers);

		for (UObject* Outer : Outers)
		{
			UGeneralBlackboard* SourceBlackboard = nullptr;

			UObject* InterfaceImplementor = NULL;
			{
				for (UObject* NextOuter = Outer; InterfaceImplementor == NULL && NextOuter != NULL; NextOuter = NextOuter->GetOuter())
				{
					if (NextOuter->Implements<UGeneralBlackboardInterface>())
					{
						InterfaceImplementor = NextOuter;
					}
				}
			}

			if (InterfaceImplementor)
			{
				FEditorScriptExecutionGuard Guard;
				SourceBlackboard = IGeneralBlackboardInterface::Execute_GetGeneralBlackboard(InterfaceImplementor);
			}
			else
			{
				SourceBlackboard = Outer->GetTypedOuter<UGeneralBlackboard>();
			}

			SourceBlackboards.Add(SourceBlackboard);
		}
	}

	if (SourceBlackboards.Num() == 0)
	{
		bHasError = true;
		bNoSourceError = true;
		Options.Add(MakeShared<FString>(TEXT("No source")));		
	}


	UGeneralBlackboard* Blackboard = nullptr;
	if (!bHasError && SourceBlackboards.Num() >= 1)
	{
		Blackboard = SourceBlackboards[0];
		for (UGeneralBlackboard* SourceBlackboard : SourceBlackboards)
		{
			if (SourceBlackboard != Blackboard)
			{
				bHasError = true;
				Options.Add(MakeShared<FString>(TEXT("Multiple sources")));
				break;
			}
		}
		if (bHasError)
		{
			for (UGeneralBlackboard* SourceBlackboard : SourceBlackboards)
			{
				Options.Add(MakeShared<FString>(FString::Printf(TEXT(" %s"), *GetNameSafe(SourceBlackboard))));
			}
		}
	}
	
	if (!bHasError && Blackboard == nullptr)
	{
		bHasError = true;
		bNoSourceError = true;
		Options.Add(MakeShared<FString>(TEXT("No source")));
	}

	if (!bHasError)
	{
		bool bAlowNone = true;

		bool bHasFilter = false;
		TSet<TSubclassOf<UGeneralBlackboardKey>> TypeFilter;
		
		// Collect data from structs
		{
			TArray<void*> RawStructData;
			StructHandle->AccessRawData(RawStructData);
			for (int32 Index = 0; Index < RawStructData.Num(); Index++)
			{
				FGeneralBlackboardKeySelector* StructPtr = reinterpret_cast<FGeneralBlackboardKeySelector*>(RawStructData[Index]);
				if (StructPtr)
				{
					bAlowNone = bAlowNone && StructPtr->bAlowNone;

					// Get valid type filters
					TSet<TSubclassOf<UGeneralBlackboardKey>> StructFilter;
					Algo::CopyIf(StructPtr->AllowedTypes, StructFilter, [](const auto& Type) { return Type != nullptr; });

					// Keep intersecting filters with current one
					if (StructFilter.Num() > 0)
					{
						if (bHasFilter)
						{							
							TypeFilter = TypeFilter.Intersect(StructFilter);
						}
						else
						{
							bHasFilter = true;
							TypeFilter = StructFilter;
						}
					}
				}
			}
		}

		// Collect keys by filter
		const TMap<FName, FGeneralBlackboardKeyData>& KeyMap = Blackboard->GetKeyMap();
		for (const auto& KeyPair : KeyMap)
		{
			TSubclassOf<UGeneralBlackboardKey> KeyType = KeyPair.Value.KeyType ? KeyPair.Value.KeyType->GetClass() : nullptr;
			KeyTypes.Add(KeyPair.Key, KeyType);

			if (KeyType && (!bHasFilter || TypeFilter.Contains(KeyType)))
			{
				Options.Add(MakeShared<FString>(KeyPair.Key.ToString()));
			}
		}

		if (bAlowNone)
		{
			Options.Insert(MakeShared<FString>(TEXT("None")), 0);
		}
	}

	if (ComboBox)
	{
		ComboBox->RefreshOptions();
	}
}

TSharedRef<SWidget> FGeneralBlackboardKeySelectorCustomization::GenerateOption(TSharedPtr<FString> OptionString) const
{
	if (bHasError)
	{
		return SNew(STextBlock)
			.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
			.Text(FText::FromString(*OptionString));
	}

	return GenerateKeyWidget(FName(*OptionString));
}

void FGeneralBlackboardKeySelectorCustomization::OnSelectionChanged(TSharedPtr<FString> Option, ESelectInfo::Type Type)
{
	if (!bHasError)
	{
		NameHandle->SetValue(*Option);
	}	
}

TSharedPtr<FString> FGeneralBlackboardKeySelectorCustomization::GetCurrentOption() const
{
	FName Name;
	if (NameHandle->GetValue(Name) == FPropertyAccess::Success)
	{
		const TSharedPtr<FString>* Current = Options.FindByPredicate([&Name](const TSharedPtr<FString>& Entry) { return Name == FName(*Entry.Get()); });
		if (Current)
		{
			return *Current;
		}
	}	
	return {};
}

TSharedRef<SWidget> FGeneralBlackboardKeySelectorCustomization::GenerateKeyWidget(FName KeyName) const
{
	return 
		SNew(SBox)
		.Padding(FMargin(0, 2))
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 10, 0)
			[
				SNew(SImage)
				.Image(FGeneralBlackboardCustomizationHelpers::GetKeyTypeBrush(KeyTypes.FindRef(KeyName)))
			]
			+ SHorizontalBox::Slot()
			[
				SNew(STextBlock)
				.Font(IPropertyTypeCustomizationUtils::GetRegularFont())
				.Text(FText::FromName(KeyName))
			]
		];
}

const FSlateBrush* FGeneralBlackboardKeySelectorCustomization::GetCurrentKeyBrush() const
{
	FName KeyName = NAME_None;
	NameHandle->GetValue(KeyName);
	return FGeneralBlackboardCustomizationHelpers::GetKeyTypeBrush(KeyTypes.FindRef(KeyName));
}

FText FGeneralBlackboardKeySelectorCustomization::GetCurrentKeyName() const
{
	FName Name = NAME_None;
	NameHandle->GetValue(Name);
	return FText::FromName(Name);
}

FSlateColor FGeneralBlackboardKeySelectorCustomization::GetCurrentKeyColor() const
{
	return bNoSourceError ? FSlateColor(FLinearColor::Red) : FSlateColor::UseForeground();
}

FText FGeneralBlackboardKeySelectorCustomization::GetTooltipText() const
{
	return bNoSourceError ? INVTEXT("No source blackboard") : FText::GetEmpty();
}

#undef LOCTEXT_NAMESPACE