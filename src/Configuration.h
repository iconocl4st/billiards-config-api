//
// Created by thallock on 9/19/21.
//

#ifndef IDEA_CONFIGURATION_H
#define IDEA_CONFIGURATION_H

#include "billiards_common/config/Table.h"

namespace billiards::config {
	class Configuration : public json::Serializable {
	public:
        // Put the urls of the api's things in here...
		Table table;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("table");
			table.to_json(writer);
			writer.end_object();
		};

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "table", table, "Configuration must have a table");
		};
	};
}

#endif //IDEA_CONFIGURATION_H
