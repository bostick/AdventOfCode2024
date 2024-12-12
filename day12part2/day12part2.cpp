
#include "day12part2.h"

#undef NDEBUG

#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"

#include <set>
#include <vector>


#define TAG "day12part2"


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



struct Garden {

    int rowCount;
    int colCount;

    char **grid;

    std::vector<std::set<Coord>> regions;

    Garden(int rowCount, int colCount) :
        rowCount(rowCount),
        colCount(colCount) {

        grid = static_cast<char **>(malloc(rowCount * sizeof(char *)));
        for (int i = 0; i < rowCount; i++) {
            grid[i] = (char *)malloc(colCount * sizeof(char));
        }
    }

    ~Garden() {
        for (int i = 0; i < rowCount; i++) {
            free(grid[i]);
        }
        free(grid);
    }
};



void printGrid(const Garden &g);

void computeRegions(Garden &g);

void printRegion(const Garden &g, const std::set<Coord> &region);

int computeRegionPrice(const Garden &g, const std::set<Coord> &region);

int computeRegionSideCount(const Garden &g, const std::set<Coord> &region);



Status computeTotalPrice(const char *path, int *totalPriceOut) {

    std::vector<uint8_t> buf;

    if (openFile(path, buf) != OK) {
        return ERR;
    }

    uint8_t *bufEnd = buf.data() + buf.size();


    const char *tosearch = reinterpret_cast<const char *>(buf.data());

    const char *newline = strchr(tosearch, '\n');


    int colCount = static_cast<int>(newline - tosearch);

    // LOGI("diff: %td", diff);


    tosearch = newline + 1;

    newline = strchr(tosearch, '\n');

    int rowCount = 1;
    while (newline != NULL) {
        rowCount++;
        tosearch = newline + 1;
        newline = strchr(tosearch, '\n');
    }


    Garden g{rowCount, colCount};


    // LOGI("colCount: %d", colCount);
    // LOGI("rowCount: %d", rowCount);


    for (int i = 0; i < g.rowCount; i++) {
        for (int j = 0; j < g.colCount; j++) {
            //
            // (rowCount + 1) for the newline
            //
            g.grid[i][j] = buf[i * (g.rowCount + 1) + j];
        }
    }


    printGrid(g);


    computeRegions(g);


    int totalPrice = 0;
    for (const auto &r : g.regions) {

        int regionPrice;
        regionPrice = computeRegionPrice(g, r);

        totalPrice += regionPrice;

    }


    LOGD("totalPrice: %d", totalPrice);


    *totalPriceOut = totalPrice;


    return OK;
}



void computeRegions(Garden &g) {


    std::set<Coord> unknownRegionCoords;


    for (int i = 0; i < g.rowCount; i++) {
        for (int j = 0; j < g.colCount; j++) {
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

        char plantType = g.grid[n.row][n.col];

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
            if (g.grid[n2.row][n2.col] != plantType) {
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

            if (n2.col+1 <= g.colCount-1) {
                q.push_back( Coord{ n2.row, n2.col+1 } );
            }

            if (n2.row+1 <= g.rowCount-1) {
                q.push_back( Coord{ n2.row+1, n2.col } );
            }

            if (0 <= n2.col-1) {
                q.push_back( Coord{ n2.row, n2.col-1 } );
            }
        
        }

        // LOGD("q is empty");

        // LOGD("region with type %c size: %zu", plantType, region.size());

        g.regions.push_back(region);
    }


    // LOGD("unknownRegionCoords is empty");
}



int computeRegionPrice(const Garden &g, const std::set<Coord> &region) {

    LOGD("compute price for region:");
    printRegion(g, region);

    int area = region.size();

    int sideCount = computeRegionSideCount(g, region);

    LOGD("region area: %d", area);
    LOGD("region sideCount: %d", sideCount);

    int price = area * sideCount;

    LOGD("region price: %d", price);

    return price;
}


int computeRegionSideCount(const Garden &g, const std::set<Coord> &region) {

    ASSERT(!region.empty());

    //
    // region bounding box
    //

    int regionBoundingBoxLeft = g.colCount;
    int regionBoundingBoxRight = 0;
    int regionBoundingBoxUp = g.rowCount;
    int regionBoundingBoxDown = 0;

    for (auto it = region.begin(); it != region.end(); it++) {

        Coord coord = *it;

        if (coord.col < regionBoundingBoxLeft) {
            regionBoundingBoxLeft = coord.col;
        }

        if (regionBoundingBoxRight < coord.col) {
            regionBoundingBoxRight = coord.col;
        }

        if (coord.row < regionBoundingBoxUp) {
            regionBoundingBoxUp = coord.row;
        }

        if (regionBoundingBoxDown < coord.row) {
            regionBoundingBoxDown = coord.row;
        }
    }

    LOGD("regionBoundingBoxLeft: %d", regionBoundingBoxLeft);
    LOGD("regionBoundingBoxRight: %d", regionBoundingBoxRight);
    LOGD("regionBoundingBoxUp: %d", regionBoundingBoxUp);
    LOGD("regionBoundingBoxDown: %d", regionBoundingBoxDown);

    int regionBoundingBoxArea = (regionBoundingBoxRight - regionBoundingBoxLeft + 1) * (regionBoundingBoxDown - regionBoundingBoxUp + 1);
    ASSERT(0 < regionBoundingBoxArea);

    LOGD("bounding box area: %d", regionBoundingBoxArea);

    if (region.size() == regionBoundingBoxArea) {
        //
        // region fills bounding box, so is rectangular
        //
        return 4;
    }

    //
    // make new, smaller garden with this bounding box for this region
    //

    Garden g2{(regionBoundingBoxDown - regionBoundingBoxUp + 1), (regionBoundingBoxRight - regionBoundingBoxLeft + 1)};

    for (int i = 0; i < g2.rowCount; i++) {
        for (int j = 0; j < g2.colCount; j++) {
            if (region.contains( Coord{i + regionBoundingBoxUp, j + regionBoundingBoxLeft} )) {
                g2.grid[i][j] = 'C';
            } else {
                g2.grid[i][j] = '.';
            }
        }
    }

    computeRegions(g2);

    LOGD("smaller garden:");
    printGrid(g2);

    int cRegionIndex;
    //
    // find which region is the 'C' region
    //
    for (int i = 0; i < g2.regions.size(); i++) {
        
        const std::set<Coord> &r = g2.regions[i];

        auto rit = r.begin();
        Coord rFirst = *rit;

        ASSERT(g2.grid[rFirst.row][rFirst.col] == 'C' || g2.grid[rFirst.row][rFirst.col] == '.');

        if (g2.grid[rFirst.row][rFirst.col] == 'C') {
            cRegionIndex = i;
            break;
        }
    }

    int sideCount = 4;

    //
    // now recursively call computeRegionSideCount
    //
    for (int i = 0; i < g2.regions.size(); i++) {

        if (i == cRegionIndex) {
            continue;
        }

        int regionSideCount;
        regionSideCount = computeRegionSideCount(g2, g2.regions[i]);

        sideCount += regionSideCount;
    }

    //
    // now account for corners
    //
    if (g2.grid[0][0] == '.') {
        sideCount -= 2;
    }
    if (g2.grid[0][g2.colCount-1] == '.') {
        sideCount -= 2;
    }
    if (g2.grid[g2.rowCount-1][0] == '.') {
        sideCount -= 2;
    }
    if (g2.grid[g2.rowCount-1][g2.colCount-1] == '.') {
        sideCount -= 2;
    }

    return sideCount;
}


void printGrid(const Garden &g) {

    for (int i = 0; i < g.rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < g.colCount; j++) {
            rowStr += g.grid[i][j];
        }
        LOGD("%s", rowStr.c_str());
    }
    LOGD();
}


void printRegion(const Garden &g, const std::set<Coord> &region) {

    for (int i = 0; i < g.rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < g.colCount; j++) {
            if (region.contains( Coord{i, j} )) {
                rowStr += g.grid[i][j];
            } else {
                rowStr += '.';
            }
        }
        LOGD("%s", rowStr.c_str());
    }
    LOGD();
}
















