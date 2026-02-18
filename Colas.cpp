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
    ~CArray(){ delete[] m_data; }

    void resize(int newCap){
        T* tmp = new T[newCap];
        for(int i=0;i<m_size;i++)
            tmp[i]=m_data[i];

        delete[] m_data;
        m_data=tmp;
        m_capacity=newCap;
    }

    int size() const { return m_size; }
    bool empty() const { return m_size==0; }
};

template <typename T>
class CQueue : public CArray<T>{
    int front_i;
    int rear_i;

    void resizeCircular(){
        int newCap = this->m_capacity==0?4:this->m_capacity*2;
        T* tmp = new T[newCap];

        for(int i=0;i<this->m_size;i++)
            tmp[i] = this->m_data[(front_i+i)%this->m_capacity];

        delete[] this->m_data;
        this->m_data = tmp;
        this->m_capacity = newCap;
        front_i = 0;
        rear_i = this->m_size - 1;
    }

public:
    CQueue(){ front_i=0; rear_i=-1; }

    void enqueue(const T& val){
        if(this->m_size == this->m_capacity)
            resizeCircular();

        rear_i = (rear_i+1) % this->m_capacity;
        this->m_data[rear_i] = val;
        this->m_size++;
    }

    T dequeue(){
        if(this->empty())
            throw underflow_error("Cola vacia");

        T val = this->m_data[front_i];
        front_i = (front_i+1) % this->m_capacity;
        this->m_size--;
        return val;
    }

    T& front(){
        if(this->empty())
            throw underflow_error("Cola vacia");
        return this->m_data[front_i];
    }

    int search(const T& val){
        for(int i=0;i<this->m_size;i++)
            if(this->m_data[(front_i+i)%this->m_capacity]==val)
                return i;
        return -1;
    }

    T removeByIndex(int idx){
        if(idx<0 || idx>=this->m_size)
            throw out_of_range("Indice fuera de rango");

        int realIdx = (front_i+idx)%this->m_capacity;
        T val = this->m_data[realIdx];

        for(int i=idx;i<this->m_size-1;i++)
            this->m_data[(front_i+i)%this->m_capacity] =
                this->m_data[(front_i+i+1)%this->m_capacity];

        rear_i = (rear_i-1+this->m_capacity)%this->m_capacity;
        this->m_size--;
        return val;
    }
};

int main(){
    CQueue<int> cola;

    cout<<"Insertando 10,20,30,40\n";
    cola.enqueue(10);
    cola.enqueue(20);
    cola.enqueue(30);
    cola.enqueue(40);

    cout<<"Front: "<<cola.front()<<endl;
    cout<<"Search 30 -> posicion: "<<cola.search(30)<<endl;

    cout<<"Dequeue: "<<cola.dequeue()<<endl;

    cout<<"Eliminar indice 1: "<<cola.removeByIndex(1)<<endl;

    cout<<"Front final: "<<cola.front()<<endl;

    return 0;
}
