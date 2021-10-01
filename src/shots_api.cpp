//
// Created by thallock on 9/17/21.
//


#include "billiards_common/3rd_party/crow_all.h"
#include "billiards_common/utils/crow_common.h"
#include "billiards_common/config/ports.h"

#include "list_shots.h"


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

					billiards::shots::ShotListParams params;
					billiards::json::ParseResult status;
					if (HAS_OBJECT(value, "params")) {
						params.parse(value["params"], status);
					} else {
						RETURN_ERROR("No params provided");
					}
					if (!status.success) {
						RETURN_ERROR("Unable to parse params");
					}

					billiards::utils::DefaultResponse def_resp{
						"Listed shots", true, "shots",
						[&params](billiards::json::SaxWriter& writer) {
							writer.begin_array();
							billiards::shots::list_shots(
								params,
								[&writer](const std::shared_ptr<billiards::shots::Shot>& ptr) {
									ptr->to_json(writer);
								});
							writer.end_array();
						}};

					crow::response resp{billiards::json::dump(def_resp)};
					resp.add_header("Access-Control-Allow-Origin", "*");
					resp.set_header("Content-Type", "application-json");
					resp.set_header("Mime-Type", "application-json");
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

					billiards::shots::ShotInfoParams params;
					billiards::json::ParseResult status;
					if (HAS_OBJECT(value, "params")) {
						params.parse(value["params"], status);
					} else {
						RETURN_ERROR("No params provided");
					}
					if (!status.success) {
						RETURN_ERROR("Unable to parse params");
					}

					billiards::shots::ShotInformation info{params.shot};
					try {
						calculate_shot(params, info);
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
