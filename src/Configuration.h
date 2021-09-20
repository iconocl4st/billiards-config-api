//
// Created by thallock on 9/19/21.
//

#ifndef IDEA_CONFIGURATION_H
#define IDEA_CONFIGURATION_H

#include "common/config/Table.h"

namespace billiards::config {
	class Configuration : public json::Serializable {
	public:
		Table table;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("table");
			table.to_json(writer);
			writer.end_object();
		};

		void parse(const nlohmann::json& value) override {
			if (value.contains("table") && value["table"].is_object()) {
				table.parse(value["table"]);
			}
		};
	};
}

#endif //IDEA_CONFIGURATION_H
