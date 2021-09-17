//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_LIST_SHOTS_H
#define IDEA_LIST_SHOTS_H

#include <functional>
#include <memory>

#include "common/shots/Layout.h"
#include "common/shots/ShotInformation.h"

#include "./ShotQueryParams.h"


namespace billiards::shots {

	void list_shots(
		const ShotQueryParams& params,
		const layout::Layout& layout,
		std::function<void(const std::shared_ptr<const ShotInformation>&)> receiver
	) {
		auto *cue_ptr = layout.cue_ball();
		if (cue_ptr == nullptr) {
			return;
		}
		const layout::LocatedBall& cue = *cue_ptr;

		for (const auto& ball : layout.balls) {
			if (ball.info.is_cue()) {
				continue;
			}

			for (int pocket = 0; pocket < 6; pocket++) {
				auto info = std::make_shared<ShotInformation>();
				info->shot.steps.emplace_back(new CueStep{0});
			}
		}
	}

}


#endif //IDEA_LIST_SHOTS_H
