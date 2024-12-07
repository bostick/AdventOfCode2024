
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


#define TAG "day6"


struct Coord {
    int row;
    int col;
};


struct Guard {
    Coord coord;
    char direction;
};



int rowCount;
int colCount;

char **grid;
char **gridCopy;

Guard guard;



void findGuard();

void computeGuardPath();

void countX();

void print();



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


    colCount = static_cast<int>(newline - tosearch);

    // LOGI("diff: %td", diff);


    tosearch = newline + 1;

    newline = strchr(tosearch, '\n');

    rowCount = 1;
    while (newline != NULL) {
        rowCount++;
        tosearch = newline + 1;
        newline = strchr(tosearch, '\n');
    }


    // LOGI("colCount: %d", colCount);
    // LOGI("rowCount: %d", rowCount);



    grid = static_cast<char **>(malloc(rowCount * sizeof(char *)));

    for (int i = 0; i < rowCount; i++) {
        grid[i] = (char *)malloc(colCount * sizeof(char));
        for (int j = 0; j < colCount; j++) {
            //
            // (rowCount + 1) for the newline
            //
            grid[i][j] = buf[i * (rowCount + 1) + j];
        }
    }

    gridCopy = static_cast<char **>(malloc(rowCount * sizeof(char *)));

    for (int i = 0; i < rowCount; i++) {
        gridCopy[i] = (char *)malloc(colCount * sizeof(char));
        for (int j = 0; j < colCount; j++) {
            //
            // (rowCount + 1) for the newline
            //
            gridCopy[i][j] = buf[i * (rowCount + 1) + j];
        }
    }



    // for (int i = 0; i < rowCount; i++) {
    //     LOGI("%c%c%c%c%c%c%c%c%c%c", grid[i][0], grid[i][1], grid[i][2], grid[i][3], grid[i][4], grid[i][5], grid[i][6], grid[i][7], grid[i][8], grid[i][9]);
    // }


    findGuard();

    computeGuardPath();

    countX();

    return EXIT_SUCCESS;
}


void findGuard() {

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (grid[i][j] == '^' || grid[i][j] == '>' || grid[i][j] == 'v' || grid[i][j] == '<') {

                guard = Guard{ Coord{i, j}, grid[i][j] };
                
                gridCopy[i][j] = 'X';

                grid[i][j] = '.';

                return;
            }
        }
    }

    ASSERT(false);
}


void computeGuardPath() {


    print();


    bool exiting = false;

    while (true) {

        switch (guard.direction) {
        case '^':
            while (true) {

                if (guard.coord.row == 0) {
                    exiting = true;
                    break;
                }

                if (grid[guard.coord.row-1][guard.coord.col] == '.') {

                    guard.coord.row--;

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                } else if (grid[guard.coord.row-1][guard.coord.col] == '#') {

                    print();

                    //
                    // now face >
                    //

                    LOGI("TURN!");

                    guard = Guard{ Coord{guard.coord.row, guard.coord.col+1}, '>' };

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                    print();

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        case '>':
            while (true) {

                if (guard.coord.col == colCount-1) {
                    exiting = true;
                    break;
                }

                if (grid[guard.coord.row][guard.coord.col+1] == '.') {

                    guard.coord.col++;

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                } else if (grid[guard.coord.row][guard.coord.col+1] == '#') {

                    print();

                    //
                    // now face v
                    //

                    LOGI("TURN!");

                    guard = Guard{ Coord{guard.coord.row+1, guard.coord.col}, 'v' };

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                    print();

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        case 'v':
            while (true) {

                if (guard.coord.row == rowCount-1) {
                    exiting = true;
                    break;
                }

                if (grid[guard.coord.row+1][guard.coord.col] == '.') {

                    guard.coord.row++;

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                } else if (grid[guard.coord.row+1][guard.coord.col] == '#') {

                    print();

                    //
                    // now face <
                    //

                    LOGI("TURN!");

                    guard = Guard{ Coord{guard.coord.row, guard.coord.col-1}, '<' };

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                    print();

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        case '<':
            while (true) {

                if (guard.coord.col == 0) {
                    exiting = true;
                    break;
                }

                if (grid[guard.coord.row][guard.coord.col-1] == '.') {

                    guard.coord.col--;

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                } else if (grid[guard.coord.row][guard.coord.col-1] == '#') {

                    print();

                    //
                    // now face ^
                    //

                    LOGI("TURN!");

                    guard = Guard{ Coord{guard.coord.row-1, guard.coord.col}, '^' };

                    gridCopy[guard.coord.row][guard.coord.col] = 'X';

                    print();

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        default:
            ASSERT(false);
        }

        if (exiting) {
            break;
        }
    }


    print();
}


void countX() {

    // for (int i = 0; i < rowCount; i++) {
    //     LOGI("%c%c%c%c%c%c%c%c%c%c", gridCopy[i][0], gridCopy[i][1], gridCopy[i][2], gridCopy[i][3], gridCopy[i][4], gridCopy[i][5], gridCopy[i][6], gridCopy[i][7], gridCopy[i][8], gridCopy[i][9]);
    // }

    int total = 0;

    for (int i = 0; i < rowCount; i++) {

        for (int j = 0; j < colCount; j++) {

            if (gridCopy[i][j] == 'X') {
                total++;
            }
        }
    }

    LOGI("total: %d", total);
}


void print() {

    for (int i = 0; i < rowCount; i++) {
        std::string row;
        for (int j = 0; j < colCount; j++) {
            if (guard.coord.row == i && guard.coord.col == j) {
                row += guard.direction;
            } else {
                row += gridCopy[i][j];
            }
        }
        LOGI("%s", row.c_str());
    }
    LOGI();
}
















