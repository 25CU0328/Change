
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "Change/ETransformState.h"
#include "AnimalFormData.generated.h"

// Animation class
class UPaperZDAnimInstance;

// Data for each character form
USTRUCT(BlueprintType)
struct CHANGE_API FAnimalFormData : public FTableRowBase
{
    GENERATED_BODY()

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TSubclassOf<UPaperZDAnimInstance> AnimationInstance;

    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float WalkMaxSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float DashMaxSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Acceleration = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float Deceleration = 0.0f;

    // Collider
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider")
    float ColliderRadius = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collider")
    float ColliderHalfHeight = 0.0f;
};
