#include "string_iteration.h"
#include "csv_format.h"

char string_iteration::get() {
    if (this->eof()) {
        return _STD char_traits<char>::eof();
    } else {
        return *it++;
    }
}

void string_iteration::to_csv_block(std::string &s) {
    bool prohibited_symbols = false;//comma, newline, double quote
    for (auto i = s.begin(); i != s.end(); ++i) {
        if (*i == ',' || *i == '\n') {
            prohibited_symbols = true;
        }
        if (*i == '\"') {
            prohibited_symbols = true;
            i = _STD next(s.insert(i,'\"'));//insert возвращает итератор на вставленный символ
        }
    }
    if (prohibited_symbols) {
        s.insert(s.begin(), '\"');
        s.insert(s.end(), '\"');
    }
}

_STD string string_iteration::make_csv_block(::std::string_view s) {
    _STD string str;
    bool prohibited_symbols = false;
    _STD for_each(s.cbegin(), s.cend(),
                  [&](const char &c) {
                      str.append(1, c);
                      if (c == ',' || c == '\n') {
                          prohibited_symbols = true;
                      }
                      if (c == '\"') {
                          prohibited_symbols = true;
                          str.append(1, '\"');
                      }
                  });
    if (prohibited_symbols) {
        str.insert(str.begin(), '\"');
        str.insert(str.end(), '\"');
    }
    return str;
}

string_iteration &string_iteration::unget() {
    if (it != s.begin()) {
        --it;
    }
    return *this;
}

char string_iteration::skip_block() {
    return _CSV skip_block(*this, ',');
}

void string_iteration::seek_end_of_line() {
    if (this->eof()) {
        return;
    }

    char last;
    do {
        last = this->skip_block();
    } while (last == ',');

    if (last == '\n') {
        this->unget();
    }
}

void string_iteration::insert_string(::std::string_view str) {
    it = s.insert(it, str.cbegin(), str.cend());
}

void string_iteration::insert_delimiter() {
    it = _STD next(s.insert(it, ','));
}

void string_iteration::make_columns(_STD size_t n) {
    while (n--) {
        it = s.insert(it, '\n');
    }
}

void string_iteration::make_leftmost_column() {
    this->make_columns(10);
    this->suppl_row_newline<_STD string>("parameters", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("quantity", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("empty", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("unique", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("min", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("max", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("median", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("quartile range", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("mean", csv_string::raw, no_comma);
    this->suppl_row_newline<_STD string>("standard deviation", csv_string::raw, no_comma);
    this->seek_start();
}

void string_iteration::write_to_file(const char *filepath) const {
    _STD ofstream output(filepath, _STD ios::binary | _STD ios::trunc);
    output.write(s.data(), (_STD streamsize)s.size());
    output.close();
}

void string_iteration::write_to(std::ostream &stream) const {
    stream.write(s.data(), (_STD streamsize)s.size());
}
