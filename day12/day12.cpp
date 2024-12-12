
#include "day12.h"

#undef NDEBUG

#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"

#include <set>
#include <vector>


#define TAG "day12"


struct Coord {
    int row;
    int col;

    bool operator<(const Coord& other) const {
        if (row < other.row) {
            return true;
        }
        if (other.row < row) {
            return false;
        }
        if (col < other.col) {
            return true;
        }
        return false;
    }
};


// struct Cursor {
//     int row;
//     int col;

// };




size_t colCount;
size_t rowCount;

char **grid;


void printGrid();

void computeRegions();

void printRegion(std::set<Coord> region);

int computeRegionPrice(std::set<Coord> region);

int computeRegionPerimeter(std::set<Coord> region);


std::vector<std::set<Coord>> regions;




Status computeTotalPrice(const char *path, int *totalPriceOut) {

    std::vector<uint8_t> buf;

    if (openFile(path, buf) != OK) {
        return ERR;
    }

    uint8_t *bufEnd = buf.data() + buf.size();


    const char *tosearch = reinterpret_cast<const char *>(buf.data());

    const char *newline = strchr(tosearch, '\n');


    colCount = static_cast<size_t>(newline - tosearch);

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


    printGrid();


    computeRegions();


    int totalPrice = 0;
    for (const auto &r : regions) {

        int regionPrice;
        regionPrice = computeRegionPrice(r);

        totalPrice += regionPrice;

    }


    LOGD("totalPrice: %d", totalPrice);


    *totalPriceOut = totalPrice;



    for (int i = 0; i < rowCount; i++) {
        free(grid[i]);
    }
    free(grid);

    return OK;
}



void computeRegions() {


    std::set<Coord> unknownRegionCoords;


    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            unknownRegionCoords.insert( Coord{i, j} );
        }
    }


    // LOGD("work through regions");

    while (true) {

        if (unknownRegionCoords.empty()) {
            break;
        }

        // LOGD("unknownRegionCoords size: %zu", unknownRegionCoords.size());


        //
        // pick first coord with unknown region
        //
        auto it = unknownRegionCoords.begin();
        Coord n = *it;

        char plantType = grid[n.row][n.col];

        // LOGD("n: %d %d %c", n.row, n.col, plantType);

        //
        // flood fill based on n
        //

        std::vector<Coord> q;

        std::set<Coord> alreadyProcessed;

        std::set<Coord> region;
        
        
        q.push_back(n);

        while (true) {

            if (q.empty()) {
                break;
            }

            // LOGD("q size: %zu", q.size());

            Coord n2 = q.front();
            q.erase(q.begin());

            //
            // test if inside region
            //
            if (grid[n2.row][n2.col] != plantType) {
                continue;
            }

            if (alreadyProcessed.contains(n2)) {
                continue;
            }

            region.insert(n2);

            alreadyProcessed.insert(n2);
            
            unknownRegionCoords.erase(n2);


            if (0 <= n2.row-1) {
                q.push_back( Coord{ n2.row-1, n2.col } );
            }

            if (n2.col+1 <= colCount-1) {
                q.push_back( Coord{ n2.row, n2.col+1 } );
            }

            if (n2.row+1 <= rowCount-1) {
                q.push_back( Coord{ n2.row+1, n2.col } );
            }

            if (0 <= n2.col-1) {
                q.push_back( Coord{ n2.row, n2.col-1 } );
            }
        
        }

        // LOGD("q is empty");

        // LOGD("region with type %c size: %zu", plantType, region.size());

        regions.push_back(region);
    }


    // LOGD("unknownRegionCoords is empty");
}



int computeRegionPrice(std::set<Coord> region) {

    LOGD("compute price for region:");
    printRegion(region);

    int area = region.size();

    int perimeter = computeRegionPerimeter(region);

    LOGD("region area: %d", area);
    LOGD("region perimeter: %d", perimeter);

    int price = area * perimeter;

    LOGD("region price: %d", price);

    return price;
}


int computeRegionPerimeter(std::set<Coord> region) {

    ASSERT(!region.empty());

    int perimeter = 0;

    for (auto it = region.begin(); it != region.end(); it++) {

        Coord coord = *it;
        
        perimeter += 4;
        
        if (0 <= coord.row-1 && region.contains( Coord{coord.row-1, coord.col} )) {
            perimeter--;
        }
        
        if (coord.col+1 <= colCount-1 && region.contains( Coord{coord.row, coord.col+1} )) {
            perimeter--;
        }
        
        if (coord.row+1 <= rowCount-1 && region.contains( Coord{coord.row+1, coord.col} )) {
            perimeter--;
        }
        
        if (0 <= coord.col-1 && region.contains( Coord{coord.row, coord.col-1} )) {
            perimeter--;
        }
    }

    // if (region.size() == 1) {
    //     return 4;
    // }

    // auto it = region.begin();
    // Coord  = *it;

    // Cursor firstCursor = Cursor{ n.row, n.col };

    // //
    // // we know n is first, sorted by row and col
    // // so n is an upper-left corner
    // //
    // // we know we can move 1 to the right, so just start here
    // //
    // Cursor cursor = Cursor{ n.row, n.col+1 };

    // int perimeter = 1;

    // while (true) {

    //     if (cursor == firstCursor) {
    //         break;
    //     }

    //     //
    //     // move clockwise
    //     //

    //     if (region.contains( Coord{n.row, n.col+1} )) {

    //         //
    //         // move right
    //         //

    //         cursor.col++;

    //         perimeter += 1;


    //     } else if (region.contains( Coord{n.row+1, n.col} )) {

    //         //
    //         // move down
    //         //

    //         perimeter += 1;

    //     } else {
    //         ASSERT(false);
    //     }
    // }

    // LOGD("cursor reached back to start");

    return perimeter;
}


void printGrid() {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < colCount; j++) {
            rowStr += grid[i][j];
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}


void printRegion(std::set<Coord> region) {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < colCount; j++) {
            if (region.contains( Coord{i, j} )) {
                rowStr += grid[i][j];
            } else {
                rowStr += '.';
            }
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}
















