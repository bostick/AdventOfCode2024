
#include "common/status.h"


struct Coord {
    int row;
    int col;

    Coord() {}

    Coord(int row, int col) :
        row(row), col(col) {}

    bool operator==(const Coord &other) const {
        return row == other.row && col == other.col;
    }
};

Status computeFirstBadCoord(const char *path, Coord *firstBadCoord);
















