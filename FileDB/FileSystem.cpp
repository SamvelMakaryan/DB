#include "FileSystem.hpp"

namespace DB {

    FileSystem& FileSystem::getFileSystemInstance() {
        static FileSystem instance;
        return instance;
    }

    void FileSystem::checkExistence(const std::string& name, const std::string& message) const {
        m_descriptor.open("meta.txt", std::ios::in);
        std::string line;
        std::stringstream ss;
        while (std::getline(m_descriptor, line)) {
            ss.str(line);
            std::getline(ss, line, ':');
            if (line == "object") {
                std::getline(ss, line, ':');
                if (line == name) {
                    m_descriptor.close();
                    throw std::invalid_argument("The object '" + name + "' " + message);
                }
            }
        }
        m_descriptor.close();
    }

    std::string FileSystem::checkAbsense(const std::string& name, const std::string& message) const {
        m_descriptor.open("meta.txt", std::ios::in);
        std::string line;
        std::stringstream ss;
        while (std::getline(m_descriptor, line)) {
            ss.str(line);
            std::getline(ss, line, ':');
            if (line == "object") {
                std::getline(ss, line, ':');
                if (line == name) {
                    std::getline(m_descriptor, line);
                    m_descriptor.close();
                    return line;
                }
            }
        }
        m_descriptor.close();
        throw std::invalid_argument("The object '" + name + "' "+ message);
    }

    void FileSystem::readID() {
        m_descriptor.open("id.txt", std::ios::in);
        m_descriptor >> m_id;
        m_descriptor.close();
    }

    void FileSystem::writeID() {
        m_descriptor.open("id.txt", std::ios::out);
        m_descriptor << m_id;
        m_descriptor.close();
    }

    FileSystem::FileSystem()
    : m_id(0)
    {
        readID();
    }

    FileSystem::~FileSystem() {
        if (m_descriptor.is_open()) {
            m_descriptor.close();
        }
        writeID();
    }

    void FileSystem::create(const std::string& name, std::vector<std::pair<std::string, std::string>> args) {
        if (name == "__tmp" || name == "meta" || name == "id") {
            throw std::invalid_argument(name + " is a reserved name.");
        }
        checkExistence(name, " already exist.");
        m_descriptor.open("meta.txt", std::ios::app);
        m_descriptor << "object:" << name << ":\n";
        for (const auto& arg : args) {
            m_descriptor << arg.first << ":";
        }
        m_descriptor << '\n';
        m_descriptor.close();
    }

    void FileSystem::display(const std::string& name) const {
        checkAbsense(name, " doesn't exist."); 
        std::cout << name << '\n';     
        m_descriptor.open(name + ".txt", std::ios::in);
        std::string line;
        bool empty = true;
        while (std::getline(m_descriptor, line)) {
            empty = false;
            std::stringstream ss(line);
            std::cout << "------------------\n";
            while (std::getline(ss, line, ':')) {
                std::cout << '\t' << line << '\n';
            }
            std::cout << "------------------\n";
        }
        if (empty) {
            std::cout <<"\tempty\n";
        }
        m_descriptor.close(); 
    }

    void FileSystem::insert(const std::string& name, std::vector<std::pair<std::string, std::string>> args) {
        std::string metaInfo = checkAbsense(name, " doesn't exist.");
        std::stringstream ss(metaInfo);
        std::string variable;
        std::vector<std::string> vars;
        vars.reserve(args.size());
        while (std::getline(ss, variable, ':')) {
            vars.push_back(variable);
        }
        m_descriptor.open(name + ".txt", std::ios::app);
        std::string arguments;
        std::string absense;
        for (const auto& var : vars) {
            if (auto it = std::find_if(args.begin(), args.end(), [&var](const auto& i) 
                                    {return i.first == var;}); it != args.end()) {
                arguments += it->first + "=" + it->second + ":";
            } else {
                absense += var + ", ";
            }
        }
        if (!absense.empty()) {
            m_descriptor.close();
            throw std::invalid_argument("Too few arguments for " + name + "\n" + absense + "are absense.");
        }
        arguments = "id=" + std::to_string(++m_id) + ":" + arguments;
        m_descriptor << arguments << '\n';
        m_descriptor.close();
    }

    void FileSystem::remove(const std::string& name) {
        std::string metaInfo = checkAbsense(name, " doesn't exist.");
        std::fstream tmp("__tmp.txt", std::ios::out);
        m_descriptor.open("meta.txt", std::ios::in);
        std::string object = "object:" + name + ":";
        std::string line;
        while (std::getline(m_descriptor, line)) {
            if (line != object && line != metaInfo) {
                tmp << line << '\n';
            }
        }
        std::rename("__tmp.txt", "meta.txt");
        std::remove((name + ".txt").c_str());
        m_descriptor.close();
    }

    void FileSystem::remove(const std::string& name, std::size_t id) {
        checkAbsense(name, " doesn't exist.");
        std::fstream tmp("__tmp.txt", std::ios::out);
        m_descriptor.open(name + ".txt", std::ios::in);
        std::string line;
        std::string target = "id=" + std::to_string(id);
        std::string id_num;
        while (std::getline(m_descriptor, line)) {
            std::stringstream ss(line);
            std::getline(ss, id_num, ':');
            if (id_num != target) {
                tmp << line << '\n';
            }
        }
        std::rename("__tmp.txt", (name + ".txt").c_str());
        m_descriptor.close();
    }

    void FileSystem::remove(const std::string& name, std::vector<std::pair<std::string, std::string>> args) {
        std::string metaInfo = checkAbsense(name, " doesn't exist.");
        std::fstream tmp("__tmp.txt", std::ios::out);
        m_descriptor.open(name + ".txt", std::ios::in);
        std::vector<std::string> targets;
        targets.reserve(args.size());
        std::stringstream ss(metaInfo);
        std::string var;
        while (std::getline(ss, var, ':')) {
            if (auto it = std::find_if(args.begin(), args.end(), [&var](const auto& i)
                                        {return i.first == var;}); it != args.end()) {
                targets.push_back(var + "=" + it->second);
            }
        }
        std::string line;
        while (std::getline(m_descriptor, line)) {
            std::stringstream ss(line);
            bool not_found = true;
            while (std::getline(ss, var, ':')) {
                if (auto it = std::find(targets.begin(), targets.end(), var); it != targets.end()) {
                    not_found = false;
                    break;
                }
            }
            if (not_found) {
                tmp << line << '\n';
            }
        }
        std::rename("__tmp.txt", (name + ".txt").c_str());
        m_descriptor.close();
    }

    void FileSystem::update(const std::string& name, std::vector<std::pair<std::string, std::string>> args, std::size_t id) {
        std::string metaInfo = checkAbsense(name, " doesn't exist.");
        std::fstream tmp("__tmp.txt", std::ios::out);
        m_descriptor.open(name + ".txt", std::ios::in);
        std::string line;
        std::string id_num;
        std::string var;
        while (std::getline(m_descriptor, line)) {
            std::stringstream ss(line);
            std::getline(ss, id_num, ':');
            if (id_num == "id=" + std::to_string(id)) {
                line.clear();
                line += id_num + ':';
                while (std::getline(ss, var, ':')) {
                    if (auto it = std::find_if(args.begin(), args.end(), [&](const auto& i)
                                            {return match(i.first, var);}); it != args.end()) {
                        line += it->first + '=' + it->second + ':';
                    } else {
                        line += var + ':';
                    }
                }
                tmp << line << '\n';    
            } else {
                tmp << line << '\n';
            }
        }
        std::rename("__tmp.txt", (name + ".txt").c_str());
        m_descriptor.close();
    }

    bool FileSystem::match(const std::string& s1, const std::string& s2) const {
        for (std::size_t i = 0; i < s1.size(); ++i) {
            if (s1[i] != s2[i]) {
                return false;
            }
        }
        return true;
    }

} //namespace DB
