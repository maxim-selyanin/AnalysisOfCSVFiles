#pragma once
#include <functional>
#include "common.h"

#define _CSV ::csv::

namespace csv {
    template<typename Stream>
    char process_block(Stream &stream, char delimiter,
            std::function<char(Stream&, char, char)> process);
    template<typename Stream>
    char process_quoted_block(Stream &stream,
            std::function<char(Stream&, char, char)> process);//starts after the initial quote

    template<typename Stream>
    char skip_block(Stream &stream, char delimiter);
    template<typename Stream>
    char read_block(Stream &s, char delimiter, _STD string &str);
}

template<typename Stream>
char csv::process_block(Stream &stream, char delimiter,
        std::function<char(Stream&, char, char)> process) {
    //проверка потока производится в вызывающей функции
    if (stream.get() == '\"') {//the field is quoted
        return _CSV process_quoted_block<Stream>(stream,
                std::move(process));//starts with initial quote being discarded
    } else {//the field is simple (unquoted)
        stream.unget();
        return process(stream, delimiter, '\n');
    }
}

template<typename Stream>//starts after the initial quote
char csv::process_quoted_block(Stream &stream, std::function<char(Stream&, char, char)> process) {
    //проверка потока производится в вызывающей функции
    char c;
    while (stream.good()) {
        process(stream, '\"', '\"');//ради обобщения пришлось написать так
        c = stream.get();
        if (c != '\"') {//получили delimiter, newline или end of file
            return c;
        }
    }
    return '\0';
}

template<typename Stream>
char csv::skip_block(Stream &stream, char delimiter) {//проверка потока производится в вызывающей функции
    return _CSV process_block<Stream>(stream, delimiter,
            [](Stream &s, char delim_1, char delim_2) {
                return _CMN ignore<Stream>(s,
                        std::numeric_limits<std::streamsize>::max(), delim_1, delim_2);
            }
    );
}

template<typename Stream>
char csv::read_block(Stream &stream, char delimiter, _STD string &str) {
    //проверка потока производится в вызывающей функции
    bool first = true;
    return _CSV process_block<Stream>(stream, delimiter,
        [&str, first](Stream &s, char delim_1, char delim_2) mutable {
            if (first) {
                str.erase();
                first = false;
            } else {
                str.append(1, '\"');
            }

            _STD string tmp;
            char c = _CMN getline<Stream>(s, tmp, delim_1, delim_2);
            str.append(tmp);
            return c;
        });
}