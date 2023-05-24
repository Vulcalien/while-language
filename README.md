## Language:
- [🇮 🇹  Italiano](#interprete-per-il-linguaggio-while)

---

# Interprete per il linguaggio WHILE
In questo repository si trova un'implementazione interpretativa del
linguaggio WHILE, descritto in
[questo documento](https://www.dmi.unict.it/barba/FONDAMENTI/PROGRAMMI-TESTI/READING-MATERIAL/SEMANTICA/sos-while.pdf).

## Download
L'interprete già compilato per Linux (x86-64) può essere scaricato a
[questo link](https://github.com/Vulcalien/while-language/releases/download/1.0/while-interpreter_linux64).
Se la versione già compilata non dovesse funzionare, o per utilizzarlo
su Windows, è raccomandato compilare l'interprete.

## Compilazione
Per compilare l'interprete è necessario avere installato un compilatore
C e GNU Make. Dopo di che, basta eseguire il comando `make build`.

Il progetto usa GCC di default. Se si preferisce usare un'altro
compilatore, modificare la linea `CC := gcc` nel file `Makefile`.

## Esecuzione
L'interprete può sia leggere input dall'utente che leggere il programma
da un file. Se si esegue l'interprete _senza_ passare alcun argomento,
esso leggerà input da terminale. Altrimenti, proverà a leggere il file
passato come argomento.

Per esempio, `./while-interpreter test.while` legge ed esegue il file
'test.while'. Invece `./while-interpreter` chiederà all'utente di
inserire un programma WHILE.
