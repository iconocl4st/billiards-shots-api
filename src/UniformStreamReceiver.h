//
// Created by thallock on 10/6/21.
//

#ifndef IDEA_UNIFORMSTREAMRECEIVER_H
#define IDEA_UNIFORMSTREAMRECEIVER_H

#include "billiards_common/shots/Shot.h"
#include <ctime>

namespace billiards::shots {

	class UniformStreamReceiver {
	public:
		bool assigned{false};
		std::shared_ptr<Shot> shot{nullptr};
		int count{0};

		[[nodiscard]] inline
		std::function<void(const std::shared_ptr<billiards::shots::Shot>&)> get_receiver() {
			return [&](const std::shared_ptr<billiards::shots::Shot>& ptr) {
				// TODO: Make this a consistent seed...
				if (++count == 1 || count - 1 == (std::rand() % count)) {
					assigned = true;
					shot = ptr;
				}
			};
		}
	};
}

#endif //IDEA_UNIFORMSTREAMRECEIVER_H
