#ifndef STYLE_H
#define STYLE_H

#include <string>

struct StyleConfig {
    std::string strokeColor;
    std::string backgroundColor;
    int strokeWidth;
};

class Style {
public:
    static void init(StyleConfig config);
    static void destroy();

    static inline const StyleConfig& get() { return instance->config; };
private: 
    Style(StyleConfig config);
    StyleConfig config;

    static Style *instance;
};

#endif