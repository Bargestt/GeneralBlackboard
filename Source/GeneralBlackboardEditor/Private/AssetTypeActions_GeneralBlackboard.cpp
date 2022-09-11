#include "AssetTypeActions_GeneralBlackboard.h"
#include "GeneralBlackboard.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions_GeneralBlackboard"

FAssetTypeActions_GeneralBlackboard::FAssetTypeActions_GeneralBlackboard(EAssetTypeCategories::Type InAssetCategory)
	: AssetCategory(InAssetCategory)
{

}

FText FAssetTypeActions_GeneralBlackboard::GetName() const
{
	return LOCTEXT("AssetTypeActions_GeneralBlackboard", "GeneralBlackboard");
}

uint32 FAssetTypeActions_GeneralBlackboard::GetCategories()
{
	return AssetCategory;
}

FColor FAssetTypeActions_GeneralBlackboard::GetTypeColor() const
{
	return FColor(255, 55, 55);
}

UClass* FAssetTypeActions_GeneralBlackboard::GetSupportedClass() const
{
	return UGeneralBlackboard::StaticClass();
}


#undef LOCTEXT_NAMESPACE
