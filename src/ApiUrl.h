//
// Created by thallock on 11/2/21.
//

#ifndef IDEA_APIURL_H
#define IDEA_APIURL_H

#include <utility>

#include "billiards_common/utils/Serializable.h"

namespace billiards::config {
    class ApiUrl : public json::Serializable {
    public:
        std::string api_name;
        std::string url;

        ApiUrl() : api_name{"no name"}, url{"localhost"} {}

        ApiUrl(std::string api_name, const std::string& host, int port)
            : api_name{std::move(api_name)}
            , url{}
        {
            std::stringstream ss;
            ss << "http://" << host << ":" << port << "/";
            url = ss.str();
        }

        ~ApiUrl() override = default;

        void to_json(json::SaxWriter& writer) const override {
            writer.begin_object();
            writer.field("api-name", api_name);
            writer.field("url", url);
            writer.end_object();
        };

        void parse(const nlohmann::json& value, json::ParseResult& status) override {
            ENSURE_STRING(status, value, "url", "The api url must have a url");
            url = value["url"].get<std::string>();
            ENSURE_STRING(status, value, "api-name", "The api must have a name");
            api_name = value["api-name"].get<std::string>();
        };
    };
}

#endif //IDEA_APIURL_H
