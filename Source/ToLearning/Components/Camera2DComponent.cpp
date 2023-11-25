// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera2DComponent.h"

UCamera2DComponent::UCamera2DComponent() {
	SetProjectionMode(ECameraProjectionMode::Orthographic);
	SetOrthoWidth(1530);
}

FVector UCamera2DComponent::GetOrthoSize() const {
	return FVector(OrthoWidth, 0, OrthoWidth / AspectRatio);
}
