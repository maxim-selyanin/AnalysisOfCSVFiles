#include "mainwindow.h"
#include "table.h"
#include <QApplication>
#include "corr.h"

constexpr char input_filepath[] = "D:/study/academic practice/NFA 2018.csv";
constexpr char output_filepath[] = "D:/study/academic practice/output.csv";

template <std::size_t Ns, std::size_t Ni, std::size_t Nd>
void fill(::table<std::string, Ns> &string_table,
                    ::table<int, Ni> &int_table,
             ::table<double, Nd> &double_table) {
    ::icsv f(input_filepath);
    string_table.fill(f, 0);
    int_table.fill(f, Ns);
    double_table.fill(f, Ns + Ni);
}

template <std::size_t Ns, std::size_t Ni, std::size_t Nd>
void put_out(const ::table<std::string, Ns> &string_table,
                    const ::table<int, Ni> &int_table,
                    const ::table<double, Nd> &double_table) {
    _SIT s;
    s.make_leftmost_column();

    string_table.out(s);
    int_table.out(s);
    double_table.out(s);

    s.write_to_file(output_filepath);
}

template<typename Container>
void correlations(const Container &v1, const Container &v2) {
    std::cout << "Fechner: " << CORR Fechner(v1, v2) << '\n';
    std::cout << "Pearson: " << CORR Pearson(v1, v2) << '\n';
}

int main(int argc, char *argv[])
{
    ::table<std::string, 1> t_str;
    ::table<int, 1> t_int;
    ::table<double, 9> t_d;

    fill(t_str, t_int, t_d);
    correlations(t_d.data[6].values, t_d.data[8].values);
    //return 0;

    QApplication a(argc, argv);
    //сюда подставлять любую выборку
    MainWindow w(t_d.data[4]);
    w.show();
    return a.exec();
}
