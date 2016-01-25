
#pragma once

#include "AnimNotifyWithActor.generated.h"

/**
 * This strategy spawns AStaticMeshActor in order to create a valid UStaticMeshComponent object, that will be bound to
 * SkeletalMesh that the animation is playing for.
 */
UCLASS()
class UAnimNotifyWithActor : public UAnimNotifyState
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

	UPROPERTY()
	AStaticMeshActor* MeshActor;

	virtual void NotifyBegin( USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration ) override;

	virtual void NotifyEnd( USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation ) override;
};
