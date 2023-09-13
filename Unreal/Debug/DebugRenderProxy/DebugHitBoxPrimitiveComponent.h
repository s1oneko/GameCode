// Copyright angoo inc.,All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DebugRenderSceneProxy.h"
#include "DebugHitBoxPrimitiveComponent.generated.h"


UCLASS(Blueprintable,meta=(BlueprintSpawnableComponent))
class AVENGER_CLIENT_API UDebugHitBoxPrimitiveComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UDebugHitBoxPrimitiveComponent(const FObjectInitializer& objectInitializer);

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	//virtual bool ShouldRecreateProxyOnUpdateTransform() const override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

};
