//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_SHOT_H
#define IDEA_SHOT_H

#include "shot_calculation/cue.h"
#include "shot_calculation/kiss.h"
#include "shot_calculation/rail.h"
#include "shot_calculation/strike.h"
#include "shot_calculation/pocket.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign(AssignmentStatus& ass) {
		switch (ass.info.get_shot_type(dest)) {
			case billiards::shots::step_type::CUE:
				return maybe_assign_cue(ass);
			case billiards::shots::step_type::RAIL:
				return maybe_assign_rail(ass);
			case billiards::shots::step_type::POCKET:
				return maybe_assign_rail(ass);
			case billiards::shots::step_type::STRIKE:
				return maybe_assign_strike(ass);
			case billiards::shots::step_type::Kiss:
				return maybe_assign_strike(ass);
			case billiards::shots::step_type::UNKNOWN:
			default:
				return false;
		}
	}

	inline
	bool calculate_shot(const ShotInfoParams& params, ShotInformation& info) {
		// TODO: Fit the Locations to the Table...

		const auto num_steps = info.shot.steps.size();
		for (int i = 0; i < num_steps; i++) {
			info.destinations.emplace_back(i);
		}

		Assignment assignment{params, info, num_steps};
		while (true) {
			bool progress = false;
			bool has_unassigned = false;
			for (int i = 0; i < num_steps; i++) {
				if (assigned[i].assigned()) {
					continue;
				}
				has_unassigned = true;
				progress |= maybe_assign(assignment, i);
			}
			if (!has_unassigned) {
				break;
			}
			if (!progress) {
				return false;
			}
		}

		// assign cueing

		return true;
	}
}
#endif //IDEA_SHOT_H
