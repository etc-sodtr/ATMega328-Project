Acest folder contine o varianta un pic imbunatatita a folderului "exemplu".

Programul din sus-numitul folder nu a mers corect. Ce s-a intamplat? Eu doream sa aprind pe rand toate ledurile din portul C. Pentru aceasta, la un moment dat,  scriam 1 pe fiecare pin, si deci si pe pinul PC6. Totusi, uitandu-se domn profesor peste datasheet, a constatat ca PC6 era pinul de reset. (!!!) Deci eu dadeam reset la circuit fara sa imi dau seama. Acum programul opreste parcurgerea ledurilor la PC; totul merge bine acum.

O alta imbunatatire:
Am adaugat inca un task care face sa clipeasca ledul de pe PB5. Atentie! Si aici am avut o problema. La un moment dat nu am mai apelat nicio intrerupere in subrutina "vBlinkLed", pentru ca ledul sa ramana permanent aprins. De aceea, programul s-a blocat in aceasta subrutina, nemaiputand sa permita si subrutinei "blink" sa se execute . Solutia: adaugarea unei intreruperi

Va urma...
