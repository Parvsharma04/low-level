class Entity {
    int key;
    int value;

    Entity(int key, int value) {
        this->key = key;
        this->value = value;
    }
};

class HashMap {
private:
    int size;
    vector<vector<Entity>> map;

    int hashFunction(int key) {
        return key % size;
    }

public:
    HashMap(int size){
        this->size = size;
        this->map.resize(size);
    }

    int get(int key) {
        int hash = hashFunction(key);
        for(Entity &e: map[hash]){
            if(e->key == key)
                return e->value;
        }
        throw run_time_error('key does not exist');
    }

    void insert(int key, int value) {
        int hash = hashFunction(key);
        for(Entity &e: map[hash]){
            if(e->key == key){
                e->value = value;
                return;
            }
        }
        map[hash].push_back(Entity(key, value));
    }

    void remove (int key) {
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