#include <stdexcept>
#include <iostream>
#include <sstream>
#include <csignal>
#include <cstdlib>
#include <vector>
#include <string>
#include <tuple>

#include "FileSystem.hpp"
#include "Parser.hpp"

void signal_handler(int signal) {
	std::cout << "\nClosing DB" << std::endl;
	std::exit(EXIT_SUCCESS);
}

int main() {
	std::signal(SIGINT, signal_handler);
    constexpr const int command = 0;
    constexpr const int name = 1;
    constexpr const int arguments = 2;
    DB::FileSystem& db = DB::FileSystem::getFileSystemInstance();
    DB::Parser& parser = DB::Parser::getParserInstance();
    std::string input;
    bool is_thrown;
    std::tuple<std::string, std::string, std::vector<std::pair<std::string, std::string>>> args;
    while (true) {
        std::getline(std::cin, input);
        is_thrown = false;
        try {
            args = parser.parse(input);
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
            is_thrown = true;
        }
        if (is_thrown) {
            continue;
        }
        if (std::get<command>(args) == "exit") {
            return 0;
        }
        try {
            if (std::get<command>(args) == "create") {
                db.create(std::get<name>(args), std::get<arguments>(args));
            } else if (std::get<command>(args) == "insert") {
                db.insert(std::get<name>(args), std::get<arguments>(args));
            } else if (std::get<command>(args) == "delete") {
                if (std::get<arguments>(args).empty()) {
                    db.remove(std::get<name>(args));
                } else {
                    std::vector<std::pair<std::string, std::string>> tmp = std::get<arguments>(args);
                    std::size_t id;
                    if (tmp.size() == 1 && tmp.front().first == "id") {
                        try {
                            id = std::stoull(tmp.front().second);
                        } catch (...) {
                            throw std::invalid_argument("Invalid ID.");
                        }
                        db.remove(std::get<name>(args), id);
                    } else {
                        db.remove(std::get<name>(args), tmp);
                    }
                }
            } else if (std::get<command>(args) == "update") {
                std::vector<std::pair<std::string, std::string>> tmp = std::get<arguments>(args);
                std::size_t id;
                id = std::stoull(parser.findID(input));
                tmp.erase(std::remove_if(tmp.begin(), tmp.end(),[](const std::pair<std::string, std::string>& pair)
                                        {return pair.first == "id";}), tmp.end());
                db.update(std::get<name>(args), tmp, id);
            } else if (std::get<command>(args) == "display") {
                db.display(std::get<name>(args));
            }
        } catch (const std::exception& ex) {
            std::cerr << ex.what() << std::endl;
        }
    }
}
