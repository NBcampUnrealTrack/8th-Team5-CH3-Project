#include "UI/EnemyHP/EnemyHPBarWidget.h"
#include "Enemy/BaseEnemy.h"
#include "Enemy/ActorComponent/Enemy_StatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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

        const FEnemyStat& Stat = Enemy->GetEnemyStatComp()->GetEnemyStat();

        UpdateHP(Stat.HP, Stat.MaxHP);

        if (EnemyNameText)
        {
            EnemyNameText->SetText(FText::FromName(Stat.EnemyName));
        }
    }
}