


#include "Actor/PointCollection.h"

#include "Kismet/KismetMathLibrary.h"
#include "Library/AuraWidgetControllerLibrary.h"


APointCollection::APointCollection()
{
 	
	PrimaryActorTick.bCanEverTick = false;
	
	Pt_0 = CreateDefaultSubobject<USceneComponent>("Pt_0");
	ImmutablePts.Add(Pt_0);
	SetRootComponent(Pt_0);
	
	Pt_1 = CreateDefaultSubobject<USceneComponent>("Pt_1");
	ImmutablePts.Add(Pt_1);
	Pt_1->SetupAttachment(Pt_0);
	
	Pt_2 = CreateDefaultSubobject<USceneComponent>("Pt_2");
	ImmutablePts.Add(Pt_2);
	Pt_2->SetupAttachment(Pt_0);
	
	Pt_3 = CreateDefaultSubobject<USceneComponent>("Pt_3");
	ImmutablePts.Add(Pt_3);
	Pt_3->SetupAttachment(Pt_0);
	
	Pt_4 = CreateDefaultSubobject<USceneComponent>("Pt_4");
	ImmutablePts.Add(Pt_4);
	Pt_4->SetupAttachment(Pt_0);
	
	Pt_5 = CreateDefaultSubobject<USceneComponent>("Pt_5");
	ImmutablePts.Add(Pt_5);
	Pt_5->SetupAttachment(Pt_0);
	
	Pt_6 = CreateDefaultSubobject<USceneComponent>("Pt_6");
	ImmutablePts.Add(Pt_6);
	Pt_6->SetupAttachment(Pt_0);
	
	Pt_7 = CreateDefaultSubobject<USceneComponent>("Pt_7");
	ImmutablePts.Add(Pt_7);
	Pt_7->SetupAttachment(Pt_0);
	
	Pt_8 = CreateDefaultSubobject<USceneComponent>("Pt_8");
	ImmutablePts.Add(Pt_8);
	Pt_8->SetupAttachment(Pt_0);
	
	Pt_9 = CreateDefaultSubobject<USceneComponent>("Pt_9");
	ImmutablePts.Add(Pt_9);
	Pt_9->SetupAttachment(Pt_0);
	
	Pt_10 = CreateDefaultSubobject<USceneComponent>("Pt_10");
	ImmutablePts.Add(Pt_10);
	Pt_10->SetupAttachment(Pt_0);
	

}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, int32 NumPoints,float YawOverride)
{
	check(ImmutablePts.Num() >= NumPoints);
	
	TArray<USceneComponent*> ArrayCopy;
	
	for (USceneComponent* Pt : ImmutablePts)
	{
		if (ArrayCopy.Num() > NumPoints) return ArrayCopy;
		
		//根据原点的位置进行旋转设置世界位置
		if (Pt!= Pt_0)
		{
			FVector ToPoint = Pt->GetComponentLocation() - Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride,FVector::UpVector);
			Pt->SetWorldLocation(Pt_0->GetComponentLocation() + ToPoint);
		}
		
		//上下检测的起始和结尾
		FVector StartPoint = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		FVector EndPoint = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		//获取检测附近的Actor(玩家或者怪物)
		TArray<AActor*> IgnoreActor;
		UAuraWidgetControllerLibrary::GetLifeActorWithingRadius(this,IgnoreActor,TArray<AActor*>(),1500.f,Pt->GetComponentLocation());
		
		//最后检测函数
		FHitResult Hit;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(IgnoreActor);
		GetWorld()->LineTraceSingleByProfile(Hit,StartPoint,EndPoint,FName("BlockAll"),QueryParams);
		
		//检测完后设置场景组件的位置和旋转
		FVector AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y,Hit.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal));
		
		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}


void APointCollection::BeginPlay()
{
	Super::BeginPlay();
	
}



