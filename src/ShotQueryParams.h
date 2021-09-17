//
// Created by thallock on 9/17/21.
//

#ifndef IDEA_SHOTQUERYPARAMS_H
#define IDEA_SHOTQUERYPARAMS_H

#include "common/config/Table.h"

namespace billiards::shots {

	class ShotQueryParams : public json::Serializable {
	public:
		double cut_tolerance;
		config::Table table;

		// bool max_depth...
		// bool include_carems...
		// bool include_banks...

		ShotQueryParams()
			: cut_tolerance{0.1}
			, table{}
		{}

		~ShotQueryParams() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			// TODO
		}
	};
}


#endif //IDEA_SHOTQUERYPARAMS_H
