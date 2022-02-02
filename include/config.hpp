#pragma once

#include <string>
#include <cstring>
#include <stdexcept>

enum io_mode : std::int64_t
{
    error = 0L,          //  000000
    cin = 1L << 0L,      //  000001
    ifstream = 1L << 1L, //  000010
    cout = 1L << 2L,     //  000100
    ofstream = 1L << 3L  //  001000
};

inline io_mode
operator~(io_mode other)
{
    return static_cast<io_mode>(~static_cast<std::int64_t>(other));
}

inline constexpr io_mode
operator|(io_mode left, io_mode right)
{
    return static_cast<io_mode>(
        static_cast<std::int64_t>(left) |
        static_cast<std::int64_t>(right));
}

inline constexpr io_mode
operator&(io_mode left, io_mode right)
{
    return static_cast<io_mode>(
        static_cast<std::int64_t>(left) &
        static_cast<std::int64_t>(right));
}

inline constexpr io_mode
operator^(io_mode left, io_mode right)
{
    return static_cast<io_mode>(
        static_cast<std::int64_t>(left) ^
        static_cast<std::int64_t>(right));
}

inline io_mode const &
operator|=(io_mode &left, io_mode right)
{
    return left = left | right;
}

inline io_mode const &
operator&=(io_mode &left, io_mode right)
{
    return left = left & right;
}

inline io_mode const &
operator^=(io_mode &left, io_mode right)
{
    return left = left ^ right;
}

struct config final
{
    static io_mode mode;
    static std::string path_in;
    static std::string path_out;

    static void
    parse(int argc, char *argv[])
    {
        for (auto index{1}; index < argc; ++index)
        {
            if (!std::strcmp("-cin", argv[index]))
            {
                set_flag(io_mode::cin);

                //  mode |= io_mode::cin;
            }
            else if (!std::strncmp("-ifstream=", argv[index], 10UL))
            {
                set_flag(io_mode::ifstream);
                set_path_to_read(std::string(argv[index]).substr(10UL));

                //  mode |= io_mode::ifstream;
                //  path_in = std::string(argv[index]).substr(10UL);
            }
            else if (!std::strcmp("-cout", argv[index]))
            {
                set_flag(io_mode::cout);

                //  mode |= io_mode::cout;
            }
            else if (!std::strncmp("-ofstream=", argv[index], 10UL))
            {
                set_flag(io_mode::ofstream);
                set_path_to_write(std::string(argv[index]).substr(10UL));

                //  mode |= io_mode::ofstream;
                //  path_out = std::string(argv[index]).substr(10UL);
            }
        }

        evaluation_of_flags();

        // switch (mode)
        // {
        // case io_mode::error:
        //     throw std::runtime_error("For the program to work, you must "
        //                              "specify the input and output stream.");
        //     break;
        // case (io_mode::cin | io_mode::ifstream):
        //     throw std::runtime_error("It is not possible to open a stream for "
        //                              "reading from cin and ifstream at the "
        //                              "same time.");
        //     break;
        // case (io_mode::cout | io_mode::ofstream):
        //     throw std::runtime_error("It is not possible to open a stream for "
        //                              "writing to cout and ofstream at the same"
        //                              " time.");
        //     break;
        // default:
        //     break;
        // }
    }

private:
    static void
    set_flag(io_mode const &flag)
    {
        mode |= flag;
    }

    static void
    set_path_to_read(std::string const &path)
    {
        path_in.assign(path);
    }

    static void
    set_path_to_write(std::string const &path)
    {
        path_out.assign(path);
    }

private:
    static void
    evaluation_of_flags()
    {
        flags_set();
        only_one_input_stream_is_set();
        only_one_output_stream_is_set();
    }

private:
    static void
    flags_set()
    {
        if (mode == io_mode::error)
        {
            throw std::runtime_error("For the program to work, you must "
                                     "specify the input and output stream.");
        }
    }

    static void
    only_one_output_stream_is_set()
    {
        if (mode == (io_mode::cout | io_mode::ofstream))
        {
            throw std::runtime_error("It is not possible to open a stream "
                                     "for writing to cout and ofstream at "
                                     "the same time.");
        }
        else if (mode & io_mode::ofstream)
        {
            path_is_specified_for_ofstream();
        }
        else if (!(mode & io_mode::cout))
        {
            throw std::runtime_error("For the program to work, you must "
                                     "specify the input and output stream.");
        }
    }

    static void
    only_one_input_stream_is_set()
    {
        if (mode == (io_mode::cin | io_mode::ifstream))
        {
            throw std::runtime_error("It is not possible to open a stream "
                                     "for reading from cin and ifstream at "
                                     "the same time.");
        }
        else if (mode & io_mode::ifstream)
        {
            path_is_specified_for_ifstream();
        }
        else if (!(mode & io_mode::cin))
        {
            throw std::runtime_error("For the program to work, you must "
                                     "specify the input and output stream.");
        }
    }

private:
    static void
    path_is_specified_for_ifstream()
    {
        if (path_in.empty())
        {
            throw std::runtime_error("To read a file, you must "
                                     "specify the path to it");
        }
    }

    static void
    path_is_specified_for_ofstream()
    {
        if (path_out.empty())
        {
            throw std::runtime_error("To write a file, you must "
                                     "specify the path to it");
        }
    }
};
