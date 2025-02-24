#include "methods.h"

#include <string>

void stc::string::replace (std::string& input, char replaceable, char target) {
    for (int i = 0; input[i]; ++i)
        if (input[i] == replaceable)
            input[i] = target;
}
