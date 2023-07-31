#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <stdexcept>
#include <utility>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include <tuple>

namespace DB {

    class Parser {
    public:
        static Parser& getParserInstance();
    public:
        std::tuple<std::string, std::string, std::vector<std::pair<std::string, std::string>>> parse(const std::string&) const;    
        std::string findID(const std::string&) const;
    private:
        Parser() = default;
        Parser(const Parser&) = delete;        
        Parser(Parser&&) = delete;        
        ~Parser() = default;
    private:
        void checkCommand(const std::string&) const;
    };

} //namespace DB

#endif //PARSER_HPP_