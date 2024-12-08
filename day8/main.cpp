
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


#define TAG "day8"


struct Coord {
    int row;
    int col;
};


size_t rowCount;
size_t colCount;


char **grid;
char **antinodes;


std::map<char, std::vector<Coord>> locations;


void print();

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


    computeAntinodeTotal();


    return EXIT_SUCCESS;
}


void print() {

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


void computeLocations() {

    for (int i = 0; i < rowCount; i++) {
        const char *row = grid[i];
        for (int j = 0; j < colCount; j++) {

            char c = row[j];

            if (islower(c) || isupper(c) || isdigit(c)) {

                if (!locations.contains(c)) {
                    locations[c] = {};
                }

                std::vector<Coord> &coords = locations[c];

                coords.push_back(Coord{i, j});
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

    for (const auto& p : locations) {
        
        char c = p.first;

        const std::vector<Coord> &coords = p.second;

        for (int i = 0; i < coords.size()-1; i++) {

            Coord a = coords[i];

            for (int j = 0; j < coords.size(); j++) {

                Coord b = coords[j];

                Coord diff1 = { a.row - b.row, a.col - b.col };

                Coord antinode1 = { a.row + diff1.row, a.col + diff1.col };

                Coord diff2 = { b.row - a.row, b.col - a.col };

                Coord antinode2 = { b.row + diff2.row, b.col + diff2.col };

                if (0 <= antinode1.row && antinode1.row < rowCount && 0 <= antinode1.col && antinode1.col < colCount) {

                    if (grid[antinode1.row][antinode1.col] != c) {
                        // ASSERT(false);
                        antinodes[antinode1.row][antinode1.col] = '#';
                    }

                    // antinodes[antinode1.row][antinode1.col] = '#';
                }

                if (0 <= antinode2.row && antinode2.row < rowCount && 0 <= antinode2.col && antinode2.col < colCount) {

                    if (grid[antinode2.row][antinode2.col] != c) {
                        // ASSERT(false);
                        antinodes[antinode2.row][antinode2.col] = '#';
                    }

                    // antinodes[antinode2.row][antinode2.col] = '#';
                }
            }
        }
    }
}


void computeAntinodeTotal() {


    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        const char *row = antinodes[i];
        for (int j = 0; j < colCount; j++) {
            rowStr += row[j];
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();


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
















