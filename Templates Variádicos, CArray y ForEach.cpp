#include <iostream>

// -------------------------------------------------------------------
// FUNCIONES DE EJEMPLO (Objetos Funcion)
// -------------------------------------------------------------------

// f1: incrementa en 1 (sin argumentos extra)
template <typename T>
void f1(T &n) {
    n++;
}

// f2: incrementa en p1 (1 argumento extra)
template <typename T>
void f2(T &n, int p1) {
    n += p1;
}

// f3: operación con múltiples argumentos extra
template <typename T>
void f3(T &n, int a, int b, int c, int d) {
    n = n * a + b - c + d;
}

// -------------------------------------------------------------------
// CLASE CARRAY CON FOREACH VARIADICO
// -------------------------------------------------------------------

template <typename T>
class CArray {
    T* m_data;
    int m_size;
    int m_capacity;

public:
    using value_type = T;

    // Iterador simple (puntero)
    T* begin() { return m_data; }
    T* end()   { return m_data + m_size; }
    
    // Const iterators para funciones const
    const T* begin() const { return m_data; }
    const T* end() const   { return m_data + m_size; }

    CArray() : m_data(nullptr), m_size(0), m_capacity(0) {}
    ~CArray() { delete[] m_data; }

    void push_back(const T& val) {
        if (m_size == m_capacity) {
            int newCap = m_capacity == 0 ? 4 : m_capacity * 2;
            T* tmp = new T[newCap];
            for (int i = 0; i < m_size; i++) tmp[i] = m_data[i];
            delete[] m_data;
            m_data = tmp;
            m_capacity = newCap;
        }
        m_data[m_size++] = val;
    }

    // BUSQUEDA por valor
    int search(const T& val) const {
        for (int i = 0; i < m_size; i++)
            if (m_data[i] == val) return i;
        return -1;
    }

    // ELIMINACION por indice
    void removeAt(int idx) {
        for (int i = idx; i < m_size - 1; i++)
            m_data[i] = m_data[i + 1];
        m_size--;
    }

    // ELIMINACION por valor
    bool removeByValue(const T& val) {
        int idx = search(val);
        if (idx == -1) return false;
        removeAt(idx);
        return true;
    }

    int size() const { return m_size; }
    
    // Acceso a elementos (para depuración)
    T& operator[](int index) { return m_data[index]; }
    const T& operator[](int index) const { return m_data[index]; }

    // ?? FOREACH VARIÁDICO ??
    template <typename ObjFunc, typename ...Args>
    void Foreach(ObjFunc fn, Args... args) {
        auto iter = begin();
        for (; iter != end(); ++iter)
            fn(*iter, args...);
    }
};

// -------------------------------------------------------------------
// FOREACH GENERICO CON CONTENEDOR E ITERADORES
// -------------------------------------------------------------------

// Nivel alto: recibe contenedor completo
template <typename Container, typename FuncObj, typename ...Args>
void ForEach(Container &cont, FuncObj fn, Args... args) {
    auto iter = cont.begin();
    for (; iter != cont.end(); ++iter)
        fn(*iter, args...);
}

// Nivel bajo: recibe iteradores explícitamente
template <typename Iterator, typename FuncObj, typename ...Args>
void ForEach(Iterator begin, Iterator end,
             FuncObj fn, Args... args) {
    for (; begin != end; ++begin)
        fn(*begin, args...);
}

// -------------------------------------------------------------------
// FUNCION PARA IMPRIMIR ARREGLO
// -------------------------------------------------------------------

template <typename T>
void printArray(const CArray<T>& arr, const std::string& msg) {
    std::cout << msg << ": [";
    for (int i = 0; i < arr.size(); i++) {
        std::cout << arr[i];
        if (i < arr.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

// -------------------------------------------------------------------
// MAIN - DEMOSTRACION
// -------------------------------------------------------------------

int main() {
    std::cout << "=== DEMOSTRACION DE TEMPLATES VARIADICOS Y FOREACH ===\n\n";
    
    CArray<int> arr;
    arr.push_back(7);
    arr.push_back(4);
    arr.push_back(9);
    arr.push_back(2);
    arr.push_back(5);

    printArray(arr, "Arreglo original");

    // Caso 1: fn sin argumentos extra
    arr.Foreach(f1<int>);
    printArray(arr, "Despues de f1 (incrementar en 1)");

    // Caso 2: fn con 1 argumento extra
    arr.Foreach(f2<int>, 5);
    printArray(arr, "Despues de f2 (incrementar en 5)");

    // Caso 3: fn con 4 argumentos extra
    arr.Foreach(f3<int>, 7, 4, 8, 14);
    printArray(arr, "Despues de f3 (elem * 7 + 4 - 8 + 14)");

    // Demostracion de ForEach generico (nivel alto)
    std::cout << "\n--- Demostracion de ForEach generico ---\n";
    CArray<int> arr2;
    arr2.push_back(1);
    arr2.push_back(2);
    arr2.push_back(3);
    
    printArray(arr2, "arr2 original");
    ForEach(arr2, f2<int>, 10);
    printArray(arr2, "arr2 despues de ForEach genérico (+10)");

    // Demostracion de ForEach con iteradores (nivel bajo)
    std::cout << "\n--- Demostracion de ForEach con iteradores ---\n";
    ForEach(arr2.begin(), arr2.end(), f1<int>);
    printArray(arr2, "arr2 despues de ForEach con iteradores (+1)");

    return 0;
}

