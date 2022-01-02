//
// Created by thallock on 10/1/21.
//

#ifndef IDEA_SHOTLISTPARAMS_H
#define IDEA_SHOTLISTPARAMS_H

#include <optional>

#include "billiards_common/config/TableDimensions.h"
#include "billiards_common/shots/StepWildCard.h"

namespace billiards::shots {

	class ShotListParams : public json::Serializable {
	public:
		std::optional<double> cut_tolerance;
		config::PoolConfiguration table;
		billiards::layout::Locations locations;
		std::vector<ShotStepWildCard> step_types;

		int range_begin;
		int range_end;

		bool random;

		// bool max_depth...
		// bool include_carems...
		// bool include_banks...

		ShotListParams()
			: cut_tolerance{0.1}
			, table{}
			, step_types{} // step_type::CUE, step_type::STRIKE, step_type::POCKET
			, range_begin{0}
			, range_end{50}
		{}

		~ShotListParams() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("table");
			table.to_json(writer);
			writer.key("range");
			writer.begin_object();
			writer.field("begin", range_begin);
			writer.field("end", range_end);
			writer.end_object();
			writer.key("step-wild-cards");
			writer.begin_array();
			for (const auto& card : step_types) {
				card.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			if (HAS_OBJECT(value, "range")) {
				auto& range = value["range"];
				if (HAS_NUMBER(range, "begin")) {
					range_begin = range["begin"].get<int>();
				}
				if (HAS_NUMBER(range, "end")) {
					range_end = range["end"].get<int>();
				}
			}

			REQUIRE_CHILD(status, value, "table", table, "List parameters must have a table");
			REQUIRE_CHILD(status, value, "locations", locations, "List parameters must have locations");

			ENSURE_ARRAY(status, value, "step-wild-cards", "Must have step wild cards");
			step_types.clear();
			for (const auto& v : value["step-wild-cards"]) {
				step_types.emplace_back();
				PARSE_CHILD(status, v, step_types.back());
			}
		}
	};
}
#endif //IDEA_SHOTLISTPARAMS_H
