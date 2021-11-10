#include <iostream>
#include <vector>

#define WIDTH0 4
#define HEIGHT0 3

uint8_t image0[HEIGHT0][WIDTH0] = {
  {20, 16, 16, 18},
  {255, 255, 0, 0},
  {32, 32, 32, 32}
};

extern "C" uint16_t EightBitHistogram(uint16_t width, uint16_t height, uint8_t *p_image, uint16_t *p_histogram);

using namespace std;

int main() {
    uint16_t resultStatus;
    std::vector<uint16_t> histogram(256,170);
    resultStatus = EightBitHistogram(WIDTH0,HEIGHT0, (uint8_t*)&image0, (uint16_t*)&histogram);
    if (resultStatus == 0)
    {
      std::cout << "Programa falhou, estourou o valor limite de tamanho de 64k!! \n";
    } 
    else
    {
      std::cout << "Programa funcionou com sucesso! \n";
    } 
    return 0;
}