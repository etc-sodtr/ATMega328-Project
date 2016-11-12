# Adăugare FreeRTOS la un proiect

FreeRTOS poate fi descărcat de la adresa [www.freertos.org](http://www.freertos.org).
Fișierul descărcat (prin meniul **Download source**) este o arhivă ce conține
codul sursă a sistemului de operare, o serie de demo-uri pentru fiecare
arhitectură de microcontroler și compilator suportat respectiv o serie de
biblioteci extinse pentru unele funcționalități des întâlnite în sisteme
încorporate (mai ales pentru IoT).

### Structura arhivei

Arhiva este structurată într-o arbore de directoare cu următoarea formă principală

- `FreeRTOS` (_sistemul de operare propriu-zis_)
    - `Demo` (_proiecte demo pentru fiecare arhitectură și compilator_)
        - `...`
        - `AVR_ATMega323_WinAVR` (_demo pentru AVR - util în laborator_)
        - `...`
        - `Common` (_un director ce conține surse comune pentru demo-uri inclusiv și pentru demoul AVR_)
        - `...`
    - `Source` (_sursele sistemului de operare_)
        - `include` (_headerele necesare, trebuie introdus în calea de căutare a compilatorului_)
        - `portable` (_fișierele sursă dependente de arhitectură organizat după diferite compilatoare_)
            - `...`
            - `GCC/ATMega323` (_fișierele aferente microcontrolerelor AVR_)
            - `...`
            - `MemMang` (_cinci variante pentru management al memoriei, poate fi ales după necesități_)
            - `...`
        - `croutine.c` (_rutine cooperative_)
        - `event_groups.c` (_tratare de evenimente_)
        - `list.c` (_liste dinamice folosite de SO_)
        - `queue.c` (_cozi de mesaje, mutecși, semafoare_)
        - `tasks.c` (_planificatorul_)
        - `timers.c` (_timere software_)
- `FreeRTOS-Plus` (_biblioteci generice oferind funcționalitate sporită_)
    - `Demo` (_demo-uri pentru folosirea bibliotecilor_)
    - `Source` (_sursele bibliotecilor_)
        - `FreeRTOS-Plus-CLI` (_linie de comandă_)
        - `FreeRTOS-Plus-FAT-SL` (_sistem de fișiere_)
        - `FreeRTOS-Plus-Trace` (_facilitate de depanare avansată a taskurilor paralele_)
        - `FreeRTOS-Plus-UDP` (_protocolul UDP_)
        - `etc ...`

### Creare proiect pornind de la demo

Cel mai simplu mod de a crea un proiect nou este pornind de la demo-ul aferent
arhitecturii alese. Acest demo conține pe lângă sursele ce demonstrează
funcționalitățile sistemului de operare (ce poate fi înlăturat în proiectul final)
și fișiere de configurare pentru compilatorul/IDE-ul ales, în cele mai multe
cazuri în forma unor fișiere Makefile care includ tot ce e nevoie pentru
compilare și de asemenea configurează compilatorul în mod optim pentru sistemul
de operare.

Se poate crea un nou proiect și de la zero, respectiv adăuga sistemul de operare
la un proiect deja existent. În acest caz trebuie adăugat la proiect fișierele
sursă din `FreeRTOS/Source` (cel puțin `list.c`, `task.c` și `queue.c`, restul
fiind opțional), sursele aferente arhitecturii din subdirectorul (`FreeRTOS/Source/portable`),
un manager de memorie din același subdirector (se poate opta și la o alocare
statică dacă resursele de memorie sunt limitate, dar atunci se poate
folosi numai un set restrâns de funcții din sistemul de operare), trebuie inserat
directorul (sau headerele din) `FreeRTOS/Source/include` în calea de căutare
a headerelor pentru compilator și trebuie creat un fișier numit `FreeRTOSConfig.h`
în care se configurează funcționalitățile folosite din sistemul de operare.
