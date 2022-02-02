#include <cstring>
#include <fstream>
#include <iostream>

#include "config.hpp"
#include "stylistic_editor.hpp"

auto main([[maybe_unused]] int argc,
          [[maybe_unused]] char *argv[]) -> int
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0]
                  << " [ -cin ][ -ifstream=path ]"
                  << " [ -cout ][ -ofstream=path ]\n"
                  << "\n\tspecify a stream to read:\n"
                  << "\n\t-cin\t\t- standard input stream.\n"
                  << "\t-ifstream=path\t- where path is the path to "
                  << "the file from which to read.\n"
                  << "\n\tspecify a stream to write:\n"
                  << "\n\t-cout\t\t- standard output stream.\n"
                  << "\t-ofstream=path\t- where path is the path to "
                  << "the file to write to.\n\n";
    }
    else
    {
        try
        {
            config::parse(argc, argv);
        }
        catch (std::runtime_error &error)
        {
            std::cerr << error.what() << std::endl;
            return EXIT_FAILURE;
        }

        std::string content{};
        if (config::mode & io_mode::ifstream)
        {
            std::ifstream file(config::path_in, std::ifstream::in);
            if (file.is_open())
            {
                file.unsetf(file.skipws);
                content.assign(std::istream_iterator<char>{file},
                               std::istream_iterator<char>{});
                file.close();
            }
            else
            {
                std::cerr << "Premature termination of the program.\n"
                          << "The file could not be opened at the "
                          << "specified path.\n\"" << config::path_in
                          << "\"\n";
            }
        }
        else if (config::mode & io_mode::cin)
        {
            std::cin.unsetf(std::cin.skipws);
            content.assign(std::istream_iterator<char>{std::cin},
                           std::istream_iterator<char>{});
        }

        stylistic_editor::analysis(content);

        if (config::mode & io_mode::ofstream)
        {
            std::ofstream file(config::path_out,
                               std::ofstream::out | std::ofstream::trunc);
            if (file.is_open())
            {
                stylistic_editor::review(file);
                file.close();
            }
            else
            {
                std::cerr << "Premature termination of the program.\n"
                          << "The file could not be opened at the "
                          << "specified path.\n\"" << config::path_out
                          << "\"\n";
            }
        }
        else if (config::mode & io_mode::cout)
        {
            stylistic_editor::review(std::cout);
        }
    }
}