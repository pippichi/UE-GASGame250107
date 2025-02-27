// QYF Legal Description


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "Kismet\KismetSystemLibrary.h"
#include "AbilitySystem\AuraAbilitySystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	//const int32 ScaledDamage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			"<Default>Launches a bolt of fire, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIRE BOLT</>\n\n"

			// Level
			"<Small>Level: </><Level>%d</>\n"
			// ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			// Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Number of FireBolts
			"<Default>Launches %d bolts of fire, "
			"exploding on impact and dealing: </>"

			// Damage
			"<Damage>%d</><Default> fire damage with"
			" a chance to burn</>"),

			// Values
			Level,
			ManaCost,
			Cooldown,
			FMath::Min(Level, NumProjectiles),
			ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	//const int32 ScaledDamage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const int32 ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL: </>\n\n"

		// Level
		"<Small>Level: </><Level>%d</>\n"
		// ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		// Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Number of FireBolts
		"<Default>Launches %d bolts of fire, "
		"exploding on impact and dealing: </>"

		// Damage
		"<Damage>%d</><Default> fire damage with"
		" a chance to burn</>"),

		// Values
		Level,
		ManaCost,
		Cooldown,
		FMath::Min(Level, NumProjectiles),
		ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;

	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);

	//NumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());

	TArray<FVector> Directions = UAuraAbilitySystemLibrary::TEvenlyDirectors<FVector>(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::TEvenlyDirectors<FRotator>(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

	for (const auto& Vector : Directions) {
		const FVector Start = SocketLocation + FVector(0,0,5);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Vector * 75.f,
			1,
			FLinearColor::Blue,
			120,
			1
		);
	}
	for (const auto& Rotator : Rotators) {
		const FVector Start = SocketLocation + FVector(0, 0, 15);
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Start,
			Start + Rotator.Vector() * 75.f,
			1,
			FLinearColor::Red,
			120,
			1
		);
	}

	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		SocketLocation,
		SocketLocation + Forward * 75.f,
		1,
		FLinearColor::Gray,
		120,
		1
	);
}
