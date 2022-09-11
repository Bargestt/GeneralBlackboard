#pragma once

#include "AssetTypeActions_Base.h"
#include "Toolkits/IToolkitHost.h"

class FAssetTypeActions_GeneralBlackboard : public FAssetTypeActions_Base
{
public:
	FAssetTypeActions_GeneralBlackboard(EAssetTypeCategories::Type InAssetCategory);

	virtual FText GetName() const override;
	virtual uint32 GetCategories() override;
	virtual FColor GetTypeColor() const override;
	virtual UClass* GetSupportedClass() const override;

private:
	EAssetTypeCategories::Type AssetCategory;
};
