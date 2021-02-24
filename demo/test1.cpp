#include <iostream>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/rotating_file_sink.h"

int main(int argc, char* argv[])
{
    auto rotate_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logfile", 1024 * 1024, 3);
    auto logger1 = std::make_shared<spdlog::logger>("logger1", rotate_sink);
    auto logger2 = std::make_shared<spdlog::logger>("logger2", rotate_sink);
    auto logger3 = std::make_shared<spdlog::logger>("logger3", rotate_sink);


    logger1->info("Welcome to spdlog!");
    logger1->error("Some error message with arg: {}", 1);

    logger2->warn("Easy padding in numbers like {:08d}", 12);
    logger1->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger2->info("Support for floats {:03.2f}", 1.23456);
    logger1->info("Positional args are {1} {0}..", "too", "supported");
    logger2->info("{:<30}", "left aligned");

    logger1->debug("This message should be displayed..");
    logger1->info("This message should not be displayed..");

    logger3->info("Welcome to spdlog!");
    logger3->error("Some error message with arg: {}", 1);

    logger2->warn("Easy padding in numbers like {:08d}", 12);
    logger2->critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    logger2->info("Support for floats {:03.2f}", 1.23456);
    logger1->info("Positional args are {1} {0}..", "too", "supported");
    logger3->info("{:<30}", "left aligned");

    logger3->debug("This message should be displayed..");
    logger3->info("This message should not be displayed..");

	return 0;
}