#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>

#include <fstream>

using namespace std;
// Tokenizador simple basado en espacios y algunos caracteres especiales
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

// Parser basado en la gramática proporcionada
// ------------------------------------------------------------------
// Clase principal del parser. Aquí se implementan los métodos para
// cada tipo de gramática: condicionales, declaraciones y expresiones.
// ------------------------------------------------------------------
class Parser {
    const vector<string>& tokens;
    int pos;

public:
    Parser(const vector<string>& tokens) : tokens(tokens), pos(0) {}

    // Condicionales if-else
    bool parseIF() {
        int startPos = pos;
        
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
        
        // Verificar que se consumieron todos los tokens
        if (pos != (int)tokens.size()) {
            pos = startPos;
            return false;
        }

        return true;
    }

    // Condición: expr relop expr
    bool parseCOND() {
        if (!parseEXPR()) return false;
        if (!parseRELOP()) return false;
        if (!parseEXPR()) return false;
        return true;
    }

    // Operadores relacionales
    bool parseRELOP() {
        if (pos >= (int)tokens.size()) return false;
        string t = tokens[pos];
        if (t == "<" || t == ">" || t == "==" || t == "!=") {
            pos++;
            return true;
        }
        return false;
    }

    // Expresiones aritméticas (suma/resta)
    bool parseEXPR() {
        // expr -> term {(+|-) term}
        if (!parseTerm()) return false; // primer término
        while (pos < (int)tokens.size() && (tokens[pos] == "+" || tokens[pos] == "-")) {
            pos++; // consumir operador
            if (!parseTerm()) return false; // siguiente término
        }
        return true;
    }

    // Termino aritmético (multiplicación/división)
    bool parseTerm() {
        // term -> factor {(*|/) factor}
        if (!parseFactor()) return false; // primer factor
        while (pos < (int)tokens.size() && (tokens[pos] == "*" || tokens[pos] == "/")) {
            pos++; // consumir operador
            if (!parseFactor()) return false; // siguiente factor
        }
        return true;
    }

    // Factor: número, identificador o expresión entre paréntesis
    bool parseFactor() {
        if (pos >= (int)tokens.size()) return false;
        if (tokens[pos] == "(") { // paréntesis
            pos++;
            if (!parseEXPR()) return false;
            if (pos >= (int)tokens.size() || tokens[pos] != ")") return false;
            pos++;
            return true;
        }
        if (esID(tokens[pos]) || esValor(tokens[pos])) { // número o identificador
            pos++;
            return true;
        }
        return false;
    }

    // --------------------------------------------------------------
    // PARSER PARA INSTRUCCIONES (declaraciones)
    // Analiza instrucciones: una o más declaraciones o asignaciones
    bool parseINSTRUCCIONES() {
        // Aceptamos 0 o más instrucciones (declaraciones o asignaciones)
        while (pos < (int)tokens.size() && tokens[pos] != "}") {
            int startPos = pos;
            
            if (!parseDECL() && !parseASIGNACION()) {
                pos = startPos;
                break;
            }
        }
        return true;
    }

    // Nueva función para manejar asignaciones: ID = EXPR ;
    bool parseASIGNACION() {
        int startPos = pos;
        
        if (!parseID()) {
            pos = startPos;
            return false;
        }
        if (!match("=")) {
            pos = startPos;
            return false;
        }
        if (!parseEXPR()) {
            pos = startPos;
            return false;
        }
        if (!match(";")) {
            pos = startPos;
            return false;
        }
        
        return true;
    }

    // --------------------------------------------------------------
    // PARSER PARA DECLARACIONES DE VARIABLES
    // --------------------------------------------------------------
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

    // --------------------------------------------------------------
    // PARSER PARA TIPOS DE VARIABLES (int, float, char)
    // --------------------------------------------------------------
    bool parseTIPO() {
        if (pos >= (int)tokens.size()) return false;
        string t = tokens[pos];
        if (t == "int" || t == "float" || t == "char") {
            pos++;
            return true;
        }
        return false;
    }

    // --------------------------------------------------------------
    // PARSER PARA IDENTIFICADORES
    // --------------------------------------------------------------
    bool parseID() {
        if (pos >= (int)tokens.size()) return false;
        if (esID(tokens[pos])) {
            pos++;
            return true;
        }
        return false;
    }

    // --------------------------------------------------------------
    // PARSER PARA VALORES (numéricos o char)
    // --------------------------------------------------------------
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

    bool esID(const string& s) {
        if (s.empty()) return false;
        if (!isalpha(s[0])) return false;
        for (char c : s) {
            if (!isalnum(c)) return false;
        }
        return true;
    }

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

// Función para extraer una porción de tokens desde una posición
vector<string> extraerTokens(const vector<string>& tokens, int inicio, int fin) {
    vector<string> resultado;
    for (int i = inicio; i < fin && i < (int)tokens.size(); i++) {
        resultado.push_back(tokens[i]);
    }
    return resultado;
}

int main() {
    // Leer y tokenizar archivo.txt una sola vez
    std::ifstream archivo("archivo.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir archivo.txt" << endl;
        return 1;
    }
    std::string codigo, linea;
    while (getline(archivo, linea)) {
        codigo += linea + " ";
    }
    archivo.close();

    vector<string> tokens = tokenizar(codigo);

    // Mostrar tokens para debug
    cout << "Tokens encontrados: ";
    for (const auto& token : tokens) {
        cout << "'" << token << "' ";
    }
    cout << endl << endl;

    // Separar en bloques de código
    vector<pair<int, int>> bloques; // (inicio, fin) de cada bloque
    int inicio = 0;
    for (int i = 0; i < (int)tokens.size(); i++) {
        if (tokens[i] == ";") {
            bloques.push_back({inicio, i + 1});
            inicio = i + 1;
        }
        else if (tokens[i] == "if") {
            int nivelLlaves = 0;
            int j = i;
            bool encontroElse = false;
            while (j < (int)tokens.size()) {
                if (tokens[j] == "{") nivelLlaves++;
                if (tokens[j] == "}") {
                    nivelLlaves--;
                    if (nivelLlaves == 0 && encontroElse) {
                        bloques.push_back({inicio, j + 1});
                        inicio = j + 1;
                        i = j;
                        break;
                    }
                }
                if (tokens[j] == "else" && nivelLlaves == 0) {
                    encontroElse = true;
                }
                j++;
            }
        }
    }
    if (inicio < (int)tokens.size()) {
        bloques.push_back({inicio, (int)tokens.size()});
    }

    // Analizar cada bloque por separado
    for (int b = 0; b < (int)bloques.size(); b++) {
        vector<string> tokensBloque = extraerTokens(tokens, bloques[b].first, bloques[b].second);
        cout << "Analizando bloque " << (b + 1) << ": ";
        for (const auto& token : tokensBloque) {
            cout << token << " ";
        }
        cout << endl;

        // Test condicionales IF
        Parser parser1(tokensBloque);
        if (parser1.parseIF()) {
            cout << "  -> Cumple con la gramática IF completa" << endl;
            // Analizar instrucciones dentro de llaves
            for (size_t i = 0; i < tokensBloque.size(); ++i) {
                if (tokensBloque[i] == "{") {
                    size_t j = i + 1;
                    while (j < tokensBloque.size() && tokensBloque[j] != "}") {
                        size_t start = j;
                        while (j < tokensBloque.size() && tokensBloque[j] != ";" && tokensBloque[j] != "}") {
                            ++j;
                        }
                        if (j > start) {
                            vector<string> instr(tokensBloque.begin() + start, tokensBloque.begin() + j + 1);
                            Parser p(instr);
                            if (p.parseDECL()) {
                                cout << "    -> Declaración válida: ";
                                for (const auto& t : instr) cout << t << " ";
                                cout << endl;
                            } else if (p.parseASIGNACION()) {
                                cout << "    -> Asignación válida: ";
                                for (const auto& t : instr) cout << t << " ";
                                cout << endl;
                            } else {
                                cout << "    -> Instrucción no reconocida: ";
                                for (const auto& t : instr) cout << t << " ";
                                cout << endl;
                            }
                        }
                        ++j;
                    }
                }
            }
            continue;
        } else {
            // Si el bloque parece un IF pero no es válido, analizar instrucciones internas
            bool tieneLlaves = false;
            for (size_t i = 0; i < tokensBloque.size(); ++i) {
                if (tokensBloque[i] == "{") {
                    tieneLlaves = true;
                    size_t j = i + 1;
                    while (j < tokensBloque.size() && tokensBloque[j] != "}") {
                        size_t start = j;
                        while (j < tokensBloque.size() && tokensBloque[j] != ";" && tokensBloque[j] != "}") {
                            ++j;
                        }
                        if (j > start) {
                            vector<string> instr(tokensBloque.begin() + start, tokensBloque.begin() + j + 1);
                            Parser p(instr);
                            if (p.parseDECL()) {
                                cout << "    -> Declaración válida: ";
                                for (const auto& t : instr) cout << t << " ";
                                cout << endl;
                            } else if (p.parseASIGNACION()) {
                                cout << "    -> Asignación válida: ";
                                for (const auto& t : instr) cout << t << " ";
                                cout << endl;
                            } else {
                                cout << "    -> Instrucción no reconocida: ";
                                for (const auto& t : instr) cout << t << " ";
                                cout << endl;
                            }
                        }
                        ++j;
                    }
                }
            }
            if (tieneLlaves) {
                cout << "  -> IF/ELSE inválido, pero se analizaron instrucciones internas." << endl;
            }
        }
        // Test condiciones
        Parser parser2(tokensBloque);
        if (parser2.parseCOND()) {
            cout << "  -> Cumple con la gramática COND" << endl;
            continue;
        }
        // Test declaraciones
        Parser parser3(tokensBloque);
        if (parser3.parseDECL()) {
            cout << "  -> Cumple con la gramática DECL" << endl;
            continue;
        }
        // Test asignaciones
        Parser parser4(tokensBloque);
        if (parser4.parseASIGNACION()) {
            cout << "  -> Cumple con la gramática ASIGNACION" << endl;
            continue;
        }
        cout << "  -> No cumple con ninguna gramática reconocida" << endl;
    }
    return 0;
}