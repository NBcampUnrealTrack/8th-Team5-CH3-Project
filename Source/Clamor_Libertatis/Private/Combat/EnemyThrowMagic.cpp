#include "Combat/EnemyThrowMagic.h"

bool AEnemyThrowMagic::ShouldReactToHit(AActor* OtherActor) const
{
	if (!OtherActor) return false;

	// 플레이어는 항상 반응
	if (OtherActor->ActorHasTag(TEXT("Player"))) return true;

	// Pawn이 아닌 경우 (바닥, 벽 등 지형) → 반응해서 투사체 파괴
	if (!Cast<APawn>(OtherActor)) return true;

	// Pawn이지만 Player 태그 없음 = 적/미니언 → 무시하고 통과
	return false;
}
