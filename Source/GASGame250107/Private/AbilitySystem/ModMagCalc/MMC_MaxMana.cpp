// QYF Legal Description


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligentDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	IntelligentDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligentDef.bSnapshot = false; // 这里分1、create时snapshot和2、apply时snapshot两种情形
	RelevantAttributesToCapture.Add(IntelligentDef); // 加入捕获队列
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Intelligent = 0.f;
	GetCapturedAttributeMagnitude(IntelligentDef, Spec, EvaluateParameters, Intelligent);
	Intelligent = FMath::Max(0.f, Intelligent);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

	return 50.f + 2.5f * Intelligent + 15.f * PlayerLevel;
}
