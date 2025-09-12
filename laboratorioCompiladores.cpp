#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>

#include <fstream>

using namespace std;

// ---------------------------------------------
// Gramática de expresiones aritméticas (1/3)
// ---------------------------------------------
// La función 'tokenizar' identifica y separa tokens, incluyendo operadores aritméticos y relacionales.
// Esta función es utilizada por todos los módulos, pero es esencial para el análisis de expresiones.
vector<string> tokenizar(const string& texto) {
    vector<string> tokens;
    string token;
    string delimiters = " (){}=;<>!";

    for (size_t i = 0; i < texto.size();) {
        if (isspace(texto[i])) {
            i++;
            continue;
        }

        // Detectar operadores relop de 2 caracteres (==, !=)
        if (i + 1 < texto.size()) {
            string twoChars = texto.substr(i, 2);
            if (twoChars == "==" || twoChars == "!=") {
                tokens.push_back(twoChars);
                i += 2;
                continue;
            }
        }

        // Si es delimitador de 1 char
        if (delimiters.find(texto[i]) != string::npos) {
            tokens.push_back(string(1, texto[i]));
            i++;
            continue;
        }

        // Tokenizar palabra o número
        size_t j = i;
        while (j < texto.size() && delimiters.find(texto[j]) == string::npos && !isspace(texto[j]))
            j++;

        token = texto.substr(i, j - i);
        tokens.push_back(token);
        i = j;
    }

    return tokens;
}

    // ---------------------------------------------
    // Gramática de condicionales (2/3)
    // ---------------------------------------------
    // La clase 'Parser' implementa el análisis de condicionales tipo IF-ELSE.
    // El método 'parseIF' verifica la estructura de un condicional completo.
class Parser {
    const vector<string>& tokens;
    int pos;

public:
    Parser(const vector<string>& tokens) : tokens(tokens), pos(0) {}

        // Analiza la gramática de condicionales: if (COND) {INSTRUCCIONES} else {INSTRUCCIONES}
    bool parseIF() {
        if (!match("if")) return false;
        if (!match("(")) return false;
        if (!parseCOND()) return false;
        if (!match(")")) return false;
        if (!match("{")) return false;
        if (!parseINSTRUCCIONES()) return false;
        if (!match("}")) return false;
        if (!match("else")) return false;
        if (!match("{")) return false;
        if (!parseINSTRUCCIONES()) return false;
        if (!match("}")) return false;
        return true;
    }

        // Analiza la condición dentro del IF: EXPR RELOP EXPR
    bool parseCOND() {
        if (!parseEXPR()) return false;
        if (!parseRELOP()) return false;
        if (!parseEXPR()) return false;
        return true;
    }

        // Analiza operadores relacionales: <, >, ==, !=
    bool parseRELOP() {
        if (pos >= (int)tokens.size()) return false;
        string t = tokens[pos];
        if (t == "<" || t == ">" || t == "==" || t == "!=") {
            pos++;
            return true;
        }
        return false;
    }

        // ---------------------------------------------
        // Gramática de expresiones aritméticas (1/3)
        // ---------------------------------------------
        // Analiza expresiones: identificadores o valores numéricos
    bool parseEXPR() {
        if (pos >= (int)tokens.size()) return false;
        // Simplificación: expr es ID o valor numérico literal
        if (esID(tokens[pos]) || esValor(tokens[pos])) {
            pos++;
            return true;
        }
        return false;
    }

        // ---------------------------------------------
        // Gramática de declaraciones (3/3)
        // ---------------------------------------------
        // Analiza instrucciones: una o más declaraciones
    bool parseINSTRUCCIONES() {
        // Simplificamos: una o más DECLARACIONES
        while (parseDECL()) {
            // parseDECL avanza pos si tiene éxito
        }
        return true; // aceptamos 0 o más declaraciones
    }

        // Analiza una declaración: tipo, identificador, valor opcional, punto y coma
    bool parseDECL() {
        int startPos = pos;
        if (!parseTIPO()) return false;
        if (!parseID()) return false;

        if (match("=")) {
            if (!parseVALOR()) return false;
        }

        if (!match(";")) {
            pos = startPos; // rollback
            return false;
        }

        return true;
    }

        // Analiza el tipo de dato: int, float, char
    bool parseTIPO() {
        if (pos >= (int)tokens.size()) return false;
        string t = tokens[pos];
        if (t == "int" || t == "float" || t == "char") {
            pos++;
            return true;
        }
        return false;
    }

        // Analiza identificadores
    bool parseID() {
        if (pos >= (int)tokens.size()) return false;
        if (esID(tokens[pos])) {
            pos++;
            return true;
        }
        return false;
    }

        // Analiza valores: numéricos o char
    bool parseVALOR() {
        if (pos >= (int)tokens.size()) return false;
        if (esValor(tokens[pos])) {
            pos++;
            return true;
        }
        return false;
    }

private:
    bool match(const string& expected) {
        if (pos < (int)tokens.size() && tokens[pos] == expected) {
            pos++;
            return true;
        }
        return false;
    }

        // Verifica si el string es un identificador válido
    bool esID(const string& s) {
        if (s.empty()) return false;
        if (!isalpha(s[0])) return false;
        for (char c : s) {
            if (!isalnum(c)) return false;
        }
        return true;
    }

        // Verifica si el string es un valor válido (numérico o char)
    bool esValor(const string& s) {
        // Para simplificar consideramos valor como:
        // un número entero, un número con punto decimal o un char entre comillas
        // numérico
        bool punto = false;
        int start = 0;
        if (s.empty()) return false;

        if (s[0] == '\'') { // char literal simple 'a'
            return s.size() == 3 && s[2] == '\'';
        }

        for (size_t i = 0; i < s.size(); i++) {
            if (s[i] == '.') {
                if (punto) return false;
                punto = true;
            } else if (!isdigit(s[i])) {
                return false;
            }
        }
        return true;
    }
};

int main() {
    // Leer el contenido de archivo.txt
    std::ifstream archivo("archivo.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir archivo.txt" << endl;
        return 1;
    }
    string codigo, linea;
    while (getline(archivo, linea)) {
        codigo += linea + " ";
    }
    archivo.close();

    vector<string> tokens = tokenizar(codigo);

    Parser parser(tokens);

    // Aquí se prueba la gramática de condicionales IF
    if (parser.parseIF()) {
        cout << "El código es válido según la gramática IF." << endl;
    } else {
        cout << "Error: código inválido." << endl;
    }

    return 0;
}
