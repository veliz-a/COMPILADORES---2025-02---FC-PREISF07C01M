#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>

vector<string> tokenizar(const string& texto) {
    vector<string> tokens;
    stringstream ss(texto);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

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
