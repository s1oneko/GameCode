// Copyright angoo inc.,All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "CoreMinimal.h"
#include "DebugRenderSceneProxy.h"

/**
 * FDebugRenderSceneProxy拡張
 */
class FAvengerDebugRenderSceneProxy : public FDebugRenderSceneProxy
{
public:
	FAvengerDebugRenderSceneProxy(const UPrimitiveComponent* InComponent)
		: FDebugRenderSceneProxy(InComponent)
	{
		DrawType = SolidAndWireMeshes;
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily,
	                                    uint32 VisibilityMap, FMeshElementCollector& Collector) const override;

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	struct FDebugCircle
	{
		FDebugCircle(const FVector& InCentre, const FVector& InX, const FVector& InY, float InRadius, int InNumSegments,
		             const FColor& InColor, float InThickness = 0) :
			Centre(InCentre),
			X(InX),
			Y(InY),
			Radius(InRadius),
			NumSegments(InNumSegments),
			Color(InColor),
			Thickness(InThickness)
		{
		}

		FVector Centre;
		FVector X;
		FVector Y;
		float Radius;
		int NumSegments;
		FColor Color;
		float Thickness;
	};

	/// arc
	struct FDebugArc
	{
		FDebugArc(const FVector& InCentre, const FVector& InX, const FVector& InY, float InMinAngle, float InMaxAngle,
		          float InRadius, int InNumSegments, const FColor& InColor) :
			Centre(InCentre),
			X(InX),
			Y(InY),
			MinAngle(InMinAngle),
			MaxAngle(InMaxAngle),
			Radius(InRadius),
			NumSegments(InNumSegments),
			Color(InColor)
		{
		}

		FVector Centre;
		FVector X;
		FVector Y;
		float MinAngle;
		float MaxAngle;
		float Radius;
		int NumSegments;
		FColor Color;
	};

	/// Replacement for FWireCylinder because that's garbage and doesn't reflect the component transform
	struct FDebugCylinder
	{
		FDebugCylinder(const FVector &InCentre, const FVector& InX, const FVector& InY, const FVector& InZ, const float InRadius, const float InHalfHeight, int InNumSegments, const FColor &InColor) :
			Centre(InCentre),
			X(InX), Y(InY), Z(InZ),
			Radius(InRadius),
			HalfHeight(InHalfHeight),
			NumSegments(InNumSegments),
			Color(InColor)
		{
		}

		FVector Centre;
		FVector X;
		FVector Y;
		FVector Z;
		float Radius;
		float HalfHeight;
		int NumSegments;
		FColor Color;
	};

	struct FDebugMesh
	{
		FDebugMesh(const FMatrix& InLocalToWorld,
			const TArray<FDynamicMeshVertex>& InVertices,
			const TArray<uint32>& InIndices,
			const FColor& InColor)
			: LocalToWorld(InLocalToWorld),
			  Vertices(InVertices),
			  Indices(InIndices),
			  Color(InColor)
		{
		}

		FMatrix LocalToWorld;
		TArray<FDynamicMeshVertex>	Vertices;
		TArray <uint32> Indices;
		FColor Color;
	};

	TArray<FDebugCircle> Circles;
	TArray<FDebugArc> Arcs;
	TArray<FArrowLine> ArrowLinesImproved;
	TArray<FSphere> SphereImproved;
	TArray<FDebugBox> BoxImproved;
	TArray<FDebugCylinder> CylindersImproved; 
	TArray<FCapsule> CapsulesImproved; 
	TArray<FDebugMesh> MeshesImproved; 

};