
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


#define TAG "day2"


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


    int safeCount = 0;

    while (newline != NULL) {

        std::string line = std::string(tosearch, newline);
        // LOGI("line: %s", line.c_str());

        const char *lineEnd = line.c_str() + line.size();

        std::vector<int> report;

        const char *tosearch2 = line.c_str();

        const char *space = strchr(tosearch2, ' ');

        while (space != NULL) {

            std::string levelStr = std::string(tosearch2, space);
            int level = parseInt(levelStr);
            report.push_back(level);

            tosearch2 = space + 1;
            ASSERT(tosearch2 <= lineEnd);
            
            space = strchr(tosearch2, ' ');
        }

        std::string levelStr = std::string(tosearch2, lineEnd);
        int level = parseInt(levelStr);
        report.push_back(level);

        // LOGI("report: %zu", report.size());


        bool safe = true;

        ASSERT(report.size() >= 2);

        if (report[0] < report[1]) {

            //
            // increasing
            //

            for (int i = 0; i < report.size() - 1; i++) {

                if (!(report[i] < report[i+1])) {
                    safe = false;
                    break;
                }

                int diff = (report[i+1] - report[i]);
                if (!(1 <= diff && diff <= 3)) {
                    safe = false;
                    break;
                }
            }

        } else if (report[0] > report[1]) {

            //
            // decreasing
            //

            for (int i = 0; i < report.size() - 1; i++) {

                if (!(report[i] > report[i+1])) {
                    safe = false;
                    break;
                }

                int diff = (report[i] - report[i+1]);
                if (!(1 <= diff && diff <= 3)) {
                    safe = false;
                    break;
                }
            }

        } else {
            
            //
            // same
            //

            safe = false;
        }

        if (safe) {
            safeCount++;
        }

        tosearch = newline + 1;
        ASSERT(tosearch <= reinterpret_cast<const char *>(bufEnd));
        
        newline = strchr(tosearch, '\n');
    }


    LOGI("safeCount: %d", safeCount);


    return EXIT_SUCCESS;
}
















