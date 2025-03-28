// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

// ECC_GameTraceChannel1代指自定义的第一个Channel（这里是Projectile），如果有自定义的第二个Channel就叫ECC_GameTraceChannel2
#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2
#define ECC_ExcludePlayers ECollisionChannel::ECC_GameTraceChannel3

static void LogOnScreen(UObject* WorldContext, FString Msg, FColor Color = FColor::White, float Duration = 5.0f) {
	if (!ensure(WorldContext)) {
		return;
	}

	UWorld* World = WorldContext->GetWorld();
	if (!ensure(World)) {
		return;
	}
	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT]" : "[SERVER]";
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}

static void PrintAuth(UObject* WorldContext, bool bHasAuthority) {
	LogOnScreen(WorldContext, bHasAuthority ? FString("S") : FString("C"), FColor::Cyan);
}