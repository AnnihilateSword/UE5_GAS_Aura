// Copyright AnnihilateSword.


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);

	UAuraAssetManager* AuraAssetManager = Cast<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	// 这里是我们调用 FAuraGameplayTags::InitializeNativeGameplayTags 的最佳位置
	FAuraGameplayTags::InitializeNativeGameplayTags();
}
