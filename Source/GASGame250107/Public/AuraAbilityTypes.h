#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }
	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }

	FORCEINLINE void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	FORCEINLINE void SetIsBlockedHit(bool bInIsBlockedHit) { bIsBlockedHit = bInIsBlockedHit; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	FORCEINLINE virtual UScriptStruct* GetScriptStruct() const
	{
		//// UE5.3以前
		//return FGameplayEffectContext::StaticStruct(); 
		// UE5.3以后
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	//// UE5.3以前
	//virtual FGameplayEffectContext* Duplicate() const
	//{
	//	FGameplayEffectContext* NewContext = new FGameplayEffectContext();
	//	*NewContext = *this;
	//	if (GetHitResult())
	//	{
	//		// Does a deep copy of the hit result
	//		NewContext->AddHitResult(*GetHitResult(), true);
	//	}
	//	return NewContext;
	//}
	// UE5.3以后
	virtual FGameplayEffectContext* Duplicate() const // TODO: 记得改回FAuraGameplayEffectContext
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);


protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};

template<>
struct TStructOpsTypeTraits< FAuraGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FAuraGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};