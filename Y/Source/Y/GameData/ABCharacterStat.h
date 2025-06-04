
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ABCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FABCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FABCharacterStat()
		: MaxHp(0.0f), Attack(0.0f), AttackRange(0.0f), AttackSpeed(0.0f), MovementSpeed(0.0f)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat")
	float MovementSpeed;

	/*float GetValue(const FName& PropKey) const
	{
		if (PropKey == "MaxHp") return MaxHp;
		if (PropKey == "Attack") return Attack;
		if (PropKey == "AttackRange") return AttackRange;
		if (PropKey == "AttackSpeed") return AttackSpeed;
		if (PropKey == "MovementSpeed") return MovementSpeed;
		return 0.0f;
	}*/

	FABCharacterStat operator+(const FABCharacterStat& Other) const
	{	
		const float* const ThisPtr = reinterpret_cast<const float*>(this);
		const float* const OtherPtr = reinterpret_cast<const float*>(&Other);

		FABCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		for (int32 i = 0; i < sizeof(FABCharacterStat) / sizeof(float); ++i)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		/*i++ it is bad*/
		
		return Result;
	}

};