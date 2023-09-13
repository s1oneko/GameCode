// Copyright angoo inc.,All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugRenderSceneProxy.h"
#include "DebugHitBoxVisualizeComponent.generated.h"


UCLASS(Blueprintable,meta=(BlueprintSpawnableComponent))
class AVENGER_CLIENT_API UDebugHitBoxVisualizeComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UDebugHitBoxVisualizeComponent(const FObjectInitializer& objectInitializer);


	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	TArray<struct FHitBoxParam> CollectDefaultHitBoxParams() const;
};
