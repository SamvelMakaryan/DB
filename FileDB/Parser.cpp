#include "Parser.hpp"

namespace DB {

    Parser& Parser::getParserInstance() {
        static Parser instance;
        return instance;
    }

    std::tuple<std::string, std::string, std::vector<std::pair<std::string, std::string>>> Parser::parse(const std::string& input) const {
        std::stringstream ss(input);
        std::string command;
        ss >> command;
        std::string name;
        ss >> name; 
        checkCommand(command);
        std::vector<std::pair<std::string, std::string>> vec;
        if (command == "exit" || command == "display") {
            return std::make_tuple(command, name, vec);
        }
        std::string argument;
        std::string value;
        std::string variable;
        bool is_command_create = false;
        if (command == "create") {
            is_command_create = true;
        }
        while (ss >> argument   ) {
            std::stringstream iss(argument);
            if (is_command_create) {
                iss >> variable;
                value = "";
            } else {
                std::getline(iss, variable, '=');
                iss >> value;
            }
            vec.push_back(std::make_pair(variable, value));
        }
        return std::make_tuple(command, name, vec);
    }

    void Parser::checkCommand(const std::string& name) const {
        if (name == "insert" || name == "delete" || name == "update" || name == "create" || name == "display" || name == "exit") {
            return;
        }
        throw std::invalid_argument("No such command.");
    }

    std::string Parser::findID(const std::string& args) const {
        std::regex pattern("id=(\\d+)");
        std::smatch match;
        if (std::regex_search(args, match, pattern)) {
            return match[1];
        }
        throw std::invalid_argument("No ID is given or invalid ID.");
    }


} //namespace DB