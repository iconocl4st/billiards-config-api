//
// Created by thallock on 9/19/21.
//

#ifndef IDEA_CONFIGURATION_H
#define IDEA_CONFIGURATION_H

#include "billiards_common/config/ports.h"
#include "billiards_common/config/Table.h"
#include "ApiUrl.h"
#include "get_default_host.h"

namespace billiards::config {
	class Configuration : public json::Serializable {
	public:
        // Put the urls of the api's things in here...
		Table table;
        std::string default_ip;
        std::array<ApiUrl, 6> urls;

        Configuration()
            : table{}
            , default_ip{get_default_host()}
            , urls{
                // ApiUrl{"Config", get_default_host(), CONFIG_API_PORT},
                ApiUrl{"Projector", default_ip, ports::PROJECTOR_API_PORT},
                ApiUrl{"Shots", default_ip, ports::SHOTS_API_PORT},
                ApiUrl{"Graphics", default_ip, ports::GRAPHICS_API_PORT},
                ApiUrl{"Layouts", default_ip, ports::LAYOUTS_API_PORT},
                ApiUrl{"Images", default_ip, ports::IMAGE_API_PORT},
                ApiUrl{"Attempts", default_ip, ports::ATTEMPTS_API_PORT},
            }
        {}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();

			writer.key("table");
			table.to_json(writer);

            writer.key("urls");
            writer.begin_array();
            for (const auto& url : urls) {
                url.to_json(writer);
            }
            writer.end_array();

			writer.end_object();
		};

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
            if (HAS_OBJECT(value, "table")) {
                PARSE_CHILD(status, value["table"], table);
            }
            if (HAS_ARRAY(value, "urls")) {
                if (value["urls"].size() != urls.size()) {
                    status.success = false;
                    status.error_msg << "Wrong number of apis: " << value["urls"].size() << std::endl;
                    return;
                }
                for (int i=0; i<urls.size(); i++) {
                    urls[i].parse(value["urls"][i], status);
                    if (!status.success) {
                        return;
                    }
                }
            }
		};
	};
}

#endif //IDEA_CONFIGURATION_H
