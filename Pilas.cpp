/* 
#include <iostream>
#include <stdexcept>
using namespace std;

template <typename T>
class CArray {
protected:
    T* m_data;
    int m_size;
    int m_capacity;

public:
    CArray() : m_data(nullptr), m_size(0), m_capacity(0) {}
    ~CArray() { delete[] m_data; }

    void resize(int newCap) {
        T* tmp = new T[newCap];
        for (int i = 0; i < m_size; i++)
            tmp[i] = m_data[i];

        delete[] m_data;
        m_data = tmp;
        m_capacity = newCap;
    }

    int size() const { return m_size; }
    bool empty() const { return m_size == 0; }
};

template <typename T>
class CStack : public CArray<T> {

public:

    void push(const T& val) {
        if (this->m_size == this->m_capacity)
            this->resize(this->m_capacity == 0 ? 4 : this->m_capacity * 2);

        this->m_data[this->m_size++] = val;
    }

    T pop() {
        if (this->empty())
            throw underflow_error("Pila vacia");

        return this->m_data[--this->m_size];
    }

    T& top() {
        if (this->empty())
            throw underflow_error("Pila vacia");

        return this->m_data[this->m_size - 1];
    }

    int search(const T& val) const {
        for (int i = this->m_size - 1; i >= 0; i--)
            if (this->m_data[i] == val)
                return (this->m_size - 1 - i);

        return -1;
    }

    bool removeByValue(const T& val) {
        for (int i = this->m_size - 1; i >= 0; i--) {
            if (this->m_data[i] == val) {
                for (int j = i; j < this->m_size - 1; j++)
                    this->m_data[j] = this->m_data[j + 1];

                this->m_size--;
                return true;
            }
        }
        return false;
    }

    T removeByIndex(int idx) {
        int realIdx = this->m_size - 1 - idx;

        if (realIdx < 0 || realIdx >= this->m_size)
            throw out_of_range("Indice fuera de rango");

        T val = this->m_data[realIdx];

        for (int j = realIdx; j < this->m_size - 1; j++)
            this->m_data[j] = this->m_data[j + 1];

        this->m_size--;
        return val;
    }
};

int main() {
    CStack<int> pila;

    cout << "Insertando: 10, 20, 30\n";
    pila.push(10);
    pila.push(20);
    pila.push(30);

    cout << "Top: " << pila.top() << endl;
    cout << "Search 10 (distancia desde tope): "
         << pila.search(10) << endl;

    cout << "Pop: " << pila.pop() << endl;

    cout << "Eliminando valor 10\n";
    pila.removeByValue(10);

    cout << "Nuevo top: " << pila.top() << endl;

    cout << "Insertando 40 y 50\n";
    pila.push(40);
    pila.push(50);

    cout << "Eliminar por indice (0=tope): "
         << pila.removeByIndex(0) << endl;

    cout << "Top final: " << pila.top() << endl;

    return 0;
}
*/

#include <iostream>
#include <string>
using namespace std;

// -------------------------------------------------------------------
// CLASE DE EJEMPLO
// -------------------------------------------------------------------

class Persona {
public:
    string nombre;
    int edad;

    Persona(string n = "", int e = 0) : nombre(n), edad(e) {}

    void mostrar() const {
        cout << "Nombre: " << nombre << " | Edad: " << edad << endl;
    }

    void cumplirAnios() {
        edad++;
    }

    void sumarEdad(int x) {
        edad += x;
    }

    void resetearEdad() {
        edad = 0;
    }
};

// -------------------------------------------------------------------
// FUNCIONES AUXILIARES PARA TRABAJAR CON PUNTEROS A MIEMBROS
// -------------------------------------------------------------------

void ejecutarMetodo(Persona& p, void (Persona::*metodo)()) {
    (p.*metodo)();
}

void ejecutarMetodoConArg(Persona& p,
                          void (Persona::*metodo)(int),
                          int valor) {
    (p.*metodo)(valor);
}

// -------------------------------------------------------------------
// MAIN - DEMOSTRACIÓN COMPLETA
// -------------------------------------------------------------------

int main() {
    cout << "=== DEMOSTRACION DE PUNTEROS A MIEMBROS ===\n\n";

    Persona p1("Ana", 20);

    // ---------------------------------------------------------------
    // 1. PUNTERO A ATRIBUTO
    // ---------------------------------------------------------------
    int Persona::*ptrEdad = &Persona::edad;
    string Persona::*ptrNombre = &Persona::nombre;

    cout << "Acceso por puntero a atributo:\n";
    cout << "Nombre: " << p1.*ptrNombre << endl;
    cout << "Edad: " << p1.*ptrEdad << endl;

    // Modificacion usando puntero
    p1.*ptrEdad = 25;
    cout << "Edad modificada via puntero: " << p1.edad << endl;

    // ---------------------------------------------------------------
    // 2. PUNTERO A METODO (sin parametros)
    // ---------------------------------------------------------------
    void (Persona::*ptrMetodo)() = &Persona::cumplirAnios;

    cout << "\nInvocando metodo cumplirAnios() via puntero\n";
    (p1.*ptrMetodo)();
    p1.mostrar();

    // ---------------------------------------------------------------
    // 3. PUNTERO A METODO (con parametros)
    // ---------------------------------------------------------------
    void (Persona::*ptrMetodoArg)(int) = &Persona::sumarEdad;

    cout << "\nInvocando metodo sumarEdad(5) via puntero\n";
    (p1.*ptrMetodoArg)(5);
    p1.mostrar();

    // ---------------------------------------------------------------
    // 4. ARREGLO DE PUNTEROS A METODOS
    // ---------------------------------------------------------------
    void (Persona::*acciones[3])();

    acciones[0] = &Persona::cumplirAnios;
    acciones[1] = &Persona::resetearEdad;
    acciones[2] = &Persona::cumplirAnios;

    cout << "\nEjecutando arreglo de punteros a metodos:\n";
    for (int i = 0; i < 3; i++) {
        (p1.*acciones[i])();
        p1.mostrar();
    }

    // ---------------------------------------------------------------
    // 5. USO DE FUNCIONES GENERICAS CON PUNTEROS A METODOS
    // ---------------------------------------------------------------
    cout << "\nUso de funciones auxiliares:\n";

    ejecutarMetodo(p1, &Persona::cumplirAnios);
    ejecutarMetodoConArg(p1, &Persona::sumarEdad, 10);
    p1.mostrar();

    cout << "\n=== FIN DE DEMOSTRACION ===\n";
    return 0;
}

