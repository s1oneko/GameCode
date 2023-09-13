// Copyright angoo inc.,All Rights Reserved.


#include "DebugHitBoxPrimitiveComponent.h"

#include "AvengerDebugRenderSceneProxy.h"
#include "HitBox.h"
#include "HitBoxComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

// Sets default values for this component's properties
UDebugHitBoxPrimitiveComponent::UDebugHitBoxPrimitiveComponent(const FObjectInitializer& initializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCastShadow(false);
#if WITH_EDITORONLY_DATA
	// Note: this makes this component invisible on level instances, not sure why
	SetIsVisualizationComponent(true);
#endif
	SetHiddenInGame(true);
	bVisibleInReflectionCaptures = false;
	bVisibleInRayTracing = false;
	bVisibleInRealTimeSkyCaptures = false;
	AlwaysLoadOnClient = false;
	bIsEditorOnly = true;
	// ...
}


FPrimitiveSceneProxy* UDebugHitBoxPrimitiveComponent::CreateSceneProxy()
{
	auto ret = new FAvengerDebugRenderSceneProxy(this);

	AHitBox* hitBox = Cast<AHitBox>(GetOwner());
	if(!hitBox)
	{
		return ret;
	}
	

	FHitBoxParam param = hitBox->HitBoxParam;	
	{
		FColor color = FHitBoxParam::GetHitBoxColor(param.HitChannel);
		auto shape = param.ShapeParam;

		FVector scale = shape.Unscaled ? FVector::OneVector : GetOwner()->GetActorScale();
		
		float radius = shape.Radius * (scale.X < scale.Y ? scale.X : scale.Y);
		// そもそも親のHitboxが生成している段階でスケール計算しているからここでやる必要はなさそうなのでコメントアウト（ド）
		// FVector boxHalfExtent = shape.HalfExtent * scale;
		float halfHeight = shape.HalfHeight * scale.Z;

		FTransform trans = FTransform(ret->GetLocalToWorld());
		switch(param.ShapeParam.ShapeType)
		{
		case EHitBoxShape::Box:
			// 二重ぽい（ド）
			// ret->BoxImproved.Add(FDebugRenderSceneProxy::FDebugBox(FBox( -boxHalfExtent,boxHalfExtent),color));
			ret->BoxImproved.Add(FDebugRenderSceneProxy::FDebugBox(FBox( -shape.HalfExtent,shape.HalfExtent),color));
			break;

		case EHitBoxShape::Sphere:

			ret->SphereImproved.Add(FDebugRenderSceneProxy::FSphere(radius,FVector::ZeroVector,color));
			break;

		case EHitBoxShape::Capsule:

			ret->CapsulesImproved.Add(FDebugRenderSceneProxy::FCapsule(FVector::ZeroVector,radius,FVector::ForwardVector,FVector::RightVector,FVector::UpVector,halfHeight,color));
			break;

		case EHitBoxShape::LineTrace:
			ret->ArrowLinesImproved.Add(FDebugRenderSceneProxy::FArrowLine(FVector::ZeroVector,FVector::ForwardVector * shape.TraceLength,color));
			break;

		case EHitBoxShape::SphereTrace:

			FQuat q = FQuat(FVector::RightVector, FMath::DegreesToRadians(90.0f));// Forward方向が高さ方向になるようにカプセルを寝かす
			ret->CapsulesImproved.Add(FDebugRenderSceneProxy::FCapsule( 0.5f * FVector::ForwardVector * shape.TraceLength,radius,q.GetForwardVector(),q.GetRightVector(),q.GetUpVector(),shape.TraceLength * 0.5f,color));

			break;
		}
	}
	
	return ret;
}


FBoxSphereBounds UDebugHitBoxPrimitiveComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds B = Super::CalcBounds(LocalToWorld);
	AHitBox* hitBox = Cast<AHitBox>(GetOwner());
	if(!hitBox)
	{
		return B;
	}

	switch(hitBox->HitBoxParam.ShapeParam.ShapeType)
	{
		case EHitBoxShape::Box:
			{
				if(auto box = hitBox->FindComponentByClass<UBoxComponent>())
				{
					return box->CalcBounds(LocalToWorld);
				}
			}
			break;

		case EHitBoxShape::Sphere:
			{
				if(auto sphere = hitBox->FindComponentByClass<USphereComponent>())
				{
					return sphere->CalcBounds(LocalToWorld);
				}
				
			}
			break;
			
		case EHitBoxShape::Capsule:
			{
				if(auto capsule = hitBox->FindComponentByClass<UCapsuleComponent>())
				{
					return capsule->CalcBounds(LocalToWorld);
				}
			}
			break;

		case EHitBoxShape::LineTrace:
			{
				FShapeParam shape = hitBox->HitBoxParam.ShapeParam;
				FVector BoxPoint = FVector(shape.TraceLength * 0.5f,shape.TraceLength * 0.5f,shape.TraceLength * 0.5f);
				return FBoxSphereBounds(FVector::ZeroVector, BoxPoint, shape.TraceLength * 0.5f).TransformBy(LocalToWorld);
			}
			break;

		case EHitBoxShape::SphereTrace:
			{
				FShapeParam shape = hitBox->HitBoxParam.ShapeParam;
				FVector BoxPoint = FVector(shape.TraceLength * 0.5f,shape.TraceLength * 0.5f,shape.TraceLength * 0.5f);
				return FBoxSphereBounds(FVector::ZeroVector, BoxPoint, shape.TraceLength * 0.5f).TransformBy(LocalToWorld);
			}
			break;
	}
	
	return B;
}




