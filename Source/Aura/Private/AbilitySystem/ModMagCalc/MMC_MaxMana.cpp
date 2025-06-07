// Copyright AnnihilateSword.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	m_IntelligenceDef.AttributeToCapture = UAuraAttributeSet::Getm_IntelligenceAttribute();
	m_IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	m_IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(m_IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	/*************************/
	/** 在这里返回我们的计算值 */
	/*************************/

	// Gather tags from source and target.（这个我们这里暂时没用，只是示范下获取方法）
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTags;
	EvaluateParameters.TargetTags = TargetTags;

	float Intelligence = 0.0f;
	// 获取我们这个初始化设置的 Intelligence Magnitude!!
	GetCapturedAttributeMagnitude(m_IntelligenceDef, Spec, EvaluateParameters, Intelligence);
	// 确保 Intelligence 不会是负值
	Intelligence = FMath::Max(Intelligence, 0.0f);

	// 我们希望 m_MaxHealth 不仅取决于活力 Intelligence，还取决于角色的等级 Level
	// 只要这个 GE 有一个 Source Object，我们就可以把它 Cast
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	return 50.0f + 2.5f * Intelligence + 15.0f * PlayerLevel;
}
