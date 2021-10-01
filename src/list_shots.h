//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_LIST_SHOTS_H
#define IDEA_LIST_SHOTS_H

#include <functional>
#include <memory>

#include "common/shots/Locations.h"
#include "common/shots/ShotInformation.h"
#include "./calculate_shot.h"

#include "./ShotQueryParams.h"
#include "ShotsIterator.h"


namespace billiards::shots {

	[[nodiscard]] inline
	bool shot_is_possible(
		const config::Table& table,
		const layout::Locations& locations,
		const std::shared_ptr<Shot>& shot
	) {
		ShotInformation info;
		info.shot = *shot;
		calculate_shot(table, locations, info);
		return info.is_possible(table, locations);
	}

	inline
	void list_shots(
		const ShotQueryParams& params,
		const layout::Locations& locations,
		const std::function<void(std::shared_ptr<Shot>)>& receiver
	) {
		int cue_index = locations.cue_ball_index();
		if (cue_index < 0) {
			return;
		}

		int shot_count = 0;
		ShotsIterator iterator;
		iterator.assign(params.table, locations, params.step_types);
		do {
			auto shot = std::make_shared<Shot>();
			for (const auto& it : iterator.children) {
				shot->steps.emplace_back(it.create_step());
			}

			if (!shot_is_possible(params.table, locations, shot)) {
				continue;
			}

			if (shot_count >= params.range_begin) {
				receiver(shot);
			}
			if (shot_count > params.range_end) {
				return;
			}
			++shot_count;
		} while (iterator.increment());
	}
}


#endif //IDEA_LIST_SHOTS_H

