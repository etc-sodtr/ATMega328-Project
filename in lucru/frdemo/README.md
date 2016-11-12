# Proiect demo cu FreeRTOS

În acest director găsiți un mic proiect care folosește FreeRTOS. Proiectul
a fost construit pornind de la zero pentru a demonstra prin comparație cu
proiectul demo din FreeRTOS necesarul minim de a porni un astfel de proiect.
În acest sens a fost inclus numai codurile sursă necesare și implementat un
task simplu care manipulează ledurile conectate la portul C.

Prin acest demo puteți înțelege mai bine configurarea necesară și construirea
unui makefile care să includă tot ce e nevoie pentru a crea un program pentru
AVR.

Demo-ul a fost gândit pentru plăcuța de dezvoltare cu ATmega128, care rulează
la 11.0592MHz, astfel a fost nevoie de o adaptare a codurilor sursă aferente
din demo-ul FreeRTOS (gândit pentru ATmega323 rulând la 8MHz).
