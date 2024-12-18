
#include "day18part2.h"

#undef NDEBUG

#include "common/abort.h"
#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"
#include "common/string_utils.h"

#include <regex>
#include <set>
#include <vector>


#define TAG "day18part2"


const int COST_MAX = 1000000000;


// enum Dir {
//     NORTH,
//     EAST,
//     SOUTH,
//     WEST,
//     UNKNOWN,
// };

// std::string dirStr(Dir d) {
//     switch (d) {
//     case NORTH:
//         return "NORTH";
//     case EAST:
//         return "EAST";
//     case SOUTH:
//         return "SOUTH";
//     case WEST:
//         return "WEST";
//     default:
//         ABORT("unhandled");
//     }
// }


// struct Coord {
//     int row;
//     int col;

//     Coord() {}

//     Coord(int row, int col) :
//         row(row), col(col) {}
// };


struct Node {

    int row;
    int col;
    // Dir dir;

    Node(int row, int col) :
        row(row), col(col) {}

    bool operator==(const Node &other) const {
        return row == other.row &&
            col == other.col;
    }
};


struct Details {

    int parentRow;
    int parentCol;
    // Dir parentDir;

    int cost;
};



size_t colCount;
size_t rowCount;

char **grid;

Details **detailss;



void printGrid();


Status searchMap(Node start, Node end);

std::vector<Node> calculateSuccessors(Node n);

void printNode(Node n);

// int calculateStepCount(Node start, Node end);



std::regex r("(\\d+),(\\d+)");



Status computeFirstBadCoord(const char *path, Coord *firstBadCoord) {


    if (strcmp(path, "example.txt") == 0) {

        rowCount = 7;
        colCount = 7;

    } else if (strcmp(path, "input.txt") == 0) {

        rowCount = 71;
        colCount = 71;

    } else {
        ABORT("unhandled");
    }


    std::vector<uint8_t> buf;

    if (openFile(path, buf) != OK) {
        return ERR;
    }

    uint8_t *bufEnd = buf.data() + buf.size();


    std::vector<Coord> bytePositions;


    const char *tosearch = reinterpret_cast<const char *>(buf.data());

    const char *newline = strchr(tosearch, '\n');

    while (newline != NULL) {

        std::string line = std::string(tosearch, newline);
        // LOGI("line: %s", line.c_str());

        std::smatch m;

        ASSERT(std::regex_match(line, m, r));

        auto capture1 = m.str(1);

        auto capture2 = m.str(2);
                
        int n1;
        parseInt(capture1, &n1);
        int n2;
        parseInt(capture2, &n2);

        bytePositions.push_back( Coord{n2, n1} );

        tosearch = newline + 1;
        ASSERT(tosearch <= reinterpret_cast<const char *>(bufEnd));
        
        newline = strchr(tosearch, '\n');
    }


    Coord startCoord;
    Coord endCoord;

    if (strcmp(path, "example.txt") == 0) {

        startCoord = Coord{ 0, 0 };
        endCoord = Coord{ 6, 6 };

    } else if (strcmp(path, "input.txt") == 0) {

        startCoord = Coord{ 0, 0 };
        endCoord = Coord{ 70, 70 };

    } else {
        ABORT("unhandled");
    }



    Node start = Node{startCoord.row, startCoord.col};

    Node end = Node{endCoord.row, endCoord.col};


    int byteCount;
    for (byteCount = 0; byteCount < bytePositions.size(); byteCount++) {

        LOGI("byteCount: %d", byteCount);

        grid = static_cast<char **>(malloc(rowCount * sizeof(char *)));
        for (int i = 0; i < rowCount; i++) {
            grid[i] = (char *)malloc(colCount * sizeof(char));
            for (int j = 0; j < colCount; j++) {
                grid[i][j] = '.';
            }
        }

        for (int i = 0; i < byteCount; i++) {
            Coord bytePos = bytePositions[i];
            grid[bytePos.row][bytePos.col] = '#';
        }

        printGrid();

        if (searchMap(start, end) == OK) {

        } else {

            LOGI("found first bad byte!");
            break;
            
        }
    }

    ASSERT(byteCount != bytePositions.size());

    LOGI("first bad byte count: %d", byteCount);

    *firstBadCoord = bytePositions[byteCount-1];

    LOGI("first bad byte position (x,y): %d,%d", firstBadCoord->col, firstBadCoord->row);

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

    bool **closed;


    detailss = (Details **)malloc(rowCount * sizeof(Details *));
    for (int i = 0; i < rowCount; i++) {
        detailss[i] = (Details *)malloc(colCount * sizeof(Details));
    }

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            detailss[i][j] = Details{ -1, -1, COST_MAX };
        }
    }


    closed = (bool **)malloc(rowCount * sizeof(bool *));
    for (int i = 0; i < rowCount; i++) {
        closed[i] = (bool *)malloc(colCount * sizeof(bool));
    }

    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            closed[i][j] = false;
        }
    }


    detailss[start.row][start.col].cost = 0;

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
        
        if (closed[q.row][q.col]) {
            // LOGD("already processed at a lower cost and closed");
            continue;
        }

        closed[q.row][q.col] = true;

        LOGD("node is now closed:");
        printNode(q);

        auto successors = calculateSuccessors(q);

        for (auto s : successors) {

            LOGD("s:");
            printNode(s);

            // if (find(closed.begin(), closed.end(), s) != closed.end()) {
            //  continue;
            // }
            if (closed[s.row][s.col] == true) {

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

                // ASSERT(s.dir == q.dir);

                cost_new = detailss[q.row][q.col].cost + 1;

            } else if (dist == 0) {

                //
                // turning
                //

                // ASSERT(s.dir != q.dir);

                cost_new = detailss[q.row][q.col].cost + 1000;

            } else {
                ABORT("unhandled");
            }


            if (cost_new < detailss[s.row][s.col].cost) {

                detailss[s.row][s.col] = Details{ q.row, q.col, cost_new };

                // if (s.row == 5 && s.col == 11 && s.dir == SOUTH) {
                //     LOGD();
                // }
                
                // LOGD("inserting s into open based on cost_new %d:", cost_new);
                // printNode(s);

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
                        if (cost_new < detailss[tmp.row][tmp.col].cost) {
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

    // switch (n.dir) {
    // case NORTH:

    if (0 <= n.row-1 && grid[n.row-1][n.col] != '#') {
//            ASSERT(grid[n.row-1][n.col] == '.');
        ns.push_back( Node{n.row-1, n.col} );
    }

        // ns.push_back( Node{n.row, n.col, EAST} );
        // ns.push_back( Node{n.row, n.col, WEST} );

    //     break;
    // case EAST:

    if (n.col+1 <= colCount-1 && grid[n.row][n.col+1] != '#') {
//            ASSERT(grid[n.row][n.col+1] == '.');
        ns.push_back( Node{n.row, n.col+1} );
    }

        // ns.push_back( Node{n.row, n.col, NORTH} );
        // ns.push_back( Node{n.row, n.col, SOUTH} );

    //     break;
    // case SOUTH:

    if (n.row+1 <= rowCount-1 && grid[n.row+1][n.col] != '#') {
//            ASSERT(grid[n.row+1][n.col] == '.');
        ns.push_back( Node{n.row+1, n.col} );
    }

        // ns.push_back( Node{n.row, n.col, EAST} );
        // ns.push_back( Node{n.row, n.col, WEST} );

        // break;
    // case WEST:

    if (0 <= n.col-1 && grid[n.row][n.col-1] != '#') {
//            ASSERT(grid[n.row][n.col-1] == '.');
        ns.push_back( Node{n.row, n.col-1} );
    }

    //     ns.push_back( Node{n.row, n.col, NORTH} );
    //     ns.push_back( Node{n.row, n.col, SOUTH} );

    //     break;
    // case UNKNOWN:
    //     ABORT("unhandled");
    // }

    return ns;
}


void printNode(Node n) {
    LOGD("node: %d %d", n.row, n.col);
}


// int calculateStepCount(Node start, Node end) {

//     int stepCount = 0;

//     auto n = end;

//     while (true) {

//         if (n == start) {
            
//             break;
            
//         } else {

//             auto details = detailss[n.row][n.col];
            
//             if (details.parentRow == -1) {
//                 LOGD("this particular map was not solved");
//                 return INT_MAX;
//             }

//             int dist = (abs(details.parentRow - n.row) + abs(details.parentCol - n.col));

//             ASSERT(dist == 1);

//             stepCount++;

//             n = Node{details.parentRow, details.parentCol};
//         }
//     }

//     return stepCount;
// }
















