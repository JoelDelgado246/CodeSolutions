#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <regex>
#include <cstdlib>
#include <windows.h>

using namespace std;

struct Miembro {
    string nombre;
    string apellido;
    string rol;
    string ci;
    string contacto;
};

struct Equipo {
    string id;
    string nombre;
    vector<Miembro> miembros;
    int numeroProductos;
    vector<string> productos;
};

struct Producto {
    string id;
    string estado;
    string nombre;
    string lenguaje;
    string descripcion;
    string version;
    string corporacion;
    string nombreEmpresa;
    string contacto;
    string ruc;
};
// Cargar archivos 
// EQUIPOS
void cargarEquiposDesdeCSV(vector<Equipo>& equipos) {
    ifstream archivo("equipos.csv");
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo equipos.csv.\n";
        return;
    }

    string linea;
    getline(archivo, linea);

    Equipo equipoActual;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string id, nombre, nombreMiembro, apellidoMiembro, rolMiembro, ciMiembro, contactoMiembro, numeroProductosStr, productosStr;
        int numeroProductos;

        getline(ss, id, ',');
        getline(ss, nombre, ',');
        getline(ss, nombreMiembro, ',');
        getline(ss, apellidoMiembro, ',');
        getline(ss, rolMiembro, ',');
        getline(ss, ciMiembro, ',');
        getline(ss, contactoMiembro, ',');
        getline(ss, numeroProductosStr, ',');
        numeroProductos = stoi(numeroProductosStr);

        if (equipoActual.id != id && !equipoActual.id.empty()) {
            equipos.push_back(equipoActual);
            equipoActual = Equipo{};
        }

        equipoActual.id = id;
        equipoActual.nombre = nombre;
        equipoActual.numeroProductos = numeroProductos;

        Miembro miembro{nombreMiembro, apellidoMiembro, rolMiembro, ciMiembro, contactoMiembro};
        equipoActual.miembros.push_back(miembro);

        string producto;
        while (getline(ss, producto, '|')) {
            equipoActual.productos.push_back(producto);
        }
    }
    if (!equipoActual.id.empty()) {
        equipos.push_back(equipoActual);
    }

    archivo.close();
}

// CARGA DE ARCHIVOS 

void guardarEquiposCSV(const vector<Equipo>& equipos) {
    ofstream archivo("equipos.csv");
    if (archivo.is_open()) {
        archivo << "ID,Nombre del Equipo,Nombre Miembro,Apellido,Rol,CI,Contacto,Numero de Productos,Productos\n";
        for (const auto& equipo : equipos) {
            for (const auto& miembro : equipo.miembros) {
                archivo << equipo.id << "," << equipo.nombre << ",";
                archivo << miembro.nombre << "," << miembro.apellido << "," << miembro.rol << "," << miembro.ci << "," << miembro.contacto << ",";
                archivo << equipo.numeroProductos << ",";
                for (size_t i = 0; i < equipo.productos.size(); ++i) {
                    archivo << equipo.productos[i];
                    if (i < equipo.productos.size() - 1) archivo << "|";
                }
                archivo << "\n";
            }
        }
        archivo.close();
        cout << "Equipos guardados en equipos.csv con exito.\n";
    } else {
        cout << "Error al abrir el archivo equipos.csv.\n";
    }
}

void guardarProductosCSV(const vector<Producto>& productos) {
    ofstream archivo("productos.csv");
    if (archivo.is_open()) {
        archivo << "ID,Estado,Nombre,Lenguaje,Descripcion,Version,Corporacion,NombreEmpresa,Contacto,RUC\n";
        for (const auto& producto : productos) {
            archivo << producto.id << "," << producto.estado << "," << producto.nombre << "," << producto.lenguaje << "," 
                    << producto.descripcion << "," << producto.version << "," << producto.corporacion << "," 
                    << producto.nombreEmpresa << "," << producto.contacto << "," << producto.ruc << "\n";
        }
        archivo.close();
        cout << "Productos guardados en productos.csv con exito.\n";
    } else {
        cout << "Error al abrir el archivo productos.csv.\n";
    }
}


//PRODUCTOS 

void cargarProductosDesdeCSV(vector<Producto>& productos) {
    ifstream archivo("productos.csv");
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo productos.csv.\n";
        return;
    }

    string linea;
    getline(archivo, linea);

    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string id, estado, nombre, lenguaje, descripcion, version, corporacion, nombreEmpresa, contacto, ruc;

        getline(ss, id, ',');
        getline(ss, estado, ',');
        getline(ss, nombre, ',');
        getline(ss, lenguaje, ',');
        getline(ss, descripcion, ',');
        getline(ss, version, ',');
        getline(ss, corporacion, ',');
        getline(ss, nombreEmpresa, ',');
        getline(ss, contacto, ',');
        getline(ss, ruc, ',');

        Producto producto{id, estado, nombre, lenguaje, descripcion, version, corporacion, nombreEmpresa, contacto, ruc};
        productos.push_back(producto);
    }
    archivo.close();
}

// lIMPIAR PANTALLA 
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void cambiarColorConsola(int colorTexto, int colorFondo) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (colorFondo << 4) | colorTexto);
}

// VALIDACIONES
/// Validar ID 
bool validarID(const string& id) {
    if (id.length() != 4) return false;   // Debe tener exactamente 4 caracteres
    if (id[0] != 'E') return false;        // El primer carácter debe ser 'E'
    for (size_t i = 1; i < id.length(); ++i) {
        if (!isdigit(id[i])) return false; // Los otros caracteres deben ser dígitos
    }
    return true;
}
// VERIFICAR DUPLICADO ID
bool idDuplicado(const vector<Equipo>& equipos, const string& id) {
    for (const auto& equipo : equipos) {
        if (equipo.id == id) return true;
    }
    return false;
}

// VALIDAR CI 
bool validarCI(const string& ci) {
    regex formato("\\d{10}");
    return regex_match(ci, formato);
}

//VALIDAR CONTACTO
bool validarContacto(const string& contacto) {
    regex formato("\\d{9,10}");
    return regex_match(contacto, formato);
}
// AGERGAR MIEMBRO
void agregarMiembro(Equipo& equipo, const Miembro& miembro) {
    equipo.miembros.push_back(miembro);
}
// AGREGAR PRODUCTO EN EQUIPO
void agregarProducto(Equipo& equipo, const string& producto) {
    equipo.productos.push_back(producto);
}

// VALIDAR NRO DE PRODUCTO MAYOR A -1

bool validarNumeroProductos(int numeroProductos) {
    return numeroProductos >= 0;
}
// VALIDAR ID PRODUCTO
bool validarIDProducto(const string& id) {
    regex formato("P\\d{4}");
    return regex_match(id, formato);
}
//VALIDAR ESTADO ACTIVO O NO
bool validarEstado(const string& estado) {
    return estado == "activo" || estado == "inactivo";
}
// VALIDAR VERSION X.Y
bool validarVersion(const string& version) {
    regex formato("\\d+\\.\\d+");
    return regex_match(version, formato);
}
// VALIDAR RUC
bool validarRUC(const string& ruc) {
    regex formato("\\d{13}");
    return regex_match(ruc, formato);
}
//VERIFICAR SI SE REPITE
bool idProductoExiste(const vector<Producto>& productos, const string& id) {
    for (size_t i = 0; i < productos.size(); ++i) {
        if (productos[i].id == id) {
            return true;
        }
    }
    return false;
}

// 1. registrar Equipo

void registrarEquipo(vector<Equipo>& equipos) {
    string id, nombre;
    int cantidad;

    while (true) {
        cout << "Ingrese ID del equipo (E###): ";
        cin >> id;
        cin.ignore();

        if (!validarID(id)) {
            cout << "ID invalido. Debe seguir el formato E###.\n";
            continue;
        }
        
        if (idDuplicado(equipos, id)) {
            cout << "El ID ya existe. Por favor, ingrese un ID diferente.\n";
            continue;
        }
        
        break;
    }
    cout << "Ingrese nombre del equipo: ";
    getline(cin, nombre);

    Equipo nuevoEquipo{id, nombre, {}, 0, {}};  

    cout << "Ingrese la cantidad de miembros: ";
    cin >> cantidad;
    cin.ignore();
    for (int i = 0; i < cantidad; ++i) {
        Miembro miembro;
        cout << "Ingrese el nombre del miembro " << i + 1 << ": ";
        getline(cin, miembro.nombre);
        cout << "Ingrese el apellido del miembro " << i + 1 << ": ";
        getline(cin, miembro.apellido);
        cout << "Ingrese el rol del miembro " << i + 1 << ": ";
        getline(cin, miembro.rol);
        do {
            cout << "Ingrese el CI del miembro " << i + 1 << ": ";
            getline(cin, miembro.ci);
            if (!validarCI(miembro.ci)) {
                cout << "CI invalido. Debe contener exactamente 10 digitos.\n";
            }
        } while (!validarCI(miembro.ci));
        
        do {
            cout << "Ingrese el contacto del miembro " << i + 1 << " (9 o 10 digitos): ";
            getline(cin, miembro.contacto);
            if (!validarContacto(miembro.contacto)) {
                cout << "Contacto invalido. Debe contener 9 o 10 digitos.\n";
            }
        } while (!validarContacto(miembro.contacto));

        agregarMiembro(nuevoEquipo, miembro);
    }

    cout << "Ingrese el numero de productos vinculados al equipo: ";
    cin >> nuevoEquipo.numeroProductos;
    cin.ignore();
    if (!validarNumeroProductos(nuevoEquipo.numeroProductos)) {
        cout << "Numero de productos invalido. Debe ser un valor positivo.\n";
        return;
    }

    for (int i = 0; i < nuevoEquipo.numeroProductos; ++i) {
        string producto;
        cout << "Ingrese el nombre del producto " << i + 1 << ": ";
        getline(cin, producto);
        agregarProducto(nuevoEquipo, producto);
    }
    equipos.push_back(nuevoEquipo);
    cout << "Equipo registrado con exito.\n";

    guardarEquiposCSV(equipos);
}



// 2. REGISTRAR PRODUCTOS
void registrarProducto(vector<Producto>& productos) {
    Producto producto;
do {
    cout << "Ingrese el ID del producto (P####): ";
    cin >> producto.id;
    if (!validarIDProducto(producto.id)) {
        cout << "ID invalido. Debe tener el formato P####.\n";
    } else if (idProductoExiste(productos, producto.id)) {
        cout << "ID ya existe. Por favor ingrese un ID unico.\n";
    }
} while (!validarIDProducto(producto.id) || idProductoExiste(productos, producto.id));

    do {
        cout << "Ingrese el estado del producto (activo/inactivo): ";
        cin >> producto.estado;
        if (!validarEstado(producto.estado)) {
            cout << "Estado invalido. Debe ser 'activo' o 'inactivo'.\n";
        }
    } while (!validarEstado(producto.estado));

    cout << "Ingrese el nombre del producto: ";
    cin.ignore();
    getline(cin, producto.nombre);

    cout << "Ingrese el lenguaje de programacion: ";
    getline(cin, producto.lenguaje);

    cout << "Ingrese la descripcion: ";
    getline(cin, producto.descripcion);

    do {
        cout << "Ingrese la version del producto (formato #.#): ";
        cin >> producto.version;
        if (!validarVersion(producto.version)) {
            cout << "Version invalida. Debe tener el formato #.#.\n";
        }
    } while (!validarVersion(producto.version));

    cout << "Ingrese la corporacion: ";
    cin.ignore();
    getline(cin, producto.corporacion);

    cout << "Ingrese el nombre de la empresa: ";
    getline(cin, producto.nombreEmpresa);

    do {
        cout << "Ingrese el contacto (9 o 10 digitos): ";
        cin >> producto.contacto;
        if (!validarContacto(producto.contacto)) {
            cout << "Contacto invalido. Debe contener 9 o 10 digitos.\n";
        }
    } while (!validarContacto(producto.contacto));

    do {
        cout << "Ingrese el RUC (13 digitos): ";
        cin >> producto.ruc;
        if (!validarRUC(producto.ruc)) {
            cout << "RUC invalido. Debe contener exactamente 13 digitos.\n";
        }
    } while (!validarRUC(producto.ruc));

    productos.push_back(producto);
    cout << "Producto registrado con exito.\n";

    guardarProductosCSV(productos); // Guardar automáticamente después de registrar
}
// VALIDACIONES 

//3. Actualizar equipo 
void actualizarEquipo(vector<Equipo>& equipos) {
    string id;
    cout << "Ingrese el ID del equipo a actualizar: ";
    cin >> id;

    for (auto& equipo : equipos) {
        if (equipo.id == id) {
            cout << "Ingrese el nuevo nombre del equipo: ";
            cin.ignore();
            getline(cin, equipo.nombre);

            int numeroMiembros;
            cout << "Ingrese el nuevo numero de miembros: ";
            cin >> numeroMiembros;
            equipo.miembros.clear();  // Limpia los miembros anteriores

            for (int i = 0; i < numeroMiembros; ++i) {
                Miembro miembro;
                cout << "Ingrese el nombre del miembro: ";
                cin >> miembro.nombre;
                cout << "Ingrese el apellido del miembro: ";
                cin >> miembro.apellido;
                cout << "Ingrese el rol del miembro: ";
                cin >> miembro.rol;
                cout << "Ingrese el CI del miembro: ";
                cin >> miembro.ci;
                if (!validarCI(miembro.ci)) {
                    cout << "CI no es valido.\n";
                    return;
                }
                cout << "Ingrese el contacto del miembro: ";
                cin >> miembro.contacto;
                if (!validarContacto(miembro.contacto)) {
                    cout << "Contacto no es valido.\n";
                    return;
                }
                equipo.miembros.push_back(miembro);
            }

            cout << "Ingrese el nuevo numero de productos: ";
            cin >> equipo.numeroProductos;
            if (!validarNumeroProductos(equipo.numeroProductos)) {
                cout << "Numero de productos no es valido.\n";
                return;
            }

            equipo.productos.clear();  // Limpia los productos anteriores
            for (int i = 0; i < equipo.numeroProductos; ++i) {
                string producto;
                cout << "Ingrese el nombre del producto " << i + 1 << ": ";
                cin.ignore();
                getline(cin, producto);
                equipo.productos.push_back(producto);
            }

            guardarEquiposCSV(equipos);
            return;
        }
    }
    cout << "Equipo no encontrado.\n";
}

//4. Actualizar producto
void actualizarProducto(vector<Producto>& productos) {
    string id;
    cout << "Ingrese ID del producto a actualizar: ";
    cin >> id;
    cin.ignore();

    for (auto& producto : productos) {
        if (producto.id == id) {
            cout << "Producto encontrado. Actualizando...\n";

            cout << "Ingrese nuevo estado del producto (activo/inactivo): ";
            string estado;
            getline(cin, estado);
            if (validarEstado(estado)) {
                producto.estado = estado;
            } else {
                cout << "Estado invalido. Debe ser 'activo' o 'inactivo'.\n";
                return;
            }

            cout << "Ingrese nuevo nombre del producto: ";
            getline(cin, producto.nombre);

            cout << "Ingrese nuevo lenguaje del producto: ";
            getline(cin, producto.lenguaje);

            cout << "Ingrese nueva descripcion del producto: ";
            getline(cin, producto.descripcion);

            cout << "Ingrese nueva version del producto (formato X.Y): ";
            getline(cin, producto.version);
            if (!validarVersion(producto.version)) {
                cout << "Version invalida. Debe seguir el formato X.Y.\n";
                return;
            }

            cout << "Ingrese nueva corporacion del producto: ";
            getline(cin, producto.corporacion);

            cout << "Ingrese nuevo nombre de la empresa: ";
            getline(cin, producto.nombreEmpresa);

            cout << "Ingrese nuevo contacto del producto: ";
            getline(cin, producto.contacto);

            cout << "Ingrese nuevo RUC de la empresa (11 digitos): ";
            getline(cin, producto.ruc);
            if (!validarRUC(producto.ruc)) {
                cout << "RUC invalido. Debe contener 11 digitos.\n";
                return;
            }

            cout << "Actualizacion del producto completada con exito.\n";
            guardarProductosCSV(productos); 
            return;
        }
    }
    cout << "Producto no encontrado.\n";
}

// 5. Eliminar Producto

void eliminarProducto(vector<Producto>& productos) {
    string id;
    cout << "Ingrese ID del producto a eliminar: ";
    cin >> id;
    cin.ignore();

    auto it = remove_if(productos.begin(), productos.end(), [&](const Producto& producto) {
        return producto.id == id;
    });

    if (it != productos.end()) {
        productos.erase(it, productos.end());
        cout << "Producto eliminado con exito.\n";
        guardarProductosCSV(productos); 
    } else {
        cout << "Producto no encontrado.\n";
    }
}

// 6. MOSTRAR TODOS LOS EQUIPOS 


void mostrarDatosEquipo(const Equipo& equipo) {
    cout << "ID: " << equipo.id << endl;
    cout << "Nombre: " << equipo.nombre << endl;
    cout << "Numero de productos: " << equipo.numeroProductos << endl;
    cout << "Miembros: " << endl;
    for (const auto& miembro : equipo.miembros) {
        cout << "\tNombre: " << miembro.nombre << " " << miembro.apellido << endl;
        cout << "\tRol: " << miembro.rol << endl;
        cout << "\tCI: " << miembro.ci << endl;
        cout << "\tContacto: " << miembro.contacto << endl;
    }
    cout << "Productos: " << endl;
    for (const auto& producto : equipo.productos) {
        cout << "\t" << producto << endl;
    }
}

void mostrarTodosLosEquipos(const vector<Equipo>& equipos) {
    for (const auto& equipo : equipos) {
        mostrarDatosEquipo(equipo);
        cout << "------------------------\n";
    }
}

// 7. MOSTRAR PRODUCTOS

void mostrarDatosProducto(const Producto& producto) {
    cout << "ID: " << producto.id << endl;
    cout << "Estado: " << producto.estado << endl;
    cout << "Nombre: " << producto.nombre << endl;
    cout << "Lenguaje: " << producto.lenguaje << endl;
    cout << "Descripcion: " << producto.descripcion << endl;
    cout << "Version: " << producto.version << endl;
    cout << "Corporacion: " << producto.corporacion << endl;
    cout << "Nombre de la empresa: " << producto.nombreEmpresa << endl;
    cout << "Contacto: " << producto.contacto << endl;
    cout << "RUC: " << producto.ruc << endl;
}


void mostrarTodosLosProductos(const vector<Producto>& productos) {
    for (const auto& producto : productos) {
        mostrarDatosProducto(producto);
        cout << "------------------------\n";
    }
}

//8. ELIMINAR MIEMBRO EQUIPO

void eliminarMiembroPorSeleccion(vector<Equipo>& equipos) {
    string idEquipo;
    cout << "Ingrese el ID del equipo del que desea eliminar un miembro: ";
    cin >> idEquipo;
    cin.ignore();

    for (auto& equipo : equipos) {
        if (equipo.id == idEquipo) {
            cout << "Miembros del equipo " << equipo.nombre << ":\n";
            for (size_t i = 0; i < equipo.miembros.size(); ++i) {
                const auto& miembro = equipo.miembros[i];
                cout << i + 1 << ". " << miembro.nombre << " " << miembro.apellido << " (Rol: " << miembro.rol << ")\n";
            }

            int seleccion;
            cout << "Ingrese el número del miembro que desea eliminar: ";
            cin >> seleccion;
            cin.ignore();

            if (seleccion > 0 && seleccion <= equipo.miembros.size()) {
                equipo.miembros.erase(equipo.miembros.begin() + seleccion - 1);
                cout << "Miembro eliminado con exito.\n";
                guardarEquiposCSV(equipos);  
            } else {
                cout << "Seleccion invalida.\n";
            }
            return;
        }
    }
    cout << "Equipo no encontrado.\n";
}


void guardarEquiposCSV(const vector<Equipo>& equipos);
void guardarProductosCSV(const vector<Producto>& productos);

int main() {
	cambiarColorConsola(0, 15);
    vector<Equipo> equipos;
    vector<Producto> productos;

    cargarEquiposDesdeCSV(equipos);
    cargarProductosDesdeCSV(productos);

    int opcion;
    do {
        limpiarPantalla();  
       	cout << "\n";
       	cout << "Bienvenido al sistema de registro de CodeSolutions\n";
       	cout << "\n";
       	cout << "==================================================\n";
       	cout << "\n";
    	cout << "                ********    ********\n";
    	cout << "                *           *   \n";
    	cout << "                *           *   \n";
    	cout << "                *           ********\n";
    	cout << "                *                   *\n";
    	cout << "                *                   *\n";
    	cout << "                ********    ********\n";
       	cout << "\n";
		cout << "================= Menu Principal =================\n";
       	cout << "\n";
		cout << "Seleccione una opcion: ";
        cout << "\n";
        cout << "\n";
		cout << "1. Registrar equipo\n";
    	cout << "\n";
        cout << "2. Registrar producto\n";
    	cout << "\n";
        cout << "3. Actualizar equipo\n";
    	cout << "\n";
        cout << "4. Actualizar producto\n";
		cout << "\n";
        cout << "5. Eliminar producto\n";
        cout << "\n";
		cout << "6. Mostrar todos los equipos\n";
        cout << "\n";
		cout << "7. Mostrar todos los productos\n";
        cout << "\n";
		cout << "8. Eliminar un miembro de un equipo\n";
        cout << "\n";
		cout << "9. Salir\n";
        cout << "\n";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarEquipo(equipos);
                break;
            case 2:
                registrarProducto(productos);
                break;
            case 3:
                actualizarEquipo(equipos);
                break;
            case 4:
                actualizarProducto(productos);
                break;
            case 5:
                eliminarProducto(productos);
                break;
            case 6:
                mostrarTodosLosEquipos(equipos);
                break;
            case 7:
                mostrarTodosLosProductos(productos);
                break;
            case 8:
                eliminarMiembroPorSeleccion(equipos);
                break;
            case 9:
                cout << "Saliendo del programa...\n";
                break;
            default:
                cout << "Opción no válida. Intente de nuevo.\n";
                break;
        }

        if (opcion != 9) {
            cout << "Presione Enter para continuar...";
            cin.ignore();
            cin.get(); 
        }

    } while (opcion != 9);

    return 0;
}
