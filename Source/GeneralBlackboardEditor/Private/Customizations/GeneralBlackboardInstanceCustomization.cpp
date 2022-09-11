// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#include "Customizations/GeneralBlackboardInstanceCustomization.h"
#include "PropertyEditing.h"

#define LOCTEXT_NAMESPACE "FGeneralBlackboardInstanceCustomization"

void FGeneralBlackboardInstanceCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{

}

void FGeneralBlackboardInstanceCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	ChildBuilder.AddProperty(PropertyHandle->GetChildHandle(0).ToSharedRef())
		.CustomWidget(true)
		.NameContent()
		[
			PropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			SNullWidget::NullWidget
		];
}


#undef LOCTEXT_NAMESPACE