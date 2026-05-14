#include "UI/EnemyHP/EnemyHPBarWidget.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

void UEnemyHPBarWidget::UpdateHP(float CurrentHP, float MaxHP)
{
    if (HPBar && MaxHP > 0.f)
    {
        HPBar->SetPercent(CurrentHP / MaxHP);
    }
}

void UEnemyHPBarWidget::SetTargetEnemy(ABaseEnemy* Enemy)
{
    if (TargetEnemy.IsValid() && TargetEnemy->GetEnemyStatComp())
    {
        TargetEnemy->GetEnemyStatComp()->OnHPChanged.RemoveDynamic(
            this, &UEnemyHPBarWidget::UpdateHP
        );
    }

    TargetEnemy = Enemy;

    if (Enemy && Enemy->GetEnemyStatComp())
    {
        Enemy->GetEnemyStatComp()->OnHPChanged.AddDynamic(
            this, &UEnemyHPBarWidget::UpdateHP
        );

        UpdateHP(
            Enemy->GetEnemyStatComp()->GetEnemyStat().HP,
            Enemy->GetEnemyStatComp()->GetEnemyStat().MaxHP
        );
    }
}
