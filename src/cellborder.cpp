#include "cellborder.h"
#include <tuple>

LineBorder::LineBorder(double x1, double y1, double x2, double y2)
    : x1_(x1), y1_(y1), x2_(x2), y2_(y2) {}

LineBorder::LineBorder(std::tuple<double, double, double, double> xy) {
  std::tie(x1_, y1_, x2_, y2_) = xy;
}

std::string LineBorder::GnuplotPrintString() const {
  return "set arrow from " + std::to_string(x1_) + "," + std::to_string(y1_) +
         " to " + std::to_string(x2_) + "," + std::to_string(y2_) +
         " nohead lt -1 lw 2";
}

ArcBorder::ArcBorder(double cx, double cy, double r, double theta1,
                     double theta2)
    : cx_(cx), cy_(cy), r_(r), theta1_(theta1), theta2_(theta2) {}

std::string ArcBorder::GnuplotPrintString() const {
  return "set parametric; plot [" + std::to_string(theta1_) + ":" +
         std::to_string(theta2_) + "] " + std::to_string(cx_) + "+cos(t)*" +
         std::to_string(r_) + "," + std::to_string(cy_) + "+sin(t)*" +
         std::to_string(r_) + " w l lw 2 lt -1 notitle;unset parametric";
}
