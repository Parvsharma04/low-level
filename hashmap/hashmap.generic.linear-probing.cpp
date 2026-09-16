enum class State {
    EMPTY,
    OCCUPIED,
    DELETED
};

template<typename K, typename V>
class Entity {
public:
    K key;
    V value;
    State status = State::EMPTY;

    Entity(K key, V value) {
        this->key = key;
        this->value = value;
        this->status = State::OCCUPIED;
    }
};

template<typename K, typename V>
class HashMap {
private:
    int capacity;
    int size;
    vector<Entity<K, V>> map;

    int hashFunction(const K& key) {
        return std::hash<K>{}(key) % capacity;
    }

    void resize(){
        int newCapacity = capacity * 2;
        capacity = newCapacity;

        vector<Entity<K, V>> newMap(capacity);

        for(auto e: map){
            if (e.status != State::OCCUPIED) continue;
            int hash = hashFunction(e.key);
            for (int j = 0; j < capacity; j++) {
                int i = (hash + j) % capacity;
                if (newMap[i].status == State::EMPTY) {
                    newMap[i] = Entity<K, V>(e.key, e.value);
                    break;
                }
            }
        }
        map = move(newMap);
    }

public:
    HashMap(int cap) : capacity(cap), size(0), map(cap) {}

    V get(K key) {
        int hash = hashFunction(key);
        for (int j = 0; j < capacity; j++) {
            int i = (hash + j) % capacity;
            if (map[i].status == State::EMPTY) break; // if any space is empty, clearly means that the key does not exist
            if (map[i].status == State::OCCUPIED && map[i].key == key)
                return map[i].value;
        }
        throw runtime_error('key does not exist');
    }

    void insert(K key, V value) {
        if(size>=capacity/2)
            resize();
        int hash = hashFunction(key);
        int firstDeleted = -1;
        
        for (int j = 0; j < capacity; j++) {
            int i = (hash + j) % capacity;
            if (map[i].status == State::OCCUPIED && map[i].key == key) {
                map[i].value = value;
                return;
            } else if (map[i].status == State::DELETED) {
                if (firstDeleted == -1) firstDeleted = i;
            } else if(map[i].status == State::EMPTY){
                if (firstDeleted != -1) i = firstDeleted;
                map[i] = Entity(key, value);
                size++;
                return;
            } 
        }
    }

    void remove (K key) {
        int hash = hashFunction(key);

        for (int j = 0; j < capacity; j++) {
            int i = (hash + j) % capacity;
            if(map[i].status == State::EMPTY) break;
            if(map[i].status == State::OCCUPIED && map[i].key == key){
                map[i].status = State::DELETED;
                size--;
                return;
            }
        }

        throw runtime_error('key does not exist');
    }
};