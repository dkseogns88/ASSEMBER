// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"

ABatteryPickup::ABatteryPickup()
{
	GetMesh()->SetSimulatePhysics(true);

	BatteryPower = 150.f;

}

void ABatteryPickup::WasCollected_Implementation()
{
	// �θ� Ŭ������ �Լ� ȣ��
	Super::WasCollected_Implementation();
	// ��ü �ı�
	Destroy();
}

float ABatteryPickup::GetPower()
{
	return BatteryPower;
}