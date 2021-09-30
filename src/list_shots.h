//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_LIST_SHOTS_H
#define IDEA_LIST_SHOTS_H

#include <functional>
#include <memory>

#include "common/shots/Locations.h"
#include "common/shots/ShotInformation.h"

#include "./ShotQueryParams.h"
#include "ShotsIterator.h"


namespace billiards::shots {

	/*
	inline
	bool simple_shot_is_possible(
		const config::Table& table,
		const layout::Locations& locations,
		const Shot& shot
	) {
		// simple shot only...
		auto *cs = (CueStep *) shot.steps[0].get();
		auto *ss = (StrikeStep *) shot.steps[1].get();
		auto *ps = (PocketStep *) shot.steps[2].get();
		auto& cue_loc = locations.balls[cs->cue_ball].location;
		auto& obj_loc = locations.balls[ss->object_ball].location;
		auto poc_loc = table.pockets[ps->pocket].center();

		auto dir1 = obj_loc - cue_loc;
		auto dir2 = poc_loc - obj_loc;

		auto dot = dir1.dot(dir2);
		return dot > 0;
	} */

	[[nodiscard]] inline
	bool shot_is_possible(const std::shared_ptr<Shot>& shot) {
		return true;
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
			if (!shot_is_possible(shot)) {
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

