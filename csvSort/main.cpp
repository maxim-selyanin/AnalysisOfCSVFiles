#include "table.h"

constexpr char test_filepath[] = "D:/study/programming/trials/test.csv";
constexpr char input_filepath[] = "D:/study/academic practice/NFA 2018.csv";
constexpr char output_filepath[] = "D:/study/academic practice/output.csv";

int main() {
    ::icsv f(input_filepath);
    _SIT s;
    s.make_leftmost_column();

    ::table<std::string, 1> t_str;
    ::table<int, 1> t_int;
    ::table<double, 9> t_d;

    t_str.fill(f, 0);
    t_int.fill(f, 1);
    t_d.fill(f, 2);

    t_str.out(s);
    t_int.out(s);
    t_d.out(s);

    s.write_to_file(output_filepath);

    std::cout << "\nend\n";
    return 0;
}