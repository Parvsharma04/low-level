class Message;

class User {
public:
    int id;
    string username;

    User(string username): id(), username(username) {}

    void update(string username) {
        this->username = username;
    }
};

class Chat {
public:
    int id;
    string chatname;
    vector<User*> members;
    map<User*, vector<Message*>> texts;

    Chat(string chatname): chatname(chatname) {}

    void addMember(User* user) {
        for(auto &e: members)
            if(e->id == user->id)
                throw runtime_error("User already exists");
        members.push_back(user);
    }

    vector<string> getMembers(){
        vector<string> response;
        for(auto &e: members){
            response.push_back(e->username);
        }
        return response;
    }

    void removeMember(User* user) {
        for (int i = 0; i<members.size(); i++){
            if(members[i]->id == user->id){
                members.erase(members.begin() + i);
                break;
            }
        }
    }

    void send(User* user, string content) {
        Message* text = new Message(user, content);
        texts[user].push_back(text);
    }

    ~Chat() {
        for (auto &[user, messages] : texts) {
            for (auto message : messages) {
                delete message;
            }
        }
    }
};

class Message {
public:
    int id;
    User* from;
    int timestamp;
    string content;
    Chat* chat;

    Message(User* from, string content): from(from), content(content) {}
};