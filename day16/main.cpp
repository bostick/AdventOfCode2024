
#include "day16.h"

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


#define TAG "day16"


int main(int argc, char **argv) {

    // SetLogLevel(LOGLEVEL_TRACE);
    SetLogLevel(LOGLEVEL_INFO);

    const char *path = argv[1];

    int lowestScore;

    Status status;

    status = computeLowestScore(path, &lowestScore);

    if (status != OK) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
















