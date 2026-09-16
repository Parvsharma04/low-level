template<typename K, typename V>
class Entity {
    K key;
    V value;

    Entity(K key, V value) {
        this->key = key;
        this->value = value;
    }
};

template<typename K, typename V>
class HashMap {
private:
    int size;
    vector<vector<Entity<K, V>>> map;

    int hashFunction(const K& key) {
        return std::hash<K>{}(key) % size;
    }

public:
    HashMap(int size){
        this->size = size;
        this->map.resize(size);
    }

    V get(K key) {
        int hash = hashFunction(key);
        for(Entity &e: map[hash]){
            if(e->key == key)
                return e->value;
        }
        throw run_time_error('key does not exist');
    }

    void insert(K key, V value) {
        int hash = hashFunction(key);
        for(Entity &e: map[hash]){
            if(e->key == key){
                e->value = value;
                return;
            }
        }
        map[hash].push_back(Entity(key, value));
    }

    void remove (K key) {
        int hash = hashFunction(key);

        for(int i = 0; i<map[hash].size(); i++){
            if(map[hash][i]->key == key){
                map[hash].erase(map[hash].begin() + i);
                return;
            }
        }

        throw run_time_error('key does not exist');
    }
};