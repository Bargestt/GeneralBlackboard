
#include "GeneralBlackboardStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "EditorStyleSet.h"
#include "Interfaces/IPluginManager.h"
#include "SlateOptMacros.h"


#define IMAGE_PLUGIN_BRUSH( RelativePath, ... ) FSlateImageBrush( FGeneralBlackboardStyle::InContent( RelativePath, ".png" ), __VA_ARGS__ )
#define IMAGE_BRUSH(RelativePath, ...) FSlateImageBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_BRUSH(RelativePath, ...) FSlateBoxBrush(StyleSet->RootToContentDir(RelativePath, TEXT(".png")), __VA_ARGS__)
#define BOX_PLUGIN_BRUSH(RelativePath, ...) FSlateBoxBrush( FGeneralBlackboardStyle::InContent( RelativePath, ".png" ), __VA_ARGS__)
#define DEFAULT_FONT(...) FCoreStyle::GetDefaultFontStyle(__VA_ARGS__)

FString FGeneralBlackboardStyle::InContent(const FString& RelativePath, const ANSICHAR* Extension)
{
	static FString ContentDir = IPluginManager::Get().FindPlugin(TEXT("GeneralBlackboard"))->GetContentDir();
	return (ContentDir / RelativePath) + Extension;
}

TSharedPtr< FSlateStyleSet > FGeneralBlackboardStyle::StyleSet = nullptr;
TSharedPtr< class ISlateStyle > FGeneralBlackboardStyle::Get() { return StyleSet; }

FName FGeneralBlackboardStyle::GetStyleSetName()
{
	static FName PaperStyleName(TEXT("GeneralBlackboardStyle"));
	return PaperStyleName;
}

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void FGeneralBlackboardStyle::Initialize()
{
	// Const icon sizes
	const FVector2D Icon8x8(8.0f, 8.0f);
	const FVector2D Icon10x10(10.0f, 10.0f);
	const FVector2D Icon12x12(12.0f, 12.0f);
	const FVector2D Icon16x16(16.0f, 16.0f);
	const FVector2D Icon20x20(20.0f, 20.0f);
	const FVector2D Icon40x40(40.0f, 40.0f);

	// Only register once
	if (StyleSet.IsValid())
	{
		return;
	}
	StyleSet = MakeShareable(new FSlateStyleSet(GetStyleSetName()));
	StyleSet->SetContentRoot(FPaths::EngineContentDir() / TEXT("Editor/Slate"));
	StyleSet->SetCoreContentRoot(FPaths::EngineContentDir() / TEXT("Slate"));

	//////////////////////////////////////////////////////////////////////////
	// Register styles
	// 
	
	
	// Blackboard classes
	StyleSet->Set("ClassIcon.None", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.7f, 0.7f, 0.7f, 1.0f)));

	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Bool", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.300000f, 0.0f, 0.0f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Class", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.1f, 0.0f, 0.5f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Enum", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.0f, 0.160000f, 0.131270f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Float", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.357667f, 1.0f, 0.060000f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Int", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.013575f, 0.770000f, 0.429609f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Name", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.607717f, 0.224984f, 1.0f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_NativeEnum", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.0f, 0.160000f, 0.131270f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Object", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.0f, 0.4f, 0.910000f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Rotator", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.353393f, 0.454175f, 1.0f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_String", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(1.0f, 0.0f, 0.660537f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Text", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(0.8f, 0.2f, 0.4f, 1.0f)));
	StyleSet->Set("ClassIcon.GeneralBlackboardKey_Vector", new IMAGE_BRUSH("Icons/pill_16x", Icon16x16, FLinearColor(1.0f, 0.591255f, 0.016512f, 1.0f)));


	//////////////////////////////////////////////////////////////////////////

	FSlateStyleRegistry::RegisterSlateStyle(*StyleSet.Get());
};

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

#undef IMAGE_PLUGIN_BRUSH
#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef DEFAULT_FONT

void FGeneralBlackboardStyle::Shutdown()
{
	if (StyleSet.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleSet.Get());
		ensure(StyleSet.IsUnique());
		StyleSet.Reset();
	}
}
