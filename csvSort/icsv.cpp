#include "icsv.h"
#include "csv_format.h"

constexpr char double_quote = '\"';

icsv::icsv(const char *name, icsv_headline _has_headline, char _delimiter)
: stream(name, _STD ifstream::binary)
, delimiter(_delimiter)
, _headline(_has_headline == icsv::headline)
{}

char icsv::get_value(std::string &str, std::size_t offset) {
    //проверка на открытость файла производится в вызывающей функции
    for (std::size_t i = 0; i < offset; ++i) {
        skip_block();
    }

    return this->read_block(str);
}

void icsv::get_column(std::string &str, std::size_t column) {
    if (!this->fine()) {
        return;
    }

    char last_delimiter = this->get_value(str, column);
    //доходим до конца текущей строки
    while (stream.good() && last_delimiter != '\n') {
        last_delimiter = this->skip_block();
    }
}

char icsv::skip_block() {
    return _CSV skip_block<_STD ifstream>(this->stream, this->delimiter);
}

char icsv::read_block(_STD string &str) {
    return _CSV read_block<_STD ifstream>(this->stream, this->delimiter, str);
}

_STD string icsv::get_block() {
    _STD string s;
    this->read_block(s);
    return s;
}

void icsv::try_end() {
    if (this->fine() && this->stream.get() != _STD char_traits<char>::eof()) {
        this->stream.unget();
    }
}