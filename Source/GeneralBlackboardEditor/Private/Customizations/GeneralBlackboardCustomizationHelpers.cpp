// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#include "GeneralBlackboardCustomizationHelpers.h"
#include <ClassViewerModule.h>
#include <ClassViewerFilter.h>
#include "../GeneralBlackboardStyle.h"

TSharedRef<SWidget> FGeneralBlackboardCustomizationHelpers::CreateKeyTypePickerOptions(FOnClassPicked OnPicked)
{
	class FGeneralBlackboardClassFilter : public IClassViewerFilter
	{
	public:

		virtual bool IsClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const UClass* InClass, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			if (InClass != nullptr)
			{
				return !InClass->HasAnyClassFlags(CLASS_Abstract | CLASS_HideDropDown) &&
					InClass->HasAnyClassFlags(CLASS_EditInlineNew) &&
					InClass->IsChildOf(UGeneralBlackboardKey::StaticClass());
			}
			return false;
		}

		virtual bool IsUnloadedClassAllowed(const FClassViewerInitializationOptions& InInitOptions, const TSharedRef< const IUnloadedBlueprintData > InUnloadedClassData, TSharedRef< FClassViewerFilterFuncs > InFilterFuncs) override
		{
			return InUnloadedClassData->IsChildOf(UGeneralBlackboardKey::StaticClass());
		}
	};

	FClassViewerInitializationOptions Options;
	Options.bShowUnloadedBlueprints = true;
	Options.NameTypeToDisplay = EClassViewerNameTypeToDisplay::DisplayName;
	Options.ClassFilter = MakeShareable(new FGeneralBlackboardClassFilter);

	return SNew(SBox)
	.WidthOverride(280)
	[
		SNew(SVerticalBox)
		+SVerticalBox::Slot()
		.AutoHeight()
		.MaxHeight(500)
		[
			FModuleManager::LoadModuleChecked<FClassViewerModule>("ClassViewer").CreateClassViewer(Options, OnPicked)
		]			
	];
}

const FSlateBrush* FGeneralBlackboardCustomizationHelpers::GetKeyTypeBrush(UClass* KeyType)
{
	const FString StyleName = FString::Printf(TEXT("ClassIcon.%s"), *GetNameSafe(KeyType));
	return FGeneralBlackboardStyle::Get()->GetBrush(*StyleName);
}


void FGeneralBlackboardCustomizationHelpers::BlackboardOperation(FText TransactionName, TSharedPtr<IPropertyHandle> Property, TFunctionRef<EPropertyChangeType::Type(class UGeneralBlackboard*)> Operation)
{
	check(Property);

	TArray<UGeneralBlackboard*> Blackboards;
	{
		TArray<UObject*> Objects;
		Property->GetOuterObjects(Objects);
		for (UObject* Entry : Objects)
		{
			if (UGeneralBlackboard* Blackboard = Cast<UGeneralBlackboard>(Entry))
			{
				Blackboards.Add(Blackboard);
			}
		}
	}

	const FScopedTransaction Transaction(TransactionName);

	for (UGeneralBlackboard* Blackboard : Blackboards)
	{
		Blackboard->SetFlags(RF_Transactional);
		Blackboard->Modify();

		Blackboard->PreEditChange(Property->GetProperty());

		EPropertyChangeType::Type ChangeType = Operation(Blackboard);		

		FPropertyChangedEvent PropertyChangedEvent(Property->GetProperty(), ChangeType);
		Blackboard->PostEditChangeProperty(PropertyChangedEvent);
	}

}
