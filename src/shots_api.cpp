//
// Created by thallock on 9/17/21.
//


#include "common/3rd_party/crow_all.h"
#include "common/utils/crow_common.h"
#include "common/config/ports.h"

#include "./list_shots.h"


int main(int argc, char **argv) {
	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/")
		.methods("POST"_method, "OPTIONS"_method)
		([](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "POST"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);

				billiards::shots::ShotQueryParams params;
				if (value.contains("params") && value["params"].is_object()) {
					params.parse(value["params"]);
				}

				billiards::layout::Layout layout;
				if (value.contains("layout") && value["layout"].is_object()) {
					layout.parse(value["layout"]);
				}

				billiards::utils::DefaultResponse def_resp{
					"Listed shots", true, "shot-infos",
					[&params, &layout](billiards::json::SaxWriter& writer) {
						writer.begin_array();
						billiards::shots::list_shots(
							params, layout,
							[&writer](std::shared_ptr<const billiards::shots::ShotInformation> ptr) {
								ptr->to_json(writer);
							});
						writer.end_array();
					}};

				crow::response resp{billiards::json::dump(def_resp)};
				resp.add_header("Access-Control-Allow-Origin", "*");
				return resp;
			} else {
				return crow::response(404);
			}
		});
	// Add graphics...

	app.port(billiards::config::ports::SHOTS_API_PORT).run();
	return 0;
}
