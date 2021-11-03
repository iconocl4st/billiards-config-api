//
// Created by thallock on 9/14/21.
//

#include "billiards_common/3rd_party/crow_all.h"
#include "billiards_common/utils/crow_common.h"

#include "get_default_host.h"

#include "Configuration.h"
#include "billiards_common/utils/SerializableCache.h"

int main(int argc, char **argv) {
    // Could read as arg...
    // Could use /app/ ...

    std::string config_location{"./config.json"};
    for (int i = 0; i < argc - 1; i++) {
        if ("--config-file" == std::string{argv[i]} || "-f" == std::string{argv[i]}) {
            config_location = argv[i+1];
            break;
        }
    }

    billiards::json::SerializableCache<billiards::config::Configuration> cache{config_location};

	crow::SimpleApp app;

	DO_STATUS_ENDPOINT();

	CROW_ROUTE(app, "/")
		.methods("GET"_method, "PUT"_method, "OPTIONS"_method)
		([&](const crow::request& req) {
			if (req.method == "OPTIONS"_method) {
				HANDLE_OPTIONS;
			} else if (req.method == "GET"_method) {
				RETURN_SUCCESS_WITH_DATA("Retrieved current configuration", "config", cache.cached);
			} else if (req.method == "PUT"_method) {
				nlohmann::json value = nlohmann::json::parse(req.body);
				billiards::json::ParseResult result;
				if (HAS_OBJECT(value, "config")) {
					cache.cached.parse(value["config"], result);
				} else {
					RETURN_ERROR("No config provided");
				}
				if (!result.success) {
					std::cerr << "Invalid request" << std::endl;
					std::cerr << result.error_msg.str() << std::endl;
					RETURN_ERROR("Unable to parse config");
				}
                if (!cache.save()) {
                    RETURN_ERROR("Loaded configuration, but unable to save.");
                }

				RETURN_SUCCESS("Loaded configuration");
			} else {
				return crow::response(404);
			}
		});

    CROW_ROUTE(app, "/reload")
        .methods("POST"_method, "OPTIONS"_method)
        ([&](const crow::request& req) {
            if (req.method == "OPTIONS"_method) {
                HANDLE_OPTIONS;
            } else if (req.method == "POST"_method) {
                if (!cache.reload()) {
                    RETURN_ERROR("Unable to read configuration.");
                }
                RETURN_SUCCESS("Loaded configuration from file.");
            } else {
                return crow::response(404);
            }
        });

	app.port(billiards::config::ports::CONFIG_API_PORT).run();
	return 0;
}
