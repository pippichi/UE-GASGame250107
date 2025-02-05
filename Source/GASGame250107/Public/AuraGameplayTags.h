// QYF Legal Description

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 * Singleton containing native gameplay tags
 */
struct FAuraGameplayTags
{
public:
 FORCEINLINE static const FAuraGameplayTags& Get()
 {
  /**
    * 局部静态特性的方式实现单实例。
    * 静态局部变量只在当前函数内有效，其他函数无法访问。
    * 静态局部变量只在第一次被调用的时候初始化，也存储在静态存储区，生命周期从第一次被初始化起至程序结束止。
    */
  // static FAuraGameplayTags TempGameplayTags;
  // return TempGameplayTags;
  return GameplayTags;
 }

 FGameplayTag Attributes_Primary_Strength;
 FGameplayTag Attributes_Primary_Intelligence;
 FGameplayTag Attributes_Primary_Resilience;
 FGameplayTag Attributes_Primary_Vigor;

 FGameplayTag Attributes_Secondary_Armor;
 FGameplayTag Attributes_Secondary_ArmorPenetration;
 FGameplayTag Attributes_Secondary_BlockChance;
 FGameplayTag Attributes_Secondary_CriticalHitChance;
 FGameplayTag Attributes_Secondary_CriticalHitDamage;
 FGameplayTag Attributes_Secondary_CriticalHitResistance;
 FGameplayTag Attributes_Secondary_HealthRegeneration;
 FGameplayTag Attributes_Secondary_ManaRegeneration;
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_MaxMana;

 FGameplayTag InputTag_LMB;
 FGameplayTag InputTag_RMB;
 FGameplayTag InputTag_1;
 FGameplayTag InputTag_2;
 FGameplayTag InputTag_3;
 FGameplayTag InputTag_4;

 FGameplayTag Damage;
 
 FGameplayTag Effects_HitReact;
 
 static FAuraGameplayTags GameplayTags;
 static void InitNativeGameplayTags();
 
 ~FAuraGameplayTags() { }
private:
 FAuraGameplayTags() { }
 // FAuraGameplayTags(const FAuraGameplayTags &) = delete;
 // FAuraGameplayTags(const FAuraGameplayTags &&) = delete;
 // FAuraGameplayTags &operator=(const FAuraGameplayTags &) = delete;
};
