#ifndef include_LEAShared_StringUtils_h
#define include_LEAShared_StringUtils_h

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>

// Fuck you, Microsoft.
#undef min
#undef max

class StringUtils
{
public:
    //--------------------------------------------------------------------------
    // Wrap
    // ----
    /// Wrap `line' somewhere between `min' and `max' characters (longer is
    /// better).  Successively look through characters from the strings in
    /// `splitters', or else just cut off at `max'.  Resulting lines are
    /// appended to `lines'.
    ///
    /// @param[in] line: Input text.
    /// @param[out] lines: Wrapped output lines.
    /// @param[in] min: Minimum number of characters in each line of `lines'
    ///                 (except perhaps the last).
    /// @param[in] max: Maximum number of characters in each line of `lines'.
    /// @param[in] splitters: Determines how the line will be split.  If
    ///                       splitters = {";", ",. "}, then first the algorithm
    ///                       tries to split on ';', then on ',', '.', or ' '.
    ///                       If all of these fail, the algorithm simply splits
    ///                       at `max' characters.
    ///
    /// @rework If this used a std::back_inserter, a cute recursive algorithm
    ///         could be done.
    static void Wrap(const std::string& line, std::vector<std::string>& lines, int min, int max, const std::vector<std::string>& splitters)
    {
        if (line.size() <= max)
        {
            lines.push_back(line);
            return;
        }

        int n = 0;
        while (n < line.size())
        {
            if (std::isspace((unsigned char)line[n]))
            {
                n++;
                continue;
            }

            int start = n;
            int minend = start + min - 1, maxend = start + max - 1;
            minend = std::min(minend, (int) line.size() - 1);
            maxend = std::min(maxend, (int) line.size() - 1);
            //! @todo check if minend or maxend is over the edge of the string.

            bool split = false;
            for (auto it = splitters.begin(); it != splitters.end(); it++)
            {
                if (split)
                    break;
                std::string splitter = *it;
                std::sort(splitter.begin(), splitter.end());

                bool found = false;
                for (int i = maxend; i >= minend; i--)
                {
                    // Check if the ith character is a splitter
                    char c = line[i];
                    if (std::binary_search(splitter.begin(), splitter.end(), c))
                    {
                        // wrap here.
                        std::string wrapped = line.substr(start, i - start + 1);
                        boost::trim_right(wrapped);
                        lines.push_back(wrapped);
                        n = i + 1;
                        split = true;
                        break;
                    }
                }
            }

            if (!split)
            {
                // wrap at maxend.
                lines.push_back(line.substr(start, maxend - start + 1));
                n = maxend + 1;
            }
        }
    }
private:
    StringUtils(void) {}
    ~StringUtils(void) {}
};

#endif