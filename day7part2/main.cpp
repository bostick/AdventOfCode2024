
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


#define TAG "day7part2"


bool compute(int64_t acc, std::vector<int64_t> rest, int64_t lhs);


int main(int argc, char **argv) {

    SetLogLevel(LOGLEVEL_TRACE);

    const char *path = argv[1];

    std::vector<uint8_t> buf;

    if (openFile(path, buf) != OK) {
        return EXIT_FAILURE;
    }


    uint8_t *bufEnd = buf.data() + buf.size();


    const char *tosearch = reinterpret_cast<const char *>(buf.data());

    const char *newline = strchr(tosearch, '\n');


    int64_t total = 0;


    while (newline != NULL) {


        std::vector<int64_t> rands;


        std::string line = std::string(tosearch, newline);
        
        const char *lineEnd = line.c_str() + line.size();

        const char *tosearch2 = line.c_str();

        const char *colon = strchr(tosearch2, ':');

        std::string lhsStr = std::string(tosearch2, colon);

        int64_t lhs = parseInt64(lhsStr);

        tosearch2 = colon + 1;

        tosearch2 = tosearch2 + 1;
        
        const char *space = strchr(tosearch2, ' ');

        while (space != NULL) {

            std::string randStr = std::string(tosearch2, space);

            int64_t rand = parseInt64(randStr);

            rands.push_back(rand);

            tosearch2 = space + 1;

            space = strchr(tosearch2, ' ');
        }
        
        std::string randStr = std::string(tosearch2, lineEnd);

        int64_t rand = parseInt64(randStr);

        rands.push_back(rand);


        int acc = rands[0];

        auto rest = rands;
        rest.erase(rest.begin());

        bool res;
        res = compute(acc, rest, lhs);


        if (res) {
            total += lhs;
        }


        tosearch = newline + 1;
        
        newline = strchr(tosearch, '\n');
    }

    LOGI("total: %" PRId64, total);


    return EXIT_SUCCESS;
}



int64_t concat(int64_t a, int64_t b);


bool compute(int64_t acc, std::vector<int64_t> rest, int64_t lhs) {

    if (rest.empty()) {
        if (acc == lhs) {
            return true;
        } else {
            return false;
        }
    }

    int64_t first = rest[0];
    
    rest.erase(rest.begin());

    if (compute(acc + first, rest, lhs)) {
        return true;
    }

    if (compute(acc * first, rest, lhs)) {
        return true;
    }

    if (compute(concat(acc, first), rest, lhs)) {
        return true;
    }

    return false;
}


int64_t concat(int64_t a, int64_t b) {
    std::string res = std::to_string(a) ;
    res += std::to_string(b);
    return parseInt64(res);
}
















