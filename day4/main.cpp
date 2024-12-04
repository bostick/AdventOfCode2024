
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


#define TAG "day4"


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



    std::vector<Coord> xCoords;

    //
    // search for "X"s
    //
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (grid[i][j] == 'X') {
                // LOGI("x: %d %d", i, j);
                xCoords.push_back(Coord{i, j});
            }
        }
    }

    LOGI("xCoords: %zu", xCoords.size());


    int total = 0;

    for (auto coordIn : xCoords) {

        //
        // NW
        //
        auto coord = coordIn;
        coord.row--;
        coord.col--;
        if (0 <= coord.row && 0 <= coord.col && grid[coord.row][coord.col] == 'M') {
            coord.row--;
            coord.col--;
            if (0 <= coord.row && 0 <= coord.col && grid[coord.row][coord.col] == 'A') {
                coord.row--;
                coord.col--;
                if (0 <= coord.row && 0 <= coord.col && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }

        //
        // N
        //
        coord = coordIn;
        coord.row--;
        if (0 <= coord.row && grid[coord.row][coord.col] == 'M') {
            coord.row--;
            if (0 <= coord.row && grid[coord.row][coord.col] == 'A') {
                coord.row--;
                if (0 <= coord.row && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }

        //
        // NE
        //
        coord = coordIn;
        coord.row--;
        coord.col++;
        if (0 <= coord.row && coord.col <= colCount-1 && grid[coord.row][coord.col] == 'M') {
            coord.row--;
            coord.col++;
            if (0 <= coord.row && coord.col <= colCount-1 && grid[coord.row][coord.col] == 'A') {
                coord.row--;
                coord.col++;
                if (0 <= coord.row && coord.col <= colCount-1 && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }

        //
        // W
        //
        coord = coordIn;
        coord.col--;
        if (0 <= coord.col && grid[coord.row][coord.col] == 'M') {
            coord.col--;
            if (0 <= coord.col && grid[coord.row][coord.col] == 'A') {
                coord.col--;
                if (0 <= coord.col && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }

        //
        // E
        //
        coord = coordIn;
        coord.col++;
        if (coord.col <= colCount-1 && grid[coord.row][coord.col] == 'M') {
            coord.col++;
            if (coord.col <= colCount-1 && grid[coord.row][coord.col] == 'A') {
                coord.col++;
                if (coord.col <= colCount-1 && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }

        //
        // SW
        //
        coord = coordIn;
        coord.row++;
        coord.col--;
        if (coord.row <= rowCount-1 && 0 <= coord.col && grid[coord.row][coord.col] == 'M') {
            coord.row++;
            coord.col--;
            if (coord.row <= rowCount-1 && 0 <= coord.col && grid[coord.row][coord.col] == 'A') {
                coord.row++;
                coord.col--;
                if (coord.row <= rowCount-1 && 0 <= coord.col && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }

        //
        // S
        //
        coord = coordIn;
        coord.row++;
        if (coord.row <= rowCount-1 && grid[coord.row][coord.col] == 'M') {
            coord.row++;
            if (coord.row <= rowCount-1 && grid[coord.row][coord.col] == 'A') {
                coord.row++;
                if (coord.row <= rowCount-1 && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }

        //
        // SE
        //
        coord = coordIn;
        coord.row++;
        coord.col++;
        if (coord.row <= rowCount-1 && coord.col <= colCount-1 && grid[coord.row][coord.col] == 'M') {
            coord.row++;
            coord.col++;
            if (coord.row <= rowCount-1 && coord.col <= colCount-1 && grid[coord.row][coord.col] == 'A') {
                coord.row++;
                coord.col++;
                if (coord.row <= rowCount-1 && coord.col <= colCount-1 && grid[coord.row][coord.col] == 'S') {
                    total++;
                }
            }
        }
    }

    
    LOGI("total: %d", total);


    return EXIT_SUCCESS;
}
















