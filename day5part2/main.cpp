
#include "common/assert.h"
#include "common/file.h"
#include "common/logging.h"
#include "common/string_utils.h"

#include <algorithm>
#include <regex>
#include <set>
#include <vector>
#include <cstdio>
#include <cinttypes>
#include <cstring>


#define TAG "day5part2"


std::regex ruleRegex("(\\d+)\\|(\\d+)");


struct Rule {
    int a;
    int b;
};


std::vector<Rule> rules;


// bool globalInOrder;

bool globalPageNumberCompare(int a, int b) {
    
    // LOGD("calling globalPageNumberCompare %d %d", a, b);

    for (auto r : rules) {

        if (r.a == a && r.b == b) {
            // LOGD("rule applies! %d %d", a, b);
            return true;
        }

        if (r.a == b && r.b == a) {
            // LOGD("backward rule applies! %d %d", a, b);
            // globalInOrder = false;
            return false;
        }
    }

    return false;
}


struct PageNumber {
    
    int a;

    bool operator<(const PageNumber& other) const {
        return globalPageNumberCompare(a, other.a);
    }

    bool operator!=(const PageNumber& other) const {
        return a != other.a;
    }
};


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


    while (true) {

        std::string line = std::string(tosearch, newline);

        if (line.empty()) {
            break;
        }

        std::smatch m;

        ASSERT(std::regex_match(line, m, ruleRegex));

        auto capture1 = m.str(1);

        auto capture2 = m.str(2);
                
        int n1;
        parseInt(capture1, &n1);
        int n2;
        parseInt(capture2, &n2);

        Rule rule = Rule{n1, n2};

        rules.push_back(rule);

        tosearch = newline + 1;
        
        newline = strchr(tosearch, '\n');
    }

    LOGD("rules size: %zu", rules.size());


    tosearch = newline + 1;

    newline = strchr(tosearch, '\n');


    std::vector<std::vector<PageNumber>> updates;


    while (newline != NULL) {

        std::string line = std::string(tosearch, newline);
        
        const char *lineEnd = line.c_str() + line.size();


        std::vector<PageNumber> update;


        const char *tosearch2 = line.c_str();

        const char *comma = strchr(tosearch2, ',');

        while (comma != NULL) {

            std::string uStr = std::string(tosearch2, comma);

            int u;
            parseInt(uStr, &u);

            update.push_back(PageNumber{u});

            tosearch2 = comma + 1;

            comma = strchr(tosearch2, ',');
        }

        std::string uStr = std::string(tosearch2, lineEnd);

        int u;
        parseInt(uStr, &u);

        update.push_back(PageNumber{u});


        updates.push_back(update);
        
        tosearch = newline + 1;
        
        newline = strchr(tosearch, '\n');
    }

    LOGD("updates size: %zu", updates.size());

    // for (auto update : updates) {
    //     LOGD("update size: %zu", update.size());
    // }


    int total = 0;

    for (auto update : updates) {

        LOGD("update:");

        std::set<PageNumber> updateSet;


        // globalInOrder = true;

        for (auto pageNumber : update) {
            updateSet.insert(pageNumber);
        }


        // if (globalInOrder) {
        //     LOGD("update WAS in order");
        // } else {
        //     LOGD("update was NOT in order");
        // }


        std::vector<PageNumber> sortedUpdate;
        for (auto it = updateSet.begin(); it != updateSet.end(); ++it) {
            sortedUpdate.push_back(*it);
        }

        // LOGD("sorted update:");
        // for (auto pageNumber : sortedUpdate) {
        //     LOGD("%d", pageNumber.a);
        // }
        // LOGD();

        ASSERT(update.size() == sortedUpdate.size());

        ASSERT(update.size() % 2 == 1);

        bool inOrder = true;
        for (int i = 0; i < update.size(); i++) {
            auto a = update[i];
            auto b = sortedUpdate[i];
            if (a != b) {
                LOGD("out of order!");
                inOrder = false;
                break;
            }
        }

        if (!inOrder) {

            int middleIndex = (update.size() - 1) / 2;

            LOGD("middle of out-of-order update: %d", sortedUpdate[middleIndex].a);

            total += sortedUpdate[middleIndex].a;

        }

        LOGD();
    }


    LOGI("total: %d", total);


    return EXIT_SUCCESS;
}
















