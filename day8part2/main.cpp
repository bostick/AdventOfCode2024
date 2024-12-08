
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


#define TAG "day8part2"


struct Coord {
    int row;
    int col;
};


size_t rowCount;
size_t colCount;


char **grid;
char **antinodes;


std::map<char, std::vector<Coord>> locations;


void printGrid();

void printAntinodes();

void computeLocations();

void computeAntinodes();

void computeAntinodeTotal();


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


    colCount = static_cast<size_t>(newline - tosearch);

    // LOGI("colCount: %zu", colCount);


    tosearch = newline + 1;

    newline = strchr(tosearch, '\n');

    rowCount = 1;
    while (newline != NULL) {
        rowCount++;
        tosearch = newline + 1;
        newline = strchr(tosearch, '\n');
    }


    // LOGI("colCount: %zu", colCount);
    // LOGI("rowCount: %zu", rowCount);


    grid = static_cast<char **>(malloc(rowCount * sizeof(char *)));
    antinodes = static_cast<char **>(malloc(rowCount * sizeof(char *)));
    for (int i = 0; i < rowCount; i++) {
        grid[i] = (char *)malloc(colCount * sizeof(char));
        antinodes[i] = (char *)malloc(colCount * sizeof(char));
        for (int j = 0; j < colCount; j++) {
            //
            // (rowCount + 1) for the newline
            //
            grid[i][j] = buf[i * (rowCount + 1) + j];
            antinodes[i][j] = grid[i][j];
        }
    }

    
    // print();


    computeLocations();


    computeAntinodes();


    printGrid();

    printAntinodes();


    computeAntinodeTotal();


    return EXIT_SUCCESS;
}


void printGrid() {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        const char *row = grid[i];
        for (int j = 0; j < colCount; j++) {
            rowStr += row[j];
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}

void printAntinodes() {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        const char *row = antinodes[i];
        for (int j = 0; j < colCount; j++) {
            rowStr += row[j];
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}


void computeLocations() {

    for (int i = 0; i < rowCount; i++) {
        const char *row = grid[i];
        for (int j = 0; j < colCount; j++) {

            char c = row[j];

            if (islower(c) || isupper(c) || isdigit(c)) {

                std::vector<Coord> &coords = locations[c];

                coords.push_back( {i, j} );
            }
        }
    }

    // for (const auto& p : locations) {
    //     LOGD("key: %c", p.first);
    //     LOGD("value: %zu", p.second.size());
    //     LOGD();
    // }
}


void computeAntinodes() {

    for (const auto &p : locations) {

        const std::vector<Coord> &coords = p.second;

        for (int i = 0; i < coords.size()-1; i++) {

            Coord a = coords[i];

            for (int j = i+1; j < coords.size(); j++) {

                Coord b = coords[j];

                int rowDiff = a.row - b.row;
                int colDiff = a.col - b.col;


                Coord antinode = a;

                while (true) {

                    if (!(0 <= antinode.row && antinode.row < rowCount && 0 <= antinode.col && antinode.col < colCount)) {
                        break;
                    }

                    antinodes[antinode.row][antinode.col] = '#';

                    antinode.row += rowDiff;
                    antinode.col += colDiff;
                }


                antinode = a;

                while (true) {

                    if (!(0 <= antinode.row && antinode.row < rowCount && 0 <= antinode.col && antinode.col < colCount)) {
                        break;
                    }

                    antinodes[antinode.row][antinode.col] = '#';

                    antinode.row -= rowDiff;
                    antinode.col -= colDiff;
                }
            }
        }
    }
}


void computeAntinodeTotal() {

    int total = 0;

    for (int i = 0; i < rowCount; i++) {
        const char *row = antinodes[i];
        for (int j = 0; j < colCount; j++) {
            if (row[j] == '#') {
                total++;
            }
        }
    }

    LOGI("total: %d", total);
}
















