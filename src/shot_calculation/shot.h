//
// Created by thallock on 10/5/21.
//

#ifndef IDEA_SHOT_H
#define IDEA_SHOT_H

#include <fstream>

#include "shot_calculation/cue.h"
#include "shot_calculation/kiss.h"
#include "shot_calculation/rail.h"
#include "shot_calculation/strike.h"
#include "shot_calculation/pocket.h"
#include "shot_calculation/cueing.h"

namespace billiards::shots {

	[[nodiscard]] inline
	bool maybe_assign(AssignmentStatus& ass) {
		switch (ass.info.get_shot_type(ass.get_info())) {
			case billiards::shots::step_type::CUE:
				return maybe_assign_cue(ass);
			case billiards::shots::step_type::RAIL:
				return maybe_assign_rail(ass);
			case billiards::shots::step_type::POCKET:
				return maybe_assign_pocket(ass);
			case billiards::shots::step_type::STRIKE:
				return maybe_assign_strike(ass);
			case billiards::shots::step_type::KISS:
				return maybe_assign_kiss(ass);
			case billiards::shots::step_type::UNKNOWN:
			default:
				return false;
		}
	}

	[[nodiscard]] inline
	bool calculate_shot(const ShotInfoParams& params, ShotInformation& info) {
		// TODO: Fit the Locations to the Table...

		const int num_steps = (int) info.shot.steps.size();
		for (int i = 0; i < num_steps; i++) {
			info.infos.emplace_back(i);
		}

		std::ofstream outf{"output.txt"};

		Assignment assignment{params, info, num_steps};
		int index = 0;
		outf << "=============================" << std::endl;
		while (true) {
			outf << "Assignment iteration " << index++ << std::endl;
			outf << assignment << std::endl;

			bool progress = false;
			bool has_unassigned = false;
			for (int i = 0; i < num_steps; i++) {
				if (assignment.statuses[i].assigned()) {
					continue;
				}
				has_unassigned = true;
				progress |= maybe_assign(assignment.statuses[i]);
			}
			if (!has_unassigned) {
				break;
			}
			if (!progress) {
				outf << "No progress" << std::endl;
				outf << assignment << std::endl;
				return false;
			}
		}

		assign_cueing(assignment);
		return info.valid;
	}
}
#endif //IDEA_SHOT_H
