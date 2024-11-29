#include <iostream>
#include <cstdint>
#include <string>
#include <print>
#include <tuple>
#include <vector>
#include <format>

#include <stb_image.h>

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    int width, height, channels;
    uint8_t* data = stbi_load(argv[1], &width, &height, &channels, 0);
    if (!data) {
        return 1;
    }
    std::println("{} {} {}", width, height, channels);
}
