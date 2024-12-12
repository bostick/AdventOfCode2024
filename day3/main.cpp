
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


#define TAG "day3"


std::regex r("mul\\((\\d{1,3}),(\\d{1,3})\\)");


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

    while (it != end) {

        std::smatch match = *it;

        LOGI("match: %s %s", match.str(1).c_str(), match.str(2).c_str());

        auto str1 = match.str(1);
        auto str2 = match.str(2);

        int i1;
        parseInt(str1, &i1);
        int i2;
        parseInt(str2, &i2);

        auto prod = i1 * i2;
        
        total += prod;

        ++it;
    }


    LOGI("total: %d", total);


    return EXIT_SUCCESS;
}
















