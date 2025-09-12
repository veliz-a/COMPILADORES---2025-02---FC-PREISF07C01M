#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::ifstream archivo("archivo.txt"); 
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    char c;
    std::string palabra = "";

    while (archivo.get(c)) {
        if (c == '\n' || c == '\r') {
            continue; // Ignorar saltos de línea
        }

        if (c == ' ') {
            if (!palabra.empty()) {
                std::cout << "Palabra: " << palabra << std::endl;
                palabra.clear(); // limpia la cadena
            }
        } else {
            palabra += c; // construir la palabra
        }
    }

    // mostrar la última palabra
    if (!palabra.empty()) {
        std::cout << "Palabra: " << palabra << std::endl;
    }

    archivo.close();
    return 0;
}
