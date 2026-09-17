/*
    2: director
    1: supervisor
    0: operator

    - calls to be queued when employees unavailable.
    - call assignment in the above order i.e 0->1->2.
    - upon call completion pick waiting call from queue
*/
class CallCenter;
class Employee;

enum class CallState {
    READY,
    INPROGRESS,
    ENDED,
};

enum class Rank {
    OPERATOR,
    SUPERVISOR,
    DIRECTOR,
};


class Call {
public:

    Employee *employee;
    Employee *closedBy;
    CallState status;


    Call(): status(CallState::READY), employee(nullptr) {}
};

class Employee {
public:

    int employeeId;
    Rank position;
    Call* call;
    CallCenter* center;

    Employee(int employeeId, Rank position, CallCenter* center): employeeId(employeeId), position(position), call(nullptr), center(center), closedBy(nullptr) {}

    bool isFree() {
        return call == nullptr;
    }

    void takeCall(Call* call) {
        this->call = call;
        call->status = CallState::INPROGRESS;
        call->employee = this;
    }

    virtual void escalateCall() {}

    virtual void completeCall() {
        this->call->status = CallState::ENDED;
        this->call->closedBy = this;
        this->call->employee = nullptr;
        this->call = nullptr;
        this->center->dispatchCall();
    }
};

class Operator : public Employee {
public:

    Operator(int employeeId, CallCenter* center): Employee(employeeId, Rank::OPERATOR, center) {}

    void escalateCall() override {
        this->call->employee = nullptr;
        this->call->status = CallState::READY;
        Call* call = this->call;
        this->call = nullptr;
        this->center->dispatchCall(call, this->center->supervisors);
    }

};

class Supervisor : public Employee {
public:

    Supervisor(int employeeId, CallCenter* center): Employee(employeeId, Rank::SUPERVISOR, center) {}

    void escalateCall() override {
        this->call->employee = nullptr;
        this->call->status = CallState::READY;
        Call* call = this->call;
        this->call = nullptr;
        this->center->dispatchCall(call, this->center->directors);
    }

};

class Director : public Employee {
public:

    Director(int employeeId, CallCenter* center): Employee(employeeId, Rank::DIRECTOR, center) {}

    void escalateCall() override {
        throw runtime_error("Director cannot escalate");
    }

};

class CallCenter {
public:

    vector<Operator*> operators;
    vector<Supervisor*> supervisors;
    vector<Director*> directors;

    queue<Call*> callBacklog;

    CallCenter() {}

    void addEmployee(int employeeId, Rank position) {
        switch(position) {
            case Rank::OPERATOR:
                operators.push_back(new Operator(employeeId, this));
                break;

            case Rank::SUPERVISOR:
                supervisors.push_back(new Supervisor(employeeId, this));
                break;

            case Rank::DIRECTOR:
                directors.push_back(new Director(employeeId, this));
                break;
        }
    }

    //queue assignment
    void dispatchCall() {
        if(callBacklog.empty()) return;
        Call* call = callBacklog.front();
        callBacklog.pop();
        if(assignment(call, operators)) return;
        if(assignment(call, supervisors)) return;
        if(assignment(call, directors)) return;
        callBacklog.push(call);
    }

    //direct assignment
    void dispatchCall(Call* call) {
        if(assignment(call, operators)) return;
        if(assignment(call, supervisors)) return;
        if(assignment(call, directors)) return;
        callBacklog.push(call);
    }

    //reassignment
    template <typename T>
    void dispatchCall(Call* call, vector<T*>& pool) {
        if(assignment(call, pool)) return;
        if(assignment(call, directors)) return;
        callBacklog.push(call);
    }

    template <typename T>
    bool assignment(Call* call, vector<T*> &pool) {
        for (auto e : pool) {
            if (e->isFree()) {
                e->takeCall(call);
                return true;
            }
        }
        return false;
    }
};