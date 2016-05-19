#pragma once

#include "HashTable.h"

// Items of type T should overload operator==
// and must implement method "int getHash() const"

template <typename T>
class HashSet
{
public:
    HashSet(int tableSize);
    ~HashSet();

    bool add(T item);
    void clear();
    bool contains(const T& item);
    T* find(const T& item);
    bool isEmpty() const;
    bool remove(const T& item);
    int size() const;
    std::vector<T> elements();

    //template <typename T>
    //friend std::ostream& operator<<(std::ostream& out, const HashSet<T>& right);

private:

    HashTable<T> table;

};

template <typename T>
HashSet<T>::HashSet(int tableSize) :
    table(tableSize)
{

}

template <typename T>
HashSet<T>::~HashSet()
{

}

template <typename T>
bool HashSet<T>::add(T item)
{
    if (table.find(item) == NULL) {
        table.add(item);
        return true;
    }

    return false;
}

template <typename T>
void HashSet<T>::clear()
{
    table.clear();
}

template <typename T>
bool HashSet<T>::contains(const T& item)
{
    return table.find(item) == NULL ? false : true;
}

template <typename T>
T* HashSet<T>::find(const T& item)
{
    return table.find(item);
}

template <typename T>
bool HashSet<T>::isEmpty() const
{
    return table.isEmpty();
}

template <typename T>
bool HashSet<T>::remove(const T& item)
{
    return table.remove(item);
}

template <typename T>
int HashSet<T>::size() const
{
    return table.size();
}

template <typename T>
std::vector<T> HashSet<T>::elements()
{
    return table.elements();
}

/*
template <typename T>
std::ostream& operator<<(std::ostream& out, const HashSet<T>& right)
{
    out << right.table;
    return out;
}
*/