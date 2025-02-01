// QYF Legal Description


#include "AuraAssetManager.h"

#include "AbilitySystemGlobals.h"
#include "AuraGameplayTags.h"

UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	return *Cast<UAuraAssetManager>(GEngine->AssetManager);
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FAuraGameplayTags::InitNativeGameplayTags();

	// // UE5.4以前，This is required to use Target Data!
	// UAbilitySystemGlobals::Get().InitGlobalData(); // UE5.4以后不需要再调用这个函数，引擎已经调用过了
}
