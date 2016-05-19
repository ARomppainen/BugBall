#pragma once

#include <ostream>
#include <vector>

// Items of type T should overload operator==
// and must implement method "int getHash() const"

// http://www.math.ucla.edu/~wittman/10b.1.10w/Lectures/Lec23.pdf
template <typename T>
class HashTable
{
public:
    HashTable(int tableSize);
    ~HashTable();

    void add(T item);
    void clear();
    T* find(const T& item);
    bool isEmpty() const;
    bool remove(const T& item);
    int size() const;
    std::vector<T> elements();

    //template <typename T>
    //friend std::ostream& operator<<(std::ostream& out, const HashTable<T>& right);

private:

    std::vector<std::vector<T> > table;
    int _size;
};

template <typename T>
HashTable<T>::HashTable(int tableSize)
{
    table.resize(tableSize);
    _size = 0;
}

template <typename T>
HashTable<T>::~HashTable()
{
}

template <typename T>
void HashTable<T>::add(T item)
{
    int index = item.getHash();
    index = index % table.size();
    table[index].push_back(item);
    ++_size;
}

template <typename T>
void HashTable<T>::clear()
{
    table.clear();
    _size = 0;
}

template <typename T>
T* HashTable<T>::find(const T& item)
{
    int index = item.getHash();
    index = index % table.size();

    for (unsigned int i = 0; i < table[index].size(); ++i) {
        if (table[index][i] == item) {
            return &table[index][i];
        }
    }

    return NULL;
}

template <typename T>
bool HashTable<T>::isEmpty() const
{
    return (_size == 0) ? true : false;
}

template <typename T>
bool HashTable<T>::remove(const T& item)
{
    int index = item.getHash();
    index = index % table.size();
    unsigned int i = 0;
    bool found = false;

    while (i < table[index].size()) {
        if (table[index][i] == item) {
            found = true;
            break;
        } else {
            ++i;
        }
    }

    if (found) {

        for (unsigned int j = i; j < table[index].size() - 1; ++j) {
            table[index][j] = table[index][j+1];
        }
    
        table[index].pop_back();
        --_size;
        return true;
    }

    return false;
}

template <typename T>
int HashTable<T>::size() const
{
    return _size;
}

template <typename T>
std::vector<T> HashTable<T>::elements()
{
    std::vector<T> items;

    for (size_t i = 0; i < table.size(); ++i) {
        for (size_t j = 0; j < table[i].size(); ++j) {
            items.push_back(table[i][j]);
        }
    }

    return items;
}

/*
template <typename T>
std::ostream& operator<<(std::ostream& out, const HashTable<T>& right)
{
    for (unsigned int i = 0; i < right.table.size(); ++i) {
        for (unsigned int j = 0; j < right.table[i].size(); ++j) {
            out << "Bucket " << i << ", Item " << j << std::endl;
            out << right.table[i][j] << std::endl << std::endl;
        }
    }

    return out;
}
*/