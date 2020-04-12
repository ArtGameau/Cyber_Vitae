// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define SURFACE_FLESHDEFAULT SurfaceType1
#define SURFACE_FLESHVULNERABLE SurfaceType2

#define COLLISION_WEAPON	ECC_GameTraceChannel1
#define COLLISION_INTERACTABLE ECC_GameTraceChannel2


/* Stencil index mapping to PP_OutlineColored */
#define STENCIL_FRIENDLY_OUTLINE 252;
#define STENCIL_NEUTRAL_OUTLINE 253;
#define STENCIL_ENEMY_OUTLINE 254;
#define STENCIL_ITEMHIGHLIGHT 255;