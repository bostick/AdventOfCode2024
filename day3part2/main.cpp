
#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"
#include "common/string_utils.h"

#include <algorithm>
#include <regex>
#include <vector>
#include <cstdio>
#include <cinttypes>
#include <cstring>


#define TAG "day3part2"


std::regex r("(mul\\((\\d{1,3}),(\\d{1,3})\\))|(do\\(\\))|(don't\\(\\))");


int main(int argc, char **argv) {

    SetLogLevel(LOGLEVEL_TRACE);

    const char *path = argv[1];

    std::vector<uint8_t> buf;

    if (openFile(path, buf) != OK) {
        return EXIT_FAILURE;
    }

    std::string input = std::string(buf.data(), buf.data() + buf.size());

    // LOGD("input: %s", input.c_str());

    // LOGD("input: %zu", input.size());


    std::sregex_iterator it(input.begin(), input.end(), r);
    std::sregex_iterator end;


    int total = 0;

    bool enabled = true;

    while (it != end) {

        std::smatch match = *it;

        // LOGI("match size: %zu", match.size());

        LOGI("match: %s", match.str(0).c_str());

        if (match.str(0) == "don't()") {

            enabled = false;

            LOGD("enabled: false");

        } else if (match.str(0) == "do()") {

            enabled = true;

            LOGD("enabled: true");

        } else {

            if (enabled) {

                auto str2 = match.str(2);
                auto str3 = match.str(3);

                // LOGD("str2: %s", str2.c_str());
                // LOGD("str3: %s", str3.c_str());

                auto i2 = parseInt(str2);
                auto i3 = parseInt(str3);

                auto prod = i2 * i3;
                
                total += prod;
            }
        }

        ++it;
    }


    LOGI("total: %d", total);


    return EXIT_SUCCESS;
}
















