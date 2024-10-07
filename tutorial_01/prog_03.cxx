#include <iostream>
#include <format>
#include <numbers>
#include <cmath>
#define fn  auto
#define let auto

fn main() -> int
{
    let const r = double{5.6};
    let const volume = (4.0 / 3.0) * std::numbers::pi * std::pow(r, 3.0);
    std::cout << std::format(
        "Given `r = {}`, the volume of the sphere `volume = {}`\n", 
        r, volume
    );
}