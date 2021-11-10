  PUBLIC EightBitHistogram

  SECTION .text : CODE (2)
  THUMB

EightBitHistogram 

  PUSH {LR}
  MOV R6, #0 ;seta R6 como 0
  MOV R7, #0 ;seta R6 como 0
  LDR R4, [R3]; carrega da memória o endereço primeiro valor do vetor e salva em R4
  MUL R0,R1
  MOV R8, R0 ; armazena o tamanho da imagem no registrador R8
  CMP R0, #0x10000 ;compara se APSR(N) = 1, é menor que 64k
  IT MI ; Se flag N = 1 o tamanho é menor que 64k
  BLMI ResetaHistograma
  MOV R5, #0
  MOV R0, #0
  MOV R7, #0
  LDR R4, [R3]; carrega da memória o endereço primeiro valor do vetor e salva em R4
  B PreencheHistograma
  
  
  
  ;ITT PL ;Se flag N = 0
  ;MOVPL R0, #0
  ;BXPL LR  

                  
ResetaHistograma: ;loop para limpar o histograma
  
  MOV R5, #0; zera o valor do vetor
  STR R5, [R4], #2; armazena valor
  ADD R7, #1 ;soma um no registrador contador
  MOV R6, #4 ; seta em 4 bits para deslocar o vetor
  CMP R7, #256 ;compara o valor com o tamanho do vetor/2, pois cada valor do vetor ocupa half word
  IT NE ; se não for igual volta ao começo do loop
  ;ADDNE R4, #2 ; soma quatro para o proximo endereço
  BNE ResetaHistograma ;retorna o loop


  BX LR 
  
PreencheHistograma
  
  LDRB R5, [R2] ;carrega o valor do pixel em R5
  LDR R6, [R4, R5, LSL #1]    ;desloca o valor do endereço de memoria para onde está o vetor histogram 
  ADD R6, #1 ; Soma um
  STR R6, [R4, R5, LSL #1] ; armazena o valor dentro do endereço de memória R4 + R5
  ADD R2, #1 ;desloca um na memória
  ADD R0, #1 ;soma um no contador de pixels
  ADD R7, #1 ;itera contador
  CMP R7, R8 ;compara se já varreu toda a memória
  BNE PreencheHistograma ;reseta loop
  POP {LR}
  BX LR
  
  
  
  
  
  


  END