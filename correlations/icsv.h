#pragma once
#include <fstream>

class icsv {
    std::ifstream stream{};
    const bool _headline = true;
    const char delimiter = ',';

    char skip_block();
    char read_block(_STD string &str);

    char get_value(std::string &str, std::size_t offset);

public:
    enum icsv_headline{headline, no_headline};
    explicit icsv(const char *name, icsv_headline _has_headline = headline, char _delimiter = ',');

    [[nodiscard]] bool fine() const {return stream.is_open() && stream.good();}
    void seek_start() {if (fine()) {stream.seekg(0);}}
    [[nodiscard]] bool has_headline() const {return _headline;}
    void clear() {stream.clear();}
    void try_end();

    void get_column(std::string &str, std::size_t column);
    [[nodiscard]] _STD string get_block();

    ~icsv() {stream.close();}
};
