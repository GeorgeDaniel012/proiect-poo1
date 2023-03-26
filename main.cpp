#include <iostream>
#include <cstring>

using namespace std;

int maxId = 0;

class User{ //fiecare user are un username si un id care creste cu 1 pentru fiecare user creat
    char username[20];
    int id;
public:
    User(char* name){
        strcpy(username, name);
        id = maxId++;
        //cout<<"Created user "<<username<<" with id "<<id<<'\n';
    }

    User(){
        strcpy(username, "\0");
        id = maxId;
        //cout<<"Created (empty) user "<<username<<" with id "<<id<<'\n';
    }

    User(User& u){
        strcpy(username, u.username);
        id = u.id;
        //cout<<"Copied user\n";
    }

    ~User(){
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
};

class RunTime{ //timp masurat in ore, minute, secunde, milisecunde
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

    ~RunTime(){
        //cout<<"Deleted Run Time!\n";
    }

    void printTime(){ //printeaza pe ecran timpul
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

class Run{ //fiecare run are un user care a dat submit (submitter), un timp si o descriere
    User* submitter;
    RunTime* tm;
    char description[100];
public:
    Run(User& s, RunTime& t, char* desc){
        submitter = &s;
        tm = &t;
        strcpy(description, desc);
        //cout<<"Created run!\n";
    }

    Run(){
        submitter = new User;
        tm = new RunTime;
        //cout<<"Created run (empty)!\n";
    }

    ~Run(){
        //cout<<"Deleted run!\n";
    }

    void afis(){
        submitter->afis();
        tm->printTime();
        cout<<"Run Description: "<<description<<'\n';
    }

    User* getSubmitter(){
        return submitter;
    }

    RunTime* getRunTime(){
        return tm;
    }

    char* getDescription(){
        return description;
    }

    void setDescription(char* desc){
        strcpy(description, desc);
    }

    void operator = (Run& r){
        submitter = r.submitter;
        tm = r.tm;
        strcpy(description, r.description);
    }
};

class Category{ //fiecare categorie are o lista de run-uri (mai exact un array de pointeri la run-uri) si descrierea ei proprie
    Run** run_list;
    int run_nr = 0;
    char name[20];
    //int run_nr = 0;
    char description[100];
    //User** mod_list;
    //int mod_nr = 1;
public:
    Category(char* nm, char* desc){ //cand creem o categorie, ii putem adauga run-uri in run_list
        run_nr = 0;
        strcpy(name, nm);
        strcpy(description, desc);
        //cout<<"Created category!\n";
    }

    Category(){
        strcpy(name, "\0");
        strcpy(description, "\0");
        //cout<<"Created category (empty)!\n";
    }

    ~Category(){
        if(run_nr!=0){
            delete[] run_list;
        }
        //cout<<"Deleted category!\n";
    }
    void addRun(Run*& r){ //functia adauga un run la categorie si incrementeaza nr de run-uri ale categoriei
        if(run_nr!=0){
            Run** t = new Run*[run_nr];
            copy(run_list, run_list+run_nr, t);
            delete run_list;
            run_list = new Run*[++run_nr];
            copy(t, t+run_nr-1, run_list);
            run_list[run_nr-1] = r;
        }
        else{
            run_list = new Run*[1];
            run_nr++;
            run_list[0] = r;
        }
    }

    void removeRun(int run_index){
        //sterge un run dintr-o categorie, run_index/id-ul este local
        //(bazat pe lista categoriei de run-uri, nu pe lista globala)
        if(run_index<run_nr){
            Run** t = new Run*[run_nr];
            copy(run_list, run_list+run_nr, t);
            delete run_list;
            run_list = new Run*[--run_nr];
            if(run_index!=0){
                copy(t, t+run_index, run_list);
            }
            if(run_index!=run_nr){
                copy(t+run_index+1, t+run_nr+1, run_list+run_index);
            }
        }
        else{
            cout<<"Run Index out of bounds."; //daca run_index este inafara run_list-ului
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

    int getRunNr(){
        return run_nr;
    }

    void afis(){
        cout<<"Category name: "<<name<<'\n';
        cout<<"Category description: "<<description<<'\n';
        cout<<"There are "<<run_nr<<" runs in this category."<<'\n';
        if(run_nr != 0){
            cout<<"Runs:"<<'\n';
            for(int i=0;i<run_nr;i++){
                run_list[i]->afis();
                cout<<"Local id: "<<i<<'\n';
            }
        }
    }

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
        cout<<"There are "<<run_nr<<" runs in this category."<<'\n';
        if(run_nr != 0){
            cout<<"Runs:"<<'\n';
            for(int i=0;i<run_nr;i++){
                sorted_list[i]->afis();
                cout<<"Place: "<<i+1<<'\n';
            }
        }
    }

    RunTime getTotalTime(){
        RunTime t;
        if(run_nr != 0){
            for(int i=0;i<run_nr;i++){
                t += *(run_list[i]->getRunTime());
            }
        }
        return t;
    }

};

class Game{ //fiecare joc are mai multe categorii (array de pointeri la categorii ale jocului), plus un nume si o descriere
    Category** cat_list;
    int cat_nr = 0;
    char name[20];
    char description[100];
public:
    Game(char* nm, char* desc){
        cat_list = new Category*[0];
        strcpy(name, nm);
        strcpy(description, desc);
        cat_nr = 0;
        //cout<<"Created game!\n";
    }

    Game(){
        cat_list = new Category*[0];
        strcpy(name, "\0");
        strcpy(description, "\0");
        cat_nr = 0;
        //cout<<"Created (empty) game!\n";
    }

    ~Game(){
        if(cat_nr!=0){
            delete[] cat_list;
        }
        //cout<<"Deleted game!\n";
    }

    void addCategory(Category*& cat){ //functia adauga o categorie la joc si incrementeaza nr de categorii ale jocului
        if(cat_nr!=0){
            Category** t = new Category*[cat_nr];
            copy(cat_list, cat_list+cat_nr, t);
            delete cat_list;
            cat_list = new Category*[++cat_nr];
            copy(t, t+cat_nr-1, cat_list);
            cat_list[cat_nr-1] = cat;
        }
        else{
            cat_list = new Category*[1];
            cat_nr++;
            cat_list[0] = cat;
        }
    }

    int getCategoryNr(){
        return cat_nr;
    }

    void setName(char* nm){
        strcpy(name, nm);
    }

    void setDescription(char* desc){
        strcpy(description, desc);
    }

    void afis(){
        cout<<"Game name: "<<name<<'\n';
        cout<<"Game description: "<<description<<'\n';
        cout<<"List of categories:\n";
        if(cat_nr!=0){
            for(int i=0;i<cat_nr;i++){
                cout<<"Category name: "<<cat_list[i]->getName()<<"\nCategory description: ";
                cout<<cat_list[i]->getDesc()<<"\nNumber of runs in category: "<<cat_list[i]->getRunNr()<<" runs\n";
                cout<<"Local id "<<i<<'\n';
            }
        }
    }
};

int user_nr = 0; //global
int run_nr = 0; //global
int cat_nr = 0; //global
int game_nr = 0; //global

User** user_list;
Run** run_list;
Category** cat_list;
Game** game_list;

void addUser(User*& u){
    if(user_nr!=0){
        User** t = new User*[user_nr];
        copy(user_list, user_list+user_nr, t);
        delete user_list;
        user_list = new User*[++user_nr];
        copy(t, t+user_nr-1, user_list);
        user_list[user_nr-1] = u;
    }
    else{
        user_list = new User*[1];
        user_nr++;
        user_list[0] = u;
    }
}

void addRun(Run*& r){
    if(run_nr!=0){
        Run** t = new Run*[run_nr];
        copy(run_list, run_list+run_nr, t);
        delete run_list;
        run_list = new Run*[++run_nr];
        copy(t, t+run_nr-1, run_list);
        run_list[run_nr-1] = r;
    }
    else{
        run_list = new Run*[1];
        run_nr++;
        run_list[0] = r;
    }
}

void addCategory(Category*& cat){
    if(cat_nr!=0){
        Category** t = new Category*[cat_nr];
        copy(cat_list, cat_list+cat_nr, t);
        delete cat_list;
        cat_list = new Category*[++cat_nr];
        copy(t, t+cat_nr-1, cat_list);
        cat_list[cat_nr-1] = cat;
    }
    else{
        cat_list = new Category*[1];
        cat_nr++;
        cat_list[0] = cat;
    }
}

void addGame(Game*& g){
    if(game_nr!=0){
        Game** t = new Game*[game_nr];
        copy(game_list, game_list+game_nr, t);
        delete game_list;
        game_list = new Game*[++game_nr];
        copy(t, t+game_nr-1, game_list);
        game_list[game_nr-1] = g;
    }
    else{
        game_list = new Game*[1];
        game_nr++;
        game_list[0] = g;
    }
}

int main(){
    bool isRunning = true; //daca suntem in "meniu"
    char command[20];
    while(isRunning){ //un loop pentru "meniu"
        cout<<"What do you want to do?\n";
        cin>>command;
        if(strcmp(command, "quit")==0){
            isRunning = false;
            cout<<"Shutting down.";
        }else if(strcmp(command, "addUser")==0){ //creeaza un user nou
            char name[20];
            cout<<"What is the user's name?\n";
            cin>>name;
            User* u = new User(name);
            addUser(u);
        }else if(strcmp(command, "userList")==0){ //afiseaza lista globala de useri
            for(int i=0;i<user_nr;i++){
                user_list[i]->afis();
            }
        }else if(strcmp(command, "editUser")==0){
            int userId = user_nr;
            while(userId>=user_nr||userId<0){
                cout<<"What user do you want to change? (global id)\n";
                cin>>userId;
                if(userId>=user_nr||userId<0){
                    cout<<"That user id does not exist.\n";
                }
            }
            cout<<"Current name for this user is "<<user_list[userId]->getUsername()<<'\n';
            cout<<"What name do you want to give to this user?\n";
            char name[20];
            cin>>name;
            user_list[userId]->setUsername(name);
        }else if(strcmp(command, "addRun")==0){ //creeaza un run nou
            int h, m, s, ms, userId = user_nr;
            char description[100];
            cout<<"Please type out the time of the run: hours, minutes, seconds, milliseconds.\n";
            cin>>h>>m>>s>>ms;
            RunTime* rt = new RunTime(h, m, s, ms);
            while(userId>=user_nr||userId<0){
                cout<<"Which user submitted the run? (global id)\n";
                cin>>userId;
                if(userId>=user_nr||userId<0){
                    cout<<"That user id does not exist.\n";
                }
            }
            cout<<"Type the description of this run.\n";
            cin.get();
            cin.getline(description, 100);
            Run* r = new Run(*user_list[userId], *rt, description);
            addRun(r);
        }else if(strcmp(command, "runList")==0){ //afiseaza lista globala de run-uri
            for(int i=0;i<run_nr;i++){
                run_list[i]->afis();
                cout<<"Run global id "<<i<<'\n';
            }
        }else if(strcmp(command, "runShow")==0){ //afiseaza informatii despre un run anume
            int runId = run_nr;
            while(runId>=run_nr||runId<0){
                cout<<"Which run do you want to see? (global id)\n";
                cin>>runId;
                if(runId>=run_nr||runId<0){
                    cout<<"That run id does not exist.\n";
                }
            }
            run_list[runId]->afis();
        }else if(strcmp(command, "editRun")==0){
            int runId = run_nr;
            while(runId>=run_nr||runId<0){
                cout<<"What run do you want to change? (global id)\n";
                cin>>runId;
                if(runId>=run_nr||runId<0){
                    cout<<"That run id does not exist.\n";
                }
            }
            char desc[100];
            cout<<"What description do you want to give to your run?\n";
            cout<<"Current description: "<<run_list[runId]->getDescription()<<'\n';
            cin.get();
            cin.getline(desc, 100);
            run_list[runId]->setDescription(desc);
        }else if(strcmp(command, "addCategory")==0){ //creeaza o categorie noua
            char name[20], description[20];
            cout<<"What is the name of this category?\n";
            cin.get();
            cin.getline(name, 20);
            cout<<"Type the description of this category.\n";
            cin.getline(description, 100);
            Category* cat = new Category(name, description);
            addCategory(cat);
        }else if(strcmp(command, "catList")==0){ //afiseaza lista globala de categorii
            for(int i=0;i<cat_nr;i++){
                cat_list[i]->afis();
                cout<<"Category global id "<<i<<'\n';
            }
        }else if(strcmp(command, "addRunToCat")==0){ //adauga un run la o categorie
            int runId=run_nr, catId=cat_nr;
            while(catId>=cat_nr||catId<0){
                cout<<"What category do you want to add the run to? (global id)\n";
                cin>>catId;
                if(catId>=cat_nr||catId<0){
                    cout<<"That category id does not exist.\n";
                }
            }
            while(runId>=run_nr||runId<0){
                cout<<"What run do you want to add to the category? (global id)\n";
                cin>>runId;
                if(runId>=run_nr||runId<0){
                    cout<<"That run id does not exist.\n";
                }
            }
            cat_list[catId]->addRun(run_list[runId]);
        }else if(strcmp(command, "removeRunFromCat")==0){ //elimina un run dintr-o categorie
            int catId = cat_nr;
            while(catId>=cat_nr||catId<0){
                cout<<"What category do you want to remove the run from? (global id)\n";
                cin>>catId;
                if(catId>=cat_nr||catId<0){
                    cout<<"That category id does not exist.\n";
                }
            }
            int runId = cat_list[catId]->getRunNr();
            while(runId>=run_nr||runId<0){
                cout<<"What run do you want to remove from the category? (LOCAL/category-based id)\n";
                cin>>runId;
                if(runId>=run_nr||runId<0){
                    cout<<"That run id does not exist.\n";
                }
            }
            cat_list[catId]->removeRun(runId);
        }else if(strcmp(command, "showTotalTimeCat")==0){
            int catId = cat_nr;
            while(catId>=cat_nr||catId<0){
                cout<<"What category do you want to get the total run time from? (global id)\n";
                cin>>catId;
                if(catId>=cat_nr||catId<0){
                    cout<<"That category id does not exist.\n";
                }
            }
            cout<<cat_list[catId]->getTotalTime();
        }else if(strcmp(command, "leaderboard")==0){
            int catId = cat_nr;
            while(catId>=cat_nr||catId<0){
                cout<<"Which category's leaderboards do you want to see? (global id)\n";
                cin>>catId;
                if(catId>=cat_nr||catId<0){
                    cout<<"That category id does not exist.\n";
                }
            }
            cat_list[catId]->afisSorted();
        }else if(strcmp(command, "catRunList")==0){
            int catId = cat_nr;
            while(catId>=cat_nr||catId<0){
                cout<<"Which category's leaderboards do you want to see? (global id)\n";
                cin>>catId;
                if(catId>=cat_nr||catId<0){
                    cout<<"That category id does not exist.\n";
                }
            }
            cat_list[catId]->afis();
        }else if(strcmp(command, "addGame")==0){ //creeaza un joc nou
            char name[20], description[20];
            cout<<"What is the name of this game?\n";
            cin.get();
            cin.getline(name, 20);
            cout<<"Type the description of this game.\n";
            cin.getline(description, 100);
            Game* g = new Game(name, description);
            addGame(g);
        }else if(strcmp(command, "gameList")==0){ //afiseaza lista de jocuri
            for(int i=0;i<game_nr;i++){
                game_list[i]->afis();
                cout<<"Game global id "<<i<<'\n';
            }
        }else if(strcmp(command, "addCatToGame")==0){ //adauga o categorie la un joc
            int gameId=game_nr, catId=cat_nr;
            while(gameId>=game_nr||gameId<0){
                cout<<"What game do you want to add the category to? (global id)\n";
                cin>>gameId;
                if(gameId>=game_nr||gameId<0){
                    cout<<"That game id does not exist.\n";
                }
            }
            while(catId>=cat_nr||catId<0){
                cout<<"What category do you want to add to the game? (global id)\n";
                cin>>catId;
                if(catId>=cat_nr||catId<0){
                    cout<<"That category id does not exist.\n";
                }
            }
            game_list[gameId]->addCategory(cat_list[catId]);
        }else{ //daca comanda data nu corespunde cu nici o comanda de mai sus
            cout<<"Invalid command.";
        }
    }
    //stergem listele globale dupa ce se termina executia loop-ului de "meniu"
    for(int i=0;i<game_nr;i++){
        delete game_list[i];
    }
    delete[] game_list;
    for(int i=0;i<cat_nr;i++){
        delete cat_list[i];
    }
    delete[] cat_list;
    for(int i=0;i<run_nr;i++){
        delete run_list[i];
    }
    delete[] run_list;
    for(int i=0;i<user_nr;i++){
        delete user_list[i];
    }
    delete[] user_list;
    return 0;
}
