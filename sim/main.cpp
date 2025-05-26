#include <chrono>
#include <string>
#include <sstream>
#include <cmath>

#include "CLI/CLI.hpp"
#include "executor.hpp"
#include "hart.hpp"
#include "logger.hpp"

int main(int argc, char **argv) {
    CLI::App app{"RISV RV64_I simulator"};
    std::string elf_file;
    app.add_option("-f,--file", elf_file, "Required RISC-V elf file")
        ->required()
        ->check(CLI::ExistingFile);

    Logger::severity_level log_level;
    app.add_option("-l,--log_severity", log_level,
                   "Sets log level:\n"
                   "\t0: release\n"
                   "\t1: trace\n"
                   "\t2: standard\n"
                   "\t3: verbose\n"
                   "default = standard")
        ->default_val(Logger::severity_level::standard)
        ->check(CLI::Range(Logger::severity_level::release, Logger::severity_level::verbose));

    CLI11_PARSE(app, argc, argv);

    Logger &myLogger = Logger::getInstance();
    myLogger.init(log_level);
    // myLogger.message(Logger::release, "main", "RISV RV64_I simulator");

    sim::Hart hart{elf_file};

    size_t instr_counter = 0;

    const auto begin = std::chrono::steady_clock::now();
    sim::Executor::run(hart, instr_counter);
    const auto end = std::chrono::steady_clock::now();

    // myLogger.message(Logger::severity_level::release, "main", hart.format_registers());

    auto time = static_cast<std::chrono::duration<double>>(end - begin).count();

    // myLogger.message(Logger::release, "main", std::to_string(time) + " seconds");
    // myLogger.message(Logger::release, "main", std::to_string(instr_counter) + " instructions");

    auto ips = instr_counter / time / std::pow(10, 6);
    // myLogger.message(Logger::release, "main", std::to_string(ips) + " mips");

}
