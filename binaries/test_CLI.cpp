#include <CLI/CLI.hpp>

int main(int argc, char** argv) {
    CLI::App app{"Donut CLI"};
    int level = 0;
    app.add_option("-l,--level", level, "Set animation level");
    CLI11_PARSE(app, argc, argv);
    return 0;
}
