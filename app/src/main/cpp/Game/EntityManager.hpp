struct player_t {
    void* obj = nullptr;
    void* player = nullptr;
    int id = -1;
    std::string username = "";
    int teamIndex = -1;
};

class EntityManager {
public:
    std::vector<player_t> entities = std::vector<player_t>();
    std::vector<player_t> get() { return entities; }

    EntityManager() {
        entities = std::vector<player_t>();
    }

    void add(player_t p) {
        entities.push_back(p);
    }

    void remove(void* character) {
        if(entities.size() == 0) return;
        for(int i = 0; i < entities.size(); i++){
            if(entities[i].obj == character){
                entities.erase(entities.begin() + i);
                return;
            }
        }
    }

    void clear() { entities.clear(); }
};