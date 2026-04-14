#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;

// ------------------ STRUCTS ------------------
struct ConfigLiga {
    string nombre;
    int puntosVictoria;
    int puntosEmpate;
    int puntosDerrota;
    vector<string> equipos;
};

struct Partido {
    string fecha;
    string equipoLocal;
    string equipoVisitante;
    int golesLocal;
    int golesVisitante;
};

struct Equipo {
    string nombre;
    int PJ = 0, PG = 0, PE = 0, PP = 0;
    int GF = 0, GC = 0, DG = 0;
    int puntos = 0;
};

// ------------------ FUNCIONES ------------------
ConfigLiga leerConfiguracion();
vector<Partido> leerPartidos();
void guardarPartido(const Partido& p);
vector<Equipo> construirTabla(const vector<Partido>& partidos, const ConfigLiga& config);
void actualizarEstadisticas(Equipo& e, int gf, int gc, const ConfigLiga& config);
bool compararEquipos(const Equipo& a, const Equipo& b);
void ordenarTabla(vector<Equipo>& tabla);
void mostrarTabla(const vector<Equipo>& tabla, const ConfigLiga& config, bool guardar = false);  // ✅ CORREGIDO: recibe config
void guardarTablaEnArchivo(const vector<Equipo>& tabla, const ConfigLiga& config);  // ✅ CORREGIDO: recibe config
Partido ingresarPartido(const vector<string>& equipos);
void mostrarPartidos(const vector<Partido>& partidos);
bool partidoDuplicado(const vector<Partido>& partidos, const Partido& nuevo);
void mostrarHistorialEnfrentamientos(const vector<Partido>& partidos, const vector<string>& equipos);
void editarPartido(vector<Partido>& partidos);
void reescribirPartidos(const vector<Partido>& partidos);

// ------------------ IMPLEMENTACIONES ------------------
ConfigLiga leerConfiguracion() {
    ConfigLiga config;
    ifstream archivo("config.txt");
    if (!archivo.is_open()) {
        cout << "ERROR: No se encontró config.txt\n";
        exit(1);
    }
    string linea;
    while (getline(archivo, linea)) {
        if (linea.empty() || linea[0] == '#') continue;
        size_t pos = linea.find('=');
        if (pos == string::npos) continue;
        string clave = linea.substr(0, pos);
        string valor = linea.substr(pos + 1);
        if (clave == "nombre") config.nombre = valor;
        else if (clave == "victoria") config.puntosVictoria = stoi(valor);
        else if (clave == "empate") config.puntosEmpate = stoi(valor);
        else if (clave == "derrota") config.puntosDerrota = stoi(valor);
        else if (clave == "equipo") config.equipos.push_back(valor);
    }
    return config;
}

vector<Partido> leerPartidos() {
    vector<Partido> partidos;
    ifstream archivo("partidos.txt");
    if (!archivo.is_open()) return partidos;
    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        Partido p;
        string gl, gv;
        getline(ss, p.fecha, '|');
        getline(ss, p.equipoLocal, '|');
        getline(ss, p.equipoVisitante, '|');
        getline(ss, gl, '|');
        getline(ss, gv, '|');
        p.golesLocal = stoi(gl);
        p.golesVisitante = stoi(gv);
        partidos.push_back(p);
    }
    return partidos;
}

void guardarPartido(const Partido& p) {
    ofstream archivo("partidos.txt", ios::app);
    archivo << p.fecha << "|" << p.equipoLocal << "|" << p.equipoVisitante << "|" << p.golesLocal << "|" << p.golesVisitante << endl;
}

// ------------------ TABLA ------------------
void actualizarEstadisticas(Equipo& e, int gf, int gc, const ConfigLiga& config) {
    e.PJ++;
    e.GF += gf;
    e.GC += gc;
    e.DG = e.GF - e.GC;
    if (gf > gc) {
        e.PG++;
        e.puntos += config.puntosVictoria;
    } else if (gf == gc) {
        e.PE++;
        e.puntos += config.puntosEmpate;
    } else {
        e.PP++;
        e.puntos += config.puntosDerrota;
    }
}

vector<Equipo> construirTabla(const vector<Partido>& partidos, const ConfigLiga& config) {
    vector<Equipo> tabla;
    for (const auto& nombre : config.equipos) tabla.push_back({nombre});
    for (const auto& p : partidos) {
        for (auto& e : tabla) {
            if (e.nombre == p.equipoLocal) actualizarEstadisticas(e, p.golesLocal, p.golesVisitante, config);
            if (e.nombre == p.equipoVisitante) actualizarEstadisticas(e, p.golesVisitante, p.golesLocal, config);
        }
    }
    return tabla;
}

bool compararEquipos(const Equipo& a, const Equipo& b) {
    if (a.puntos != b.puntos) return a.puntos > b.puntos;
    if (a.DG != b.DG) return a.DG > b.DG;
    return a.GF > b.GF;
}

void ordenarTabla(vector<Equipo>& tabla) {
    sort(tabla.begin(), tabla.end(), compararEquipos);
}

// ✅ CORREGIDA: Recibe ConfigLiga para mostrar puntos
void mostrarTabla(const vector<Equipo>& tabla, const ConfigLiga& config, bool guardar) {
    cout << "\n" << string(50, '=') << " TABLA " << string(50, '=') << "\n";
    cout << left << setw(4) << "#" 
         << setw(18) << "EQUIPO" 
         << setw(4) << "PJ" 
         << setw(4) << "PG" 
         << setw(4) << "PE" 
         << setw(4) << "PP" 
         << setw(4) << "GF" 
         << setw(4) << "GC" 
         << setw(4) << "DG" 
         << setw(6) << "PUNTOS" << endl;
    cout << string(60, '-') << "\n";
    
    for (size_t i = 0; i < tabla.size(); i++) {
        const auto& e = tabla[i];
        cout << setw(4) << (i+1) 
             << setw(18) << e.nombre 
             << setw(4) << e.PJ 
             << setw(4) << e.PG 
             << setw(4) << e.PE 
             << setw(4) << e.PP 
             << setw(4) << e.GF 
             << setw(4) << e.GC 
             << setw(4) << e.DG 
             << setw(6) << e.puntos << endl;
    }
    cout << string(60, '-') << "\n";
    cout << "Puntos: Victoria=" << config.puntosVictoria 
         << " Empate=" << config.puntosEmpate 
         << " Derrota=" << config.puntosDerrota << "\n";
    cout << string(110, '=') << "\n";
    
    if (guardar) {
        guardarTablaEnArchivo(tabla, config);
        cout << "✓ Tabla guardada en 'tabla.txt'\n";
    }
}

// ✅ CORREGIDA: Recibe ConfigLiga
void guardarTablaEnArchivo(const vector<Equipo>& tabla, const ConfigLiga& config) {
    ofstream archivo("tabla.txt");
    if (!archivo.is_open()) {
        cout << "ERROR: No se pudo crear tabla.txt\n";
        return;
    }
    
    archivo << string(50, '=') << " TABLA " << string(50, '=') << "\n";
    archivo << left << setw(4) << "#" 
            << setw(18) << "EQUIPO" 
            << setw(4) << "PJ" 
            << setw(4) << "PG" 
            << setw(4) << "PE" 
            << setw(4) << "PP" 
            << setw(4) << "GF" 
            << setw(4) << "GC" 
            << setw(4) << "DG" 
            << setw(6) << "PUNTOS" << "\n";
    archivo << string(60, '-') << "\n";
    
    for (size_t i = 0; i < tabla.size(); i++) {
        const auto& e = tabla[i];
        archivo << setw(4) << (i+1) 
                << setw(18) << e.nombre 
                << setw(4) << e.PJ 
                << setw(4) << e.PG 
                << setw(4) << e.PE 
                << setw(4) << e.PP 
                << setw(4) << e.GF 
                << setw(4) << e.GC 
                << setw(4) << e.DG 
                << setw(6) << e.puntos << "\n";
    }
    archivo << string(60, '-') << "\n";
    archivo << "Puntos: Victoria=" << config.puntosVictoria 
            << " Empate=" << config.puntosEmpate 
            << " Derrota=" << config.puntosDerrota << "\n";
    archivo << string(110, '=') << "\n";
}

// ------------------ PARTIDOS ------------------
Partido ingresarPartido(const vector<string>& equipos) {
    Partido p;
    time_t t = time(0);
    char fecha[11];
    strftime(fecha, sizeof(fecha), "%Y-%m-%d", localtime(&t));
    p.fecha = fecha;
    cout << "\nEquipos:\n";
    for (size_t i = 0; i < equipos.size(); i++) cout << i+1 << ". " << equipos[i] << endl;
    int l, v;
    cout << "Local: "; cin >> l; l--;
    cout << "Visitante: "; cin >> v; v--;
    p.equipoLocal = equipos[l];
    p.equipoVisitante = equipos[v];
    cout << "Goles local: "; cin >> p.golesLocal;
    cout << "Goles visitante: "; cin >> p.golesVisitante;
    return p;
}

void mostrarPartidos(const vector<Partido>& partidos) {
    cout << "\n===== PARTIDOS =====\n";
    for (const auto& p : partidos) {
        cout << p.fecha << " | " << p.equipoLocal << " " << p.golesLocal << " - " << p.golesVisitante << " " << p.equipoVisitante << endl;
    }
}

bool partidoDuplicado(const vector<Partido>& partidos, const Partido& nuevo) {
    for (const auto& p : partidos) {
        if (p.fecha == nuevo.fecha && ((p.equipoLocal == nuevo.equipoLocal && p.equipoVisitante == nuevo.equipoVisitante) || 
            (p.equipoLocal == nuevo.equipoVisitante && p.equipoVisitante == nuevo.equipoLocal))) return true;
    }
    return false;
}

void mostrarHistorialEnfrentamientos(const vector<Partido>& partidos, const vector<string>& equipos) {
    int a, b;
    cout << "\nEquipos:\n";
    for (size_t i = 0; i < equipos.size(); i++) cout << i+1 << ". " << equipos[i] << endl;
    cout << "Equipo 1: "; cin >> a; a--;
    cout << "Equipo 2: "; cin >> b; b--;
    cout << "\nHistorial " << equipos[a] << " vs " << equipos[b] << ":\n";
    bool encontrado = false;
    for (const auto& p : partidos) {
        if ((p.equipoLocal == equipos[a] && p.equipoVisitante == equipos[b]) || 
            (p.equipoLocal == equipos[b] && p.equipoVisitante == equipos[a])) {
            cout << p.fecha << " | " << p.equipoLocal << " " << p.golesLocal << " - " << p.golesVisitante << " " << p.equipoVisitante << endl;
            encontrado = true;
        }
    }
    if (!encontrado) cout << "No hay enfrentamientos.\n";
}

void editarPartido(vector<Partido>& partidos) {
    if (partidos.empty()) {
        cout << "No hay partidos para editar.\n";
        return;
    }
    for (size_t i = 0; i < partidos.size(); i++) {
        cout << i+1 << ". " << partidos[i].equipoLocal << " " << partidos[i].golesLocal << "-" 
             << partidos[i].golesVisitante << " " << partidos[i].equipoVisitante << endl;
    }
    size_t i; 
    cout << "Seleccionar (numero): "; 
    cin >> i; 
    if (i > 0 && i <= partidos.size()) {
        i--;
        cout << "Nuevos goles Local/Visitante: ";
        cin >> partidos[i].golesLocal >> partidos[i].golesVisitante;
        reescribirPartidos(partidos);
        cout << "✓ Partido editado correctamente.\n";
    } else {
        cout << "Opción inválida.\n";
    }
}

void reescribirPartidos(const vector<Partido>& partidos) {
    ofstream archivo("partidos.txt");
    for (const auto& p : partidos) 
        archivo << p.fecha << "|" << p.equipoLocal << "|" << p.equipoVisitante << "|" << p.golesLocal << "|" << p.golesVisitante << endl;
}

// ------------------ MAIN ------------------
int mostrarMenu(const string& nombre) {
    cout << "\n===== " << nombre << " =====\n";
    cout << "1. Tabla (Ver)\n";
    cout << "2. Tabla (Guardar tabla.txt)\n";
    cout << "3. Registrar partido\n";
    cout << "4. Ver partidos\n";
    cout << "5. Historial enfrentamientos\n";
    cout << "6. Editar partido\n";
    cout << "7. Salir\n";
    cout << "Opcion: ";
    int op; cin >> op;
    return op;
}

int main() {
    ConfigLiga config = leerConfiguracion();
    vector<Partido> partidos = leerPartidos();
    
    int op;
    do {
        op = mostrarMenu(config.nombre);
        switch(op) {
            case 1: {  // Ver tabla
                auto tabla = construirTabla(partidos, config);
                ordenarTabla(tabla);
                mostrarTabla(tabla, config, false);
                break;
            }
            case 2: {  // Guardar tabla
                auto tabla = construirTabla(partidos, config);
                ordenarTabla(tabla);
                mostrarTabla(tabla, config, true);
                break;
            }
            case 3: {
                Partido p = ingresarPartido(config.equipos);
                if (!partidoDuplicado(partidos, p)) {
                    guardarPartido(p);
                    partidos.push_back(p);
                    cout << "✓ Partido registrado correctamente\n";
                } else {
                    cout << "⚠️ Partido duplicado - no se registró\n";
                }
                break;
            }
            case 4: mostrarPartidos(partidos); break;
            case 5: mostrarHistorialEnfrentamientos(partidos, config.equipos); break;
            case 6: editarPartido(partidos); partidos = leerPartidos(); break;  // Recarga partidos
            case 7: cout << "¡Hasta luego!\n"; break;
            default: cout << "Opción inválida\n";
        }
    } while (op != 7);
    
    return 0;
}