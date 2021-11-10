#include <iostream>
#include <vector>
#include "images.c"


#define WIDTH0 4
#define HEIGHT0 3

/*uint8_t image0[HEIGHT0][WIDTH0] = {
  {20, 16, 16, 18},
  {255, 255, 0, 0},
  {32, 32, 32, 32}
};*/
// 0 = 2
// 16 = 2
// 18 = 1
// 20 = 1
// 32 = 4
// 255 = 2
extern "C" uint16_t EightBitHistogram(uint16_t width, uint16_t height, uint8_t *p_image, uint16_t *p_histogram);

using namespace std;

int main() {
    uint16_t resultStatus;
    std::vector<uint16_t> histogram(256,170);
    int i = 0;
    resultStatus = EightBitHistogram(WIDTH1,HEIGTH1, (uint8_t*)&image1, (uint16_t*)&histogram);
    if (resultStatus == 0)
    {
      std::cout << "Programa falhou, estourou o valor limite de tamanho de 64k!! \n";
    } 
    else
    {
      std::cout << "Sucesso! Histograma calculado e imagem dentro do tamanho permitido!" << endl;
      for (i = 0; i < 255; i++)
      {
        printf("Valor na posição %d = %d\n", i, histogram[i]);
      }
    }
    printf ("O valor de pixels contados foi: %d\n", resultStatus);
    return 0;
    
}