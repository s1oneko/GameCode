// Fill out your copyright notice in the Description page of Project Settings.

#include "AvengerDebugRenderSceneProxy.h"

#define THICKNESS  (1.0f)

void DrawLineArrow(FPrimitiveDrawInterface* PDI,const FVector &Start,const FVector &End,const FColor &Color,float Mag)
{
	// draw a pretty arrow
	FVector Dir = End - Start;
	const float DirMag = Dir.Size();
	Dir /= DirMag;
	FVector YAxis, ZAxis;
	Dir.FindBestAxisVectors(YAxis,ZAxis);
	FMatrix ArrowTM(Dir,YAxis,ZAxis,Start);
	DrawDirectionalArrow(PDI,ArrowTM,Color,DirMag,Mag,SDPG_World,THICKNESS);
}

void FAvengerDebugRenderSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views,
	const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	FDebugRenderSceneProxy::GetDynamicMeshElements(Views, ViewFamily, VisibilityMap, Collector);

	// ============================Copy From Super Class================================//
	// Draw solid spheres
	struct FMaterialCache
	{
		FMaterialCache()
			: bUseFakeLight(false)
		{}

		FMaterialRenderProxy* operator[](FLinearColor Color)
		{
			FMaterialRenderProxy* MeshColor = NULL;
			const uint32 HashKey = ::GetTypeHash(Color);
			if (MeshColorInstances.Contains(HashKey))
			{
				MeshColor = *MeshColorInstances.Find(HashKey);
			}
			else
			{
				if (bUseFakeLight && SolidMeshMaterial.IsValid())
				{
					
					MeshColor = &Collector->AllocateOneFrameResource<FColoredMaterialRenderProxy>(
						SolidMeshMaterial->GetRenderProxy(),
						Color,
						"GizmoColor"
						);
				}
				else
				{
					MeshColor = &Collector->AllocateOneFrameResource<FColoredMaterialRenderProxy>(GEngine->DebugMeshMaterial->GetRenderProxy(), Color);
				}

				MeshColorInstances.Add(HashKey, MeshColor);
			}

			return MeshColor;
		}

		void Init(FMeshElementCollector& InCollector, bool UseLight = false, class UMaterial* InMaterial = nullptr)
		{
			Collector = &InCollector;
			bUseFakeLight = UseLight;
			SolidMeshMaterial = InMaterial;
		}

		FMeshElementCollector* Collector = nullptr;
		TMap<uint32, FMaterialRenderProxy*> MeshColorInstances;
		TWeakObjectPtr<class UMaterial> SolidMeshMaterial;
		bool bUseFakeLight;
	};

	// ====================================================//

	
	FMaterialCache MaterialCache[2];
	MaterialCache[0].Init(Collector);
	MaterialCache[1].Init(Collector, true, SolidMeshMaterial.Get());

	auto& localToWorld = GetLocalToWorld();
	
	for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
	{
		if (VisibilityMap & (1 << ViewIndex))
		{
			const FSceneView* View = Views[ViewIndex];
			FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);

			for (const auto& C : ArrowLinesImproved)
			{
				::DrawLineArrow(PDI,localToWorld.TransformPosition(C.Start),localToWorld.TransformPosition(C.End),C.Color,8.0f);
			}
			
			// Draw Circles
			for (const auto& C : Circles)
			{
				DrawCircle(PDI, localToWorld.TransformPosition(C.Centre), localToWorld.TransformVector(C.X), localToWorld.TransformVector(C.Y), C.Color, C.Radius, C.NumSegments, SDPG_World, C.Thickness, 0, C.Thickness > 0);
			}

			// Draw Arcs
			for (const auto& C : Arcs)
			{
				::DrawArc(PDI, 
					localToWorld.TransformPosition(C.Centre), 
					localToWorld.TransformVector(C.X), localToWorld.TransformVector(C.Y), 
					C.MinAngle, C.MaxAngle,
					C.Radius, C.NumSegments,
					C.Color, SDPG_Foreground);	
			}

			for(const auto& C : BoxImproved)
			{
				DrawWireBox(PDI, localToWorld * C.Transform.ToMatrixWithScale(), C.Box, C.Color, SDPG_World, DrawType == SolidAndWireMeshes ? 2 : 0, 0, true);
			}

			for(const auto& C : SphereImproved)
			{
				if (PointInView(C.Location, View))
				{
					if (DrawType == SolidAndWireMeshes || DrawType == WireMesh)
					{
						DrawWireSphere(PDI, localToWorld.TransformPosition(C.Location), C.Color.WithAlpha(255), C.Radius, 20, SDPG_World, DrawType == SolidAndWireMeshes ? 2 : 0, 0, true);
					}
					if (DrawType == SolidAndWireMeshes || DrawType == SolidMesh)
					{
						GetSphereMesh(localToWorld.TransformPosition(C.Location), FVector(C.Radius), 20, 7, MaterialCache[0][C.Color.WithAlpha(DrawAlpha)], SDPG_World, false, ViewIndex, Collector);
					}
				}
			}
			
			// Draw Cylinders (properly! superclass ignores transforms)
			for (const auto& C : CylindersImproved)
			{
				::DrawWireCylinder(PDI,
				                   localToWorld.TransformPosition(C.Centre),
				                   localToWorld.TransformVector(C.X),
				                   localToWorld.TransformVector(C.Y),
				                   localToWorld.TransformVector(C.Z),
				                   C.Color,
				                   C.Radius,
				                   C.HalfHeight,
				                   C.NumSegments,
				                   SDPG_Foreground,THICKNESS);
			}
			for (const auto& C : CapsulesImproved)
			{
				if(DrawType == SolidAndWireMeshes || DrawType == WireMesh)
				{
					::DrawWireCapsule(PDI,
									   localToWorld.TransformPosition(C.Base),
									   localToWorld.TransformVector(C.X),
									   localToWorld.TransformVector(C.Y),
									   localToWorld.TransformVector(C.Z),
									   C.Color,
									   C.Radius,
									   C.HalfHeight,
									   16,
									   SDPG_Foreground,THICKNESS);
				}

				if(DrawType == SolidAndWireMeshes || DrawType == SolidMesh)
				{
					GetCapsuleMesh(localToWorld.TransformPosition(C.Base),
						localToWorld.TransformVector(C.X),
						localToWorld.TransformVector(C.Y),
						localToWorld.TransformVector(C.Z),
						C.Color,C.Radius,C.HalfHeight,16,MaterialCache[0][C.Color.WithAlpha(DrawAlpha)],SDPG_World,false,ViewIndex,Collector);
				}
			}

			
			for (const auto& Mesh : MeshesImproved)
			{
				FDynamicMeshBuilder MeshBuilder(View->GetFeatureLevel());
				MeshBuilder.AddVertices(Mesh.Vertices);
				MeshBuilder.AddTriangles(Mesh.Indices);

				// Parent caches these (only within this function) but let's assume that's not worth it. Will people really
				// have lots of meshes with a shared colour in this single context to make it worth it?
				const auto MatRenderProxy = new FColoredMaterialRenderProxy(GEngine->WireframeMaterial->GetRenderProxy(), Mesh.Color);
				FDynamicMeshBuilderSettings Settings;
				Settings.bWireframe = true;
				Settings.bUseSelectionOutline = false;
				Settings.bUseWireframeSelectionColoring = true;
				MeshBuilder.GetMesh(Mesh.LocalToWorld, MatRenderProxy, SDPG_World, Settings, nullptr, ViewIndex, Collector);
			}
			
		}
	}
}

FPrimitiveViewRelevance FAvengerDebugRenderSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	// More useful defaults than FDebugRenderSceneProxy
	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View);
	Result.bDynamicRelevance = true;
	Result.bShadowRelevance = false;
	Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
	return Result;
}

