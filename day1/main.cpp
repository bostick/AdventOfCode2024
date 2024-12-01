
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


#define TAG "day1"


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
    
    
    sort(col1.begin(), col1.end());
    sort(col2.begin(), col2.end());
    

    ASSERT(col1.size() == col2.size());


    int total = 0;

    for (int i = 0; i < col1.size(); i++) {
        int dist = abs(col1[i] - col2[i]);
        total += dist;
    }


    LOGI("total: %d", total);


    return 0;
}
















