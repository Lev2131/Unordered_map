#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

template<class Key_type, class Value_type, class Hash = hash<Key_type>>
class Unordered_map {
private:
    class node;
    class iterator;
    list<node> elements;
    vector<vector<typename list<node>::iterator>> table;
    const float max_load_factor = float(1.05);
    const size_t MULTIPLICATION_FACTOR = 2;
    Hash hasher;
public:
    class node {
    private:
        size_t cache;
    public:
        Key_type key;
        Value_type value;
        node(Key_type key = Key_type(), Value_type value = Value_type(), size_t cache = size_t()) {
            this->cache = cache;
            this->key = key;
            this->value = value;
        }
        size_t get_cache() {
            return cache;
        }
    };

    bool update_capacity() {
        if (elements.size() != 0 && size_t(table.size()) / size_t(elements.size()) > max_load_factor) return false;

        size_t N = table.size();
        table.clear();
        N *= MULTIPLICATION_FACTOR;
        if (N == 0) N = 1;
        table.resize(N);

        for (auto it = elements.begin(); it != elements.end(); ++it) {
            table[it->get_cache() % N].push_back(it);
        }
        return true;
    }



    iterator find(pair<Key_type, Value_type> value) {
        if (empty()) return end();
        size_t index_table = hasher(value.first) % table.size();
        for (auto it : table[index_table]) {
            if (it->key == value.first) {
                return iterator(it);
            }

        }
        return end();
    }

    bool empty() {
        return size_t(elements.size()) == 0;
    }

    pair<bool, iterator> insert(pair<Key_type, Value_type> new_element) {

        auto it = find(new_element);

        if (it != end()) return { false, it };
        node new_node(new_element.first, new_element.second, hasher(new_element.first));

        elements.push_back(new_node);

        if (!update_capacity()) {
            table[new_node.get_cache() % size_t(table.size())].push_back(--elements.end());
        }

        return { true, iterator(--elements.end()) };

    }

    void erase(pair<Key_type, Value_type> value) {
        if (empty()) return;
        size_t table_index = hasher(value.first) % table.size();
        for (auto it = table[table_index].begin(); it != table[table_index].end(); ++it) {
            if ((*it)->key == value.first) {
                elements.erase(*it);
                table[table_index].erase(it);
                break;
            }
        }
    }


    Value_type& operator[](const Key_type& key) {
        auto result = insert({ key, Value_type() });
        return result.second->value;
    }



    class iterator {
    private:
        typename list<node>::iterator current;
    public:
        iterator(typename list<node>::iterator it) {
            this->current = it;
        }
        node& operator *() {
            return *this->current;
        }

        node* operator->() {
            return current.operator->();
        }


        iterator& operator++() {
            ++current;
            return *this;
        }

        iterator operator ++(int) {
            auto new_it = *this;
            ++current;
            return new_it;
        }

        bool operator ==(const iterator& other) {
            return current == other.current;
        }

        iterator& operator=(const iterator& other) {
            current = other.current;
            return *this;
        }

        bool operator !=(const iterator& other) {
            return current != other.current;
        }
    };

    iterator begin() {
        return iterator(elements.begin());
    }

    iterator rbegin() {
        return iterator(elements.rbegin());
    }

    iterator end() {
        return iterator(elements.end());
    }
    void print_elements() {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            cout << it->key << " " << it->value << " " << it->get_cache() << "\n";
        }
    }
    void print_table() {
        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i].empty()) {
                cout << "* * * \n";
            }
            else {
                for (auto j : table[i]) {
                    cout << j->key << " " << j->value << " " << j->get_cache() << "\n";
                }
            }
        }
    }
};

void solve() {
    Unordered_map<int, int> mp;
    /*
    for (int i = 1; i < 25; i++) {
        pair<int, int> p = { i - 1, i };
        mp.insert(p);
    }
    */
    mp[1] = 2;
    mp[1] = 3;

    mp.print_elements();
    mp.print_table();
    mp.erase({ 1, 2 });

    //cout << mp.find({ 1, 2 })->key;
}

int main() {
    solve();
    return 0;
}
