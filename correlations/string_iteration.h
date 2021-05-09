#pragma once
#include <sstream>
#include <fstream>

#define _SIT ::string_iteration

class string_iteration {
public:
    enum comma {prepend_with_comma, no_comma};
    enum csv_string {raw, prepared};
    enum empty{};

private:
    _STD string s{};
    _STD string::iterator it{s.begin()};

    char skip_block();
    void insert_string(_STD string_view str);//it остаётся в начале вставленного блока
    void seek_end_of_line();
    void insert_delimiter();

    template<typename T>
    void insert_block(const T &t, csv_string block_is);//it остаётся в начале вставленного блока

public:
    string_iteration() = default;

    char get();
    [[nodiscard]] bool good() const {return !this->eof();}
    [[nodiscard]] bool bad() const {return !this->good();}
    [[nodiscard]] bool eof() const {return it == s.end();}
    void seek_start() {it = s.begin();}
    string_iteration  &unget();

    [[nodiscard]] const _STD string &str() const {return s;}
    void write_to(_STD ostream &stream) const;
    void write_to_file(const char *filepath) const;

    static void to_csv_block(_STD string &s);
    [[nodiscard]] static _STD string make_csv_block(_STD string_view s);

    void make_columns(_STD size_t n);
    void make_leftmost_column();

    //вставить новый блок в конец текущей строки и перейти на новую строку
    template<typename T>
    void suppl_row_newline(const T &t, csv_string block_is = csv_string::prepared,
            comma _comma = comma::prepend_with_comma);

    ~string_iteration() = default;
};

template<typename T>
void string_iteration::insert_block(const T &t, csv_string block_is) {
    _STD stringstream s{};
    s << t;
    if (block_is == csv_string::prepared) {
        this->insert_string(_STD string_view(
                string_iteration::make_csv_block(
                        _STD string_view(
                                s.str()
                        )
                )
        ));
    } else {
        this->insert_string(_STD string_view(
                s.str()
        ));
    }
}

//вставить новый блок в конец текущей строки и перейти на новую строку
template<typename T>
void string_iteration::suppl_row_newline(const T &t, csv_string block_is, comma _comma) {
    this->seek_end_of_line();//идём в конец строки
    if (_comma == comma::prepend_with_comma) {
        this->insert_delimiter();//вставляем разделитель
    }
    if constexpr (!_STD is_same_v<T, string_iteration::empty>) {//если не требуется вставить пустой блок
        this->insert_block<T>(t, block_is);//после разделителя вставляем t
    } else {//если требуется вставить пустой блок
        this->insert_block<_STD string>("NA", csv_string::raw);//льём NA, чтобы R было удобно
        Q_UNUSED(t);
        Q_UNUSED(block_is);
    }
    this->skip_block();//скипаем t, оказываясь в начале следующей строки
}
