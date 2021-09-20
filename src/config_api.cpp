//
// Created by thallock on 9/14/21.
//

#include "common/3rd_party/crow_all.h"
#include "common/utils/crow_common.h"
#include "common/config/ports.h"

#include "./Configuration.h"


int main(int argc, char **argv) {
	billiards::config::Configuration configuration;
	// load the configuration from file...

	crow::SimpleApp app;

	CROW_ROUTE(app, "/")
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)
		([&configuration](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "GET"_method) {
				RETURN_SUCCESS_WITH_DATA("Retrieved current configuration", "config", configuration);
			} else if (req.method == "PUT"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				configuration.parse(value);
				RETURN_SUCCESS("Loaded configuration");
			} else {
				return crow::response(404);
			}
		});

	app.port(billiards::config::ports::CONFIG_API_PORT).run();
	return 0;
}
