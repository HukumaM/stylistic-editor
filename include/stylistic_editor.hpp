#pragma once

#include <map>
#include <string>
#include <iomanip>
#include <iterator>
#include <algorithm>

class stylistic_editor final
{
private:
    static std::multimap<std::size_t, std::string> statistics;

public:
    static void
    analysis(std::string const &content)
    {
        auto end_sentence = [](auto left, auto right)
        {
            static std::string marks{".!?"};
            auto end{right};
            do
            {
                end = std::find_first_of(left, right, std::begin(marks),
                                         std::end(marks));
                left = std::next(left);
            } while (std::distance(end, right) > 1L &&
                     (!std::isspace(*std::next(end)) &&
                      !std::isalpha(*std::next(end))));
            return end;
        };

        auto left{std::begin(content)};
        auto right{end_sentence(left, std::end(content))};

        while (left != std::end(content) &&
               std::distance(left, right) > 0L)
        {
            auto filtered{filter({left, right})};
            if (filtered.length() > 0UL)
            {
                auto const matches{static_cast<std::size_t>(
                    1L + std::count(std::cbegin(filtered),
                                    std::cend(filtered), ' '))};
                statistics.emplace(matches, std::move(filtered));
            }
            if (right == std::end(content))
            {
                break;
            }

            left = std::next(right);
            right = end_sentence(left, std::end(content));
        }
    }

    static void
    review(std::basic_ostream<char> &stream)
    {
        auto state = stream.rdstate();
        stream.clear();

        for (auto const &[word_count, sentence] : statistics)
        {
            stream << std::right << std::setw(5) << word_count
                   << " | " << sentence << ".\n";
        }
        stream.setstate(state);
    }

private:
    static std::string
    filter(std::string const &content)
    {
        static std::string const forbidden{" \r\n\t"};
        auto const left{content.find_first_not_of(forbidden)};
        auto const right{content.find_last_not_of(forbidden)};

        if (left == std::string::npos)
        {
            return {};
        }
        return content.substr(left, right - left + 1UL);
    }
};

std::multimap<std::size_t, std::string> stylistic_editor::statistics{};
