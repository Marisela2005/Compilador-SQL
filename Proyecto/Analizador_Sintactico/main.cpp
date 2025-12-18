#include <iostream>
#include <fstream>
#include <string>
using namespace std;
// Variables globales
int pos = 0;
int linea = 0;
bool error_encontrado = false;
int tokens[1000];
int num_tokens = 0;
// Funcion para mostrar error
void mostrar_error(int codigo_esperado) {
    cout << "\nError sintactico en la linea " << linea << " se esperaba ---> ";
    if(codigo_esperado == 300) cout << "SELECT";
    else if(codigo_esperado == 301) cout << "FROM";
    else if(codigo_esperado == 302) cout << "WHERE";
    else if(codigo_esperado == 303) cout << "AND";
    else if(codigo_esperado == 304) cout << "OR";
    else if(codigo_esperado == 305) cout << "ORDER";
    else if(codigo_esperado == 306) cout << "BY";
    else if(codigo_esperado == 307) cout << "ASC";
    else if(codigo_esperado == 308) cout << "DESC";
    else if(codigo_esperado == 309) cout << "UPDATE";
    else if(codigo_esperado == 310) cout << "SET";
    else if(codigo_esperado == 311) cout << "DELETE";
    else if(codigo_esperado == 312) cout << "INSERT";
    else if(codigo_esperado == 313) cout << "INTO";
    else if(codigo_esperado == 314) cout << "VALUES";
    else if(codigo_esperado == 400) cout << ";";
    else if(codigo_esperado == 401) cout << "(";
    else if(codigo_esperado == 402) cout << ")";
    else if(codigo_esperado == 403) cout << "=";
    else if(codigo_esperado == 404) cout << "<>";
    else if(codigo_esperado == 405) cout << "<";
    else if(codigo_esperado == 406) cout << ">";
    else if(codigo_esperado == 407) cout << "<=";
    else if(codigo_esperado == 408) cout << ">=";
    else if(codigo_esperado == 409) cout << "+";
    else if(codigo_esperado == 410) cout << "-";
    else if(codigo_esperado == 411) cout << "*";
    else if(codigo_esperado == 412) cout << "/";
    else if(codigo_esperado == 413) cout << ",";
    else if(codigo_esperado == 414) cout << ".";
    else if(codigo_esperado == 415) cout << "'";
    else if(codigo_esperado == 200) cout << "un numero";
    else if(codigo_esperado == 210) cout << "identificador";
    else cout << "token " << codigo_esperado;
    cout << " <--- antes de ";
    if(pos < num_tokens) {
        int token_actual = tokens[pos];
        if(token_actual == 300) cout << "SELECT";
        else if(token_actual == 301) cout << "FROM";
        else if(token_actual == 302) cout << "WHERE";
        else if(token_actual == 303) cout << "AND";
        else if(token_actual == 304) cout << "OR";
        else if(token_actual == 305) cout << "ORDER";
        else if(token_actual == 306) cout << "BY";
        else if(token_actual == 307) cout << "ASC";
        else if(token_actual == 308) cout << "DESC";
        else if(token_actual == 309) cout << "UPDATE";
        else if(token_actual == 310) cout << "SET";
        else if(token_actual == 311) cout << "DELETE";
        else if(token_actual == 312) cout << "INSERT";
        else if(token_actual == 313) cout << "INTO";
        else if(token_actual == 314) cout << "VALUES";
        else if(token_actual == 400) cout << ";";
        else if(token_actual == 401) cout << "(";
        else if(token_actual == 402) cout << ")";
        else if(token_actual == 403) cout << "=";
        else if(token_actual == 404) cout << "<>";
        else if(token_actual == 405) cout << "<";
        else if(token_actual == 406) cout << ">";
        else if(token_actual == 407) cout << "<=";
        else if(token_actual == 408) cout << ">=";
        else if(token_actual == 409) cout << "+";
        else if(token_actual == 410) cout << "-";
        else if(token_actual == 411) cout << "*";
        else if(token_actual == 412) cout << "/";
        else if(token_actual == 413) cout << ",";
        else if(token_actual == 414) cout << ".";
        else if(token_actual == 415) cout << "'";
        else if(token_actual == 200) cout << "un numero";
        else if(token_actual == 210) cout << "identificador";
        else if(token_actual == -1) cout << "salto de linea";
        else cout << token_actual;
    } else {
    }
    error_encontrado = true;
}
// Verificar token
bool verificar(int codigo_esperado) {
    // Guardar posición y línea actual
    int pos_antes = pos;
    int linea_antes = linea;
    while(pos < num_tokens && tokens[pos] == -1) {
        linea++;
        pos++;
    }
    if(pos >= num_tokens) {
        pos = pos_antes;
        linea = linea_antes;
        mostrar_error(codigo_esperado);
        return false;
    }
    if(tokens[pos] == codigo_esperado) {
        pos++;
        return true;
    } else {
        pos = pos_antes;
        linea = linea_antes;
        mostrar_error(codigo_esperado);
        return false;
    }
}
// Token actual
int token_actual() {
    int temp_pos = pos;
    while(temp_pos < num_tokens && tokens[temp_pos] == -1) {
        temp_pos++;
    }
    if(temp_pos < num_tokens) {
        return tokens[temp_pos];
    }
    return -1;
}
// Analizar cadena (415 210 415)
bool analizar_cadena() {
    if(!verificar(415)) return false;
    if(!verificar(210)) return false;
    if(!verificar(415)) return false;
    return true;
}
// Analizar numero decimal (200 414 200) o entero (200)
bool analizar_numero() {
    if(!verificar(200)) return false;

    if(token_actual() == 414) {
        verificar(414);
        if(!verificar(200)) return false;
    }
    return true;
}
// Analizar valor (para condiciones de SELECT, UPDATE, DELETE)
bool analizar_valor() {
    if(token_actual() == 200) {
        return analizar_numero();
    }
    else if(token_actual() == 210) {
        return verificar(210);
    }
    else if(token_actual() == 415) {
        return analizar_cadena();
    }
    else {
        cout << "\nError sintactico en la linea " << linea << " se esperaba valor";
        error_encontrado = true;
        return false;
    }
}
// Analizar valor para INSERT (solo cadena o número)
bool analizar_valor_insert() {
    if(token_actual() == 415) {
        return analizar_cadena();
    }
    else if(token_actual() == 200) {
        return analizar_numero();
    }
    else {
        cout << "\nError sintactico en la linea " << linea << " se esperaba cadena o numero en VALUES";
        error_encontrado = true;
        return false;
    }
}
// Analizar expresion para UPDATE (identificador operador número)
bool analizar_expresion_update() {
    int pos_original = pos;
    int linea_original = linea;
    if(token_actual() == 210) {
        verificar(210);
        if(token_actual() == 409 || token_actual() == 410 ||
           token_actual() == 411 || token_actual() == 412) {
            verificar(token_actual());
            if(!analizar_numero()) {
                pos = pos_original;
                linea = linea_original;
                return analizar_valor();
            }
            return true;
        } else {
            return true;
        }
    }
    else if(token_actual() == 200) {
        return analizar_numero();
    }
    else if(token_actual() == 415) {
        return analizar_cadena();
    }
    return false;
}
// Analizar condiciones (para SELECT, UPDATE, DELETE)
bool condiciones() {
    if(!verificar(210)) return false;

    if(token_actual() != 403 && token_actual() != 404 &&
       token_actual() != 405 && token_actual() != 406 &&
       token_actual() != 407 && token_actual() != 408) {
        mostrar_error(403);
        return false;
    }
    verificar(token_actual());
    if(!analizar_valor()) return false;
    while(token_actual() == 303 || token_actual() == 304) {
        verificar(token_actual());
        if(!verificar(210)) return false;
        if(token_actual() != 403 && token_actual() != 404 &&
           token_actual() != 405 && token_actual() != 406 &&
           token_actual() != 407 && token_actual() != 408) {
            mostrar_error(403);
            return false;
        }
        verificar(token_actual());
        if(!analizar_valor()) return false;
    }
    return true;
}
// Analizar columnas (para SELECT)
bool columnas() {
    if(token_actual() == 411) {
        verificar(411);
        return true;
    }
    else if(token_actual() == 210) {
        verificar(210);
        while(token_actual() == 413) {
            verificar(413);
            if(!verificar(210)) return false;
        }
        return true;
    }
    mostrar_error(210);
    return false;
}
// Analizar ORDER BY (para SELECT)
bool order_by() {
    if(!verificar(305)) return false;
    if(!verificar(306)) return false;
    if(!verificar(210)) return false;
    if(token_actual() == 307 || token_actual() == 308) {
        verificar(token_actual());
    }
    return true;
}
// Analizar sentencia SELECT
bool select() {
    if(!verificar(300)) return false;
    if(!columnas()) return false;
    if(!verificar(301)) return false;
    if(!verificar(210)) return false;
    if(token_actual() == 302) {
        verificar(302);
        if(!condiciones()) return false;
    }
    if(token_actual() == 305) {
        if(!order_by()) return false;
    }
    if(!verificar(400)) return false;
    return true;
}
// Analizar asignaciones (para UPDATE)
bool asignaciones_update() {
    if(!verificar(210)) return false;
    if(!verificar(403)) return false;

    if(!analizar_expresion_update()) return false;

    while(token_actual() == 413) {
        verificar(413);
        if(!verificar(210)) return false;
        if(!verificar(403)) return false;
        if(!analizar_expresion_update()) return false;
    }

    return true;
}
// Analizar sentencia UPDATE
bool update() {
    if(!verificar(309)) return false;
    if(!verificar(210)) return false;
    if(!verificar(310)) return false;
    if(!asignaciones_update()) return false;
    if(!verificar(302)) return false;
    if(!condiciones()) return false;
    if(!verificar(400)) return false;
    return true;
}
// Analizar sentencia DELETE
bool delete_sql() {
    if(!verificar(311)) return false;
    if(!verificar(301)) return false;
    if(!verificar(210)) return false;
    if(!verificar(302)) return false;
    if(!condiciones()) return false;
    if(!verificar(400)) return false;
    return true;
}
// Analizar lista de campos para INSERT
bool lista_campos_insert() {
    if(!verificar(210)) return false;

    while(token_actual() == 413) {
        verificar(413);
        if(!verificar(210)) return false;
    }
    return true;
}
// Analizar lista de valores para INSERT
bool lista_valores_insert() {
    if(!analizar_valor_insert()) return false;

    while(token_actual() == 413) {
        verificar(413);
        if(!analizar_valor_insert()) return false;
    }
    return true;
}
// Analizar sentencia INSERT
bool insert_into() {
    if(!verificar(312)) return false;
    if(!verificar(313)) return false;
    if(!verificar(210)) return false;
    if(!verificar(401)) return false;
    if(!lista_campos_insert()) return false;
    if(!verificar(402)) return false;
    if(!verificar(314)) return false;
    if(!verificar(401)) return false;
    if(!lista_valores_insert()) return false;
    if(!verificar(402)) return false;
    if(!verificar(400)) return false;
    return true;
}
// Cargando tokens
void cargarTokens() {
    string ruta = "C:\\Diseno de compiladores\\Proyecto\\tokens.txt";
    ifstream archivo(ruta);
    if(!archivo) {
        cout << "No se pudo abrir: " << ruta << endl;
        return;
    }
    num_tokens = 0;
    string linea_archivo;
    int num_linea = 1;
    while(getline(archivo, linea_archivo)) {
        // Procesar cada línea
        int inicio = 0;
        int fin = 0;
        while(fin < linea_archivo.length()) {
            // Buscar espacios
            while(fin < linea_archivo.length() && linea_archivo[fin] != ' ') {
                fin++;
            }
            // Extraer token
            if(inicio < fin) {
                string token_str = linea_archivo.substr(inicio, fin - inicio);
                int token = stoi(token_str);

                if(num_tokens < 1000) {
                    tokens[num_tokens++] = token;
                }
            }
            // Saltar espacios
            while(fin < linea_archivo.length() && linea_archivo[fin] == ' ') {
                fin++;
            }
            inicio = fin;
        }
        if(num_tokens > 0 && tokens[num_tokens-1] != -1) {
            if(num_tokens < 1000) {
                tokens[num_tokens++] = -1;
            }
        }
        num_linea++;
    }
    archivo.close();
    cout << "Tokens cargados: ";
    for(int i = 0; i < num_tokens; i++) {
        if(tokens[i] != -1) {
            cout << tokens[i] << " ";
        }
    }
    cout << endl;
}
// Analizador sintactico
void analizadorSintacticoSQL() {
    pos = 0;
    linea = 1;
    error_encontrado = false;
    if(num_tokens == 0) {
        cout << "\nError: No hay tokens para analizar";
        return;
    }
    while(pos < num_tokens && !error_encontrado) {
        while(pos < num_tokens && tokens[pos] == -1) {
            linea++;
            pos++;
        }
        if(pos >= num_tokens) break;
        int token = tokens[pos];
        if(token == 300) {
            if(!select()) break;
        }
        else if(token == 309) {
            if(!update()) break;
        }
        else if(token == 311) {
            if(!delete_sql()) break;
        }
        else if(token == 312) {
            if(!insert_into()) break;
        }
        else {
            cout << "\nError sintactico en la linea " << linea << " sentencia no reconocida";
            error_encontrado = true;
            break;
        }
    }
    bool quedan_tokens_reales = false;
    for(int i = pos; i < num_tokens; i++) {
        if(tokens[i] != -1) {
            quedan_tokens_reales = true;
            break;
        }
    }
    if(!error_encontrado && !quedan_tokens_reales) {
    cout << "\nAnalisis sintactico completado sin errores\n";
    } else if(error_encontrado) {
        cout << "\nAnalisis sintactico completado con errores\n";
    }
}
int main() {
    cargarTokens();
    analizadorSintacticoSQL();
    return 0;
}
