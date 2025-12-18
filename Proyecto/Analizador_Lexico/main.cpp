#include <iostream>
#include <fstream>
#include <string>
using namespace std;
// Verifica si es letra
bool esLetra(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}
// Verifica si es digito (0-9)
bool esDigito(char c) {
    return c >= '0' && c <= '9';
}
// Verifica si es numero entero o decimal
bool esNumero(const string &s) {
    bool punto = false;
    if (s.empty()) return false;
    for (char c : s) {
        if (!esDigito(c)) {
            if (c == '.' && !punto) punto = true;
            else return false;
        }
    }
    return true;
}
// Convierte una cadena a minúsculas
string toLower(const string &s) {
    string r = s;
    for (int i = 0; i < (int)r.size(); i++) {
        if (r[i] >= 'A' && r[i] <= 'Z') r[i] = r[i] + 32;
    }
    return r;
}
// Verifica si es espacio en blanco
bool esEspacio(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}
// Verifica si es válido para identificadores
bool esCaracterValidoId(char c) {
    return esLetra(c);
}
// Funcion para procesar tokens y escribir sus codigos
bool procesarToken(const string &tk, ofstream &outFile, int linea, bool &error) {
    if (tk.empty()) return true;
    string low = toLower(tk);
    // Palabras reservadas
    if (low == "select") { outFile << "300 "; return true; }
    if (low == "from")   { outFile << "301 "; return true; }
    if (low == "where")  { outFile << "302 "; return true; }
    if (low == "and")    { outFile << "303 "; return true; }
    if (low == "or")     { outFile << "304 "; return true; }
    if (low == "order")  { outFile << "305 "; return true; }
    if (low == "by")     { outFile << "306 "; return true; }
    if (low == "asc")    { outFile << "307 "; return true; }
    if (low == "desc")   { outFile << "308 "; return true; }
    if (low == "update") { outFile << "309 "; return true; }
    if (low == "set")    { outFile << "310 "; return true; }
    if (low == "delete") { outFile << "311 "; return true; }
    if (low == "insert") { outFile << "312 "; return true; }
    if (low == "into")   { outFile << "313 "; return true; }
    if (low == "values") { outFile << "314 "; return true; }
    // Simbolos
    if (tk == ";")  { outFile << "400 "; return true; }
    if (tk == "(")  { outFile << "401 "; return true; }
    if (tk == ")")  { outFile << "402 "; return true; }
    if (tk == "=")  { outFile << "403 "; return true; }
    if (tk == "<>") { outFile << "404 "; return true; }
    if (tk == "<")  { outFile << "405 "; return true; }
    if (tk == ">")  { outFile << "406 "; return true; }
    if (tk == "<=") { outFile << "407 "; return true; }
    if (tk == ">=") { outFile << "408 "; return true; }
    if (tk == "+")  { outFile << "409 "; return true; }
    if (tk == "-")  { outFile << "410 "; return true; }
    if (tk == "*")  { outFile << "411 "; return true; }
    if (tk == "/")  { outFile << "412 "; return true; }
    if (tk == ",")  { outFile << "413 "; return true; }
    if (tk == ".")  { outFile << "414 "; return true; }
    if (tk == "'")  { outFile << "415 "; return true; }
    // Cadena
    if (tk.size() >= 2 && tk.front() == '\'' && tk.back() == '\'') {
        outFile << "415 210 415 ";
        return true;
    }
    // Numero
    if (esNumero(tk)) {
        outFile << "200 ";
        return true;
    }
    // Identificadores
    if (esLetra(tk[0])) {
        bool valido = true;
        for (int i = 1; i < (int)tk.size(); i++) {
            if (!esCaracterValidoId(tk[i])) {
                valido = false;
                break;
            }
        }
        if (valido) {
            outFile << "210 ";
            return true;
        }
    }
    // Error léxico
    cout << "Error lexico en la linea " << linea << ": --> " << tk << " <--" << endl;
    error = true;
    outFile << "999 ";  // Código de error para token inválido
    return true;  // Continuar procesando
}
// Analizador lexico
void analizadorLexicoSQL(const string &archivoEntrada, const string &archivoSalida) {
    ifstream file(archivoEntrada);
    if (!file.is_open()) {
        cout << "Error: No se encontro el archivo " << archivoEntrada << endl;
        return;
    }
    ofstream outFile(archivoSalida);
    if (!outFile.is_open()) {
        cout << "No se pudo crear el archivo " << archivoSalida << endl;
        file.close();
        return;
    }
    string lineaStr;
    int linea = 0;
    bool error = false;
    int totalErrores = 0;
    while (getline(file, lineaStr)) {
        linea++;
        // Procesar caracter por caracter
        string palabra = "";
        bool enCadena = false;
        string cadenaAcum = "";
        bool comentarioLinea = false;

        for (int i = 0; i < (int)lineaStr.size(); i++) {
            char c = lineaStr[i];
            // Saltar espacios
            if (esEspacio(c) && !enCadena) {
                if (!palabra.empty()) {
                    if (!procesarToken(palabra, outFile, linea, error)) {
                        totalErrores++;
                    }
                    palabra = "";
                }
                continue;
            }
            // Comentario de linea
            if (c == '-' && i+1 < (int)lineaStr.size() && lineaStr[i+1] == '-' && !enCadena) {
                comentarioLinea = true;
                break;
            }
            // Si estamos en una cadena
            if (enCadena) {
                cadenaAcum += c;
                if (c == '\'') {
                    // Verificar si no es comilla escapada
                    if (i > 0 && lineaStr[i-1] != '\\') {
                        enCadena = false;
                        if (!procesarToken(cadenaAcum, outFile, linea, error)) {
                            totalErrores++;
                        }
                        cadenaAcum = "";
                    }
                }
                continue;
            }
            // Inicio de cadena
            if (c == '\'' && !enCadena) {
                if (!palabra.empty()) {
                    if (!procesarToken(palabra, outFile, linea, error)) {
                        totalErrores++;
                    }
                    palabra = "";
                }
                enCadena = true;
                cadenaAcum = "'";
                continue;
            }

            // Simbolos simples
            if ((c == ',' || c == ';' || c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/' || c == '.') && !enCadena) {
                if (!palabra.empty()) {
                    if (!procesarToken(palabra, outFile, linea, error)) {
                        totalErrores++;
                    }
                    palabra = "";
                }
                string sym(1, c);
                if (!procesarToken(sym, outFile, linea, error)) {
                    totalErrores++;
                }
                continue;
            }
            // Operadores de comparacion
            if ((c == '<' || c == '>' || c == '!') && i+1 < (int)lineaStr.size() && !enCadena) {
                char c2 = lineaStr[i+1];
                if ((c == '<' && c2 == '=') || (c == '>' && c2 == '=') ||
                    (c == '<' && c2 == '>') || (c == '!' && c2 == '=')) {
                    if (!palabra.empty()) {
                        if (!procesarToken(palabra, outFile, linea, error)) {
                            totalErrores++;
                        }
                        palabra = "";
                    }
                    string op = "";
                    op += c;
                    op += c2;
                    if (!procesarToken(op, outFile, linea, error)) {
                        totalErrores++;
                    }
                    i++;
                    continue;
                }
            }
            // Operadores < y > solos
            if ((c == '<' || c == '>') && !enCadena) {
                // Verificar que no sea parte de un operador compuesto
                bool esOperadorCompuesto = false;
                if (i+1 < (int)lineaStr.size()) {
                    char c2 = lineaStr[i+1];
                    if ((c == '<' && (c2 == '=' || c2 == '>')) ||
                        (c == '>' && c2 == '=')) {
                        esOperadorCompuesto = true;
                    }
                }
                if (!esOperadorCompuesto) {
                    if (!palabra.empty()) {
                        if (!procesarToken(palabra, outFile, linea, error)) {
                            totalErrores++;
                        }
                        palabra = "";
                    }
                    string sym(1, c);
                    if (!procesarToken(sym, outFile, linea, error)) {
                        totalErrores++;
                    }
                    continue;
                }
            }
            // Operador igual solo
            if (c == '=' && !enCadena) {
                if (!palabra.empty()) {
                    if (!procesarToken(palabra, outFile, linea, error)) {
                        totalErrores++;
                    }
                    palabra = "";
                }
                if (!procesarToken("=", outFile, linea, error)) {
                    totalErrores++;
                }
                continue;
            }
            // Acumular caracteres normales
            palabra += c;
        }
        if (!palabra.empty() && !comentarioLinea) {
            if (!procesarToken(palabra, outFile, linea, error)) {
                totalErrores++;
            }
        }
        // Verificar si quedo cadena sin cerrar
        if (enCadena) {
            cout << "Error lexico en la linea " << linea << ": Cadena sin cerrar --> " << cadenaAcum << " <--" << endl;
            error = true;
            totalErrores++;
            // Escribir token de error para la cadena incompleta
            outFile << "998 ";
        }
        outFile << endl;  // Salto de línea después de procesar cada línea del archivo
    }
    if (!error) {
        cout << "Analisis lexico completado \nTokens en: " << archivoSalida << endl;
    } else {
        cout << "Analisis lexico terminado con errores \n";
    }
    file.close();
    outFile.close();
}
int main() {
    string archivoEntrada = "C:\\Diseno de compiladores\\Proyecto\\leer.txt";
    string archivoSalida  = "C:\\Diseno de compiladores\\Proyecto\\tokens.txt";
    analizadorLexicoSQL(archivoEntrada, archivoSalida);
    return 0;
}
