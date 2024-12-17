
#include "day16.h"

#undef NDEBUG

#include "common/abort.h"
#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"

#include <set>
#include <vector>


#define TAG "day16"


const int COST_MAX = INT_MAX;


enum Dir {
    NORTH,
    EAST,
    SOUTH,
    WEST,
    UNKNOWN,
};

std::string dirStr(Dir d) {
    switch (d) {
    case NORTH:
        return "NORTH";
    case EAST:
        return "EAST";
    case SOUTH:
        return "SOUTH";
    case WEST:
        return "WEST";
    default:
        ABORT("unhandled");
    }
}


struct Coord {
    int row;
    int col;

    Coord() {}

    Coord(int row, int col) :
        row(row), col(col) {}

    // bool operator<(const Coord& other) const {
    //     if (row < other.row) {
    //         return true;
    //     }
    //     if (other.row < row) {
    //         return false;
    //     }
    //     if (col < other.col) {
    //         return true;
    //     }
    //     return false;
    // }
};


// struct Cursor {
//     int row;
//     int col;

// };


struct Node {

    int row;
    int col;
    Dir dir;

    Node(int row, int col, Dir dir) :
        row(row), col(col), dir(dir) {}

    bool operator==(const Node &other) const {
        return row == other.row &&
            col == other.col &&
            dir == other.dir;
    }
    
    // bool operator<(const Node &other) const {

    //     if (row < other.row) {
    //         return true;
    //     }
    //     if (row > other.row) {
    //         return false;
    //     }
    //     if (col < other.col) {
    //         return true;
    //     }
    //     if (col > other.col) {
    //         return false;
    //     }
        
    //     if (dir < other.dir) {
    //         return true;
    //     }
    //     if (dir > other.dir) {
    //         return false;
    //     }
        
    //     if (segment_len < other.segment_len) {
    //         return true;
    //     }
    //     if (segment_len > other.segment_len) {
    //         return false;
    //     }
        
    //     ABORT("unhandled");
    // }
};


struct Details {

    int parentRow;
    int parentCol;
    Dir parentDir;

    int cost;
};





size_t colCount;
size_t rowCount;

char **grid;

Details ***detailss;

// Coord start;
// Coord end;

// Details ***detailss;



void printGrid();

// void computeRegions();

// void printRegion(std::set<Coord> region);

// int computeRegionPrice(std::set<Coord> region);

// int computeRegionPerimeter(std::set<Coord> region);


// std::vector<std::set<Coord>> regions;

Status searchMap(Node start, Node end);

std::vector<Node> calculateSuccessors(Node n);

void printNode(Node n);

int calculateScore(Node start, Node end);


Status computeLowestScore(const char *path, int *lowestScore) {

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


    Coord startCoord;
    Coord endCoord;
    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            if (grid[i][j] == 'S') {
                startCoord = Coord{i, j};
                continue;
            }
            if (grid[i][j] == 'E') {
                endCoord = Coord{i, j};
                continue;
            }
        }
    }


    int lowestScoreEast;
    int lowestScoreNorth;

    Node start = Node{startCoord.row, startCoord.col, EAST};

    {
        Node end = Node{endCoord.row, endCoord.col, EAST};

        if (searchMap(start, end) == OK) {

            lowestScoreEast = calculateScore(start, end);

        } else {

            lowestScoreEast = INT_MAX;
        }
    }

    {
        Node end = Node{endCoord.row, endCoord.col, NORTH};
        
        if (searchMap(start, end) == OK) {

            lowestScoreNorth = calculateScore(start, end);

        } else {

            lowestScoreNorth = INT_MAX;
        }
    }

    if (lowestScoreEast == INT_MAX && lowestScoreNorth == INT_MAX) {

        LOGI("could not solve map");

        return ERR;
    }

    if (lowestScoreEast < lowestScoreNorth) {

        *lowestScore = lowestScoreEast;

    } else {

        *lowestScore = lowestScoreNorth;
    }

    LOGI("lowest score: %d", *lowestScore);

    return OK;
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


Status searchMap(Node start, Node end) {

    LOGD("searching...");
    LOGD("start:");
    printNode(start);
    LOGD("end:");
    printNode(end);


    std::vector<Node> open;

    bool ***closed;



    detailss = (Details ***)malloc(rowCount * sizeof(Details **));
    for (int i = 0; i < rowCount; i++) {
        detailss[i] = (Details **)malloc(colCount * sizeof(Details *));
        for (int j = 0; j < colCount; j++) {
            detailss[i][j] = (Details *)malloc(4 * sizeof(Details));
        }
    }

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            for (int k = 0; k < 4; k++) {

                detailss[i][j][k] = Details{ -1, -1, UNKNOWN, COST_MAX };
            }
        }
    }


    closed = (bool ***)malloc(rowCount * sizeof(bool **));
    for (int i = 0; i < rowCount; i++) {
        closed[i] = (bool **)malloc(colCount * sizeof(bool *));
        for (int j = 0; j < colCount; j++) {
            closed[i][j] = (bool *)malloc(4 * sizeof(bool));
        }
    }

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            for (int k = 0; k < 4; k++) {
                closed[i][j][k] = false;
            }
        }
    }


    detailss[start.row][start.col][start.dir].cost = 0;

    open.push_back(start);


    while (true) {

        if (open.empty()) {
            break;
        }

        auto q = open.front();
        open.erase(open.begin());

        LOGD("q:");
        printNode(q);

        if (q == end) {
            LOGD("found!");
            return OK;
        }

        // ASSERT(grid[q.row][q.col == '.']);
        
        if (closed[q.row][q.col][q.dir]) {
            // LOGD("already processed at a lower cost and closed");
            continue;
        }

        closed[q.row][q.col][q.dir] = true;

        LOGD("node is now closed:");
        printNode(q);

        auto successors = calculateSuccessors(q);

        for (auto s : successors) {

            LOGD("s:");
            printNode(s);

            // if (find(closed.begin(), closed.end(), s) != closed.end()) {
            //  continue;
            // }
            if (closed[s.row][s.col][s.dir] == true) {

                LOGD("s is closed; continuing");

                continue;
            }

            LOGD("s is NOT closed");

            //
            // g = the movement cost to move from the starting point to a given square on the grid, following the path generated to get there. 
            //
            // s.g = q.g + distance between q and successor

            int cost_new;

            int dist = abs(s.row - q.row) + abs(s.col - q.col);

            if (dist == 1) {

                //
                // moving
                //

                ASSERT(s.dir == q.dir);

                cost_new = detailss[q.row][q.col][q.dir].cost + 1;

            } else if (dist == 0) {

                //
                // turning
                //

                ASSERT(s.dir != q.dir);

                cost_new = detailss[q.row][q.col][q.dir].cost + 1000;

            } else {
                ABORT("unhandled");
            }


            if (cost_new < detailss[s.row][s.col][s.dir].cost) {

                detailss[s.row][s.col][s.dir] = Details{ q.row, q.col, q.dir, cost_new };

                // if (s.row == 5 && s.col == 11 && s.dir == SOUTH) {
                //     LOGD();
                // }
                
                LOGD("inserting s into open based on cost_new %d:", cost_new);
                printNode(s);

                //
                // greatest value less than tmp cost
                //
                if (open.empty()) {
                    open.push_back(s);
                } else {

                    //
                    // XXX: should actually do binary search or have a heap data structure or something
                    //
                    int i;
                    for (i = 0; i < open.size(); i++) {
                        auto tmp = open[i];
                        if (cost_new < detailss[tmp.row][tmp.col][tmp.dir].cost) {
                            break;
                        }
                    }

                    open.insert(open.begin() + i, s);
                }
            }
        }
    }

    LOGD("done searching");

    return ERR;
}



std::vector<Node> calculateSuccessors(Node n) {

    std::vector<Node> ns;

    switch (n.dir) {
    case NORTH:

        if (grid[n.row-1][n.col] != '#') {
//            ASSERT(grid[n.row-1][n.col] == '.');
            ns.push_back( Node{n.row-1, n.col, NORTH} );
        }

        ns.push_back( Node{n.row, n.col, EAST} );
        ns.push_back( Node{n.row, n.col, WEST} );

        break;
    case EAST:

        if (grid[n.row][n.col+1] != '#') {
//            ASSERT(grid[n.row][n.col+1] == '.');
            ns.push_back( Node{n.row, n.col+1, EAST} );
        }

        ns.push_back( Node{n.row, n.col, NORTH} );
        ns.push_back( Node{n.row, n.col, SOUTH} );

        break;
    case SOUTH:

        if (grid[n.row+1][n.col] != '#') {
//            ASSERT(grid[n.row+1][n.col] == '.');
            ns.push_back( Node{n.row+1, n.col, SOUTH} );
        }

        ns.push_back( Node{n.row, n.col, EAST} );
        ns.push_back( Node{n.row, n.col, WEST} );

        break;
    case WEST:

        if (grid[n.row][n.col-1] != '#') {
//            ASSERT(grid[n.row][n.col-1] == '.');
            ns.push_back( Node{n.row, n.col-1, WEST} );
        }

        ns.push_back( Node{n.row, n.col, NORTH} );
        ns.push_back( Node{n.row, n.col, SOUTH} );

        break;
    case UNKNOWN:
        ABORT("unhandled");
    }

    return ns;
}


void printNode(Node n) {
    LOGD("node: %d %d %s", n.row, n.col, dirStr(n.dir).c_str());
}


int calculateScore(Node start, Node end) {

    int score = 0;

    auto n = end;

    while (true) {

        if (n == start) {
            
            break;
            
        } else {

            auto details = detailss[n.row][n.col][n.dir];
            
            if (details.parentDir == UNKNOWN) {
                LOGD("this particular map was not solved");
                return INT_MAX;
            }

            int dist = (abs(details.parentRow - n.row) + abs(details.parentCol - n.col));

            if (dist == 1) {

                score += 1;

            } else {

                score += 1000;
            }

            n = Node{details.parentRow, details.parentCol, details.parentDir};
        }
    }

    return score;
}
















