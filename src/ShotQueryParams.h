//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_SHOTQUERYPARAMS_H
#define IDEA_SHOTQUERYPARAMS_H

#include <optional>

#include "common/config/Table.h"

namespace billiards::shots {

	class ShotQueryParams : public json::Serializable {
	public:
		std::optional<double> cut_tolerance;
		config::Table table;
		std::vector<step_type::ShotStepType> step_types;

		int range_begin;
		int range_end;

		// bool max_depth...
		// bool include_carems...
		// bool include_banks...

		ShotQueryParams()
			: cut_tolerance{0.1}
			, table{}
			, step_types{step_type::CUE, step_type::STRIKE, step_type::POCKET}
			, range_begin{0}
			, range_end{50}
		{}

		~ShotQueryParams() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("table");
			table.to_json(writer);
			writer.key("range");
			writer.begin_object();
			writer.field("begin", range_begin);
			writer.field("end", range_end);
			writer.end_object();
			writer.key("step-types");
			writer.begin_array();
			for (const auto& type : step_types) {
				writer.value(step_type::to_string(type));
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			if (value.contains("range") && value["range"].is_object()) {
				auto& range = value["range"];
				if (range.contains("begin") && range["begin"].is_number()) {
					range_begin = range["begin"].get<int>();
				}
				if (range.contains("end") && range["end"].is_number()) {
					range_end = range["end"].get<int>();
				}
			}
			if (value.contains("table") && value["table"].is_object()) {
				table.parse(value["table"]);
			}

			if (value.contains("step-types") && value["step-types"].is_array()) {
				step_types.clear();
				for (const auto& str : value["step-types"]) {
					step_types.emplace_back(step_type::from_string(str.get<std::string>()));
				}
			}
		}
	};
}


#endif //IDEA_SHOTQUERYPARAMS_H
