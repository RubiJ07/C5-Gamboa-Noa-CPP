#include <iostream>

// -------------------------------------------------------------------
// ESTRUCTURA DEL NODO
// -------------------------------------------------------------------

template <typename T>
struct BSTNode {
    T data;
    BSTNode* pChild[2]; // [0]=izquierda, [1]=derecha

    BSTNode(const T& val)
        : data(val) { pChild[0] = pChild[1] = nullptr; }

    T& GetDataRef() { return data; }
    const T& GetDataRef() const { return data; }
};

// -------------------------------------------------------------------
// TRAITS PARA TIPOS DE DATOS
// -------------------------------------------------------------------

struct IntTraits {
    using value_type = int;
};

struct DoubleTraits {
    using value_type = double;
};

struct CharTraits {
    using value_type = char;
};

// -------------------------------------------------------------------
// CLASE CBINARYTREE CON TEMPLATES VARIADICOS
// -------------------------------------------------------------------

template <typename Traits>
class CBinaryTree {
public:
    using T = typename Traits::value_type;
    using Node = BSTNode<T>;

private:
    Node* m_pRoot;

    // Metodos internos recursivos (privados)
    Node* internalInsert(Node* node, const T& val);
    Node* internalRemove(Node* node, const T& val);
    Node* internalSearch(Node* node, const T& val) const;
    Node* findMin(Node* node) const;

    template <typename Func, typename ...Args>
    void internalInorden(Node* pNode, Func fn, Args... args);

    template <typename Func, typename ...Args>
    void internalInordenConst(Node* pNode, Func fn, Args... args) const;

    void internalClear(Node* node);

public:
    CBinaryTree() : m_pRoot(nullptr) {}
    ~CBinaryTree() { clear(); }

    void insert(const T& val);
    bool remove(const T& val);
    bool search(const T& val) const;
    Node* getRoot() const { return m_pRoot; }  // Getter para la raíz

    template <typename Func, typename ...Args>
    void inorden(Func fn, Args... args);

    template <typename Func, typename ...Args>
    void inorden(Func fn, Args... args) const;

    void clear();
    bool empty() const { return m_pRoot == nullptr; }
};

// -------------------------------------------------------------------
// IMPLEMENTACION DE METODOS PRIVADOS
// -------------------------------------------------------------------

// Inserción
template <typename Traits>
auto CBinaryTree<Traits>::internalInsert(Node* node, const T& val) -> Node* {
    if (!node) return new Node(val);

    if (val < node->data)
        node->pChild[0] = internalInsert(node->pChild[0], val);
    else if (val > node->data)
        node->pChild[1] = internalInsert(node->pChild[1], val);
    // si val == node->data, no se inserta (sin duplicados)

    return node;
}

// Busqueda
template <typename Traits>
auto CBinaryTree<Traits>::internalSearch(Node* node, const T& val) const -> Node* {
    if (!node) return nullptr;
    if (val == node->data) return node;
    if (val < node->data)
        return internalSearch(node->pChild[0], val);
    return internalSearch(node->pChild[1], val);
}

// Encontrar minimo
template <typename Traits>
auto CBinaryTree<Traits>::findMin(Node* node) const -> Node* {
    if (!node) return nullptr;
    while (node->pChild[0]) node = node->pChild[0];
    return node;
}

// Eliminacion
template <typename Traits>
auto CBinaryTree<Traits>::internalRemove(Node* node, const T& val) -> Node* {
    if (!node) return nullptr;

    if (val < node->data)
        node->pChild[0] = internalRemove(node->pChild[0], val);
    else if (val > node->data)
        node->pChild[1] = internalRemove(node->pChild[1], val);
    else {
        // Caso 1: hoja o un solo hijo
        if (!node->pChild[0]) {
            Node* tmp = node->pChild[1];
            delete node;
            return tmp;
        }
        if (!node->pChild[1]) {
            Node* tmp = node->pChild[0];
            delete node;
            return tmp;
        }
        // Caso 2: dos hijos
        Node* succ = findMin(node->pChild[1]);
        node->data = succ->data;
        node->pChild[1] = internalRemove(node->pChild[1], succ->data);
    }
    return node;
}

// Recorrido inorden con templates variadicos (versión no-const)
template <typename Traits>
template <typename Func, typename ...Args>
void CBinaryTree<Traits>::internalInorden(Node* pNode, Func fn, Args... args) {
    if (pNode) {
        internalInorden(pNode->pChild[0], fn, args...);
        fn(pNode->GetDataRef(), args...);
        internalInorden(pNode->pChild[1], fn, args...);
    }
}

// Recorrido inorden con templates variadicos (versión const)
template <typename Traits>
template <typename Func, typename ...Args>
void CBinaryTree<Traits>::internalInordenConst(Node* pNode, Func fn, Args... args) const {
    if (pNode) {
        internalInordenConst(pNode->pChild[0], fn, args...);
        fn(pNode->GetDataRef(), args...);
        internalInordenConst(pNode->pChild[1], fn, args...);
    }
}

// Limpiar árbol (eliminar todos los nodos)
template <typename Traits>
void CBinaryTree<Traits>::internalClear(Node* node) {
    if (node) {
        internalClear(node->pChild[0]);
        internalClear(node->pChild[1]);
        delete node;
    }
}

// -------------------------------------------------------------------
// IMPLEMENTACION DE METODOS PUBLICOS
// -------------------------------------------------------------------

template <typename Traits>
void CBinaryTree<Traits>::insert(const T& val) {
    m_pRoot = internalInsert(m_pRoot, val);
}

template <typename Traits>
bool CBinaryTree<Traits>::remove(const T& val) {
    if (!search(val)) return false;
    m_pRoot = internalRemove(m_pRoot, val);
    return true;
}

template <typename Traits>
bool CBinaryTree<Traits>::search(const T& val) const {
    return internalSearch(m_pRoot, val) != nullptr;
}

template <typename Traits>
template <typename Func, typename ...Args>
void CBinaryTree<Traits>::inorden(Func fn, Args... args) {
    internalInorden(m_pRoot, fn, args...);
}

template <typename Traits>
template <typename Func, typename ...Args>
void CBinaryTree<Traits>::inorden(Func fn, Args... args) const {
    internalInordenConst(m_pRoot, fn, args...);
}

template <typename Traits>
void CBinaryTree<Traits>::clear() {
    internalClear(m_pRoot);
    m_pRoot = nullptr;
}

// -------------------------------------------------------------------
// FUNCIONES DE EJEMPLO PARA RECORRIDOS
// -------------------------------------------------------------------

// Funcion simple para imprimir un nodo
void printNode(int& val) {
    std::cout << val << " ";
}

void printNodeConst(const int& val) {
    std::cout << val << " ";
}

// Funcion con argumentos extra (incrementar y luego imprimir)
void incrementAndPrint(int& val, int inc) {
    val += inc;
    std::cout << val << " ";
}

// Funcion con multiples argumentos extra
void complexOperation(int& val, int a, int b, int c) {
    val = val * a + b - c;
    std::cout << val << " ";
}

// Funcion para acumular valores
void accumulate(int& val, int& sum) {
    sum += val;
}

// -------------------------------------------------------------------
// FUNCION AUXILIAR PARA MOSTRAR ARBOL (CORREGIDA)
// -------------------------------------------------------------------

void printTreeStructure(const std::string& prefix, const BSTNode<int>* node, bool isLeft) {
    if (node != nullptr) {
        std::cout << prefix;
        std::cout << (isLeft ? "+--" : "+--");
        std::cout << node->data << std::endl;

        printTreeStructure(prefix + (isLeft ? "¦   " : "    "), node->pChild[0], true);
        printTreeStructure(prefix + (isLeft ? "¦   " : "    "), node->pChild[1], false);
    }
}

void printTree(const BSTNode<int>* root) {
    if (root) {
        std::cout << root->data << std::endl;
        printTreeStructure("", root->pChild[0], true);
        printTreeStructure("", root->pChild[1], false);
    } else {
        std::cout << "Arbol vacio" << std::endl;
    }
}

// -------------------------------------------------------------------
// MAIN - DEMOSTRACION COMPLETA (CORREGIDO)
// -------------------------------------------------------------------

int main() {
    std::cout << "=== DEMOSTRACION DE ARBOL BINARIO DE BUSQUEDA (BST) ===\n\n";
    
    // Crear arbol con traits para int
    CBinaryTree<IntTraits> tree;

    // Insertar valores
    std::cout << "Insertando valores: 70, 30, 80, 0, 40, 75, 90\n";
    tree.insert(70);
    tree.insert(30);
    tree.insert(80);
    tree.insert(0);
    tree.insert(40);
    tree.insert(75);
    tree.insert(90);

    // Mostrar estructura del árbol (CORREGIDO)
    std::cout << "\nEstructura del arbol:\n";
    printTree(tree.getRoot());  // Usamos el getter en lugar de search

    // Busqueda
    std::cout << "\n--- Pruebas de busqueda ---\n";
    std::cout << "¿Existe 40? " << (tree.search(40) ? "SÍ" : "NO") << std::endl;
    std::cout << "¿Existe 99? " << (tree.search(99) ? "SÍ" : "NO") << std::endl;

    // Recorrido inorden con función simple
    std::cout << "\n--- Recorrido inorden (funcion simple) ---\n";
    std::cout << "Inorden: ";
    tree.inorden(printNode);
    std::cout << std::endl;

    // Recorrido inorden con funcion constante
    std::cout << "\n--- Recorrido inorden con arbol constante ---\n";
    const CBinaryTree<IntTraits>& constTree = tree;
    std::cout << "Inorden (const): ";
    constTree.inorden(printNodeConst);
    std::cout << std::endl;

    // Recorrido con argumentos extra (incrementar)
    std::cout << "\n--- Recorrido con argumentos extra (incrementar en 5) ---\n";
    std::cout << "Antes de incrementar: ";
    tree.inorden(printNode);
    std::cout << "\nDespues de incrementar en 5: ";
    tree.inorden(incrementAndPrint, 5);
    std::cout << std::endl;

    // Recorrido con multiples argumentos extra
    std::cout << "\n--- Recorrido con multiples argumentos (val * 2 + 10 - 3) ---\n";
    std::cout << "Resultado: ";
    tree.inorden(complexOperation, 2, 10, 3);
    std::cout << std::endl;

    // Acumular valores durante el recorrido
    std::cout << "\n--- Acumulacion de valores ---\n";
    int sum = 0;
    tree.inorden(accumulate, sum);
    std::cout << "Suma de todos los valores: " << sum << std::endl;

    // Eliminacion
    std::cout << "\n--- Pruebas de eliminacion ---\n";
    
    std::cout << "Eliminando 40 (nodo hoja):\n";
    tree.remove(40);
    std::cout << "Inorden despues de eliminar 40: ";
    tree.inorden(printNode);
    std::cout << std::endl;

    std::cout << "\nEliminando 80 (nodo con un hijo):\n";
    tree.remove(80);
    std::cout << "Inorden despues de eliminar 80: ";
    tree.inorden(printNode);
    std::cout << std::endl;

    std::cout << "\nEliminando 70 (nodo con dos hijos - raiz):\n";
    tree.remove(70);
    std::cout << "Inorden despues de eliminar 70: ";
    tree.inorden(printNode);
    std::cout << std::endl;

    // Insertar mas valores para demostrar versatilidad
    std::cout << "\n--- Insertando nuevos valores ---\n";
    tree.insert(35);
    tree.insert(85);
    tree.insert(25);
    std::cout << "Inorden despues de insertar 35, 85, 25: ";
    tree.inorden(printNode);
    std::cout << std::endl;

    // Demostracion con otros tipos de datos
    std::cout << "\n=== DEMOSTRACION CON OTROS TIPOS DE DATOS ===\n";
    
    // Arbol de doubles
    CBinaryTree<DoubleTraits> doubleTree;
    doubleTree.insert(3.14);
    doubleTree.insert(2.71);
    doubleTree.insert(1.41);
    doubleTree.insert(1.73);
    
    std::cout << "\nArbol de doubles (inorden): ";
    doubleTree.inorden([](double& val) { std::cout << val << " "; });
    std::cout << std::endl;

    // Arbol de caracteres
    CBinaryTree<CharTraits> charTree;
    charTree.insert('M');
    charTree.insert('A');
    charTree.insert('R');
    charTree.insert('B');
    charTree.insert('C');
    
    std::cout << "Arbol de caracteres (inorden): ";
    charTree.inorden([](char& val) { std::cout << val << " "; });
    std::cout << std::endl;

    return 0;
}

