// Fill out your copyright notice in the Description page of Project Settings.


#include "HPPlayerState.h"

AHPPlayerState::AHPPlayerState()
{

}

int AHPPlayerState::GetHealth()
{
	return Health;
}

int AHPPlayerState::GetCurrentAmmo()
{
	return Ammo;
}

int AHPPlayerState::GetMaxAmmo()
{
	return MaxAmmo;
}
