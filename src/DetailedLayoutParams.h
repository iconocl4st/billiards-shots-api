//
// Created by thallock on 12/31/21.
//

#ifndef IDEA_DETAILEDLAYOUTPARAMS_H
#define IDEA_DETAILEDLAYOUTPARAMS_H


#include "billiards_common/config/PoolConfiguration.h"
#include "billiards_common/layouts/Layout.h"

namespace billiards::shots {

	class DetailedLayoutParams : public json::Serializable {
	public:
		config::PoolConfiguration config;
		layout::Layout layout;

		DetailedLayoutParams() = default;
		~DetailedLayoutParams() override = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("config");
			config.to_json(writer);
			writer.key("layout");
			layout.to_json(writer);
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "config", config, "List parameters must have a configuration");
			REQUIRE_CHILD(status, value, "layout", layout, "List parameters must a shot");
		}
	};
}

#endif //IDEA_DETAILEDLAYOUTPARAMS_H
