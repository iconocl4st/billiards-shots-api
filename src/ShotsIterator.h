//
// Created by thallock on 9/30/21.
//

#ifndef IDEA_SHOTSITERATOR_H
#define IDEA_SHOTSITERATOR_H

#include <utility>

#include "billiards_common/shots/ShotStep.h"
#include "billiards_common/shots/Locations.h"
#include "billiards_common/config/TableDimensions.h"

namespace billiards::shots {

	class ShotStepIterator {
	public:
		step_type::ShotStepType type;
		int current;
		std::shared_ptr<std::vector<int>> map;

		ShotStepIterator(step_type::ShotStepType type, const std::shared_ptr<std::vector<int>>& map)
			: type{type}
			, current{0}
			, map{map}
		{
			if (map->empty()) {
				throw std::runtime_error{"Cannot iterate over empty list"};
			}
		}

		ShotStepIterator(step_type::ShotStepType type, int max)
			: type{type}
			, current{0}
			, map{std::make_shared<std::vector<int>>()}
		{
			if (max == 0) {
				throw std::runtime_error{"Cannot iterate over empty set"};
			}
			for (int i = 0; i < max; i++) {
				map->push_back(i);
			}
		}

		~ShotStepIterator() = default;

		[[nodiscard]] inline
		int get() const {
			return map->at(current);
		}

		[[nodiscard]] inline
		std::shared_ptr<ShotStep> create_step() const {
			switch (type) {
				case step_type::CUE:
					return std::make_shared<CueStep>(get());
				case step_type::POCKET:
					return std::make_shared<PocketStep>(get());
				case step_type::STRIKE:
					return std::make_shared<StrikeStep>(get());
				case step_type::RAIL:
					return std::make_shared<RailStep>(get());
				case step_type::KISS:
					throw std::runtime_error{"Implement me"};
				case step_type::UNKNOWN:
				default:
					throw std::runtime_error{"Invalid shot step type"};
			}
		}

		inline
		void reset() {
			current = 0;
		}

		[[nodiscard]] inline
		bool has_next() const {
			return current < map->size() - 1;
		}

		inline
		void increment() {
			current++;
		}
	};

	class ShotsIterator {
	public:
		std::vector<ShotStepIterator> children;

		ShotsIterator() : children{} {}

		~ShotsIterator() = default;

//		inline
//		void reset() {
//			for (auto& it : children) {
//				it.reset();
//			}
//		}
//
//		[[nodiscard]] inline
//		bool has_next() const {
//			return std::any_of(
//				children.begin(),
//				children.end(),
//				[](const ShotStepIterator& it) {
//					return it.has_next();
//				});
//		}

		[[nodiscard]] inline
		bool increment() {
			for (auto& it : children) {
				if (!it.has_next()) {
					it.reset();
				} else {
					it.increment();
					return true;
				}
			}
			return false;
//			throw std::runtime_error{"Tried to call next when there is none..."};
		}


		inline void assign_any(
			const config::PoolConfiguration& table,
			const layout::Locations& locations,
			const ShotStepWildCard& wild_card
		) {
			switch (wild_card.step_type) {
				case step_type::RAIL: {
					children.emplace_back(wild_card.step_type, config::constants::NUM_RAILS);
					break;
				}
				case step_type::POCKET: {
					children.emplace_back(wild_card.step_type, config::constants::NUM_POCKETS);
					break;
				}
				case step_type::KISS:
				case step_type::CUE: {
					auto map = std::make_shared<std::vector<int>>();
					for (int i = 0; i < locations.balls.size(); i++) {
						if (layout::vball::virtual_type::is_cue_ball(locations.balls[i].get_type())) {
							map->push_back(i);
						}
					}
					children.emplace_back(wild_card.step_type, map);
					break;
				}
				case step_type::STRIKE: {
					auto map = std::make_shared<std::vector<int>>();
					for (int i = 0; i < locations.balls.size(); i++) {
						if (layout::vball::virtual_type::is_object_ball(locations.balls[i].get_type())) {
							map->push_back(i);
						}
					}
					children.emplace_back(wild_card.step_type, map);
					break;
				}
				case step_type::UNKNOWN:
				default:
					break;
			}
		}

		inline void assign(
			const config::PoolConfiguration& table,
			const layout::Locations& locations,
			const ShotStepWildCard& wild_card
		) {
			switch (wild_card.choices_type) {
				case wild_card::ONE_OF: {
					auto map = std::make_shared<std::vector<int>>();
					for (const int& v : wild_card.possible_values) {
						map->push_back(v);
					}
					children.emplace_back(wild_card.step_type, map);
					break;
				}
				case wild_card::EXACT: {
					auto map = std::make_shared<std::vector<int>>();
					map->push_back(wild_card.exact_value);
					children.emplace_back(wild_card.step_type, map);
					break;
				}
				case wild_card::ANY: {
					assign_any(table, locations, wild_card);
					break;
				}
				case wild_card::UNKNOWN:
				default:
					break;
			}
		}

		inline
		void assign(
			const config::PoolConfiguration& table,
			const layout::Locations& locations,
			const std::vector<ShotStepWildCard>& wild_cards
		) {
			for (const auto& wild_card : wild_cards) {
				assign(table, locations, wild_card);
			}
		}
	};
}
#endif //IDEA_SHOTSITERATOR_H
