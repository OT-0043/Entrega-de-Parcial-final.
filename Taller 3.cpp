#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Función para convertir string a int
int stringToInt(const string& str) {
    int result = 0;
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c >= '0' && c <= '9') {
            result = result * 10 + (c - '0');
        }
    }
    return result;
}

// Función para convertir string a double
double stringToDouble(const string& str) {
    double result = 0.0;
    double decimal = 0.0;
    bool afterDecimal = false;
    double decimalMultiplier = 0.1;
    
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c == '.') {
            afterDecimal = true;
        } else if (c >= '0' && c <= '9') {
            if (!afterDecimal) {
                result = result * 10 + (c - '0');
            } else {
                decimal += (c - '0') * decimalMultiplier;
                decimalMultiplier *= 0.1;
            }
        }
    }
    return result + decimal;
}

struct Producto {
    int idProducto;
    string nombre;
    string descripcion;
    double precio;
    int stock;
};

struct Usuario {
    int idUsuario;
    string nombre;
    string correoElectronico;
    string contrasena;
    string direccion;
    string metodoDePago;
};

struct Comentario {
    int idComentario;
    string producto;
    string usuario;
    string comentario;
    string fecha;
};

struct CarritoDeCompras {
    int idCarrito;
    vector<Producto> productos;
    double subtotal;
    double impuestos;
};

vector<Usuario> usuarios;
vector<Producto> productos;
vector<Comentario> comentarios;
CarritoDeCompras carrito;
Usuario usuarioActual;

// Función para comparar productos por stock (ascendente)
bool compararPorStock(const Producto& a, const Producto& b) {
    return a.stock < b.stock;
}

// Mostrar los 5 productos con menor stock
void mostrarProductosMenorStock() {
    cout << "\n=== 5 PRODUCTOS CON MENOR STOCK ===" << endl;
    
   
    vector<Producto> productosOrdenados = productos;
    
    // Ordenar por stock ascendente
    sort(productosOrdenados.begin(), productosOrdenados.end(), compararPorStock);
    
   
    int limite = min(5, (int)productosOrdenados.size());
    
    for (int i = 0; i < limite; i++) {
        cout << "ID: " << productosOrdenados[i].idProducto 
             << " | Nombre: " << productosOrdenados[i].nombre
             << " | Stock: " << productosOrdenados[i].stock << endl;
    }
}

//Mostrar cantidad de comentarios en una fecha específica
void mostrarComentariosPorFecha() {
    string fecha;
    cout << "\nIngrese la fecha (YYYY-MM-DD): ";
    cin >> fecha;
    
    int cantidad = 0;
    
    // Contar comentarios en la fecha especificada
    for (size_t i = 0; i < comentarios.size(); i++) {
        if (comentarios[i].fecha == fecha) {
            cantidad++;
        }
    }
    
    cout << "\n=== COMENTARIOS EN LA FECHA " << fecha << " ===" << endl;
    cout << "Fecha: " << fecha << " | Cantidad de comentarios: " << cantidad << endl;
    
    
}

// Mostrar precio máximo y mínimo de los productos
void mostrarPrecioMaximoMinimo() {
    if (productos.empty()) {
        cout << "\nNo hay productos cargados." << endl;
        return;
    }
    
    double precioMin = productos[0].precio;
    double precioMax = productos[0].precio;
    Producto productoMin = productos[0];
    Producto productoMax = productos[0];
    
    
    for (size_t i = 1; i < productos.size(); i++) {
        if (productos[i].precio < precioMin) {
            precioMin = productos[i].precio;
            productoMin = productos[i];
        }
        if (productos[i].precio > precioMax) {
            precioMax = productos[i].precio;
            productoMax = productos[i];
        }
    }
    
    cout << "\n=== PRECIO MAXIMO Y MINIMO DE PRODUCTOS ===" << endl;
    cout << "Precio Minimo: $" << precioMin << endl;
    cout << "  - Producto: " << productoMin.nombre << " (ID: " << productoMin.idProducto << ")" << endl;
    cout << "Precio Maximo: $" << precioMax << endl;
    cout << "  - Producto: " << productoMax.nombre << " (ID: " << productoMax.idProducto << ")" << endl;
}

void cargarUsuarios() {
    ifstream archivo("Usuarios.txt");
    string linea;
    
    // Leer todas las líneas
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string token;
        Usuario u;
        
        // Leer idUsuario
        if (getline(ss, token, ',')) {
            u.idUsuario = stringToInt(token);
        }
        
        // Leer nombre
        if (getline(ss, u.nombre, ',')) {
            if (!u.nombre.empty() && u.nombre[0] == ' ') {
                u.nombre = u.nombre.substr(1);
            }
        }
        
        // Leer correoElectronico
        if (getline(ss, u.correoElectronico, ',')) {
            if (!u.correoElectronico.empty() && u.correoElectronico[0] == ' ') {
                u.correoElectronico = u.correoElectronico.substr(1);
            }
        }
        
        // Leer contrasena
        if (getline(ss, u.contrasena, ',')) {
            if (!u.contrasena.empty() && u.contrasena[0] == ' ') {
                u.contrasena = u.contrasena.substr(1);
            }
        }
        
        // Leer direccion
        if (getline(ss, u.direccion, ',')) {
            if (!u.direccion.empty() && u.direccion[0] == ' ') {
                u.direccion = u.direccion.substr(1);
            }
        }
        
        // Leer metodoDePago
        if (getline(ss, u.metodoDePago)) {
            if (!u.metodoDePago.empty() && u.metodoDePago[0] == ' ') {
                u.metodoDePago = u.metodoDePago.substr(1);
            }
        }
        
        usuarios.push_back(u);
    }
    archivo.close();
}

void cargarProductos() {
    ifstream archivo("Productos.txt");
    string linea;
    
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string token;
        Producto p;
        
        // Leer idProducto
        if (getline(ss, token, ',')) {
            p.idProducto = stringToInt(token);
        }
        
        // Leer nombre
        if (getline(ss, p.nombre, ',')) {
            if (!p.nombre.empty() && p.nombre[0] == ' ') {
                p.nombre = p.nombre.substr(1);
            }
        }
        
        // Leer descripcion
        if (getline(ss, p.descripcion, ',')) {
            if (!p.descripcion.empty() && p.descripcion[0] == ' ') {
                p.descripcion = p.descripcion.substr(1);
            }
        }
        
        // Leer precio
        if (getline(ss, token, ',')) {
            p.precio = stringToDouble(token);
        }
        
        // Leer stock
        if (getline(ss, token)) {
            p.stock = stringToInt(token);
        }
        
        productos.push_back(p);
    }
    archivo.close();
}

void cargarComentarios() {
    ifstream archivo("Comentarios.txt");
    string linea;
    
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string token;
        Comentario c;
        
        // Leer idComentario
        if (getline(ss, token, ',')) {
            c.idComentario = stringToInt(token);
        }
        
        // Leer producto
        if (getline(ss, c.producto, ',')) {
            if (!c.producto.empty() && c.producto[0] == ' ') {
                c.producto = c.producto.substr(1);
            }
        }
        
        // Leer usuario
        if (getline(ss, c.usuario, ',')) {
            if (!c.usuario.empty() && c.usuario[0] == ' ') {
                c.usuario = c.usuario.substr(1);
            }
        }
        
        // Leer comentario
        if (getline(ss, c.comentario, ',')) {
            if (!c.comentario.empty() && c.comentario[0] == ' ') {
                c.comentario = c.comentario.substr(1);
            }
        }
        
        // Leer fecha
        if (getline(ss, c.fecha)) {
            if (!c.fecha.empty() && c.fecha[0] == ' ') {
                c.fecha = c.fecha.substr(1);
            }
        }
        
        comentarios.push_back(c);
    }
    archivo.close();
}

bool iniciarSesion() {
    string correo, contrasena;
    cout << "=== INICIAR SESION ===" << endl;
    cout << "Correo electronico: ";
    cin >> correo;
    cout << "Contrasena: ";
    cin >> contrasena;
    
    for (size_t i = 0; i < usuarios.size(); i++) {
        if (usuarios[i].correoElectronico == correo && usuarios[i].contrasena == contrasena) {
            usuarioActual = usuarios[i];
            cout << "\nBienvenido, " << usuarios[i].nombre << "!" << endl;
            return true;
        }
    }
    
    cout << "\nUsuario invalido!" << endl;
    cout << "Verifica que hayas escrito correctamente el correo y contrasena." << endl;
    return false;
}

void listarProductosBajoStock() {
    cout << "\n=== PRODUCTOS CON STOCK BAJO (<15 unidades) ===" << endl;
    bool encontrados = false;
    
    for (size_t i = 0; i < productos.size(); i++) {
        if (productos[i].stock < 15) {
            cout << "ID: " << productos[i].idProducto << " | " << productos[i].nombre
                 << " | Stock: " << productos[i].stock << " | Precio: $" << productos[i].precio << endl;
            encontrados = true;
        }
    }
    
    if (!encontrados) {
        cout << "No hay productos con stock bajo." << endl;
    }
}

void listarComentariosDesdeFecha() {
    string fecha;
    cout << "\nIngrese la fecha (YYYY-MM-DD): ";
    cin >> fecha;
    
    cout << "\n=== COMENTARIOS DESDE " << fecha << " ===" << endl;
    bool encontrados = false;
    
    for (size_t i = 0; i < comentarios.size(); i++) {
        if (comentarios[i].fecha >= fecha) {
            cout << "Fecha: " << comentarios[i].fecha << " | Producto: " << comentarios[i].producto
                 << " | Usuario: " << comentarios[i].usuario << endl;
            cout << "Comentario: " << comentarios[i].comentario << endl << endl;
            encontrados = true;
        }
    }
    
    if (!encontrados) {
        cout << "No hay comentarios desde esa fecha." << endl;
    }
}

void listarUsuarios() {
    cout << "\n=== LISTA DE USUARIOS ===" << endl;
    for (size_t i = 0; i < usuarios.size(); i++) {
        cout << "ID: " << usuarios[i].idUsuario << " | Nombre: " << usuarios[i].nombre
             << " | Correo: " << usuarios[i].correoElectronico << " | Direccion: "
             << usuarios[i].direccion << " | Metodo Pago: " << usuarios[i].metodoDePago << endl;
    }
}

void agregarAlCarrito() {
    int id;
    cout << "\n=== AGREGAR AL CARRITO ===" << endl;
    cout << "ID del producto: ";
    cin >> id;
   
    for (size_t i = 0; i < productos.size(); i++) {
        if (productos[i].idProducto == id) {
            carrito.productos.push_back(productos[i]);
            carrito.subtotal += productos[i].precio;
            cout << "Producto '" << productos[i].nombre << "' agregado al carrito!" << endl;
            return;
        }
    }
    cout << "Producto no encontrado!" << endl;
}

void listarCarrito() {
    cout << "\n=== CARRITO DE COMPRAS ===" << endl;
    if (carrito.productos.empty()) {
        cout << "El carrito esta vacio." << endl;
        return;
    }
   
    for (size_t i = 0; i < carrito.productos.size(); i++) {
        cout << "Producto: " << carrito.productos[i].nombre << " | Precio: $" << carrito.productos[i].precio << endl;
    }
    cout << "Subtotal: $" << carrito.subtotal << endl;
}

void guardarCarrito() {
    string nombreArchivo = "carrito_" + string(1, '0' + usuarioActual.idUsuario) + ".txt";
    ofstream archivo(nombreArchivo.c_str());
    archivo << "Carrito de: " << usuarioActual.nombre << endl;
    archivo << "Productos en carrito:" << endl;
   
    for (size_t i = 0; i < carrito.productos.size(); i++) {
        archivo << carrito.productos[i].nombre << " - $" << carrito.productos[i].precio << endl;
    }
   
    archivo << "Subtotal: $" << carrito.subtotal << endl;
    archivo.close();
    cout << "Carrito guardado en archivo!" << endl;
}

void menuPrincipal() {
    int opcion;
   
    do {
        cout << "\n=== MENU PRINCIPAL ===" << endl;
        cout << "1. Listar productos con stock bajo" << endl;
        cout << "2. Listar comentarios desde fecha" << endl;
        cout << "3. Listar usuarios" << endl;
        cout << "4. Agregar producto al carrito" << endl;
        cout << "5. Listar carrito" << endl;
        cout << "6. Guardar carrito en archivo" << endl;
        cout << "7. Reprotes" << endl;
        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
       
        switch (opcion) {
            case 1: listarProductosBajoStock(); break;
            case 2: listarComentariosDesdeFecha(); break;
            case 3: listarUsuarios(); break;
            case 4: agregarAlCarrito(); break;
            case 5: listarCarrito(); break;
            case 6: guardarCarrito(); break;
            case 7: //opcion que se agrego.
                {
                    int opcionReporte;
                    cout << "\n=== REPORTES ===" << endl;
                    cout << "1. 5 productos con menor stock" << endl;
                    cout << "2. Cantidad de comentarios por fecha" << endl;
                    cout << "3. Precio maximo y minimo de productos" << endl;
                    cout << "0. Volver al menu principal" << endl;
                    cout << "Seleccione una opcion: ";
                    cin >> opcionReporte;
                    
                    switch (opcionReporte) {
                        case 1: mostrarProductosMenorStock(); break;
                        case 2: mostrarComentariosPorFecha(); break;
                        case 3: mostrarPrecioMaximoMinimo(); break;
                        case 0: break;
                        default: cout << "Opcion invalida!" << endl;
                    }
                }
                break;
            case 0: cout << "Gracias." << endl; break;
            default: cout << "Opcion invalida!" << endl;
        }
    } while (opcion != 0);
}

int main() {
    // Inicializar carrito
    carrito.idCarrito = 1;
    carrito.subtotal = 0;
    carrito.impuestos = 0;
   
    // Cargar datos
    cargarUsuarios();
    cargarProductos();
    cargarComentarios();
   
    // Sistema de login
    if (iniciarSesion()) {
        menuPrincipal();
    }
   
    return 0;
}
