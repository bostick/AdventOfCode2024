
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


#define TAG "day4part2"


struct Coord {
    int row;
    int col;
};


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


    auto colCount = static_cast<int>(newline - tosearch);

    // LOGI("diff: %td", diff);


    tosearch = newline + 1;

    newline = strchr(tosearch, '\n');

    int rowCount = 1;
    while (newline != NULL) {
        rowCount++;
        tosearch = newline + 1;
        newline = strchr(tosearch, '\n');
    }


    // LOGI("colCount: %d", colCount);
    // LOGI("rowCount: %d", rowCount);


    auto grid = static_cast<char **>(malloc(rowCount * sizeof(char *)));
    for (int i = 0; i < rowCount; i++) {
        grid[i] = (char *)malloc(colCount * sizeof(char));
        for (int j = 0; j < colCount; j++) {
            //
            // (rowCount + 1) for the newline
            //
            grid[i][j] = buf[i * (rowCount + 1) + j];
        }
    }


    // for (int i = 0; i < rowCount; i++) {
    //     LOGI("%c%c%c%c%c%c%c%c%c%c", grid[i][0], grid[i][1], grid[i][2], grid[i][3], grid[i][4], grid[i][5], grid[i][6], grid[i][7], grid[i][8], grid[i][9]);
    // }



    std::vector<Coord> nwCoords;

    //
    // search for candidate NW corners
    //
    for (int i = 0; i < rowCount-2; i++) {
        for (int j = 0; j < colCount-2; j++) {
            if (grid[i][j] == 'M' || grid[i][j] == 'S') {
                nwCoords.push_back(Coord{i, j});
            }
        }
    }

    LOGI("nwCoords: %zu", nwCoords.size());


    int total = 0;

    for (auto coord : nwCoords) {

        if (grid[coord.row+1][coord.col+1] != 'A') {
            continue;
        }

        if (grid[coord.row][coord.col] == 'M') {

            if (grid[coord.row+2][coord.col+2] != 'S') {
                continue;
            }

        } else {

            ASSERT(grid[coord.row][coord.col] == 'S');

            if (grid[coord.row+2][coord.col+2] != 'M') {
                continue;
            }
        }

        if (grid[coord.row][coord.col+2] == 'M') {

            if (grid[coord.row+2][coord.col] != 'S') {
                continue;
            }

        } else if (grid[coord.row][coord.col+2] == 'S') {

            if (grid[coord.row+2][coord.col] != 'M') {
                continue;
            }
            
        } else {
            continue;
        }

        total++;
    }

    
    LOGI("total: %d", total);


    return EXIT_SUCCESS;
}
















