/*  
    - Curso : Compiladores - Laboratorio
    - Alumno: Alexander Arturo Baylon Ibanez

    Compilar y correr:
    $ g++ -std=c++11 -o main main.cpp helper.cpp scanner.cpp
    $ ./main "nombre del archivo de texto"
*/
#include <iostream>
#include "scanner.h"
#include "parser.h"

using namespace std;

int main(int argc, char *argv[])
{
    char * fileName = argv[1];
    FILE * fd = fopen(fileName, "r");
    cout << "programa de entrada: " << fileName << endl;

    // if (fd != NULL)
    // {
    //     fseek(fd, 0, SEEK_END);
    //     int len = ftell(fd);
    //     rewind(fd);
    //     char * buffer = (char *) malloc(len);

    //     // read program text
    //     int size = fread(buffer, 1, len, fd);
    //     string inputString(buffer);
    //     Scanner s(inputString);
    //     s.begin_scan();
    // }

	return 0;
}