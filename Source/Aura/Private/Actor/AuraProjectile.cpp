// Copyright AnnihilateSword.


#include "Actor/AuraProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	m_Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(m_Sphere);
	m_Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	m_Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	m_Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	m_Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	m_ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	m_ProjectileMovement->InitialSpeed = 550.f;
	m_ProjectileMovement->MaxSpeed = 550.f;
	m_ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	m_Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
