#include <iostream>
#include <fstream>
#include <string>
using namespace std;
// Estructura para cuartetos
struct Cuarteto {
    string op;     // Operador
    string arg1;   // Argumento 1
    string arg2;   // Argumento 2
    string res;    // Resultado
};
Cuarteto cuartetos[100];
int numCuartetos = 0;
int tempCount = 1;
// Array para condiciones simples
string condicionesSimples[20];
int numCondicionesSimples = 0;
// Funciones para cuartetos
void agregarCuarteto(string op, string a1, string a2, string r) {
    if(numCuartetos < 100) {
        cuartetos[numCuartetos].op = op;
        cuartetos[numCuartetos].arg1 = a1;
        cuartetos[numCuartetos].arg2 = a2;
        cuartetos[numCuartetos].res = r;
        numCuartetos++;
    }
}
string nuevaTemporal() {
    string temp = "R" + to_string(tempCount);
    tempCount++;
    return temp;
}
void mostrarCuartetos() {
    cout << "\n=== CUARTETOS GENERADOS ===" << endl;
    cout << "| Operador |                Argumento1                |           Argumento2           |    Resultado    |" << endl;
    cout << "|----------|------------------------------------------|--------------------------------|-----------------|" << endl;

    for(int i = 0; i < numCuartetos; i++) {
        cout << "| " << cuartetos[i].op;
        if(cuartetos[i].op.length() < 8) {
            for(int j = cuartetos[i].op.length(); j < 8; j++) cout << " ";
        }
        cout << " | " << cuartetos[i].arg1;
        if(cuartetos[i].arg1.length() < 40) {
            for(int j = cuartetos[i].arg1.length(); j < 40; j++) cout << " ";
        }
        cout << " | " << cuartetos[i].arg2;
        if(cuartetos[i].arg2.length() < 30) {
            for(int j = cuartetos[i].arg2.length(); j < 30; j++) cout << " ";
        }
        cout << " | " << cuartetos[i].res;
        if(cuartetos[i].res.length() < 15) {
            for(int j = cuartetos[i].res.length(); j < 15; j++) cout << " ";
        }
        cout << " |" << endl;
    }
}
// Funciones auxiliares
string trim(string str) {
    // Eliminar espacios al inicio
    while(!str.empty() && str[0] == ' ') {
        str = str.substr(1);
    }
    // Eliminar espacios al final
    while(!str.empty() && str.back() == ' ') {
        str.pop_back();
    }
    return str;
}
// Condiciones con AND/OR
void procesarCondicion(const string& condicion) {
    string cond = trim(condicion);
    size_t posAND = cond.find(" AND ");
    size_t posOR = cond.find(" OR ");
    if(posAND != string::npos && (posOR == string::npos || posAND < posOR)) {
        // Separar por AND
        string cond1 = cond.substr(0, posAND);
        string cond2 = cond.substr(posAND + 5);
        procesarCondicion(cond1);
        procesarCondicion(cond2);
    }
    else if(posOR != string::npos) {
        // Separar por OR
        string cond1 = cond.substr(0, posOR);
        string cond2 = cond.substr(posOR + 4);
        procesarCondicion(cond1);
        procesarCondicion(cond2);
    }
    else {
        // Condición simple
        if(!cond.empty()) {
            if(numCondicionesSimples < 20) {
                condicionesSimples[numCondicionesSimples] = cond;
                numCondicionesSimples++;
            }
        }
    }
}
// Procesar una condición simple (sin AND/OR)
void procesarCondicionSimple(const string& condicion) {
    if(condicion.find('>') != string::npos) {
        size_t opPos = condicion.find('>');
        string izq = trim(condicion.substr(0, opPos));
        string der = trim(condicion.substr(opPos + 1));
        // Verificar si es >=
        if(opPos + 1 < condicion.length() && condicion[opPos + 1] == '=') {
            der = trim(condicion.substr(opPos + 2));
            string temp = nuevaTemporal();
            agregarCuarteto(">=", izq, der, temp);
        } else {
            string temp = nuevaTemporal();
            agregarCuarteto(">", izq, der, temp);
        }
    }
    else if(condicion.find('<') != string::npos) {
        size_t opPos = condicion.find('<');
        string izq = trim(condicion.substr(0, opPos));
        string der = trim(condicion.substr(opPos + 1));
        // Verificar si es <=
        if(opPos + 1 < condicion.length() && condicion[opPos + 1] == '=') {
            der = trim(condicion.substr(opPos + 2));
            string temp = nuevaTemporal();
            agregarCuarteto("<=", izq, der, temp);
        } else {
            string temp = nuevaTemporal();
            agregarCuarteto("<", izq, der, temp);
        }
    }
    else if(condicion.find('=') != string::npos) {
        size_t opPos = condicion.find('=');
        string izq = trim(condicion.substr(0, opPos));
        string der = trim(condicion.substr(opPos + 1));
        string temp = nuevaTemporal();
        agregarCuarteto("=", izq, der, temp);
    }
}
// Sentencias SQL
void procesarSELECT(const string& linea) {
    cout << "[PROCESANDO SELECT]" << endl;
    // Buscar WHERE
    size_t wherePos = linea.find("WHERE");
    if(wherePos != string::npos) {
        string condicionCompleta = linea.substr(wherePos + 5);
        if(!condicionCompleta.empty() && condicionCompleta.back() == ';') {
            condicionCompleta.pop_back();
        }
        condicionCompleta = trim(condicionCompleta);
        // Resetear contador de condiciones
        numCondicionesSimples = 0;
        // Procesar condiciones (pueden tener AND/OR)
        procesarCondicion(condicionCompleta);
        // Procesar cada condición simple
        for(int i = 0; i < numCondicionesSimples; i++) {
            procesarCondicionSimple(condicionesSimples[i]);
        }
        // Si hay más de una condición, crear cuarteto AND/OR
        if(numCondicionesSimples > 1) {
            int ultimaTemp = tempCount - 1;
            string temp1 = "R" + to_string(ultimaTemp - numCondicionesSimples + 1);
            string temp2 = "R" + to_string(ultimaTemp);
            string tempFinal = nuevaTemporal();
            agregarCuarteto("AND", temp1, temp2, tempFinal);
        }
        // Cuarteto para SELECT
        string tabla = "";
        size_t fromPos = linea.find("FROM");
        if(fromPos != string::npos) {
            tabla = trim(linea.substr(fromPos + 4, wherePos - fromPos - 4));
        }
        string condicionFinal = "";
        if(numCondicionesSimples > 1) {
            condicionFinal = "R" + to_string(tempCount - 1);
        } else if(numCondicionesSimples == 1) {
            condicionFinal = "R" + to_string(tempCount - 1);
        } else {
            condicionFinal = "";
        }
        agregarCuarteto("SELECT", tabla, condicionFinal, "resultado");
    }
    else {
        // SELECT sin WHERE
        string tabla = "";
        size_t fromPos = linea.find("FROM");
        if(fromPos != string::npos) {
            string resto = linea.substr(fromPos + 4);
            if(!resto.empty() && resto.back() == ';') {
                resto.pop_back();
            }
            tabla = trim(resto);
        }
        agregarCuarteto("SELECT", tabla, "", "resultado");
    }
}
void procesarUPDATE(const string& linea) {
    cout << "[PROCESANDO UPDATE]" << endl;
    // Extraer tabla
    string tabla = "";
    size_t updateEnd = linea.find(" ", 7);
    if(updateEnd != string::npos) {
        tabla = trim(linea.substr(7, updateEnd - 7));
    }
    // Buscar SET
    size_t setPos = linea.find("SET");
    size_t wherePos = linea.find("WHERE");
    if(setPos != string::npos) {
        string asignaciones;
        if(wherePos != string::npos) {
            asignaciones = linea.substr(setPos + 3, wherePos - setPos - 3);
        } else {
            asignaciones = linea.substr(setPos + 3);
            if(!asignaciones.empty() && asignaciones.back() == ';') {
                asignaciones.pop_back();
            }
        }
        asignaciones = trim(asignaciones);
        // Buscar múltiples asignaciones separadas por coma
        size_t comaPos = asignaciones.find(',');
        if(comaPos != string::npos) {
            // Múltiples asignaciones
            string asign1 = trim(asignaciones.substr(0, comaPos));
            string asign2 = trim(asignaciones.substr(comaPos + 1));
            // Procesar primera asignación
            if(asign1.find('*') != string::npos) {
                size_t igualPos = asign1.find('=');
                size_t porPos = asign1.find('*');
                if(igualPos != string::npos && porPos != string::npos) {
                    string variable = trim(asign1.substr(0, igualPos));
                    string izq = trim(asign1.substr(igualPos + 1, porPos - igualPos - 1));
                    string der = trim(asign1.substr(porPos + 1));
                    string temp1 = nuevaTemporal();
                    agregarCuarteto("*", izq, der, temp1);
                    agregarCuarteto("=", temp1, "", variable);
                }
            }
            else if(asign1.find('+') != string::npos) {
                size_t igualPos = asign1.find('=');
                size_t masPos = asign1.find('+');
                if(igualPos != string::npos && masPos != string::npos) {
                    string variable = trim(asign1.substr(0, igualPos));
                    string izq = trim(asign1.substr(igualPos + 1, masPos - igualPos - 1));
                    string der = trim(asign1.substr(masPos + 1));
                    string temp1 = nuevaTemporal();
                    agregarCuarteto("+", izq, der, temp1);
                    agregarCuarteto("=", temp1, "", variable);
                }
            }
            else if(asign1.find('-') != string::npos) {
                size_t igualPos = asign1.find('=');
                size_t menosPos = asign1.find('-');
                if(igualPos != string::npos && menosPos != string::npos) {
                    string variable = trim(asign1.substr(0, igualPos));
                    string izq = trim(asign1.substr(igualPos + 1, menosPos - igualPos - 1));
                    string der = trim(asign1.substr(menosPos + 1));
                    string temp1 = nuevaTemporal();
                    agregarCuarteto("-", izq, der, temp1);
                    agregarCuarteto("=", temp1, "", variable);
                }
            }
            else if(asign1.find('=') != string::npos) {
                size_t igualPos = asign1.find('=');
                string variable = trim(asign1.substr(0, igualPos));
                string valor = trim(asign1.substr(igualPos + 1));
                agregarCuarteto("=", valor, "", variable);
            }
            // Procesar segunda asignación
            if(asign2.find('*') != string::npos) {
                size_t igualPos = asign2.find('=');
                size_t porPos = asign2.find('*');
                if(igualPos != string::npos && porPos != string::npos) {
                    string variable = trim(asign2.substr(0, igualPos));
                    string izq = trim(asign2.substr(igualPos + 1, porPos - igualPos - 1));
                    string der = trim(asign2.substr(porPos + 1));
                    string temp1 = nuevaTemporal();
                    agregarCuarteto("*", izq, der, temp1);
                    agregarCuarteto("=", temp1, "", variable);
                }
            }
            else if(asign2.find('=') != string::npos) {
                size_t igualPos = asign2.find('=');
                string variable = trim(asign2.substr(0, igualPos));
                string valor = trim(asign2.substr(igualPos + 1));
                agregarCuarteto("=", valor, "", variable);
            }
        }
        else {
            // Una sola asignación
            if(asignaciones.find('*') != string::npos) {
                size_t igualPos = asignaciones.find('=');
                size_t porPos = asignaciones.find('*');

                if(igualPos != string::npos && porPos != string::npos) {
                    string variable = trim(asignaciones.substr(0, igualPos));
                    string izq = trim(asignaciones.substr(igualPos + 1, porPos - igualPos - 1));
                    string der = trim(asignaciones.substr(porPos + 1));

                    string temp1 = nuevaTemporal();
                    agregarCuarteto("*", izq, der, temp1);
                    agregarCuarteto("=", temp1, "", variable);
                }
            }
            else if(asignaciones.find('+') != string::npos) {
                size_t igualPos = asignaciones.find('=');
                size_t masPos = asignaciones.find('+');

                if(igualPos != string::npos && masPos != string::npos) {
                    string variable = trim(asignaciones.substr(0, igualPos));
                    string izq = trim(asignaciones.substr(igualPos + 1, masPos - igualPos - 1));
                    string der = trim(asignaciones.substr(masPos + 1));
                    string temp1 = nuevaTemporal();
                    agregarCuarteto("+", izq, der, temp1);
                    agregarCuarteto("=", temp1, "", variable);
                }
            }
            else if(asignaciones.find('=') != string::npos) {
                size_t igualPos = asignaciones.find('=');
                string variable = trim(asignaciones.substr(0, igualPos));
                string valor = trim(asignaciones.substr(igualPos + 1));
                agregarCuarteto("=", valor, "", variable);
            }
        }
    }
    // Procesar WHERE si existe
    if(wherePos != string::npos) {
        string condicionCompleta = linea.substr(wherePos + 5);
        if(!condicionCompleta.empty() && condicionCompleta.back() == ';') {
            condicionCompleta.pop_back();
        }
        condicionCompleta = trim(condicionCompleta);
        // Resetear contador de condiciones
        numCondicionesSimples = 0;
        // Procesar condiciones
        procesarCondicion(condicionCompleta);
        // Procesar cada condición simple
        for(int i = 0; i < numCondicionesSimples; i++) {
            procesarCondicionSimple(condicionesSimples[i]);
        }
    }
    // Cuarteto para UPDATE
    agregarCuarteto("UPDATE", tabla, "", "");
}
void procesarINSERT(const string& linea) {
    cout << "[PROCESANDO INSERT]" << endl;
    // Extraer tabla
    string tabla = "";
    size_t intoPos = linea.find("INTO");
    size_t valuesPos = linea.find("VALUES");
    if(intoPos != string::npos && valuesPos != string::npos) {
        tabla = trim(linea.substr(intoPos + 4, valuesPos - intoPos - 4));
    }
    // Buscar VALUES
    if(valuesPos != string::npos) {
        string valores = linea.substr(valuesPos + 6);
        // Buscar paréntesis
        size_t parenIni = valores.find('(');
        size_t parenFin = valores.find(')');
        if(parenIni != string::npos && parenFin != string::npos) {
            string listaValores = valores.substr(parenIni + 1, parenFin - parenIni - 1);
            // Procesar cada valor
            string valorActual = "";
            int valorNum = 1;
            for(char c : listaValores) {
                if(c == ',') {
                    if(!valorActual.empty()) {
                        valorActual = trim(valorActual);
                        string columna = "col" + to_string(valorNum);
                        agregarCuarteto("=", valorActual, "", columna);
                        valorActual = "";
                        valorNum++;
                    }
                } else if(c != ' ') {
                    valorActual += c;
                }
            }
            // Último valor
            if(!valorActual.empty()) {
                valorActual = trim(valorActual);
                if(!valorActual.empty() && valorActual.back() == ';') {
                    valorActual.pop_back();
                    valorActual = trim(valorActual);
                }
                string columna = "col" + to_string(valorNum);
                agregarCuarteto("=", valorActual, "", columna);
            }
        }
    }
    // Cuarteto para INSERT
    agregarCuarteto("INSERT", tabla, "", "");
}
void procesarDELETE(const string& linea) {
    cout << "[PROCESANDO DELETE]" << endl;
    // Extraer tabla
    string tabla = "";
    size_t fromPos = linea.find("FROM");
    size_t wherePos = linea.find("WHERE");
    if(fromPos != string::npos) {
        if(wherePos != string::npos) {
            tabla = trim(linea.substr(fromPos + 4, wherePos - fromPos - 4));
        } else {
            string resto = linea.substr(fromPos + 4);
            // Quitar ; si no hay WHERE
            if(!resto.empty() && resto.back() == ';') {
                resto.pop_back();
            }
            tabla = trim(resto);
        }
    }
    // Procesar WHERE si existe
    if(wherePos != string::npos) {
        string condicionCompleta = linea.substr(wherePos + 5);
        if(!condicionCompleta.empty() && condicionCompleta.back() == ';') {
            condicionCompleta.pop_back();
        }
        condicionCompleta = trim(condicionCompleta);
        // Resetear contador de condiciones
        numCondicionesSimples = 0;
        // Procesar condiciones
        procesarCondicion(condicionCompleta);
        // Procesar cada condición simple
        for(int i = 0; i < numCondicionesSimples; i++) {
            procesarCondicionSimple(condicionesSimples[i]);
        }
        // Si hay más de una condición, crear cuarteto AND
        if(numCondicionesSimples > 1) {
            int ultimaTemp = tempCount - 1;
            string temp1 = "R" + to_string(ultimaTemp - numCondicionesSimples + 1);
            string temp2 = "R" + to_string(ultimaTemp);
            string tempFinal = nuevaTemporal();
            agregarCuarteto("AND", temp1, temp2, tempFinal);
            // Cuarteto para DELETE con condición combinada
            agregarCuarteto("DELETE", tabla, tempFinal, "");
        } else if(numCondicionesSimples == 1) {
            string temp = "R" + to_string(tempCount - 1);
            agregarCuarteto("DELETE", tabla, temp, "");
        }
    } else {
        // DELETE sin WHERE
        agregarCuarteto("DELETE", tabla, "", "");
    }
}
void procesarLineaSQL(const string& linea) {
    string lineaTrim = trim(linea);
    if(lineaTrim.empty()) {
        return;
    }
    // Verificar tipo de sentencia
    if(lineaTrim.find("SELECT") == 0) {
        procesarSELECT(lineaTrim);
    }
    else if(lineaTrim.find("UPDATE") == 0) {
        procesarUPDATE(lineaTrim);
    }
    else if(lineaTrim.find("INSERT") == 0) {
        procesarINSERT(lineaTrim);
    }
    else if(lineaTrim.find("DELETE") == 0) {
        procesarDELETE(lineaTrim);
    } else {
        cout << "[SENTENCIA NO RECONOCIDA: " << lineaTrim << "]" << endl;
    }
}
// Generación de tercetos
void generarTercetos() {
    cout << "\n\n=== GENERACION DE TERCETOS ===" << endl;
    cout << "| ID  | Operador |                 Operando1                |            Operando2           |" << endl;
    cout << "|-----|----------|------------------------------------------|--------------------------------|" << endl;
    for(int i = 0; i < numCuartetos; i++) {
        // Mostrar como terceto (operador, operando1, operando2)
        string arg1 = cuartetos[i].arg1;
        string arg2 = cuartetos[i].arg2;
        cout << "| " << (i+1);
        if(i+1 < 10) cout << "   | ";
        else if(i+1 < 100) cout << "  | ";
        else cout << " | ";
        cout << cuartetos[i].op;
        if(cuartetos[i].op.length() < 8) {
            for(int j = cuartetos[i].op.length(); j < 8; j++) cout << " ";
        }
        cout << " | " << arg1;
        if(arg1.length() < 40) {
            for(int j = arg1.length(); j < 40; j++) cout << " ";
        }
        cout << " | " << arg2;
        if(arg2.length() < 30) {
            for(int j = arg2.length(); j < 30; j++) cout << " ";
        }
        cout << " |" << endl;
    }
}

int main() {
    string ruta = "C:\\Diseno de compiladores\\Proyecto\\leer.txt";
    ifstream archivo(ruta);
    string linea;
    int numLinea = 1;
    int lineasProcesadas = 0;
    if(!archivo) {
        cout << "Error: No se pudo abrir el archivo: " << ruta << endl;
        return 1;
    }
    cout << "=== ANALIZADOR SEMANTICO SQL ===" << endl;
    cout << "Leyendo archivo: " << ruta << endl;
    cout << "==================================================\n" << endl;
    while(getline(archivo, linea)) {
        string lineaTrim = trim(linea);
        if(!lineaTrim.empty()) {
            cout << "Linea " << numLinea << ": " << lineaTrim << endl;
            cout << string(50, '-') << endl;
            // Procesar la línea SQL
            procesarLineaSQL(lineaTrim);
            lineasProcesadas++;
            cout << endl;
        }
        numLinea++;
    }
    archivo.close();
    if(lineasProcesadas == 0) {
        cout << "No se encontraron sentencias SQL en el archivo." << endl;
        return 0;
    }
    // Mostrar todos los cuartetos generados
    mostrarCuartetos();
    // Generar y mostrar tercetos
    generarTercetos();
    cout << "\n- Total de sentencias procesadas: " << lineasProcesadas << endl;
    cout << "- Total de cuartetos generados: " << numCuartetos << endl;
    cout << "- Total de tercetos generados: " << numCuartetos << endl;
    return 0;
}
