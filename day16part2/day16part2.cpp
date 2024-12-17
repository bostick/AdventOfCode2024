
#include "day16part2.h"

#undef NDEBUG

#include "common/abort.h"
#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"

#include <set>
#include <vector>


#define TAG "day16part2"


const int COST_MAX = 1000000000;


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
};


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
};


std::string nodeStr(Node n) {
    return std::string("{") + std::to_string(n.row) + "," + std::to_string(n.col) + "," + dirStr(n.dir) + "}";
}


struct Details {

    int parentRow;
    int parentCol;
    Dir parentDir;

    int cost;
};





size_t colCount;
size_t rowCount;

char **grid;

char **bestPathTiles;

Details ***detailss;

bool ***closed;



void printGrid();

void printBestPathTiles();

void printClosed();

void printCosts();

void dumpCosts();

Status searchMap(Node start, Node end, bool continueUntilAllNodesAreClosed);

std::vector<Node> calculateSuccessors(Node n);

void printNode(Node n);

int calculateScore(Node start, Node end);

int calculateTurnCount(Node start, Node end);

int calculateMoveCount(Node start, Node end);

// std::string computePathHash(Node start, Node end);

void saveBestPathTiles(Node start, Node end);



Status computeBestPathTilesCount(const char *path, int *bestPathTilesCount) {

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

    bestPathTiles = static_cast<char **>(malloc(rowCount * sizeof(char *)));
    for (int i = 0; i < rowCount; i++) {
        bestPathTiles[i] = (char *)malloc(colCount * sizeof(char));
        for (int j = 0; j < colCount; j++) {
            bestPathTiles[i][j] = grid[i][j];
        }
    }

    detailss = (Details ***)malloc(rowCount * sizeof(Details **));
    for (int i = 0; i < rowCount; i++) {
        detailss[i] = (Details **)malloc(colCount * sizeof(Details *));
        for (int j = 0; j < colCount; j++) {
            detailss[i][j] = (Details *)malloc(4 * sizeof(Details));
        }
    }

    closed = (bool ***)malloc(rowCount * sizeof(bool **));
    for (int i = 0; i < rowCount; i++) {
        closed[i] = (bool **)malloc(colCount * sizeof(bool *));
        for (int j = 0; j < colCount; j++) {
            closed[i][j] = (bool *)malloc(4 * sizeof(bool));
        }
    }

    // printGrid();


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


    int scoreEast;
    int scoreNorth;

    int turnCountEast;
    int turnCountNorth;

    int moveCountEast;
    int moveCountNorth;

    // std::set<std::string> pathHashes;

    Node start = Node{startCoord.row, startCoord.col, EAST};

    {
        Node end = Node{endCoord.row, endCoord.col, EAST};

        if (searchMap(start, end, false) == OK) {

            scoreEast = calculateScore(start, end);

            turnCountEast = calculateTurnCount(start, end);

            moveCountEast = calculateMoveCount(start, end);

        } else {

            scoreEast = COST_MAX;
        }
    }

    {
        Node end = Node{endCoord.row, endCoord.col, NORTH};
        
        if (searchMap(start, end, false) == OK) {

            scoreNorth = calculateScore(start, end);

            turnCountNorth = calculateTurnCount(start, end);

            moveCountNorth = calculateMoveCount(start, end);

        } else {

            scoreNorth = COST_MAX;
        }
    }

    if (scoreEast == COST_MAX && scoreNorth == COST_MAX) {

        LOGI("could not solve map");

        return ERR;
    }

    int lowestScore;
    int turnCount;
    int moveCount;

    if (scoreEast < scoreNorth) {

        LOGD("only ending on EAST gives shortest path");

        lowestScore = scoreEast;
        turnCount = turnCountEast;
        moveCount = moveCountEast;

    } else {

        if (scoreNorth < scoreEast) {

            LOGD("only ending on NORTH gives shortest path");

            lowestScore = scoreNorth;
            turnCount = turnCountNorth;
            moveCount = moveCountNorth;

        } else {

            LOGD("ending on both EAST and NORTH gives shortest path");

            lowestScore = scoreNorth;
            turnCount = turnCountNorth;
            moveCount = moveCountNorth;
        }
    }
    

    LOGD("we have lowest score: %d", lowestScore);
    LOGD("we have turn count: %d", turnCount);
    LOGD("we have move count: %d", moveCount);


    // {
    //     Node end = Node{endCoord.row, endCoord.col, EAST};

    //     if (searchMap(start, end, true) == OK) {

    //     } else {

    //     }

    //     printCosts();
    // }

    Node end = Node{endCoord.row, endCoord.col, NORTH};
    
    if (searchMap(start, end, true) == OK) {

    } else {

    }

    printCosts();

    dumpCosts();


    printBestPathTiles();


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

void printClosed() {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < colCount; j++) {
            bool c = true;
            for (int k = 0; k < 4; k++) {
                c &= closed[i][j][k];
            }
            rowStr += (c ? '1' : '0');
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}

void printCosts() {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < colCount; j++) {
            if (grid[i][j] == '#') {
                rowStr += '#';
                continue;
            }
            bool maxCost = true;
            for (int k = 0; k < 4; k++) {
                if (detailss[i][j][k].cost != COST_MAX) {
                    maxCost = false;
                    break;
                }
            }
            if (maxCost) {
                rowStr += '!';
                ASSERT(false);
            } else {
                rowStr += '0';
            }
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}


void dumpCosts() {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < colCount; j++) {
            if (grid[i][j] == '#') {
                rowStr += '#';
                rowStr += ',';
                continue;
            }
            rowStr += '{';
            for (int k = 0; k < 4; k++) {
                rowStr += std::to_string(detailss[i][j][k].cost);
                rowStr += ',';
            }
            rowStr += '}';
            rowStr += ',';
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}


void printBestPathTiles() {

    for (int i = 0; i < rowCount; i++) {
        std::string rowStr;
        for (int j = 0; j < colCount; j++) {
            rowStr += bestPathTiles[i][j];
        }
        LOGI("%s", rowStr.c_str());
    }
    LOGI();
}




Status searchMap(Node start, Node end, bool continueUntilAllNodesAreClosed) {

    LOGD("searching...");
    LOGD("continueUntilAllNodesAreClosed: %d", continueUntilAllNodesAreClosed);

    LOGT("start:");
    printNode(start);
    LOGT("end:");
    printNode(end);


    std::vector<Node> open;


    for (int i = 0; i < rowCount; i++) {
        for (int j = 0; j < colCount; j++) {
            for (int k = 0; k < 4; k++) {
                detailss[i][j][k] = Details{ -1, -1, UNKNOWN, COST_MAX };
            }
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

        LOGT("q:");
        printNode(q);

        if (q == end) {

            if (!continueUntilAllNodesAreClosed) {
                LOGD("path found!");
                return OK;
            }
        }
        
        if (closed[q.row][q.col][q.dir]) {
            // LOGD("already processed q at a lower cost and closed: %s", nodeStr(q).c_str());
            continue;
        }

        closed[q.row][q.col][q.dir] = true;

        LOGT("node is now closed:");
        printNode(q);

        auto successors = calculateSuccessors(q);

        for (auto s : successors) {

            LOGT("s:");
            printNode(s);

            if (closed[s.row][s.col][s.dir] == true) {

                LOGT("s is closed; continuing");

                continue;
            }

            LOGT("s is NOT closed");

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
                
                LOGT("inserting s into open based on cost_new %d:", cost_new);
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

    if (!continueUntilAllNodesAreClosed) {

        LOGD("NO path found!");

        return ERR;
    }

    return OK;
}



std::vector<Node> calculateSuccessors(Node n) {

    std::vector<Node> ns;

    switch (n.dir) {
    case NORTH:

        if (grid[n.row-1][n.col] != '#') {
            ns.push_back( Node{n.row-1, n.col, NORTH} );
        }

        ns.push_back( Node{n.row, n.col, EAST} );
        ns.push_back( Node{n.row, n.col, WEST} );

        break;
    case EAST:

        if (grid[n.row][n.col+1] != '#') {
            ns.push_back( Node{n.row, n.col+1, EAST} );
        }

        ns.push_back( Node{n.row, n.col, NORTH} );
        ns.push_back( Node{n.row, n.col, SOUTH} );

        break;
    case SOUTH:

        if (grid[n.row+1][n.col] != '#') {
            ns.push_back( Node{n.row+1, n.col, SOUTH} );
        }

        ns.push_back( Node{n.row, n.col, EAST} );
        ns.push_back( Node{n.row, n.col, WEST} );

        break;
    case WEST:

        if (grid[n.row][n.col-1] != '#') {
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
    LOGT("node: %d %d %s", n.row, n.col, dirStr(n.dir).c_str());
}


int calculateScore(Node start, Node end) {

    int score = 0;

    auto n = end;

    while (true) {

        if (n == start) {
            
            break;
            
        } else {

            auto details = detailss[n.row][n.col][n.dir];
            
            ASSERT(details.parentDir != UNKNOWN);

            int dist = (abs(details.parentRow - n.row) + abs(details.parentCol - n.col));

            if (dist == 1) {

                ASSERT(n.dir == details.parentDir);

                score += 1;

            } else if (dist == 0) {

                ASSERT(n.dir != details.parentDir);

                score += 1000;
            }

            n = Node{details.parentRow, details.parentCol, details.parentDir};
        }
    }

    return score;
}


int calculateTurnCount(Node start, Node end) {

    int turnCount = 0;

    auto n = end;

    while (true) {

        if (n == start) {
            
            break;
            
        } else {

            auto details = detailss[n.row][n.col][n.dir];
            
            ASSERT(details.parentDir != UNKNOWN);

            int dist = (abs(details.parentRow - n.row) + abs(details.parentCol - n.col));

            if (dist == 1) {

                ASSERT(n.dir == details.parentDir);

            } else if (dist == 0) {

                ASSERT(n.dir != details.parentDir);

                turnCount++;
            }

            n = Node{details.parentRow, details.parentCol, details.parentDir};
        }
    }

    return turnCount;
}


int calculateMoveCount(Node start, Node end) {

    int moveCount = 0;

    auto n = end;

    while (true) {

        if (n == start) {
            
            break;
            
        } else {

            auto details = detailss[n.row][n.col][n.dir];
            
            ASSERT(details.parentDir != UNKNOWN);

            int dist = (abs(details.parentRow - n.row) + abs(details.parentCol - n.col));

            if (dist == 1) {

                ASSERT(n.dir == details.parentDir);

                moveCount++;

            } else if (dist == 0) {

                ASSERT(n.dir != details.parentDir);
            }

            n = Node{details.parentRow, details.parentCol, details.parentDir};
        }
    }

    return moveCount;
}


void saveBestPathTiles(Node start, Node end) {

    auto n = end;

    bestPathTiles[n.row][n.col] = 'O';

    while (true) {

        if (n == start) {
            
            break;
            
        } else {

            auto details = detailss[n.row][n.col][n.dir];
            
            ASSERT(details.parentDir != UNKNOWN);

            n = Node{details.parentRow, details.parentCol, details.parentDir};

            bestPathTiles[n.row][n.col] = 'O';
        }
    }
}













