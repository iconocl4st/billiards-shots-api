//
// Created by thallock on 9/17/21.
//


#include "common/3rd_party/crow_all.h"
#include "common/utils/crow_common.h"
#include "common/config/ports.h"

#include "./list_shots.h"
#include "./calculate_shot.h"


int main(int argc, char **argv) {
	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/")
		.methods("POST"_method, "OPTIONS"_method)
		([](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "POST"_method) {
				try {
					nlohmann::json value = nlohmann::json::parse(req.body);

					billiards::shots::ShotQueryParams params;
					if (value.contains("params") && value["params"].is_object()) {
						params.parse(value["params"]);
					}

					billiards::layout::Locations locations;
					if (value.contains("locations") && value["locations"].is_object()) {
						locations.parse(value["locations"]);
					}

					billiards::utils::DefaultResponse def_resp{
						"Listed shots", true, "shots",
						[&params, &locations](billiards::json::SaxWriter& writer) {
							writer.begin_array();
							billiards::shots::list_shots(
								params, locations,
								[&writer](const std::shared_ptr<billiards::shots::Shot>& ptr) {
									ptr->to_json(writer);
								});
							writer.end_array();
						}};

					crow::response resp{billiards::json::dump(def_resp)};
					resp.add_header("Access-Control-Allow-Origin", "*");
					return resp;
				} catch (std::exception& e) {
					std::cout << e.what() << std::endl;
				}
				RETURN_ERROR("Unable to list shots");
			} else {
				return crow::response(404);
			}
		});

	CROW_ROUTE(app, "/info/")
		.methods("POST"_method, "OPTIONS"_method)
			([](const crow::request& req) {
				if (req.method == "OPTIONS"_method) {
					HANDLE_OPTIONS;
				} else if (req.method == "POST"_method) {
					nlohmann::json value = nlohmann::json::parse(req.body);

//					billiards::shots::ShotQueryParams params;
//					if (value.contains("params") && value["params"].is_object()) {
//						params.parse(value["params"]);
//					}

					billiards::config::Table table;
					if (value.contains("table") && value["table"].is_object()) {
						table.parse(value["table"]);
					}

					billiards::layout::Locations locations;
					if (value.contains("locations") && value["locations"].is_object()) {
						locations.parse(value["locations"]);
					}

					billiards::shots::Shot shot;
					if (value.contains("shot") && value["shot"].is_object()) {
						shot.parse(value["shot"]);
					}

					billiards::shots::ShotInformation info;
					info.shot = shot;
					try {
						calculate_shot(table, locations, info);
					} catch (const std::exception &exc) {
						std::cerr << exc.what();
						RETURN_ERROR("Unable to calculate shot");
					}

					RETURN_SUCCESS_WITH_DATA("Calculated shot", "shot-info", info);
				} else {
					return crow::response(404);
				}
			});

	app.port(billiards::config::ports::SHOTS_API_PORT).run();
	return 0;
}
