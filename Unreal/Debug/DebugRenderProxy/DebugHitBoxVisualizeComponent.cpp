// Copyright angoo inc.,All Rights Reserved.


#include "DebugHitBoxVisualizeComponent.h"

#include "AvengerDebugRenderSceneProxy.h"
#include "HitBoxComponent.h"
#include "AvengerPartsComponent.h"

// Sets default values for this component's properties
UDebugHitBoxVisualizeComponent::UDebugHitBoxVisualizeComponent(const FObjectInitializer& initializer)
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


FPrimitiveSceneProxy* UDebugHitBoxVisualizeComponent::CreateSceneProxy()
{
	if (!GetWorld() || GetWorld()->WorldType != EWorldType::EditorPreview)
	{
		return Super::CreateSceneProxy();
	}

	auto ret = new FAvengerDebugRenderSceneProxy(this);

	UHitBoxComponent* hitBoxComp = GetOwner()->FindComponentByClass<UHitBoxComponent>();

	USceneComponent* attachTargetComponent = nullptr;
	if (IHitBoxTargetProvider* hitBoxTargetProvider = Cast<IHitBoxTargetProvider>(GetOwner()))
	{
		attachTargetComponent = hitBoxTargetProvider->FindTargetComponent();
	}
	
	// TargetComponent がない場合のビルトイン検索
	if(!attachTargetComponent)
	{
		attachTargetComponent = GetOwner()->FindComponentByClass<UMeshComponent>();

		if (!attachTargetComponent)
		{
			attachTargetComponent = GetOwner()->GetRootComponent();
		}
	}

	if(hitBoxComp && attachTargetComponent)
	{
		for(auto param :  CollectDefaultHitBoxParams())
		{
			FColor color = FHitBoxParam::GetHitBoxColor(param.HitChannel);
			auto shape = param.ShapeParam;

			FTransform attachTrans = attachTargetComponent->GetSocketTransform(shape.AttachSocketName,RTS_Actor); 

			FVector scale = shape.Unscaled ? FVector::OneVector : GetOwner()->GetActorScale();
			
			float radius = shape.Radius * (scale.X < scale.Y ? scale.X : scale.Y);
			FVector boxHalfExtent = shape.HalfExtent * scale;
			float halfHeight = shape.HalfHeight * scale.Z;

			FTransform ofsTrans = FTransform(shape.RotationOfs.Quaternion(),shape.LocationOfs,FVector::OneVector);

			attachTrans = ofsTrans * attachTrans; // オフセットかける
			
			switch(param.ShapeParam.ShapeType)
			{
			case EHitBoxShape::Box:
				
				ret->BoxImproved.Add(FDebugRenderSceneProxy::FDebugBox(FBox( -boxHalfExtent,boxHalfExtent),color,attachTrans));
				break;

			case EHitBoxShape::Sphere:

				ret->SphereImproved.Add(FDebugRenderSceneProxy::FSphere(radius,attachTrans.GetLocation(),color));
				break;

			case EHitBoxShape::Capsule:

				ret->CapsulesImproved.Add(FDebugRenderSceneProxy::FCapsule(attachTrans.GetLocation(),radius,attachTrans.GetUnitAxis(EAxis::X),attachTrans.GetUnitAxis(EAxis::Y),attachTrans.GetUnitAxis(EAxis::Z),halfHeight,color));
				break;

			case EHitBoxShape::LineTrace:

				ret->ArrowLinesImproved.Add(FDebugRenderSceneProxy::FArrowLine(attachTrans.GetLocation(),attachTrans.GetLocation() + attachTrans.GetRotation().GetForwardVector() * shape.TraceLength,color));
				break;

			case EHitBoxShape::SphereTrace:

				FQuat capsuleRot = FQuat(attachTrans.GetRotation().GetRightVector(),FMath::DegreesToRadians(90.0f)) * attachTrans.GetRotation();
				ret->CapsulesImproved.Add(FDebugRenderSceneProxy::FCapsule(attachTrans.GetLocation() + 0.5f * attachTrans.GetRotation().GetForwardVector() * shape.TraceLength,radius,capsuleRot.GetForwardVector(),capsuleRot.GetRightVector(),capsuleRot.GetUpVector(),shape.TraceLength * 0.5f,color));
				break;
			}
		}
	}
	
	return ret;
}


FBoxSphereBounds UDebugHitBoxVisualizeComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBoxSphereBounds B = Super::CalcBounds(LocalToWorld);

	if (!GetWorld() || GetWorld()->WorldType != EWorldType::EditorPreview)
	{
		return B;
	}

	UHitBoxComponent* hitBoxComp = GetOwner()->FindComponentByClass<UHitBoxComponent>();

	USceneComponent* attachTargetComponent = nullptr;
	if (IHitBoxTargetProvider* hitBoxTargetProvider = Cast<IHitBoxTargetProvider>(GetOwner()))
	{
		attachTargetComponent = hitBoxTargetProvider->FindTargetComponent();
	}
	
	// TargetComponent がない場合のビルトイン検索
	if(!attachTargetComponent)
	{
		attachTargetComponent = GetOwner()->FindComponentByClass<UMeshComponent>();

		if (!attachTargetComponent)
		{
			attachTargetComponent = GetOwner()->GetRootComponent();
		}
	}
	
	if(hitBoxComp && attachTargetComponent)
	{
		for(auto param : CollectDefaultHitBoxParams())
		{
			auto shape = param.ShapeParam;

			// コピペ from CreateSceneProxy...
			FTransform attachTrans = attachTargetComponent->GetSocketTransform(shape.AttachSocketName,RTS_Actor); 

			FVector scale = shape.Unscaled ? FVector::OneVector : GetOwner()->GetActorScale();
			
			float radius = shape.Radius * (scale.X < scale.Y ? scale.X : scale.Y);
			FVector boxHalfExtent = shape.HalfExtent * scale;
			float halfHeight = shape.HalfHeight * scale.Z;

			FTransform ofsTrans = FTransform(shape.RotationOfs.Quaternion(),shape.LocationOfs,FVector::OneVector);
			attachTrans = ofsTrans * attachTrans; // オフセットかける


			switch(param.ShapeParam.ShapeType)
			{
			case EHitBoxShape::Box:
				{
					FBox DBox(-boxHalfExtent, boxHalfExtent);
					// Apply local rotation only, world is done later
					DBox = DBox.TransformBy(attachTrans);
					B = B + FBoxSphereBounds(DBox);
				}
				break;

			case EHitBoxShape::Sphere:
				{
					B = B + FBoxSphereBounds(attachTrans.GetLocation(), FVector(radius), radius);
				}
					break;

			case EHitBoxShape::Capsule:
				{
					FVector halfSize = FVector(radius, radius, halfHeight + radius);
					FBox DBox(-halfSize, halfSize);
					// Apply local rotation only, world is done later
					DBox = DBox.TransformBy(attachTrans);
					B = B + FBoxSphereBounds(DBox);
				}
				break;

			case EHitBoxShape::LineTrace:
				{
					B = B + FBoxSphereBounds(attachTrans.GetLocation(),FVector(shape.TraceLength),shape.TraceLength);
				}
				break;
				
			case EHitBoxShape::SphereTrace:
				{
					B = B + FBoxSphereBounds(attachTrans.GetLocation(),FVector(shape.TraceLength),shape.TraceLength);
				}
				break;
			}
			
		}
	}
	
	return B.TransformBy(LocalToWorld);
}

TArray<FHitBoxParam> UDebugHitBoxVisualizeComponent::CollectDefaultHitBoxParams() const
{
	TArray<FHitBoxParam> ret;

	if(	UHitBoxComponent* hitBoxComp = GetOwner()->FindComponentByClass<UHitBoxComponent>())
	{
		ret+=hitBoxComp->DefaultHitBoxes;
	}

	if(UAvengerPartsComponent* partsComp = GetOwner()->FindComponentByClass<UAvengerPartsComponent>())
	{
		if(partsComp->PartsSetting)
		{
			for(auto& info :partsComp->PartsSetting->PartsInfo)
			{
				if(info.ReqHitBox)
				{
					for(auto& param : info.HitBoxParam)
					{
						FHitBoxParam paramCopy = param;
						paramCopy.ShapeParam.AttachSocketName = info.AttachSocketName;
						ret.Add(paramCopy);
					}
				}
			}
			
		}
	}
	return ret;
}
