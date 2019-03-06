// Copyright 2019, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Materials/MaterialInterface.h"
#include "SLSkeletalDataAsset.h"
#include "SLSkeletalDataComponent.generated.h"

/**
 * Bone semantic data structure
 */
USTRUCT()
struct FSLBoneData
{
	GENERATED_BODY()

	// Semantic class name
	UPROPERTY(EditAnywhere)
	FString Class;

	// Color mask
	UPROPERTY(EditAnywhere)
	FString MaskColorHex;

	// Mask material instance 
	UPROPERTY(EditAnywhere)
	UMaterialInterface* MaskMaterial;

	// Checks if the structure has been set
	bool IsSet() const { return !Class.IsEmpty(); };

	// Get result as string
	FString ToString() const
	{
		return FString::Printf(TEXT("Class:%s; MaskColorHex:%s; MaskMaterial:%s"), *Class, *MaskColorHex,
			MaskMaterial ? *MaskMaterial->GetName() : *FString("NULL"));
	}
};

/**
 * Stores the semantic skeletal data of its parent skeletal mesh component
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), hidecategories = (HLOD, Cooking, Transform), DisplayName="SL Skeletal Data")
class USEMLOG_API USLSkeletalDataComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USLSkeletalDataComponent();

#if WITH_EDITOR
	// Called when a property is changed in the editor
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR

public:
	// Init the component for runtime, returns true if it was already init, or init was successful
	bool Init();

	// Check if the component is init (and valid)
	bool IsInit() const { return bInit; };

private:
#if WITH_EDITOR
	// Update the data
	void LoadData();
#endif // WITH_EDITOR

	// Clear data
	void ClearData();

	// Set the skeletal parent, returns true if successful or is already set
	bool SetSkeletalParent();

	// Set the semantic parent and its data, returns true if successful or is already set
	bool SetSemanticOwnerAndData();

	// Set data for all the bones (empty for the ones without semantics)
	void SetDataForAllBones();

public:
	// Map of bones to their class names
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	TMap<FName, FSLBoneData> SemanticBonesData;

	// All bones map
	UPROPERTY(VisibleAnywhere, Category = "Semantic Logger")
	TMap<FName, FSLBoneData> AllBonesData;

	// The attach parent skeletal mesh
	UPROPERTY(VisibleAnywhere, Category = "Semantic Logger")
	USkeletalMeshComponent* SkeletalMeshParent;

	// Semantic owner
	UPROPERTY(VisibleAnywhere, Category = "Semantic Logger")
	UObject* SemanticOwner;

	// Semantic data of the owner	
	FSLEntity OwnerSemanticData;

private:
	// Flag marking the component as init (and valid) for runtime 
	bool bInit;

	// Load the bones semantic information from the skeletal data asset
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	USLSkeletalDataAsset* SkeletalDataAsset;

	// Load new data without removing previous one button
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	bool bReloadData;

	// Remove all previous data mimic button
	UPROPERTY(EditAnywhere, Category = "Semantic Logger")
	bool bClearAllData;
};
