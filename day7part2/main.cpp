
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


static_assert(sizeof(long long) == sizeof(int64_t));


bool compute(int64_t first, std::vector<int64_t> rest, int64_t last);


int main(int argc, char **argv) {

    SetLogLevel(LOGLEVEL_TRACE);

    const char *path = argv[1];

    std::vector<uint8_t> buf;
    buf.reserve(25000);

    if (openFile(path, buf) != OK) {
        return EXIT_FAILURE;
    }


    uint8_t *bufEnd = buf.data() + buf.size();


    const char *tosearch = reinterpret_cast<const char *>(buf.data());

    const char *newline = strchr(tosearch, '\n');


    int64_t total = 0;


    std::vector<int64_t> rands;
    rands.reserve(50);

    while (newline != NULL) {


        rands.clear();


        std::string line = std::string(tosearch, newline);
        
        const char *tosearch2 = line.c_str();

        const char *lineEnd = tosearch2 + line.size();

        const char *colon = strchr(tosearch2, ':');

        std::string lhsStr = std::string(tosearch2, colon);

        int64_t last;
        parseInt64(lhsStr, &last);

        tosearch2 = colon + 1;

        //
        // ' ' after ':'
        //
        tosearch2 = tosearch2 + 1;
        
        const char *space = strchr(tosearch2, ' ');

        while (space != NULL) {

            std::string randStr = std::string(tosearch2, space);

            int64_t rand;
            parseInt64(randStr, &rand);

            rands.push_back(rand);

            tosearch2 = space + 1;

            space = strchr(tosearch2, ' ');
        }
        
        std::string randStr = std::string(tosearch2, lineEnd);

        int64_t rand;
        parseInt64(randStr, &rand);

        rands.push_back(rand);


        int64_t first = rands.front();

        auto rest = rands;
        rest.erase(rest.begin());

        bool res;
        res = compute(first, rest, last);


        if (res) {
            total += last;
        }


        tosearch = newline + 1;
        
        newline = strchr(tosearch, '\n');
    }

    LOGI("total: %" PRId64, total);


    return EXIT_SUCCESS;
}


size_t intLen(int64_t a);

lldiv_t shift(int64_t a, int64_t b);


bool compute(int64_t first, std::vector<int64_t> rest, int64_t last) {

    if (rest.empty()) {
        if (first == last) {
            return true;
        } else {
            return false;
        }
    }

    int64_t pen = rest.back();

    rest.pop_back();

    auto x = lldiv(last, pen);
    if ((x.rem == 0) && compute(first, rest, x.quot)) {
        return true;
    }

    auto y = (last - pen);
    if ((0 <= y) && compute(first, rest, y)) {
        return true;
    }

    auto z = shift(last, pen);
    if ((z.rem == pen) && compute(first, rest, z.quot)) {
        return true;
    }

    return false;
}


size_t intLen(int64_t a) {

    if (a < 10) {
        return 1;
    }

    if (a < 100) {
        return 2;
    }

    if (a < 1000) {
        return 3;
    }

    ASSERT(false);

    return 0;
}


lldiv_t shift(int64_t a, int64_t b) {

    size_t bLen = intLen(b);

    if (bLen == 1) {

        return lldiv(a, 10);

    } else if (bLen == 2) {

        return lldiv(a, 100);

    } else if (bLen == 3) {

        return lldiv(a, 1000);

    } else {

        ASSERT(false);

        return {};
    }
}
















