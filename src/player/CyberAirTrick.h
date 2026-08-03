#pragma once

namespace revisited::player {
	inline void initializePlayerCyberAirTrick(app::player::Player* self) {
		if (auto* levelInfo = self->gameManager->GetService<app::level::LevelInfo>())
			if (levelInfo->GetStageData()->GetAttributeFlag(app::level::StageData::AttributeFlags::CYBER))
				if (auto* blackboardStatus = self->GetComponent<app::player::GOCPlayerBlackboard>()->blackboard->GetContent<app::player::BlackboardStatus>())
					blackboardStatus->SetCombatFlag(app::player::BlackboardStatus::CombatFlag::AIR_TRICK, true);
	}
}
