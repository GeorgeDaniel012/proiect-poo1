#include <iostream>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

class InvalidCommand: public exception{};
//tip de exceptie care este thrown cand user-ul insereaza o comanda gresita
//sau incearca sa execute o comanda pe un obiect gresit

class TimeManagement{ //interfata pentru clasele ajutatoare pentru timp (Date si RunTime)
public:
    virtual void printInfo() = 0;
    virtual ~TimeManagement(){};
};

class Date: protected TimeManagement{
    int year, month, day;
public:
    Date(int y, int m, int d){
        year=y;
        month=m;
        day=d;
    }

    Date(){
        year=0;
        month=0;
        day=0;
    }

    virtual ~Date(){};

    virtual void printInfo(){
        cout<<"Year: "<<year<<'\n';
        cout<<"Month: "<<month<<'\n';
        cout<<"Day: "<<day<<'\n';
    }
};

class RunTime: public TimeManagement{ //timp masurat in ore, minute, secunde, milisecunde
    int hours, minutes, seconds, milliseconds;
public:
    RunTime(int h, int m, int s, int ms){
        hours = h;
        minutes = m;
        seconds = s;
        milliseconds = ms;
        if(milliseconds>=1000){
            seconds += milliseconds/1000;
            milliseconds = milliseconds%1000;
        }
        if(seconds>=60){
            minutes += seconds/60;
            seconds = seconds%60;
        }
        if(minutes>=60){
            hours += minutes/60;
            minutes = minutes%60;
        }
        //cout<<"Created time: "<<hours<<"h, "<<minutes<<"m, "<<seconds<<"s, "<<milliseconds<<"ms\n";
    }

    RunTime(){
        hours = 0;
        minutes = 0;
        seconds = 0;
        milliseconds = 0;
        //cout<<"Created Run Time (empty)!\n";
    }

    RunTime(RunTime& t){
        hours = t.hours;
        minutes = t.minutes;
        seconds = t.seconds;
        milliseconds = t.milliseconds;
    }

    virtual ~RunTime(){
        //cout<<"Deleted Run Time!\n";
    }

    virtual void printInfo(){ //printeaza pe ecran timpul
        cout<<"Time: "<<hours<<"h, "<<minutes<<"m, "<<seconds<<"s, "<<milliseconds<<"ms\n";
    }

    friend ostream& operator << (ostream& os, const RunTime& rt);

    int* getTime(){
        int* t = new int[4];
        t[0] = hours;
        t[1] = minutes;
        t[2] = seconds;
        t[3] = milliseconds;
        return t;
    }

    void setTime(int h, int m, int s, int ms){ //seteaza timpul
        hours = h;
        minutes = m;
        seconds = s;
        milliseconds = ms;
        if(milliseconds>=1000){
            seconds += milliseconds/1000;
            milliseconds = milliseconds%1000;
        }
        if(seconds>=60){
            minutes += seconds/60;
            seconds = seconds%60;
        }
        if(minutes>=60){
            hours += minutes/60;
            minutes = minutes%60;
        }
    }

    RunTime operator + (RunTime& rt){
        RunTime res(rt.hours+hours,rt.minutes+minutes,rt.seconds+seconds,rt.milliseconds+milliseconds);
        return res;
    }

    void operator += (RunTime& rt){
        hours += rt.hours;
        minutes += rt.minutes;
        seconds += rt.seconds;
        milliseconds += rt.milliseconds;
        if(milliseconds>=1000){
            seconds += milliseconds/1000;
            milliseconds = milliseconds%1000;
        }
        if(seconds>=60){
            minutes += seconds/60;
            seconds = seconds%60;
        }
        if(minutes>=60){
            hours += minutes/60;
            minutes = minutes%60;
        }
    }

    void operator = (RunTime& rt){
        hours = rt.hours;
        minutes = rt.minutes;
        seconds = rt.seconds;
        milliseconds = rt.milliseconds;
    }
};

ostream& operator << (ostream& os, const RunTime& rt){
    cout<<"Time: "<<rt.hours<<"h, "<<rt.minutes<<"m, "<<rt.seconds<<"s, "<<rt.milliseconds<<"ms\n";
}

class User{ //fiecare user are un username si un id care creste cu 1 pentru fiecare user creat
    char username[20];
    int id;
    static vector<User*> user_list;
public:
    User(char* name){
        strcpy(username, name);
        user_list.push_back(this);
        //id = maxId++;
        //cout<<"Created user "<<username<<" with id "<<id<<'\n';
    }

    User(){
        strcpy(username, "\0");
        //user_list.push_back(this);
        //id = maxId;
        //cout<<"Created (empty) user "<<username<<" with id "<<id<<'\n';
    }

    User(User& u){
        strcpy(username, u.username);
        id = u.id;
        //user_list.push_back(this);
        //cout<<"Copied user\n";
    }

    ~User(){
        user_list.erase(find(user_list.begin(), user_list.end(), this));
        //cout<<"Deleted user "<<username<<" with id "<< id<<"!\n";
    }

    void operator = (User& u){
        strcpy(username, u.username);
        id = u.id;
    }

    void afis(){
        cout<<"Username: "<<username<<"\nId: "<<id<<'\n';
    }

    char* getUsername(){
        return username;
    }

    void setUsername(char* name){
        strcpy(username, name);
    }

    int getId(){
        return id;
    }

    static User* getUser(int i){
        return user_list[i];
    }

    static void printUserList(){
        cout<<"There are "<<user_list.size()<<" users on the database.\n";
        for(int i=0;i<user_list.size();i++){
            cout<<user_list[i]->username<<'\n';
            cout<<"User Id: "<<i<<'\n';
        }
    }
};

vector<User*> User::user_list;

class Record{
protected:
    User* submitter;
    char description[100];
public:
    Record(User& sub, char* desc){
        submitter = &sub;
        strcpy(description, desc);
    }

    Record(){
        submitter = new User();
    }

    virtual ~Record(){};
protected:
    virtual string getRecordType() = 0;
public:
    virtual void afis() = 0;

    User* getSubmitter(){
        return submitter;
    }

    char* getDescription(){
        return description;
    }

    void setDescription(char* desc){
        strcpy(description, desc);
    }
};

class Run: virtual public Record{ //fiecare run are un user care a dat submit (submitter), un timp si o descriere
protected:
    RunTime* tm;
public:
    Run(User& sub, int h, int m, int s, int ms, char* desc){
        submitter = &sub;
        tm = new RunTime(h, m, s, ms);
        strcpy(description, desc);
        //cout<<"Created run!\n";
    }

    Run(){
        submitter = new User();
        tm = new RunTime();
        //cout<<"Created run (empty)!\n";
    }

    virtual ~Run(){
        delete tm;
        //cout<<"Deleted run!\n";
}
protected:
    virtual string getRecordType(){
        return "Speedrun";
    }
public:
    virtual void afis(){
        submitter->afis();
        cout<<"Record of type "<<getRecordType()<<'\n';
        tm->printInfo();
        cout<<"Run Description: "<<description<<'\n';
    }

    RunTime* getRunTime(){
        return tm;
    }

    /*
    void operator = (Run& r){
        submitter = r.submitter;
        tm = r.tm;
        strcpy(description, r.description);
    }
    */
};

class CounterRec: virtual public Record{
protected:
    int counter;
    char counter_type[20];
public:
    CounterRec(User& sub, int cnt, char* desc, char* cnt_type){
        submitter = &sub;
        counter = cnt;
        strcpy(description, desc);
        strcpy(counter_type, cnt_type);
        //cout<<"Created run!\n";
    }

    CounterRec(){
        submitter = new User();
        counter = 0;
    }

    virtual ~CounterRec(){};

protected:
    virtual string getRecordType(){
        return "Counter Record";
    }
public:
    virtual void afis(){
        submitter->afis();
        cout<<"Record of type "<<getRecordType()<<'\n';
        cout<<counter_type<<": "<<counter<<'\n';
        cout<<"Run Description: "<<description<<'\n';
    }
};

class ChallengeRun: public Run, public CounterRec{
    char challenge_name[20];
public:
    ChallengeRun(User& sub, int h, int m, int s, int ms, int cnt, char* desc, char* cnt_type, char* chl_name){
        submitter = &sub;
        tm = new RunTime(h, m, s, ms);
        counter = cnt;
        strcpy(description, desc);
        strcpy(counter_type, cnt_type);
        strcpy(challenge_name, chl_name);
        //cout<<"Created run!\n";
    }

    /*
    ChallengeRun(){
        submitter = new User();
        tm = new RunTime();
        counter = 0;
    }
    */

    virtual ~ChallengeRun(){
        delete tm;
        //cout<<"Deleted run!\n";
    }
protected:
    virtual string getRecordType(){
        return "Challenge";
    }
public:
    virtual void afis(){
        submitter->afis();
        tm->printInfo();
        cout<<"Record of type "<<getRecordType()<<'\n';
        cout<<"Challenge name: "<<challenge_name<<'\n';
        cout<<counter_type<<": "<<counter<<'\n';
        cout<<"Run Description: "<<description<<'\n';
    }
};


//class

class Category{ //fiecare categorie are o lista de run-uri (mai exact un array de pointeri la run-uri) si descrierea ei proprie
    char category_type;
protected:
    vector<Record*> record_list;
    char name[20];
    char description[100];
    //User** mod_list;
    //int mod_nr = 1;
public:
    Category(char* nm, char* desc, char cat_type){ //cand creem o categorie, ii putem adauga run-uri in run_list
        strcpy(name, nm);
        strcpy(description, desc);
        category_type = cat_type;
        //cout<<"Created Category!\n";
    }

    Category(){
        strcpy(name, "\0");
        strcpy(description, "\0");
        category_type = '\0';
        //cout<<"Created Category (empty)!\n";
    }

    ~Category(){
        for(int i=record_list.size()-1;i>=0;i--){
            delete record_list[i];
        }
        //cout<<"Deleted Category!\n";
    }

    virtual void createRun(User& sub, int h, int m, int s, int ms, char* desc){ //functia creeaza un run si il adauga la categorie
        if(category_type!='s'){
            InvalidCommand i;
            throw i;
        }
        Record* r = new Run(sub, h, m, s, ms, desc);
        record_list.push_back(r);
    }

    virtual void createCounterRec(User& sub, int cnt, char* desc, char* cnt_type){
        if(category_type!='c'){
            InvalidCommand i;
            throw i;
        }
        Record* r = new CounterRec(sub, cnt, desc, cnt_type);
        record_list.push_back(r);
    }

    void removeRecord(int record_index){
        //sterge un run dintr-o categorie, run_index/id-ul este local
        try{
            if(record_index>=record_list.size()){
                out_of_range o("out of range");
                throw o;
            }
            delete record_list[record_index];
            record_list.erase(record_list.begin()+record_index);
        }
        catch(out_of_range){
            cout<<"Record Index out of bounds."; //daca run_index este inafara run_list-ului
        }
    }

    char* getName(){
        return name;
    }

    void setName(char* nm){
        strcpy(name, nm);
    }

    char* getDesc(){
        return description;
    }

    void setDescription(char* desc){
        strcpy(description, desc);
    }

    int getRecordNr(){
        return record_list.size();
    }

    vector<Record*> getRecordList(){
        return record_list;
    }

    virtual void afis(){
        int record_nr = record_list.size();
        cout<<"Category name: "<<name<<'\n';
        cout<<"Category description: "<<description<<'\n';
        cout<<"There are "<<record_nr<<" records in this category."<<'\n';
        if(record_nr != 0){
            cout<<"Records:"<<'\n';
            for(int i=0;i<record_nr;i++){
                record_list[i]->afis();
                cout<<"Local id: "<<i<<'\n';
            }
        }
    }

    char getCategoryType(){
        return category_type;
    }

    /*
    void afisSorted(){
        //normal run_list este sortat dupa id-ul local al run-urilor
        //ei bine, functia asta afiseaza run-urile sortate dupa timp (ca un leaderboard normal)
        Run** sorted_list = new Run*[run_nr];
        copy(run_list, run_list+run_nr, sorted_list);

        int* x;
        int* y;

        for(int i=0;i<run_nr-1;i++){
            for(int j=0;j<run_nr-i-1;j++){
                x = run_list[j]->getRunTime()->getTime();
                y = run_list[j+1]->getRunTime()->getTime();
                if(x[0]==y[0]){
                    if(x[1]==y[0]){
                        if(x[2]==y[2]){
                            if(x[3]>y[3]){
                                Run* t = sorted_list[j];
                                sorted_list[j] = sorted_list[j+1];
                                sorted_list[j+1] = t;
                            }
                        }else if(x[2]>y[2]){
                            Run* t = sorted_list[j];
                            sorted_list[j] = sorted_list[j+1];
                            sorted_list[j+1] = t;
                        }
                    }else if(x[1]>y[1]){
                        Run* t = sorted_list[j];
                        sorted_list[j] = sorted_list[j+1];
                        sorted_list[j+1] = t;
                    }
                }else if(x[0]>y[0]){
                    Run* t = sorted_list[j];
                    sorted_list[j] = sorted_list[j+1];
                    sorted_list[j+1] = t;
                }
            }
        }

        cout<<"Category name: "<<name<<'\n';
        cout<<"Category description: "<<description<<'\n';
        cout<<"There are "<<run_nr<<" runs in this Category."<<'\n';
        if(run_nr != 0){
            cout<<"Runs:"<<'\n';
            for(int i=0;i<run_nr;i++){
                sorted_list[i]->afis();
                cout<<"Place: "<<i+1<<'\n';
            }
        }
    }
    */

    RunTime getTotalTime(){
        int run_nr = record_list.size();
        RunTime t;
        if(run_nr != 0){
            for(int i=0;i<run_nr;i++){
                Run* r = dynamic_cast<Run*>(record_list[i]);

                if(!r){
                    bad_cast b;
                    throw b;
                }

                t += *(r->getRunTime());
            }
        }
        return t;
    }

};

class Event: public Category{
    static vector<Event*> event_list;
    Date* start_date;
    Date* end_date;
public:
    Event(char* nm, char* desc, int start_year, int start_month, int start_day, int end_year, int end_month, int end_day):Category(nm, desc, 'e'){
        //strcpy(name, nm);
        //strcpy(description, desc);
        start_date = new Date(start_year, start_month, start_day);
        end_date = new Date(end_year, end_month, end_day);
        event_list.push_back(this);
    }

    Event():Category(){
        strcpy(name, "\0");
        strcpy(description, "\0");
        start_date = new Date();
        end_date = new Date();
        event_list.push_back(this);
    }

    ~Event(){
        delete start_date;
        delete end_date;
        event_list.erase(find(event_list.begin(), event_list.end(), this));
    };

    void createChallenge(User& sub, int h, int m, int s, int ms, int cnt, char* desc, char* cnt_type, char* chl_name){
        Record* c = new ChallengeRun(sub, h, m, s, ms, cnt, desc, cnt_type, chl_name);
        record_list.push_back(c);
    }

    virtual void createRun(User& sub, int h, int m, int s, int ms, char* desc){
        Record* r = new Run(sub, h, m, s, ms, desc);
        record_list.push_back(r);
    }

    virtual void createCounterRec(User& sub, int cnt, char* desc, char* cnt_type){
        Record* r = new CounterRec(sub, cnt, desc, cnt_type);
        record_list.push_back(r);
    }

    virtual void afis(){
        int record_nr = record_list.size();
        cout<<"Event name: "<<name<<'\n';
        cout<<"Event description: "<<description<<'\n';
        cout<<"There are "<<record_nr<<" records in this event."<<'\n';
        if(record_nr != 0){
            cout<<"Records:"<<'\n';
            for(int i=0;i<record_nr;i++){
                record_list[i]->afis();
                cout<<"Local id: "<<i<<'\n';
            }
        }
    }

    static void printEventList(){
        cout<<"There are "<<event_list.size()<<" events on the database.\n";
        for(int i=0;i<event_list.size();i++){
            cout<<event_list[i]->name<<'\n';
            cout<<"Event Id: "<<i<<'\n';
        }
    }

    static int getEventListSize(){
        return event_list.size();
    }

    static Event* getEvent(int i){
        return event_list[i];
    }
};

vector<Event*> Event::event_list;

class Game{ //fiecare joc are mai multe categorii (array de pointeri la categorii ale jocului), plus un nume si o descriere
    static vector<Game*> game_list;
    vector<Category*> cat_list;
    vector<ChallengeRun*> chl_list;
    char name[20];
    char description[100];
public:
    Game(char* nm, char* desc){
        strcpy(name, nm);
        strcpy(description, desc);
        game_list.push_back(this);
        //cout<<"Created game!\n";
    }

    Game(){
        strcpy(name, "\0");
        strcpy(description, "\0");
        game_list.push_back(this);
        //cout<<"Created (empty) game!\n";
    }

    ~Game(){
        for(int i=cat_list.size()-1;i>=0;i--){
            delete cat_list[i];
        }
        for(int j=chl_list.size()-1;j>=0;j--){
            delete chl_list[j];
        }
        game_list.erase(find(game_list.begin(), game_list.end(), this));
        //cout<<"Deleted game!\n";
    }

    void createCategory(char* nm, char* desc, char cat_type){ //functia adauga o categorie la joc si incrementeaza nr de categorii ale jocului
        Category* c = new Category(nm, desc, cat_type);
        cat_list.push_back(c);
    }

    void removeCat(int cat_index){
        //sterge o categorie dintr-un joc, cat_index/id-ul este local
        try{
            if(cat_index>=cat_list.size()){
                out_of_range o("out of range");
                throw o;
            }
            delete cat_list[cat_index];
            cat_list.erase(cat_list.begin()+cat_index);
        }
        catch(out_of_range){
            cout<<"Category Index out of bounds."; //daca cat_index este inafara cat_list-ului
        }
    }

    void createChallenge(User& sub, int h, int m, int s, int ms, int cnt, char* desc, char* cnt_type, char* chl_name){
        ChallengeRun* c = new ChallengeRun(sub, h, m, s, ms, cnt, desc, cnt_type, chl_name);
        chl_list.push_back(c);
    }

    void removeChl(int chl_index){
        //sterge o categorie dintr-un joc, run_index/id-ul este local
        try{
            if(chl_index>=chl_list.size()){
                out_of_range o("out of range");
                throw o;
            }
            delete chl_list[chl_index];
            chl_list.erase(chl_list.begin()+chl_index);
        }
        catch(out_of_range){
            cout<<"Challenge Index out of bounds."; //daca chl_index este inafara cat_list-ului
        }
    }

    void setName(char* nm){
        strcpy(name, nm);
    }

    void setDescription(char* desc){
        strcpy(description, desc);
    }

    int getCategoryNr(){
        return cat_list.size();
    }

    vector<Category*> getCatList(){
        return cat_list;
    }

    void afis(){
        int cat_nr = cat_list.size();
        cout<<"Game name: "<<name<<'\n';
        cout<<"Game description: "<<description<<'\n';
        cout<<"List of categories:\n";
        if(cat_nr!=0){
            for(int i=0;i<cat_nr;i++){
                cout<<"Category name: "<<cat_list[i]->getName()<<"\nCategory description: ";
                cout<<cat_list[i]->getDesc()<<"\nNumber of records in category: "<<cat_list[i]->getRecordNr()<<" records\n";
                cout<<"Local id "<<i<<'\n';
            }
        }
        cout<<"There are "<<chl_list.size()<<" challenges submitted for this game.\n";
    }

    static void printGameList(){
        cout<<"There are "<<game_list.size()<<" games on the database.\n";
        for(int i=0;i<game_list.size();i++){
            cout<<game_list[i]->name<<'\n';
            cout<<"Game Id: "<<i<<'\n';
        }
    }

    static int getGameListSize(){
        return game_list.size();
    }

    static Game* getGame(int i){
        return game_list[i];
    }

    int getCatListSize(){
        return cat_list.size();
    }

    int getChlListSize(){
        return chl_list.size();
    }

    void catAfis(int cat_index){
        cat_list[cat_index]->afis();
    }

    void chlAfis(int chl_index){
        chl_list[chl_index]->afis();
    }
};

vector<Game*> Game::game_list;

void editCategory(Category* c){
    string cmd;
    cin>>cmd;
    cin.get();
    if(cmd == "createRun"){
        cout<<"Which user submitted the run (user id)?\n";
        User::printUserList();
        int i;
        cin>>i;
        cin.get();
        User* u = User::getUser(i);
        cout<<"What time did the run have (hours, minutes, seconds, milliseconds)?\n";
        int h, m, s, ms;
        cin>>h>>m>>s>>ms;
        cin.get();
        cout<<"Please provide a description for this run.\n";
        char desc[100];
        cin.get(desc, 100);
        cin.get();
        c->createRun(*u, h, m, s, ms, desc);
    }else if(cmd == "createCounterRecord"){
        cout<<"Which user submitted the record (user id)?\n";
        User::printUserList();
        int i;
        cin>>i;
        cin.get();
        User* u = User::getUser(i);
        cout<<"What counter does this challenge have?\n";
        int cn;
        cin>>cn;
        cin.get();
        cout<<"What counter type is this?\n";
        char ct[20];
        cin.get(ct, 20);
        cin.get();
        cout<<"Please provide a description for this challenge.\n";
        char desc[100];
        cin.get(desc, 100);
        cin.get();
        c->createCounterRec(*u, cn, desc, ct);
    }else if(cmd == "removeRecord"){
        cout<<"Which record do you want to remove (record id)?\n";
        c->afis();
        int i;
        cin>>i;
        cin.get();
        c->removeRecord(i);
    }else if(cmd == "view"){
        c->afis();
    }else if(cmd == "cancel"){}
    else{
        InvalidCommand i;
        throw i;
    }
}

void editEvent(){
    cout<<"What event do you want to edit (game id)?\n";
    Event::printEventList();
    int eid;
    cin>>eid;
    Event* e = Event::getEvent(eid);
    cout<<"What do you want to do to the game with id "<<eid<<"?\n";
    cout<<"If you want to go back, type cancel.\n";
    string cmd;
    cin>>cmd;
    cin.get();
    if(cmd == "createRun"){
        cout<<"Which user submitted the run (user id)?\n";
        User::printUserList();
        int i;
        cin>>i;
        cin.get();
        User* u = User::getUser(i);
        cout<<"What time did the run have (hours, minutes, seconds, milliseconds)?\n";
        int h, m, s, ms;
        cin>>h>>m>>s>>ms;
        cin.get();
        cout<<"Please provide a description for this run.\n";
        char desc[100];
        cin.get(desc, 100);
        cin.get();
        e->createRun(*u, h, m, s, ms, desc);
    }else if(cmd == "createCounterRecord"){
        cout<<"Which user submitted the record (user id)?\n";
        User::printUserList();
        int i;
        cin>>i;
        cin.get();
        User* u = User::getUser(i);
        cout<<"What counter does this challenge have?\n";
        int cn;
        cin>>cn;
        cin.get();
        cout<<"What counter type is this?\n";
        char ct[20];
        cin.get(ct, 20);
        cin.get();
        cout<<"Please provide a description for this challenge.\n";
        char desc[100];
        cin.get(desc, 100);
        cin.get();
        e->createCounterRec(*u, cn, desc, ct);
    }else if(cmd == "removeRecord"){
        cout<<"Which record do you want to remove (record id)?\n";
        e->afis();
        int i;
        cin>>i;
        cin.get();
        e->removeRecord(i);
    }else if(cmd == "view"){
        e->afis();
    }else if(cmd == "createChallenge"){
        cout<<"Which user submitted the challenge (user id)?\n";
        User::printUserList();
        int i;
        cin>>i;
        cin.get();
        User* u = User::getUser(i);
        cout<<"What time did the challenge run have (hours, minutes, seconds, milliseconds)?\n";
        int h, m, s, ms;
        cin>>h>>m>>s>>ms;
        cin.get();
        cout<<"What counter does this challenge have?\n";
        int c;
        cin>>c;
        cin.get();
        cout<<"What counter type is this?\n";
        char ct[20];
        cin.get(ct, 20);
        cin.get();
        cout<<"What is the name of this challenge?\n";
        char nm[20];
        cin.get(nm, 20);
        cin.get();
        cout<<"Please provide a description for this challenge.\n";
        char desc[100];
        cin.get(desc, 100);
        cin.get();
        e->createChallenge(*u, h, m, s, ms, c, desc, ct, nm);
    }else if(cmd == "cancel"){}
    else{
        InvalidCommand i;
        throw i;
    }
}

void editGame(){
    cout<<"What game do you want to edit (game id)?\n";
    Game::printGameList();
    int gid;
    cin>>gid;
    Game* g = Game::getGame(gid);
    cout<<"What do you want to do to the game with id "<<gid<<"?\n";
    cout<<"If you want to go back, type cancel.\n";
    string cmd;
    cin>>cmd;
    cin.get();
    if(cmd == "createCategory"){
        cout<<"What is the category's name?\n";
        char name[20];
        cin.get(name, 20);
        cin.get();
        cout<<"What is the category's description?\n";
        char desc[100];
        cin.get(desc, 100);
        cin.get();
        cout<<"What kind of category is this?\n";
        cout<<"s - speedrun, c - counter record\n";
        char type[1];
        cin>>type;
        cin.get();
        bool isInvalid = false;
        if(strcmp(type, "s") != 0 && strcmp(type, "c") != 0){
            isInvalid = true;
        }
        while(isInvalid){
            cout<<"Category type is invalid. Please insert either s or c.\n";
            cin>>type;
            cin.get();
            if(strcmp(type, "s") == 0 || strcmp(type, "c") == 0){
                isInvalid = false;
            }
        }
        g->createCategory(name, desc, type[0]);
    }else if(cmd == "removeCategory"){
        cout<<"Which category do you want to delete (category id)?\n";
        g->afis();
        cout<<'\n';
        int i;
        cin>>i;
        cin.get();
        g->removeCat(i);
    }else if(cmd == "view"){
        g->afis();
    }else if(cmd == "viewCategory"){
        cout<<"Which category do you want to view (category id)?\n";
        int i;
        cin>>i;
        cin.get();
        g->catAfis(i);
    }else if(cmd == "createChallenge"){
        cout<<"Which user submitted the challenge (user id)?\n";
        User::printUserList();
        int i;
        cin>>i;
        cin.get();
        User* u = User::getUser(i);
        cout<<"What time did the challenge run have (hours, minutes, seconds, milliseconds)?\n";
        int h, m, s, ms;
        cin>>h>>m>>s>>ms;
        cin.get();
        cout<<"What counter does this challenge have?\n";
        int c;
        cin>>c;
        cin.get();
        cout<<"What counter type is this?\n";
        char ct[20];
        cin.get(ct, 20);
        cin.get();
        cout<<"What is the name of this challenge?\n";
        char nm[20];
        cin.get(nm, 20);
        cin.get();
        cout<<"Please provide a description for this challenge.\n";
        char desc[100];
        cin.get(desc, 100);
        cin.get();
        g->createChallenge(*u, h, m, s, ms, c, desc, ct, nm);
    }else if(cmd == "editCategory"){
        if(g->getCatListSize()==0){
            //cout<<"There are no categories associated to this game.\n";
            range_error r("no range");
            throw r;
        }
        cout<<"What category do you want to edit (category id)?\n";
        g->afis();
        int i;
        cin>>i;
        cin.get();
        Category* c = (g->getCatList())[i];
        cout<<"What do you want to do to the category with id "<<i<<"?\n";
        cout<<"If you want to go back, type cancel.\n";
        editCategory(c);
    }else if(cmd == "removeCategory"){
        if(g->getCatListSize()==0){
            //cout<<"There are no categories associated to this game.\n";
            range_error r("no range");
            throw r;
        }
        cout<<"What category do you want to delete?\n";
        int i;
        cin>>i;
        cin.get();
        g->removeCat(i);
    }else if(cmd == "cancel"){}
    else{
        InvalidCommand i;
        throw i;
    }
}

int main(){
    /*
    User* u = new User("abc");
    //Run* r = new Run(*u, 1, 2, 3, 4, "aaa");
    //r->afis();
    Game g("name", "desc");
    Game g1("a", "gsg");
    Game g2("hhwe", "hsdhs");
    Game::printGameList();
    g.createCategory("Category", "desc", 's');
    g.createCategory("cat2", "desc2", 'c');
    //c.afis();
    (g.getCatList())[0]->createRun(*u, 1, 2, 3, 4, "aaa");
    (g.getCatList())[0]->createRun(*u, 2, 3, 4, 5, "aba");
    (g.getCatList())[1]->createCounterRec(*u, 7, "bbb", "deaths");
    //g.afis();
    g.catAfis(0);
    (g.getCatList())[0]->removeRecord(0);
    g.catAfis(0);
    //g.catAfis(1);
    //g.createChallenge(*u, 1, 2, 3, 4, 17, "aaa", "bcd", "challenge");
    //g.afis();
    //g.chlAfis(0);
    //c.afis();
    */
    User* u1 = new User("zylenox");
    User* u2 = new User("doogile");
    User* u3 = new User("reignex");
    User* u4 = new User("Goombs");
    User* u5 = new User("Rickfernello");
    User* u6 = new User("Joshimuz");

    Game* g1 = new Game("Minecraft", "The most popular block game!");
    Game* g2 = new Game("Celeste", "Help Madeline get to the summit of Mount Celeste!");

    g1->createCategory("Any% Glitchless", "Defeat the Ender Dragon on a new world with a random seed.", 's');
    g2->createCategory("100%", "Complete all of the game's chapters and get all of the collectibles.", 's');
    g2->createCategory("Fewest Deaths", "Achieve max% completion while dying as few times as possible.", 'c');

    Category* c1g1 = g1->getCatList()[0];
    Category* c1g2 = g2->getCatList()[0];
    Category* c2g2 = g2->getCatList()[1];

    c1g1->createRun(*u1, 0, 7, 45, 828, "seed: 7499203634667178692");
    c1g1->createRun(*u2, 0, 8, 43, 927, "seed: 9153513292182018950");
    c1g1->createRun(*u3, 0, 8, 58, 930, "seed: 6299140451657411366, got retimed back down");
    c1g2->createRun(*u4, 1, 41, 35, 316, "yay");
    c2g2->createCounterRec(*u5, 0, "This is the world's first max%, 202 berries Deathless Celeste run, also known as True 100% Deathless.", "Deaths");

    Event* e1 = new Event("SGDQ2023", "#SGDQ2023 is back in full swing!!", 2023, 5, 28, 2023, 6, 4);
    e1->createChallenge(*u6, 4, 0, 19, 0, 20, "Surprised that this worked out well.", "Skips", "Fewest Skips");

    bool isRunning = true;
    string command;
    while(isRunning){
        cout<<"What do you want to do?\n";
        cin>>command;
        cin.get();
        try{
            if(command == "createUser"){
                cout<<"Please provide an username for this user.\n";
                char name[20];
                cin.get(name, 20);
                cin.get();
                new User(name);
            }else if(command == "userList"){
                User::printUserList();
            }else if(command == "createGame"){
                cout<<"Please provide a name for this game.\n";
                char name[20];
                cin.get(name, 20);
                cin.get();
                cout<<"Please provide a description for this game.\n";
                char desc[100];
                cin.get(desc, 100);
                cin.get();
                new Game(name, desc);
            }else if(command == "gameList"){
                Game::printGameList();
            }else if(command == "editGame"){
                if(Game::getGameListSize()==0){
                    //cout<<"There are no games on the database yet.\n";
                    range_error r("no range");
                    throw r;
                }
                editGame();
            }else if(command == "viewGame"){
                if(Game::getGameListSize()==0){
                    //cout<<"There are no games on the database yet.\n";
                    range_error r("no range");
                    throw r;
                }
                cout<<"Which game do you want to see (game id)?\n";
                int i;
                cin>>i;
                cin.get();
                Game* g = Game::getGame(i);
                g->afis();
            }else if(command == "createEvent"){
                cout<<"Please provide a name for this event.\n";
                char name[20];
                cin.get(name, 20);
                cin.get();
                cout<<"Please provide a description for this event.\n";
                char desc[100];
                cin.get(desc, 100);
                cin.get();
                cout<<"Please provide a start date for this event.\n";
                int sy, sm, sd;
                cin>>sy>>sm>>sd;
                cin.get();
                cout<<"Please provide an end date for this event.\n";
                int ey, em, ed;
                cin>>ey>>em>>ed;
                cin.get();
                new Event(name, desc, sy, sm, sd, ey, em, ed);
            }else if(command == "eventList"){
                Event::printEventList();
            }else if(command == "editEvent"){
                if(Event::getEventListSize()==0){
                    //cout<<"There are no events on the database yet.\n";
                    range_error r("no range");
                    throw r;
                }
                editEvent();
            }else if(command == "viewEvent"){
                if(Game::getGameListSize()==0){
                    //cout<<"There are no events on the database yet.\n";
                    range_error r("no range");
                    throw r;
                }
                cout<<"Which event do you want to see (event id)?\n";
                int i;
                cin>>i;
                cin.get();
                Event* e = Event::getEvent(i);
                e->afis();
            }else if(command == "quit"){
                cout<<"Shutting down.";
                isRunning = false;
                continue;
            }else{
                InvalidCommand i;
                throw i;
            }
        }catch(range_error){
            cout<<"No objects of this type on the database yet.\n";
            InvalidCommand i;
            throw i;
        }catch(InvalidCommand){
            cout<<"This command is invalid.\n";
        }
        cout<<"\n";
    }
    return 0;
}
