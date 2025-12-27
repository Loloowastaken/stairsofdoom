# Stairs of DOOM

*Mini-joc cu 100 de nivele in care jucatorul trebuie sa urce scari si sa se bata cu inamici.*

Proiectul este configurat cu CMake.

Instrucțiuni pentru terminal:

1. Pasul de configurare
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
# sau ./scripts/cmake.sh configure
```

Sau pe Windows cu GCC:
```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -G Ninja
# sau ./scripts/cmake.sh configure -g Ninja
```

La acest pas putem cere să generăm fișiere de proiect pentru diverse medii de lucru.

## Descriere a proiectului

**Atentie**: Proiectul este facut cu structura unui joc. Totusi, este un proiect facut pentru simplul scop de a intra la colocviu. Asadar, va rog nu incercati sa il 'jucati', fiindca nu este deloc balansat.

Fiecare entitate a jocului are atributele urmatoare:
- LVL = Level, care creste pentru Player pe parcursul luptelor, iar pentru inamici creste la fiecare nivel in mod incremental.
- HP = Health, care determina cata viata are o entitate, aceasta fiind considerata 'moarta' la HP=0. HP nu poate fi mai mic decat 0.
- ATK = Attack, care determina puterea de baza cu care ataca o entitate pe alta
- DEF = Defense, care determina cat 'damage' se ia intr-un atac, bazat pe formula DMG=ATK-DEF/2.
- SPD = Speed, care determina ordinea turei intr-o lupta.

Player are in mod specific si:
- EXP=Experienta, obtinuta prin explorare si lupta, care va creste LVL la un anumit punct, dupa care este redus la 0.
- EXPTotal = cata experienta este necesara pentru a creste LVL.
- Cresterea LVL duce la cresterea atributelor.
- Gold = Aur, cu care se pot cumpara iteme (vezi Shop)
- Inventory = cu clasa template ItemContainer isi poate gestiona si folosi diverse iteme care cresc staturile anterioare.

Enemy au doua enum classuri diferite:
- Type = determina ce fel de abilitate folosesc in timpul luptei
- Difficulty = determina scaling pt atribute si aur obtinut cand sunt invinsi

- Enemy pot fi (in ordine de putere) - SLIME; GOBLIN; SKELETON; ORC
- Enemy pot fi (in ordine de dificultate) EASY (nivel<=10); MEDIUM(nivel<=30); HARD(nivel<=80); BOSS (fiecare 10 nivele si 81-100)

Atributele, aurul si experienta data cand sunt invinsi sunt calculate bazat pe aceste doua enumuri, precum si nivelul curent.

Boss se mai refera si la clasa derivata speciala lui Enemy, care poate intra in faza speciala dupa ce s-a ajuns la o anumita valoare de HP.


Jocul are 100 de nivele, fiecare avand aceeasi structura.
Pe fiecare nivel, exista 1-3 inamici, plus un 'boss' cu staturi imense odata la 10 nivele.
Acestia trebuie invinsi inainte sa se avanseze la urmatorul nivel.

Scopul jocului este completarea levelului 100 (nu este recomandat sa incercati) fara ca Player sa moara.
Jocul se termina cand Player HP = 0 sau levelcurent>100. 

Jucatorul are 4 actiuni pe fiecare nivel:
1. Explorare -> poate intampina inamici, aur, sau item
2. Shop -> poate cumpara iteme
3. Inventar (poate fi accesat si in lupta) -> poate folosi iteme
4. Avansare -> Avanseaza la urmatorul nivel (nepermis pana cand nivelul este gol)

Faza de explorare poate duce la lupta.
Lupta are trei stagii:
1. Incepe prin a determina ordinea in care se va ataca, bazata pe SPD.
2. Tura jucatorului, cand vine.
3. Tura inamicului, cand vine.

Tura jucatorului ii permite 5 alegeri:
1. Atac, dupa care alege inamicul pe care sa il atace.
2. Abilitate speciala, dupa care alege pe care sa o foloseasca
3. Foloseste un item.
4. Fuge din lupta.
5. Analizeaza inamicii, poate vedea descrierea si dificultatea lor.

Abilitatiile speciale jucatorului sunt:
1. Heroic Strike = dublu damage in urmatoarul atac. Cooldown 3 ture.
2. Shield Bash = 1.5x damage si atributele inamicului sunt reduse. Cooldown 4 ture.
3. Second Wind = Heal de urgenta. 2 folosiri per lupta.

Abilitatile speciale inamicilor sunt:
1. Goblin -> Trick = Isi creste SPD.
2. Skeleton -> Fortify = Isi creste DEF.
3. Orc -> Rage = Isi creste ATK mult, dar scade DEF.
4. Slime -> Bulwark Form = Isi creste DEF mult, dar scade ATK si SPD.

Fugirea din lupta nu reseteaza inamicii. Inamicii trebuie invinsi pentru a avansa.
Lupta se poate termina prematur cand este clar ca inamicii sunt prea slabi pentru a omori jucatorul.

Itemele sunt banale:
1. HealthPotion = restoreaza 30% din HP maxim

2,3,4. Attack/Defense/SpeedBoost = creste ATK/DEF/SPD.

Shop permite cumpararea itemelor, fiecare item are un nume si pret corespunzator. Itemele se adauga in inventar atat timp cat jucatorul are aurul mai mare decat pretul, de acolo pot fi folosite.

*Observatie:* Jocul foloseste in mod generos libraria random, pentru a determina mai multe evenimente in joc, precum:
- Daca se intampina inamici sau se gaseste aur sau se gaseste item sau se gaseste nimic in Explorare.
- Daca inamicul ataca sau isi foloseste abilitatea unica.
- Daca la avansare se creste LVL lui Player.
- Daca se poate scapa sau nu (sansa 10-90%, calculata bazata pe SPD.)
- Cat aur si cata experienta se primeste, intr-un anumit rang.
- Numele inamicilor si tipul lor, astfel incat aproape fiecare lupta este diferita.

Astfel, tastatura.txt are inputuri banale care nu interactioneaza cu aceste aspecte, dat fiind ca ar fi prea variabil.
