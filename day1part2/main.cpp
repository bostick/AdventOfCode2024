
#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"
#include "common/string_utils.h"

#include <algorithm>
#include <map>
#include <regex>
#include <vector>
#include <cstdio>
#include <cinttypes>
#include <cstring>


#define TAG "day1part2"


std::regex r("(\\d+)\\s+(\\d+)");


int main(int argc, char **argv) {

    const char *path = argv[1];

    std::vector<uint8_t> buf;

    if (openFile(path, buf) != OK) {
        return EXIT_FAILURE;
    }


    uint8_t *bufEnd = buf.data() + buf.size();


    std::vector<int> col1;
    std::vector<int> col2;


    const char *tosearch = reinterpret_cast<const char *>(buf.data());

    const char *newline = strchr(tosearch, '\n');

    while (newline != NULL) {

        std::string line = std::string(tosearch, newline);
        LOGI("line: %s", line.c_str());

        std::smatch m;

        ASSERT(std::regex_match(line, m, r));

        auto capture1 = m.str(1);

        auto capture2 = m.str(2);
                
        int n1 = parseInt(capture1);
        int n2 = parseInt(capture2);

        col1.push_back(n1);
        col2.push_back(n2);

        tosearch = newline + 1;
        ASSERT(tosearch <= reinterpret_cast<const char *>(bufEnd));
        
        newline = strchr(tosearch, '\n');
    }

    
    LOGI("col1: %zu", col1.size());
    LOGI("col2: %zu", col2.size());
    

    ASSERT(col1.size() == col2.size());



    std::map<int, int> counts;
    for (int i = 0; i < col2.size(); i++) {
        counts[col2[i]]++;
    }

    for (auto it = counts.begin(); it != counts.end(); it++) {
        LOGI("<%d, %d>", it->first, it->second);
    }



    int total = 0;

    for (int i = 0; i < col1.size(); i++) {
        int score = col1[i] * counts[col1[i]];
        LOGI("score: %d", score);
        total += score;
    }


    LOGI("total: %d", total);


    return 0;
}
















