# Proiect 1 - Curs Programare Orientata pe Obiecte
## Speedrunning Website/Database-like Program - Scurta descriere
Proiectul este o "recreatie" a unui site/baze de date pe care sunt publicate speedrun-uri (cum ar fi speedrun.com).

## Functionalitate
Programul este echipat cu un "meniu" (in terminal). Utilizatorul poate rula mai multe comenzi de creatie/modificare a unor obiecte din clasele programului: User, Run_Time, Run, Category, Game.

Utilizarea normala a programului implica adaugarea unor obiecte noi in ordinea de mai sus (User nou, apoi Run nou care e conectat de un user s.a.m.d.).

Lista de comenzi utilizabile in cadrul programului:
- `quit` - sfarseste executia programului
- `addUser` - creeaza un User nou
- `userList` - afiseaza lista cu toti userii
- `editUser` - permite modificarea numelui unui user
- `addRun` - creeaza un Run nou
- `runList` - afiseaza lista globala cu toate run-urile
- `runShow` - afiseaza informatii despre un run
- `editRun` - permite modificarea descrierii unui run
- `addCategory` - creeaza o categorie noua
- `catList` - afiseaza lista globala cu toate categoriile
- `addRunToCat` - adauga un run la o categorie
- `removeRunFromCat` - elimina un run dintr-o categorie
- `showTotalTimeCat` - afiseaza timpul adunat al tuturor run-urilor dintr-o categorie
- `addGame` - creeaza un joc nou
- `gameList` - afiseaza lista cu toate jocurile
- `addCatToGame` - adauga o categorie la un joc

## Clase
- **User** - descrie un user al site-ului
  - un User are:
    - un **username**
    - un **id**
- **Run_Time** - reprezinta un timp
  - un Run_Time are ore, minute, secunde, milisecunde
  - se poate returna si afisa timpul descris
- **Run** - este un speedrun
  - un Run are:
    - un **submitter** (obiect User) - bineinteles, fiecare run este publicat de un user
    - un **timp** (obiect RunTime) - de asemenea, un run este bazat pe timp, asa ca trebuie sa fie descris si de un timp
    - o **descriere**
- **Category** - o categorie pe care sunt postate run-uri
  - un Category are:
    - un **run_list** (un array de pointeri catre run-uri) - o lista de run-uri
    - un **run_nr** - numar de run-uri din categorie
    - un **nume**
    - o **descriere**
  - intr-un category se pot adauga si elimina run-uri, si se poate afisa timpul total al run-urilor din category
- **Game** - un joc cuprins din mai multe categorii
  - un Game are:
    - un **cat_list** (alt array de pointeri, de data asta catre categorii) - o lista de categorii
    - un **cat_nr** - numar de categorii din joc
    - un **nume**
    - o **descriere**
  - intr-un game se pot adauga categorii noi

Fiecare clasa mai are si metode getter si setter pentru majoritatea campurilor, precum si metode de afisaj (`afis()`).

## Proiect realizat de
Velisan George-Daniel
