// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#include "Customizations/GeneralBlackboardCustomization.h"
#include "GeneralBlackboard.h"
#include "GeneralBlackboardStyle.h"
#include "GeneralBlackboardCustomizationHelpers.h"
#include "Keys/GeneralBlackboardKey.h"

#include "PropertyEditing.h"
#include <IPropertyUtilities.h>
#include <PropertyCustomizationHelpers.h>

#include <EditorFontGlyphs.h>
#include <Widgets/Text/SInlineEditableTextBlock.h>




#define LOCTEXT_NAMESPACE "GeneralBlackboardCustomization"




class SBlackoardKey_Name : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SBlackoardKey_Name)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<IPropertyHandle> DataHandle)
	{
		KeyNameHandle = DataHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGeneralBlackboardKeyData, KeyName));
		TypeHandle = DataHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGeneralBlackboardKeyData, KeyType));

		SetVisibility(TAttribute<EVisibility>(this, &SBlackoardKey_Name::IsVisible));

		ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 2, 0)
			[
				SNew(SComboButton)
				.ButtonStyle(&FEditorStyle::Get().GetWidgetStyle< FButtonStyle >("ToggleButton"))
				.ForegroundColor(FCoreStyle::Get().GetSlateColor("DefaultForeground"))
				.ContentPadding(FMargin(0, 1, 5, 1))
				.IsEnabled(CanChangeClass())				
				.ToolTipText(this, &SBlackoardKey_Name::GetKeyTypeTooltip)
				.OnGetMenuContent_Static(&FGeneralBlackboardCustomizationHelpers::CreateKeyTypePickerOptions,
					FOnClassPicked::CreateRaw(this, &SBlackoardKey_Name::HandleKeyClassPicked))
				.ButtonContent()
				[
					SNew(SImage)
					.Image(this, &SBlackoardKey_Name::GetKeyImage)
				]
			]
			+ SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 2, 0)
			[
				SNew(SBox)
				.MinDesiredWidth(50.0f)
				[
					SNew(SInlineEditableTextBlock)					
					.Text(this, &SBlackoardKey_Name::GetKeyName)
					.ToolTipText(this, &SBlackoardKey_Name::GetKeyNameTooltip)
					.IsSelected(this, &SBlackoardKey_Name::IsSelected)
					.OnVerifyTextChanged(this, &SBlackoardKey_Name::VerifyNodeNameTextChanged)
					.OnTextCommitted(this, &SBlackoardKey_Name::NameTextComitted)
				]
			]
		];
	}
private:
	bool IsSelected() const
	{		
		return IsHovered() && IsEditable();
	}

	bool IsEditable() const
	{
		return !GEditor->IsPlaySessionInProgress();
	}

	EVisibility IsVisible() const
	{
		return GEditor->IsPlaySessionInProgress() ? EVisibility::HitTestInvisible : EVisibility::Visible;
	}

	TArray<UClass*> GetTypeClasses(bool bUniqueOnly) const
	{
		TArray<UClass*> Types;
		{
			TArray<FString> Values;
			TypeHandle->GetPerObjectValues(Values);

			for (const auto& Value : Values)
			{
				UObject* Type = FSoftObjectPath(Value).ResolveObject();
				UClass* TypeClass = Type ? Type->GetClass() : nullptr;

				if (bUniqueOnly)
				{
					Types.AddUnique(TypeClass);
				}		
				else
				{
					Types.Add(TypeClass);
				}
			}
		}

		return Types;
	}

	TArray<FString> GetKeyNames() const
	{
		TArray<FString> Names;
		KeyNameHandle->GetPerObjectValues(Names);

		return Names;
	}

	const FSlateBrush* GetKeyImage() const
	{
		TArray<UClass*> Types = GetTypeClasses(/*bUniqueOnly*/ true);
		return FGeneralBlackboardCustomizationHelpers::GetKeyTypeBrush((Types.Num() == 1) ? Types[0] : nullptr);
	}

	FText GetKeyName() const
	{
		FText DisplayText;
		if (KeyNameHandle->GetValueAsDisplayText(DisplayText) == FPropertyAccess::MultipleValues)
		{
			return INVTEXT("Multiple Value");
		}

		return DisplayText;
	}

	FText GetKeyNameTooltip() const
	{
		return FText::FromString(FString::Join(GetKeyNames(), LINE_TERMINATOR));
	}

	FText GetKeyTypeTooltip() const
	{
		return FText::FromString(FString::JoinBy(GetTypeClasses(/*bUniqueOnly*/ false), LINE_TERMINATOR, [](UClass* Class)
		{
			return GetNameSafe(Class);
		}));
	}

	bool VerifyNodeNameTextChanged(const FText& NewText, FText& OutErrorMessage)
	{
		TArray<FString> Errors;

		FString CurrentValue;
		if (KeyNameHandle->GetValueAsDisplayString(CurrentValue) == FPropertyAccess::Success)
		{
			if (NewText.ToString() == CurrentValue)
			{
				return true;
			}
		}

		TArray<UObject*> Objects;
		KeyNameHandle->GetOuterObjects(Objects);
		for (UObject* Entry : Objects)
		{
			if (UGeneralBlackboard* Blackboard = Cast<UGeneralBlackboard>(Entry))
			{
				if (Blackboard->GetBlackboardKey(FName(*NewText.ToString())) != nullptr)
				{
					Errors.Add(FString::Printf(TEXT("Name '%s' is taken"), *NewText.ToString()));
				}
			}
		}

		OutErrorMessage = FText::FromString(FString::Join(Errors, LINE_TERMINATOR));

		return OutErrorMessage.IsEmpty();
	}

	void NameTextComitted(const FText& Name, ETextCommit::Type CommitInfo)
	{
		FString CurrentValue;
		if (KeyNameHandle->GetValueAsDisplayString(CurrentValue) != FPropertyAccess::Success)
		{
			return;
		}		

		FGeneralBlackboardCustomizationHelpers::BlackboardOperation(
			LOCTEXT("SetKeyNameTransaction", "Set Blackboard Key Name"),
			KeyNameHandle,
			[&CurrentValue, &Name](UGeneralBlackboard* Blackboard)
			{
				Blackboard->RenameKey(*CurrentValue, *Name.ToString());
				return EPropertyChangeType::ValueSet;
			}
		);
	}

	bool CanChangeClass() const
	{		
		FString CurrentValue;
		return KeyNameHandle->GetValueAsDisplayString(CurrentValue) == FPropertyAccess::Success;
	}

	void HandleKeyClassPicked(UClass* InClass)
	{
		FSlateApplication::Get().DismissAllMenus();

		check(InClass);
		check(InClass->IsChildOf(UGeneralBlackboardKey::StaticClass()));

		FString CurrentValue;
		if (KeyNameHandle->GetValueAsDisplayString(CurrentValue) != FPropertyAccess::Success)
		{
			return;
		}

		FGeneralBlackboardCustomizationHelpers::BlackboardOperation(
			LOCTEXT("SetKeyTypeTransaction", "Set Blackboard Key Type"),
			KeyNameHandle,
			[&CurrentValue, &InClass](UGeneralBlackboard* Blackboard)
			{
				Blackboard->SetKeyType(*CurrentValue, InClass);
				return EPropertyChangeType::ValueSet;
			}
		);

	}


private:
	TSharedPtr<IPropertyHandle> KeyNameHandle;
	TSharedPtr<IPropertyHandle> TypeHandle;
};


class SBlackoardKey_Value : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SBlackoardKey_Name)
	{}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedRef<IPropertyHandle> InDataHandle)
	{
		DataHandle = InDataHandle;

		ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 5, 0)
			[
				SAssignNew(DefaultValueSlot, SBox)
				.MinDesiredWidth(50.0f)
				.IsEnabled(this, &SBlackoardKey_Value::IsValueEditable)
			]
			+ SHorizontalBox::Slot().FillWidth(1.0f)
			[
				SNew(SSpacer)
			]
			+ SHorizontalBox::Slot().AutoWidth().Padding(5, 0, 5, 0)
			[
				PropertyCustomizationHelpers::MakeDeleteButton(FSimpleDelegate::CreateSP(this, &SBlackoardKey_Value::RemoveKey))
			]
		];
	}

public:
	void GenerateValueWidget(IDetailChildrenBuilder& ChildBuilder)
	{
		TSharedPtr<class SWidget> DefaultValueWidget = SNullWidget::NullWidget;

		TSharedPtr<IPropertyHandle> TypeHandle = DataHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGeneralBlackboardKeyData, KeyType));		


		uint32 TypeChildren;
		TypeHandle->GetNumChildren(TypeChildren);
		if (TypeChildren > 0)
		{
			TSharedPtr<IPropertyHandle> DefaultValueHandle = TypeHandle->GetChildHandle(0)->GetChildHandle("Value");
			if (DefaultValueHandle.IsValid())
			{
				if (DefaultValueHandle->GetProperty()->IsA(FStructProperty::StaticClass()))
				{
					DefaultValueWidget = ChildBuilder.GenerateStructValueWidget(DefaultValueHandle.ToSharedRef());
				}

				if (DefaultValueWidget == SNullWidget::NullWidget)
				{
					// Kludge to allow filtering
					TSharedPtr<IPropertyHandle> BaseClass = TypeHandle->GetChildHandle(0)->GetChildHandle("BaseClass");
					if (BaseClass.IsValid())
					{
						FString Value;
						BaseClass->GetValueAsDisplayString(Value);
						DefaultValueHandle->GetProperty()->SetMetaData("AllowedClasses", *Value);
					}

					DefaultValueWidget = DefaultValueHandle->CreatePropertyValueWidget(true);
				}
			}
		}

		DefaultValueSlot->SetContent(DefaultValueWidget.ToSharedRef());
	}

	void RemoveKey()
	{
		TSharedPtr<IPropertyHandleArray> Array = DataHandle->GetParentHandle()->AsArray();
		Array->DeleteItem(DataHandle->GetIndexInArray());
	}

	bool IsValueEditable() const
	{
		return !GEditor->IsPlaySessionInProgress();
	}

	FText GetCurrentValueText() const
	{
		TSharedPtr<IPropertyHandle> TypeHandle = DataHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGeneralBlackboardKeyData, KeyType));

		UObject* TypeValue = nullptr;
		if (TypeHandle->GetValue(TypeValue) == FPropertyAccess::Success)
		{
			UGeneralBlackboardKey* Key = Cast<UGeneralBlackboardKey>(TypeValue);
			if (Key)
			{
				FString String;
				Key->ExportToString(String);

				return FText::FromString(String);
			}
		}
		
		return INVTEXT("");
	}

private:
	TSharedPtr<IPropertyHandle> DataHandle;
	TSharedPtr<SBox> DefaultValueSlot;
};

void FGeneralBlackboardKeyDataCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	HeaderRow
		.NameContent()
		.MinDesiredWidth(200.0f)
		.MaxDesiredWidth(2048.0f)
		[
			SNew(SBlackoardKey_Name, PropertyHandle)
		]
	.ValueContent()
		.MinDesiredWidth(100.0f)
		.MaxDesiredWidth(2048.0f)
		.HAlign(HAlign_Fill)
		[
			SAssignNew(ValueEditor, SBlackoardKey_Value, PropertyHandle)
		];
}

void FGeneralBlackboardKeyDataCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	if (ValueEditor)
	{
		ValueEditor->GenerateValueWidget(ChildBuilder);
	}

	TSharedPtr<IPropertyHandle> TypeHandle = PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FGeneralBlackboardKeyData, KeyType));

	uint32 TypeChilds;
	TypeHandle->GetNumChildren(TypeChilds);
	if (TypeChilds > 0)
	{
		TSharedPtr<IPropertyHandle> ObjectHandle = TypeHandle->GetChildHandle(0);

		uint32 Num = 0;
		ObjectHandle->GetNumChildren(Num);
		for (uint32 Index = 0; Index < Num; Index++)
		{
			TSharedPtr<IPropertyHandle> ChildHandle = ObjectHandle->GetChildHandle(Index);
			if (!ChildHandle->HasMetaData(TEXT("HideInChildren")))
			{
				ChildBuilder.AddProperty(ChildHandle.ToSharedRef());
			}			
		}
	}
}



/*--------------------------------------------
 	FGeneralBlackboardControllerCustomization
 *--------------------------------------------*/

void FGeneralBlackboardControllerCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	Utils = CustomizationUtils.GetPropertyUtilities();

	TSharedPtr<IPropertyHandle> ParentHandle = PropertyHandle->GetParentHandle();

	Keys = ParentHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(UGeneralBlackboard, KeyDisplay), false);
	Keys->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FGeneralBlackboardControllerCustomization::Refresh));
	Keys->MarkHiddenByCustomization();
}

void FGeneralBlackboardControllerCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	FText Message;
	{
		uint32 NumKeys;
		if (Keys->GetNumChildren(NumKeys) == FPropertyAccess::MultipleValues)
		{
			Message = INVTEXT("Multiple values");
		}
		else
		{
			Message = FText::FormatOrdered(INVTEXT("{0} keys"), NumKeys);
		}
	}

	ChildBuilder.AddProperty(Keys.ToSharedRef())
		.CustomWidget(true)
		.NameContent()
		[
			SNew(STextBlock)
			.Font(IDetailLayoutBuilder::GetDetailFont())
			.Text(INVTEXT("Blackboard Keys"))
		]
		.ValueContent()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot().AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(50.0f)
				.Padding(FMargin(5, 2, 2, 5))
				.ToolTipText(LOCTEXT("AddNew", "Add"))
				[
					SNew(SComboButton)
						.ButtonStyle(&FEditorStyle::Get().GetWidgetStyle< FButtonStyle >("FlatButton.Success"))
						.ForegroundColor(FCoreStyle::Get().GetSlateColor("DefaultForeground"))
						.HasDownArrow(false)
						.OnGetMenuContent_Static(&FGeneralBlackboardCustomizationHelpers::CreateKeyTypePickerOptions,
							FOnClassPicked::CreateRaw(this, &FGeneralBlackboardControllerCustomization::HandleKeyClassPicked))
						.ContentPadding(FMargin(5, 0))
						.VAlign(VAlign_Center)
						.HAlign(HAlign_Center)
						.ButtonContent()
						[
							SNew(STextBlock)
							.Font(FEditorStyle::Get().GetFontStyle("FontAwesome.11"))
							.Text(FEditorFontGlyphs::Plus)
							.Visibility(EVisibility::HitTestInvisible)
						]
				]
			]
			+ SHorizontalBox::Slot().AutoWidth().VAlign(VAlign_Center).Padding(10, 0, 0, 0)
			[
				SNew(STextBlock)
				.Font(IDetailLayoutBuilder::GetDetailFont())
				.Text(Message)
			]
		];
}




void FGeneralBlackboardControllerCustomization::Refresh()
{
	Utils->ForceRefresh();
}

void FGeneralBlackboardControllerCustomization::HandleKeyClassPicked(UClass* InClass)
{
	FSlateApplication::Get().DismissAllMenus();

	check(InClass);
	check(InClass->IsChildOf(UGeneralBlackboardKey::StaticClass()));	

	FGeneralBlackboardCustomizationHelpers::BlackboardOperation(
		LOCTEXT("AddKeyTransaction", "Add Blackboard Key"), 
		Keys, 
		[&InClass](UGeneralBlackboard* Blackboard)
		{
			Blackboard->AddNewKey(NAME_None, InClass);
			return EPropertyChangeType::ArrayAdd;
		}
	);

	Utils->ForceRefresh();
}


#undef LOCTEXT_NAMESPACE

