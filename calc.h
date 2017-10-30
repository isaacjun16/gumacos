#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "string.h"

int32_t calc_int_value(char ch) {
  switch (ch) {
    case '0':
      return 0;
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    default:
      return -1;
  }
}

char* calc(char* parameters) {
  char* subcommand;
  char* numbers;
  int space_index = string_index_of_char(parameters, ' ');
  int len = strlen(parameters);

  int hasParametersNumbers = 0;
  if(space_index >= 0){
    subcommand = substring(parameters, 0, space_index);
    numbers = substring(parameters, space_index + 1, len);
    hasParametersNumbers++;
  } else {
    subcommand = parameters;
    numbers = 0;
  }

  if(hasParametersNumbers == 1) {
    char* number1;
    char* number2;

    space_index = string_index_of_char(numbers, ' ');
    len = strlen(numbers);

    int hasNumbers = 0;
    if(space_index >= 0){
      number1 = substring(numbers, 0, space_index);
      number2 = substring(numbers, space_index + 1, len);
      hasNumbers++;
    } else  {
      number1 = 0;
      number2 = 0;
    }

    if(hasNumbers == 1) {
      int32_t numero1 = 0;
      int32_t numero2 = 0;
      int32_t decimal = 1;
      int index = 0;
      len = strlen(number1) - 1;
      for(int i = len; i >= 0; i--) {
        if(index > 0) decimal = decimal * 10;
        if(i == 0 && number1[i] == '-') {
          numero1 = numero1 * -1;
          break;
        }
        int32_t temp = calc_int_value(number1[i]);
        if(temp < 0) return "el primero numero dado no es un entero valido.\n";

        numero1 += decimal * temp;

        index++;
      }

      decimal = 1;
      index = 0;
      len = strlen(number2) - 1;
      for(int i = len; i >= 0; i--) {
        if(index > 0) decimal = decimal * 10;

        if(i == 0 && number2[i] == '-') {
          numero2 = numero2 * -1;
          break;
        }

        int32_t temp = calc_int_value(number2[i]);
        if(temp < 0) return "el segundo numero dado no es un entero valido.\n";

        numero2 += decimal * temp;

        index++;
      }

      char* resultado;
      int32_t total = 0;
      int32_t residuo = 0;
      if(strcmp("sumar", subcommand)) {
        resultado = "La suma de ";
        total = numero1 + numero2;
      } else if(strcmp("restar", subcommand)) {
        resultado = "La resta de ";
        total = numero1 - numero2;
      } else if(strcmp("multiplicar", subcommand)) {
        resultado = "La multiplicacion de ";
        total = numero1 * numero2;
      } else if(strcmp("dividir", subcommand)) {
        resultado = "la divicion de ";
        total = numero1 / numero2;
        residuo = numero1 % numero2;
      } else {
        resultado = "El parametro ";
        resultado = concat(resultado, subcommand);
        resultado = concat(resultado, " no es valido.\n");
        return resultado;
      }

      resultado = concat(resultado, number1);
      resultado = concat(resultado, " y ");
      resultado = concat(resultado, number2);
      resultado = concat(resultado, " es: ");
      resultado = concat(resultado, int_to_string(total));
      if(residuo != 0) {
        resultado = concat(resultado, " con un residuo de: ");
        resultado = concat(resultado, int_to_string(residuo));
        resultado = concat(resultado, "\n");
      } else {
        resultado = concat(resultado, "\n");
      }
      return resultado;
    } else {
      //return numbers;
      return "Es necesario que pase 2 numeros como parametros, escriba 'ayuda calc' para mas informacion.\n";
    }
  } else {
    //return parameters;
    return "Es necesario que pase numeros como parametros, escriba 'ayuda calc' para mas informacion.\n";
  }
}
