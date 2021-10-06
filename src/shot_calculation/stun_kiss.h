//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_STUN_GLANCE_H
#define IDEA_STUN_GLANCE_H

#include "shot_calculation/AssignmentStatus.h"
#include "billiards_common/shots/shot_helpers.h"
#include "shot_calculation/pocket.h"

namespace billiards::shots {


	[[nodiscard]] inline
	bool maybe_assign_stun_kiss_locations(AssignmentStatus& status) {
		if (status.already_has_source()) {

		}

	}
}

#endif //IDEA_STUN_GLANCE_H
