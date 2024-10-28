// Fill out your copyright notice in the Description page of Project Settings.


#include "GradCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"

UGradCameraMode_ThirdPerson::UGradCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{}

void UGradCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	// - 라이라에서는 이 함수 부분에서 벽 판정, 앉았을 때 등 많은 부분이 처리가 되어 있다.

	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();

	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);

	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// TargetOffsetCurve가 오버라이드되어 있다면, Curve에 값을 가져와서 적용 진행
	// - Camera 관점에서 Charater의 어느 부분에 Target으로 할지 결정하는 것으로 이해하면 됨
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}