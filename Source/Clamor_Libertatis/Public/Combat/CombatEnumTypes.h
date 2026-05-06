// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatEnumTypes.generated.h"


UENUM(BlueprintType)   
enum class ECombatEnumState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Attacking   UMETA(DisplayName = "Attacking"),
    Dodging     UMETA(DisplayName = "Dodging"),
    HitReact    UMETA(DisplayName = "HitReact"),
    Guarding    UMETA(DisplayName = "Guarding"),
    Dead        UMETA(DisplayName = "Dead")
};
