
#pragma once

#include "CoreMinimal.h"

class ARoomBase;

#include "RoomArrayData.generated.h"

// ルーム配列を管理するための構造体
USTRUCT(BlueprintType)
struct FRoomArrayData
{
    GENERATED_BODY()

    // この種類のルームテンプレート一覧
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Room")
    TArray<TSubclassOf<ARoomBase>> Rooms;
};
