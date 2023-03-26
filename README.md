# Proiect 1 - Curs Programare Orientata pe Obiecte
## Speedrunning Website/Database-like Program - Scurta descriere
Proiectul este o "recreatie" a unui site/baze de date pe care sunt publicate speedrun-uri (cum ar fi speedrun.com).

## Functionalitate
Programul este echipat cu un "meniu" (in terminal). Utilizatorul poate rula mai multe comenzi de creatie/modificare a unor obiecte din clasele programului: User, Run_Time, Run, Category, Game.
Pentru o lista intreaga de comenzi puteti scrie help in cadrul programului.

![Adaugarea unui user nou](https://imgur.com/Bi0HNEc)

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
  - intr-un category se pot adauga si elimina run-uri
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
