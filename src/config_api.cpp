//
// Created by thallock on 9/14/21.
//

#include "billiards_common/3rd_party/crow_all.h"
#include "billiards_common/utils/crow_common.h"
#include "billiards_common/config/ports.h"

#include "Configuration.h"


int main(int argc, char **argv) {
	billiards::config::Configuration configuration;
	// load the configuration from file...

	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/")
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)
		([&configuration](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "GET"_method) {
				RETURN_SUCCESS_WITH_DATA("Retrieved current configuration", "config", configuration);
			} else if (req.method == "PUT"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				billiards::json::ParseResult result;
				if (HAS_OBJECT(value, "config")) {
					configuration.parse(value, result);
				} else {
					RETURN_ERROR("No config provided");
				}
				if (!result.success) {
					RETURN_ERROR("Unable to parse config");
				}
				RETURN_SUCCESS("Loaded configuration");
			} else {
				return crow::response(404);
			}
		});

	app.port(billiards::config::ports::CONFIG_API_PORT).run();
	return 0;
}
