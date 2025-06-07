// Copyright AnnihilateSword.


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	m_VigorDef.AttributeToCapture = UAuraAttributeSet::Getm_VigorAttribute();
	m_VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	m_VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(m_VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
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

	float Vigor = 0.0f;
	// 获取我们这个初始化设置的 Vigor Magnitude!!
	GetCapturedAttributeMagnitude(m_VigorDef, Spec, EvaluateParameters, Vigor);
	// 确保 Vigor 不会是负值
	Vigor = FMath::Max(Vigor, 0.0f);

	// 我们希望 m_MaxHealth 不仅取决于活力 Vigor，还取决于角色的等级 Level
	// 只要这个 GE 有一个 Source Object，我们就可以把它 Cast
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	
	return 80.0f + 2.5f * Vigor + 10.0f * PlayerLevel;
}
