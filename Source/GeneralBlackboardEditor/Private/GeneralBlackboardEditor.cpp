// Copyright (C) Vasily Bulgakov. 2022. All Rights Reserved.

#include "GeneralBlackboardEditor.h"

#include "GeneralBlackboard.h"

#include "AssetTypeActions_GeneralBlackboard.h"
#include "Customizations/GeneralBlackboardCustomization.h"
#include "GeneralBlackboardStyle.h"
#include "GeneralBlackboardTypes.h"
#include "Customizations/GeneralBlackboardKeySelectorCustomization.h"
#include "Customizations/GeneralBlackboardInstanceCustomization.h"




#define LOCTEXT_NAMESPACE "FGeneralBlackboardEditorModule"

#define REG_CUSTOMIZATION(PropertyModule, Target, Customization) \
PropertyModule.RegisterCustomPropertyTypeLayout(RegisteredCustomizations.Add_GetRef(Target::StaticStruct()->GetFName()), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&Customization::MakeInstance)); 

#define REG_CLASS_CUSTOMIZATION(PropertyModule, Target, Customization) \
PropertyModule.RegisterCustomClassLayout(RegisteredClassCustomizations.Add_GetRef(Target::StaticClass()->GetFName()), FOnGetDetailCustomizationInstance::CreateStatic(&Customization::MakeInstance));



class FGeneralBlackboardEditorModule : public IGeneralBlackboardEditorModule
{
	void StartupModule() override
	{			
		FGeneralBlackboardStyle::Initialize();

		IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetTools.RegisterAssetTypeActions(RegisteredAssetActions.Add_GetRef(MakeShareable(new FAssetTypeActions_GeneralBlackboard(EAssetTypeCategories::Misc))));


		FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked< FPropertyEditorModule >("PropertyEditor");
		{
			REG_CUSTOMIZATION(PropertyModule, FGeneralBlackboardKeyData, FGeneralBlackboardKeyDataCustomization);
			REG_CUSTOMIZATION(PropertyModule, FGeneralBlackboardKeySelector, FGeneralBlackboardKeySelectorCustomization);
			REG_CUSTOMIZATION(PropertyModule, FGeneralBlackboardInstance, FGeneralBlackboardInstanceCustomization);
			REG_CUSTOMIZATION(PropertyModule, FGeneralBlackboardController, FGeneralBlackboardControllerCustomization);
		
		}
		PropertyModule.NotifyCustomizationModuleChanged();
	}

	void ShutdownModule() override
	{
		if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
		{
			FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

			for (FName Name : RegisteredCustomizations)
			{
				PropertyModule.UnregisterCustomPropertyTypeLayout(Name);
			}
			RegisteredCustomizations.Empty();

			for (FName Name : RegisteredClassCustomizations)
			{
				PropertyModule.UnregisterCustomClassLayout(Name);
			}
			RegisteredClassCustomizations.Empty();
		}

		if (FModuleManager::Get().IsModuleLoaded("AssetTools"))
		{
			IAssetTools& AssetTools = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
			for (const TSharedRef<IAssetTypeActions>& TypeAction : RegisteredAssetActions)
			{
				AssetTools.UnregisterAssetTypeActions(TypeAction);
			}
		}

		FGeneralBlackboardStyle::Shutdown();
	}

private:
	TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetActions;


	TArray<FName> RegisteredCustomizations;
	TArray<FName> RegisteredClassCustomizations;
};


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGeneralBlackboardEditorModule, GeneralBlackboardEditor)