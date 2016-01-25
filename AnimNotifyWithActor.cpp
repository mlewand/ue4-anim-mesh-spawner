
#include "AnimNotifyWithActor.h"


UAnimNotifyWithActor::UAnimNotifyWithActor(const class FObjectInitializer& PCIP)
	: Super(PCIP)
{
	this->Mesh = NULL;
	this->MeshComponent = NULL;
	this->MeshActor = NULL;

	this->SocketName = TEXT( "RightHand" );
}

void UAnimNotifyWithActor::NotifyBegin( USkeletalMeshComponent* AnimMeshComp, UAnimSequenceBase* Animation, float TotalDuration )
{
	Super::NotifyBegin( AnimMeshComp, Animation, TotalDuration );

	// Get world reference from animated skeletal mesh component, as notify does not have World reference.
	UWorld* World = AnimMeshComp->GetWorld();

	if ( this->Mesh == NULL ) {
		UE_LOG( LogTemp, Error, TEXT( "UAnimNotifyWithActor::NotifyBegin No static mesh to spawn" ) );
		return;
	}

	if ( World == NULL ) {
		UE_LOG( LogTemp, Error, TEXT( "UAnimNotifyWithActor::NotifyBegin No world reference" ) );
		return;
	}

	FActorSpawnParameters params;
	params.Owner = AnimMeshComp->GetOwner();

	// Spawn actor to create a valid UStaticMeshComponent object.
	auto MeshActor = World->SpawnActor<AStaticMeshActor>( params );

	if ( MeshActor == NULL ) {
		// For some reason MeshActor couldn't be created.
		UE_LOG( LogTemp, Error, TEXT( "UAnimNotifyWithActor::NotifyBegin Couldn't spawn an actor" ) );
		return;
	}

	// Extract static mesh component, and reattach it to anim component.
	auto StaticMeshComp = MeshActor->GetStaticMeshComponent();

	StaticMeshComp->SetMobility( EComponentMobility::Movable );
	StaticMeshComp->SetStaticMesh( this->Mesh );
	StaticMeshComp->SetHiddenInGame( false );
	StaticMeshComp->SetOwnerNoSee( false );
	StaticMeshComp->SetCollisionEnabled( ECollisionEnabled::NoCollision );

	StaticMeshComp->AttachTo( AnimMeshComp, this->SocketName, EAttachLocation::SnapToTargetIncludingScale );
	StaticMeshComp->RegisterComponent();

	// Hold the reference so we can remove it later on in NotifyEnd method.
	this->MeshComponent = StaticMeshComp;
	this->MeshActor = MeshActor;

	// Another Quirk: even though StaticMeshComp is detached from MeshActor and attached to AnimMeshComp, if I remove
	// MeshActor here the StaticMeshComponent will be invisible. :(

	UE_LOG( LogTemp, Log, TEXT( "UAnimNotifyWithActor::NotifyBegin Mesh Spawned" ) );
}

void UAnimNotifyWithActor::NotifyEnd( USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation )
{
	Super::NotifyEnd( MeshComp, Animation );

	// On end notify we need to remove mesh(es) from the scene.
	if ( this->MeshComponent != NULL ) {
		this->MeshComponent->GetOwner()->RemoveInstanceComponent( this->MeshComponent );
		this->MeshComponent->DestroyComponent();
		this->MeshComponent = NULL;
	}

	if ( this->MeshActor != NULL ) {
		this->MeshActor->Destroy();
		this->MeshActor = NULL;
	}
}
