  PUBLIC EightBitHistogram

  SECTION .text : CODE (2)
  THUMB

EightBitHistogram 

  PUSH {LR}
  MOV R6, #0 ;seta R6 como 0
  MOV R7, #0 ;seta R6 como 0
  LDR R4, [R3]; carrega da memória o endereço primeiro valor do vetor e salva em R4
  MUL R0,R1 
  CMP R0, #0x10000 ;compara se APSR(N) = 1, é menor que 64k
  ITT MI ; Se flag N = 1 o tamanho é menor que 64k
  BLMI ResetaHistograma
  MOVMI R0, #22 ; essa linha deverá retornar o valor de pixels processados
  POP {LR}
  
  
  ;ITT PL ;Se flag N = 0
  ;MOVPL R0, #0
  ;BXPL LR  
  BX LR
                  
ResetaHistograma: ;loop para limpar o histograma
  
  
  LDR R5, [R4] ;carrega da memória o valor do primeiro vetor e desloca o valor de R6  
  MOV R5, #0; zera o valor do vetor
  STR R5, [R4]; armazena valor
  ADD R7, #1 ;soma um no registrador contador
  MOV R6, #4 ; seta em 4 bits para deslocar o vetor
  CMP R7, #128 ;compara o valor com o tamanho do vetor/2, pois cada valor do vetor ocupa half word
  ITT NE ; se não for igual volta ao começo do loop
  ADDNE R4, #4 ; soma quatro para o proximo endereço
  BNE ResetaHistograma ;retorna o loop

  BX LR 
  
 
  
  
  
  


  END