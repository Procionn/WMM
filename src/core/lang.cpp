#include "../core.h"

Lang::Lang () {
    update_lang();
}


void Lang::update_lang () {
    if (!lang.empty())
        lang.clear();
    std::ifstream readedFile (CConfigs::CONFIG_LANGUAGES);
    std::string parameter;
    std::string indicator;
    while (configRead(readedFile, parameter, indicator))
        lang[parameter] = indicator;
}
