
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


bool compute(int64_t first, std::vector<int64_t> rest, int64_t last);


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

        int64_t last = parseInt64(lhsStr);

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


bool stringEnds(int64_t a, int64_t b);

int64_t stringDrop(int64_t a, int64_t b);


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

    if ((last % pen == 0) && compute(first, rest, last / pen)) {
        return true;
    }

    if ((0 <= last - pen) && compute(first, rest, last - pen)) {
        return true;
    }

    if (stringEnds(last, pen) && compute(first, rest, stringDrop(last, pen))) {
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

    if (a < 10000) {
        return 4;
    }

    if (a < 100000) {
        return 5;
    }

    if (a < 1000000) {
        return 6;
    }

    if (a < 10000000) {
        return 7;
    }

    if (a < 100000000) {
        return 8;
    }

    if (a < 1000000000) {
        return 9;
    }

    if (a < 10000000000) {
        return 10;
    }

    if (a < 100000000000) {
        return 11;
    }

    if (a < 1000000000000) {
        return 12;
    }

    if (a < 10000000000000) {
        return 13;
    }

    if (a < 100000000000000) {
        return 14;
    }

    if (a < 1000000000000000) {
        return 15;
    }

    if (a < 10000000000000000) {
        return 16;
    }

    if (a < 100000000000000000) {
        return 17;
    }

    if (a < 1000000000000000000) {
        return 18;
    }

    return 19;
}


bool stringEnds(int64_t a, int64_t b) {

    static_assert(sizeof(long long) == sizeof(int64_t));

    size_t bLen = intLen(b);

    if (bLen == 1) {
        
        return (a % 10) == b;
        
    } else if (bLen == 2) {
        
        return (a % 100) == b;
        
    } else if (bLen == 3) {
        
        return (a % 1000) == b;
        
    } else {
        
        for (int i = 0; i < bLen; i++) {

            auto aRes = std::lldiv(a, 10);
            auto bRes = std::lldiv(b, 10);

            if (aRes.rem != bRes.rem) {
                return false;
            }
            
            a = aRes.quot;
            b = bRes.quot;
        }
    }

    return true;
}


int64_t stringDrop(int64_t a, int64_t b) {

    size_t bLen = intLen(b);

    if (bLen == 1) {

        return (a / 10);

    } else if (bLen == 2) {

        return (a / 100);

    } else if (bLen == 3) {

        return (a / 1000);

    } else {

        for (int i = 0; i < bLen; i++) {
            a /= 10;
        }

        return a;
    }
}
















