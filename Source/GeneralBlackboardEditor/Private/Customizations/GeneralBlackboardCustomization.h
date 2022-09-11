// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include <ClassViewerModule.h>
#include <IPropertyTypeCustomization.h>


class FGeneralBlackboardKeyDataCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef< IPropertyTypeCustomization > MakeInstance()
	{
		return MakeShareable(new FGeneralBlackboardKeyDataCustomization);
	}

	//~ Begin IPropertyTypeCustomization Interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	//~ End IPropertyTypeCustomization Interface

private:
	TSharedPtr<class SBlackoardKey_Value> ValueEditor;
};



class FGeneralBlackboardControllerCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef< IPropertyTypeCustomization > MakeInstance()
	{
		return MakeShareable(new FGeneralBlackboardControllerCustomization);
	}

	//~ Begin IPropertyTypeCustomization Interface
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> PropertyHandle, IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils) override;
	//~ End IPropertyTypeCustomization Interface

private:
	void Refresh();
	void HandleKeyClassPicked(UClass* InClass);

private:
	TSharedPtr<IPropertyHandle> Keys;

	TSharedPtr< class IPropertyUtilities > Utils;
};
