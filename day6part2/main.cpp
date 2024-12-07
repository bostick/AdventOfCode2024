
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


#define TAG "day6part2"


//
// first submission:
// 1695: too low
//


const int EXITING = 1;
const int CYCLE = 2;
const int CANNOT_TURN = 3;


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

int computeGuardPath();

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
    }

    gridCopy = static_cast<char **>(malloc(rowCount * sizeof(char *)));

    for (int i = 0; i < rowCount; i++) {
        gridCopy[i] = (char *)malloc(colCount * sizeof(char));
    }


    int totalCycles = 0;

    for (int k = 0; k < rowCount; k++) {

        for (int l = 0; l < colCount; l++) {

            for (int i = 0; i < rowCount; i++) {
                for (int j = 0; j < colCount; j++) {
                    //
                    // (rowCount + 1) for the newline
                    //
                    grid[i][j] = buf[i * (rowCount + 1) + j];
                }
            }

            for (int i = 0; i < rowCount; i++) {
                for (int j = 0; j < colCount; j++) {
                    if (grid[i][j] == '.') {
                        gridCopy[i][j] = '\0';
                    } else {
                        gridCopy[i][j] = grid[i][j];
                    }
                }
            }

            findGuard();

            if (guard.coord.row == k && guard.coord.col == l) {
                continue;
            }

            grid[k][l] = 'O';
            gridCopy[k][l] = 'O';

            int res;
            res = computeGuardPath();

            if (res == CYCLE) {

                LOGI("CYCLE:");
                print();

                totalCycles++;
            }
        }
    }

    LOGI("total cycles: %d", totalCycles);

    return EXIT_SUCCESS;
}


void findGuard() {

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (grid[i][j] == '^' || grid[i][j] == '>' || grid[i][j] == 'v' || grid[i][j] == '<') {

                guard = Guard{ Coord{i, j}, grid[i][j] };

                grid[i][j] = '.';

                gridCopy[i][j] = '\0';

                return;
            }
        }
    }

    ASSERT(false);
}



//
// xxxxxxxx
//        ^ going up
//       ^ going right
//      ^ going down
//     ^ going left
//


int computeGuardPath() {

    bool exiting = false;
    bool cycle = false;
    bool cannotTurn = false;

    while (true) {

        switch (guard.direction) {
        case '^':
            while (true) {

                if ((gridCopy[guard.coord.row][guard.coord.col] & 0x1) == 0x1) {
                    cycle = true;
                    break;
                } else {
                    gridCopy[guard.coord.row][guard.coord.col] |= 0x1;
                }
                
                if (guard.coord.row == 0) {
                    exiting = true;
                    break;
                }
                
                if (grid[guard.coord.row-1][guard.coord.col] == '.') {
                    
                    guard.coord.row--;

                } else if (grid[guard.coord.row-1][guard.coord.col] == '#' || grid[guard.coord.row-1][guard.coord.col] == 'O') {
                    
                    guard.direction = '>';

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        case '>':
            while (true) {

                if ((gridCopy[guard.coord.row][guard.coord.col] & 0x2) == 0x2) {
                    cycle = true;
                    break;
                } else {
                    gridCopy[guard.coord.row][guard.coord.col] |= 0x2;
                }
                
                if (guard.coord.col == colCount-1) {
                    exiting = true;
                    break;
                }
                
                if (grid[guard.coord.row][guard.coord.col+1] == '.') {
                    
                    guard.coord.col++;

                } else if (grid[guard.coord.row][guard.coord.col+1] == '#' || grid[guard.coord.row][guard.coord.col+1] == 'O') {

                    guard.direction = 'v';

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        case 'v':
            while (true) {

                if ((gridCopy[guard.coord.row][guard.coord.col] & 0x4) == 0x4) {
                    cycle = true;
                    break;
                } else {
                    gridCopy[guard.coord.row][guard.coord.col] |= 0x4;
                }
                
                if (guard.coord.row == rowCount-1) {
                    exiting = true;
                    break;
                }
                
                if (grid[guard.coord.row+1][guard.coord.col] == '.') {

                    guard.coord.row++;

                } else if (grid[guard.coord.row+1][guard.coord.col] == '#' || grid[guard.coord.row+1][guard.coord.col] == 'O') {

                    guard.direction = '<';

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        case '<':
            while (true) {

                if ((gridCopy[guard.coord.row][guard.coord.col] & 0x8) == 0x8) {
                    cycle = true;
                    break;
                } else {
                    gridCopy[guard.coord.row][guard.coord.col] |= 0x8;
                }
                
                if (guard.coord.col == 0) {
                    exiting = true;
                    break;
                }
                
                if (grid[guard.coord.row][guard.coord.col-1] == '.') {

                    guard.coord.col--;

                } else if (grid[guard.coord.row][guard.coord.col-1] == '#' || grid[guard.coord.row][guard.coord.col-1] == 'O') {

                    guard.direction = '^';

                    break;

                } else {
                    ASSERT(false);
                }
            }
            break;
        default:
            ASSERT(false);
        }

        if (exiting || cycle || cannotTurn) {
            break;
        }
    }

    if (exiting) {
        return EXITING;
    }

    if (cycle) {
        return CYCLE;
    }

    if (cannotTurn) {
        return CANNOT_TURN;
    }

    ASSERT(false);
}


void print() {

    for (int i = 0; i < rowCount; i++) {
        std::string row;
        for (int j = 0; j < colCount; j++) {

            if (gridCopy[i][j] < 0xf) {

                bool vertical = (gridCopy[i][j] & 0x5) != 0;
                bool horizontal = (gridCopy[i][j] & 0xa) != 0;
                if (vertical && horizontal) {
                    row += '+';
                } else if (vertical) {
                    row += '|';
                } else if (horizontal) {
                    row += '-';
                } else {
                    row += '.';
                }

            } else {
                row += gridCopy[i][j];
            }
        }
        LOGI("%s", row.c_str());
    }
    LOGI();
}
















