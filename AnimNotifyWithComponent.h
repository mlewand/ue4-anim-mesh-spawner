
#pragma once

#include "AnimNotifyWithComponent.generated.h"

/*
 * A solution based on manually creating UStaticMeshComponent in NotifyBegin function.
 * It work ingame no problem, but causes weird freeze in the editor anim preview.
 */
UCLASS()
class UAnimNotifyWithComponent : public UAnimNotifyState
{
	GENERATED_UCLASS_BODY()

	// Exposed Mesh property, so that mesh can be changed in the editor.
	UPROPERTY( EditAnywhere )
	UStaticMesh* Mesh;

	// Name of the socked, where the item will be placed.
	UPROPERTY( EditAnywhere )
	FName SocketName;
private:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	virtual void NotifyBegin( USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration ) override;

	virtual void NotifyEnd( USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation ) override;
};
