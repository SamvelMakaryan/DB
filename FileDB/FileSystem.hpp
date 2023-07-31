#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <utility>
#include <sstream>
#include <cstdio>
#include <string>
#include <vector>

namespace DB {

    class FileSystem {
    public:
        static FileSystem& getFileSystemInstance();
    public:
        void create(const std::string&, std::vector<std::pair<std::string, std::string>>);
        void insert(const std::string&, std::vector<std::pair<std::string, std::string>>);
        void remove(const std::string&);
        void remove(const std::string&, std::vector<std::pair<std::string, std::string>>);
        void remove(const std::string&, std::size_t);
        void update(const std::string&, std::vector<std::pair<std::string, std::string>>, std::size_t);
        void display(const std::string&) const;
    private:
        FileSystem();
        ~FileSystem();
        FileSystem(const FileSystem&) = delete;
        FileSystem(FileSystem&&) noexcept = delete;
    private:
        std::string checkAbsense(const std::string&, const std::string&) const;
        void checkExistence(const std::string&, const std::string&) const;
        bool match(const std::string&, const std::string&) const;
        void readID();
        void writeID();
    private:
        mutable std::fstream m_descriptor;
        mutable std::size_t m_id; 
    }; 

} // namespace DB 

#endif //FILESYSTEM_HPP_
