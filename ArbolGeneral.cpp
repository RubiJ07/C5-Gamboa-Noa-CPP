#include <iostream>
using namespace std;

// ------------------------------------------------------------
// 5.2 ESTRUCTURA DEL NODO
// ------------------------------------------------------------
template <typename T, int M>
struct BTreeNode {
    T keys[M];                   // hasta M claves
    BTreeNode* children[M + 1];  // hasta M+1 hijos
    int nKeys;                   // numero actual de claves
    bool isLeaf;                 // verdadero si es hoja

    BTreeNode() {
        nKeys = 0;
        isLeaf = true;
        for (int i = 0; i <= M; i++)
            children[i] = 0;
    }
};

// ------------------------------------------------------------
// CLASE ARBOL B
// ------------------------------------------------------------
template <typename T, int M>
class BTree {
    typedef BTreeNode<T, M> Node;
    Node* root;

    // --------------------------------------------------------
    // 5.3 BUSQUEDA
    // --------------------------------------------------------
    Node* search(Node* node, const T& key) {
        if (!node) return 0;

        int i = 0;
        while (i < node->nKeys && key > node->keys[i])
            i++;

        if (i < node->nKeys && key == node->keys[i])
            return node;

        if (node->isLeaf)
            return 0;

        return search(node->children[i], key);
    }

    // --------------------------------------------------------
    // SPLIT (OVERFLOW)
    // --------------------------------------------------------
    void splitChild(Node* parent, int index) {
        Node* fullNode = parent->children[index];
        Node* rightNode = new Node();

        int mid = M / 2;

        rightNode->isLeaf = fullNode->isLeaf;
        rightNode->nKeys = M - mid - 1;

        // Copiar claves mayores
        for (int i = 0; i < rightNode->nKeys; i++)
            rightNode->keys[i] = fullNode->keys[i + mid + 1];

        // Copiar hijos si no es hoja
        if (!fullNode->isLeaf) {
            for (int i = 0; i <= rightNode->nKeys; i++)
                rightNode->children[i] =
                    fullNode->children[i + mid + 1];
        }

        fullNode->nKeys = mid;

        // Desplazar hijos del padre
        for (int i = parent->nKeys; i >= index + 1; i--)
            parent->children[i + 1] = parent->children[i];

        parent->children[index + 1] = rightNode;

        // Desplazar claves del padre
        for (int i = parent->nKeys - 1; i >= index; i--)
            parent->keys[i + 1] = parent->keys[i];

        parent->keys[index] = fullNode->keys[mid];
        parent->nKeys++;
    }

    // --------------------------------------------------------
    // INSERCION EN NODO NO LLENO
    // --------------------------------------------------------
    void insertNonFull(Node* node, const T& key) {
        int i = node->nKeys - 1;

        if (node->isLeaf) {
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                i--;
            }
            node->keys[i + 1] = key;
            node->nKeys++;
        } else {
            while (i >= 0 && key < node->keys[i])
                i--;
            i++;

            if (node->children[i]->nKeys == M) {
                splitChild(node, i);
                if (key > node->keys[i])
                    i++;
            }
            insertNonFull(node->children[i], key);
        }
    }

    // --------------------------------------------------------
    // RECORRIDO INORDEN GENERAL
    // --------------------------------------------------------
    void traverse(Node* node) {
        int i;
        for (i = 0; i < node->nKeys; i++) {
            if (!node->isLeaf)
                traverse(node->children[i]);
            cout << node->keys[i] << " ";
        }
        if (!node->isLeaf)
            traverse(node->children[i]);
    }

public:
    // --------------------------------------------------------
    // CONSTRUCTOR
    // --------------------------------------------------------
    BTree() {
        root = 0;
    }

    // --------------------------------------------------------
    // INSERCION PUBLICA (5.4)
    // --------------------------------------------------------
    void insert(const T& key) {
        if (!root) {
            root = new Node();
            root->keys[0] = key;
            root->nKeys = 1;
        } else {
            if (root->nKeys == M) {
                Node* newRoot = new Node();
                newRoot->isLeaf = false;
                newRoot->children[0] = root;

                splitChild(newRoot, 0);

                int i = (key > newRoot->keys[0]) ? 1 : 0;
                insertNonFull(newRoot->children[i], key);

                root = newRoot;
            } else {
                insertNonFull(root, key);
            }
        }
    }

    // --------------------------------------------------------
    // BUSQUEDA PUBLICA
    // --------------------------------------------------------
    bool search(const T& key) {
        return search(root, key) != 0;
    }

    // --------------------------------------------------------
    // MOSTRAR ARBOL
    // --------------------------------------------------------
    void print() {
        if (root)
            traverse(root);
        cout << endl;
    }
};

// ------------------------------------------------------------
// MAIN DE DEMOSTRACION
// ------------------------------------------------------------
int main() {
    BTree<char, 3> tree;

    tree.insert('5');
    tree.insert('7');
    tree.insert('C');
    tree.insert('M');
    tree.insert('R');
    tree.insert('8');
    tree.insert('9');
    tree.insert('A');
    tree.insert('B');
    tree.insert('D');
    tree.insert('Z');

    cout << "Recorrido del Arbol B: ";
    tree.print();

    cout << "Buscar C: " << tree.search('C') << endl;
    cout << "Buscar X: " << tree.search('X') << endl;

    return 0;
}

