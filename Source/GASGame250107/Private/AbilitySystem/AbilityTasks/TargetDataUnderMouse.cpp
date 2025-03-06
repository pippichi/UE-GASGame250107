// QYF Legal Description


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "GASGame250107\GASGame250107.h"


UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyTaskObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyTaskObj;
}

void UTargetDataUnderMouse::Activate()
{
	Super::Activate();

	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();
	} else
	{
		// We are on the server, so listen for target data
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey PredictionKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey)
			.AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	
	// APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	// FHitResult HitResult;
	// PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	// ValidData.Broadcast(HitResult.Location);
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 为什么这里要创建一个FScopedPredictionWindow呢？
	// 是因为FScopedPredictionWindow对应下面的AbilitySystemComponent->ScopedPredictionKey
	// 之所以叫Scoped是因为该PredictionKey被限制在FScopedPredictionWindow中
	// 这意味着告诉Server FScopedPredictionWindow下方的所有代码所做的事情都需要被Predict，并且在Client会被先执行
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	// PC->GetHitResultUnderFinger()
	PC->GetHitResultUnderCursor(ECC_Target, false, HitResult);
	// ValidData.Broadcast(HitResult.Location);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = HitResult;
	DataHandle.Add(Data);
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag GameplayTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey()); // 告诉GAS，已确认数据被Server接收到了，让GAS做本次网络同步的善后工作，例如清除掉数据缓存等
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
