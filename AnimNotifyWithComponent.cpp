
#include "AnimNotifyWithComponent.h"


UAnimNotifyWithComponent::UAnimNotifyWithComponent(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	this->Mesh = NULL;
	this->MeshComponent = NULL;

	this->SocketName = TEXT( "RightHand" );
}

void UAnimNotifyWithComponent::NotifyBegin( USkeletalMeshComponent* AnimMeshComp, UAnimSequenceBase* Animation, float TotalDuration )
{
	Super::NotifyBegin( AnimMeshComp, Animation, TotalDuration );

	if ( this->Mesh == NULL ) {
		UE_LOG( LogTemp, Error, TEXT( "UAnimNotifyWithComponent::NotifyBegin No static mesh to spawn" ) );
		return;
	}

	if ( this->MeshComponent != NULL ) {
		UE_LOG( LogTemp, Error, TEXT( "UAnimNotifyWithComponent::NotifyBegin Mesh comp already spawned, no need to create new" ) );
		return;
	}

	if ( AnimMeshComp == NULL ) {
		UE_LOG( LogTemp, Error, TEXT( "UAnimNotifyWithComponent::NotifyBegin No anim mesh comp" ) );
		return;
	}

	if ( AnimMeshComp->GetOuter() != NULL ) {
		UE_LOG( LogTemp, Log, TEXT( "Has an outer %s" ), *AnimMeshComp->GetOuter()->GetName() );
		if ( AnimMeshComp->GetOwner() == NULL ) {
			UE_LOG( LogTemp, Log, TEXT( "But doesnt owner" ) );
		}
	}

	// Spawn mesh component and attach it to the animated mesh component.
	auto StaticMeshComp = NewObject<UStaticMeshComponent>( AnimMeshComp->GetOuter(), TEXT( "MeshComp" ) );
	// TODO: The whole problem was caused by wrong outer object. Seems like in the editor AnimMeshComp has
	// waaaay different parent.

	if ( StaticMeshComp == NULL ) {
		UE_LOG( LogTemp, Error, TEXT( "UAnimNotifyWithComponent::NotifyBegin Couldnt spawn a mesh component" ) );
		return;
	}

	StaticMeshComp->SetMobility( EComponentMobility::Movable );
	StaticMeshComp->SetStaticMesh( this->Mesh );
	StaticMeshComp->SetHiddenInGame( false );
	StaticMeshComp->SetOwnerNoSee( false );
	StaticMeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	StaticMeshComp->AttachTo( AnimMeshComp, this->SocketName, EAttachLocation::SnapToTargetIncludingScale );
	StaticMeshComp->RegisterComponent();

	// Hold the reference so we can remove it later on in NotifyEnd method.
	this->MeshComponent = StaticMeshComp;

	UE_LOG( LogTemp, Log, TEXT( "UAnimNotifyWithComponent::NotifyBegin Mesh Spawned" ) );
}

void UAnimNotifyWithComponent::NotifyEnd( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	Super::NotifyEnd( MeshComp, Animation );

	// On end notify we need to remove mesh(es) from the scene.
	if ( this->MeshComponent != NULL ) {
		//this->MeshComponent->GetOwner()->RemoveInstanceComponent( this->MeshComponent );
		this->MeshComponent->DestroyComponent();
		//this->MeshComponent->ConditionalBeginDestroy();
		this->MeshComponent = NULL;
	}
}
