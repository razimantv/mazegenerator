#include "style.h"

#include <iostream>

Style *Style::instance = nullptr;

Style::Style(StyleConfig config) : config(config) {}

void Style::init(StyleConfig config) {
    if (!instance) instance = new Style(config);
}

void Style::destroy() {
    if (instance) delete instance;
}