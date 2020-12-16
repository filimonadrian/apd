# Laboratorul 08 - Introducere în programarea distribuită cu MPI

## Despre MPI și programarea distribuită

Până acum, ați lucrat cu programarea paralelă, care presupune existența a mai multor fire de execuție (threads), care execută instrucțiuni în mod paralel și concurent, ele având acces la aceeași zonă de memorie, în cadrul unei singure mașini de calcul, care are mai multe procesoare.

Putem extinde conceptul de programare paralelă prin folosirea a mai multor mașini de calcul, care sunt conectate în cadrul unei rețele într-un concept numit programarea distribuită.

Spre deosebire de programarea paralelă, în programarea distribuită nu există conceptul de memorie partajată (shared memory). Întrebarea este următoarea: cum poate o mașină să știe ce date are o altă mașină din cadrul rețelei? Soluția este comunicarea prin mesaje, realizată de către mașinile (de tip nod) dintr-o rețea. Comunicarea prin mesaje are două funcționalități:

- comunicarea: un nod (transmițător / sender) trimite date prin intermediul unui canal de comunicație către un alt nod (receptor / receiver)
- sincronizarea: un mesaj nu poate fi recepționat înainte ca acesta să fie transmis

**MPI (Message Passing Interface)** reprezintă un standard pentru comunicarea prin mesaje, elaborat de MPI Forum, și are la bază modelul proceselor comunicante prin mesaje.

Un proces reprezintă un program aflat în execuție și se poate defini ca o unitate de bază care poate executa una sau mai multe sarcini în cadrul unui sistem de operare. Spre deosebire de thread-uri, un proces are propriul său spațiu de adrese (propria zonă de memorie) și acesta poate avea, în cadrul său, mai multe thread-uri în execuție, care partajează resursele procesului.

În cadrul lucrului în C/C++, MPI reprezintă o bibliotecă, care are funcționalitățile implementate într-un header numit mpi.h. Pentru compilare, la MPI există un compilator specific:

- mpicc, pentru lucrul în C
- mpic++, pentru lucrul în C++
În ambele limbaje, pentru rularea unui program MPI folosim comanda mpirun, împreună cu parametrul -np, unde precizăm numărul de procese care rulează în cadrul programului distribuit.

Exemplu:

- compilare:
  - C: mpicc hello.c -o hello
  - C++: mpic++ hello.cpp -o hello
- rulare: mpirun -np 4 hello - rulare cu 4 procese


> Dacă încercați să rulați comanda mpirun cu un număr de procese mai mare decât numărul de core-uri fizice disponibile pe procesorul vostru, este posibil să primiți o eroare cum ca nu aveți destule sloturi libere. Puteți elimina acea eroare adăugând parametrul –oversubscribe atunci când rulați mpirun.


> Instalare MPI: Pentru a lucra cu MPI, trebuie să instalați biblioteca pentru MPI pe Linux, folosind următoarea comandă: `sudo apt install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev`


## Implementarea unui program distribuit în MPI

### Exemplu de program MPI - Hello World:
```c
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
 
#define  MASTER 0
 
int main (int argc, char *argv[]) {
    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);
    if (rank == MASTER)
        printf("MASTER: Number of MPI tasks is: %d\n",numtasks);
    else
        printf("WORKER: Rank: %d\n",rank);
 
    MPI_Finalize();
}

```

Un comunicator (MPI_Comm) reprezintă un grup de procese care comunică între ele. MPI_COMM_WORLD reprezintă comunicatorul default, din care fac parte toate procesele.

Funcții:

- MPI_Init - se inițializează programul MPI, mai precis se creează contextul în cadrul căruia rulează procesele. Argumentele din linie de comandă sunt pasate către contextul de rulare a proceselor.
- MPI_Comm_size - funcție care determină numărul de procese (numtasks) care rulează în cadrul comunicatorului (de regulă MPI_COMM_WORLD)
- MPI_Comm_rank - funcție care determină identificatorul (rangul) procesului curent în cadrul comunicatorului.
- MPI_Get_processor_name - determină numele procesorului
- MPI_Finalize - declanșează terminarea programului MPI

Putem crea comunicatoare dintr-un alt comunicator folosind funcția MPI_Comm_split, care împarte un comunicator în mai multe comunicatoare mai mici. Semnătura funcției este următoarea: int MPI_Comm_split(MPI_Comm comm, int color, int key, MPI_Comm * newcomm), unde:

- comm - comunicatorul este împărțit
- color - identificator al noului comunicator, din care face parte un proces (de regulă rang_vechi_proces / dimensiune_comunicator_nou)
- key - noul rang al procesului în cadrul noului comunicator (de regulă rang_vechi_proces % dimensiune_comunicator_nou)
- newcomm - noul comunicator format


## Funcții de transmisie a datelor în MPI

## MPI_Send
MPI_Send reprezintă funcția prin care un proces trimite date către un alt proces. Semnătura funcției este următoarea:

`int MPI_Send(void* data, int count, MPI_Datatype datatype, int destination, int tag, MPI_Comm communicator)`, unde:

- data (↓) - reprezintă datele trimise de la procesul sursă către procesul destinație
- count (↓) - dimensiunea datelor transmise
- datatype (↓) - tipul datelor transmise
- destination (↓) - rangul / identificatorului procesului destinație, către care se trimit datele
- tag (↓) - identificator al mesajului
- communicator (↓) - comunicatorul în cadrul căruia se face trimiterea datelor între cele două procese
MPI_Send este o funcție blocantă. Mai precis, programul se blochează până când bufferul dat ca prim parametru poate fi refolosit, chiar dacă nu se execută acțiunea de primire a mesajului transmis de procesul curent (MPI_Recv). Dacă apare cazul în care procesul P1 trimite date (MPI_Send) la procesul P2, iar P2 nu are suficient loc în buffer-ul de recepție (buffer-ul nu are suficient loc liber sau este plin) atunci P1 se va bloca.

## MPI_Recv
MPI_Recv reprezintă funcția prin care un proces primește date de la un alt proces. Semnătura funcției este următoarea:

`int MPI_Recv(void* data, int count, MPI_Datatype datatype, int source, int tag, MPI_Comm communicator, MPI_Status* status)`, unde:

- data (↑) - reprezintă datele primite de la procesul sursă de către procesul destinație
- count (↓) - dimensiunea datelor primite
- datatype (↓) - tipul datelor primite
- source (↓) - rangul / identificatorului procesului sursă, care trimite datele
- tag (↓) - identificator al mesajului
- communicator (↓) - comunicatorul în cadrul căruia se face trimiterea datelor între cele două procese
- status - conține date despre mesajul primit, MPI_Status fiind o structură ce conține informații despre mesajul primit (sursa, tag-ul mesajului, dimensiunea mesajului). Dacă nu dorim să ne folosim de datele despre mesajul primit, punem MPI_STATUS_IGNORE, prin care se ignoră status-ul mesajului.
În situația în care procesul P apelează funcția de MPI_Recv(), el se va bloca până va primi toate datele asteptate, astfel că dacă nu va primi nimic sau ceea ce primește este insuficient, P va rămâne blocat. Adică MPI_Recv() se termină doar în momentul în care buffer-ul a fost umplut cu datele așteptate.

Structura MPI_Status include următoarele câmpuri:

- int count - dimensiunea datelor primite
- int MPI_SOURCE - identificatorul procesului sursă, care a trimis datele
- int MPI_TAG - tag-ul mesajului primit
MPI_Recv este o funcție blocantă, mai precis programul se poate bloca până când se execută acțiunea de trimitere a mesajului către procesul sursă.

Un exemplu de program în care un proces trimite un mesaj către un alt proces:

```c
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
 
int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks); // Total number of processes.
    MPI_Comm_rank(MPI_COMM_WORLD,&rank); // The current process ID / Rank.
    MPI_Get_processor_name(hostname, &len);
 
    srand(42);
    int random_num = rand();
    printf("Before send: process with rank %d has the number %d.\n", rank,
            random_num);
 
    if (rank == 0) {
        MPI_Send(&random_num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
    } else {
        MPI_Status status;
        MPI_Recv(&random_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Process with rank %d, received %d with tag %d.\n",
                rank, random_num, status.MPI_TAG);
    }
 
    printf("After send: process with rank %d has the number %d.\n", rank,
            random_num);
 
    MPI_Finalize();
 
}
```


> Când un proces X trimite un mesaj către un proces Y, tag-ul T al mesajului din MPI_Send, executat de procesul X, trebuie să fie același cu tag-ul mesajului din MPI_Recv, executat de procesul Y, deoarece procesul Y așteaptă un mesaj care are tag-ul T, altfel, dacă sunt tag-uri diferite, programul se va bloca.

### MPI_Bcast
MPI_Bcast reprezintă o funcție prin care un proces trimite un mesaj către toate procesele din comunicator (message broadcast), inclusiv lui însuși.

> În cadrul implementării MPI_Bcast sunt executate acțiunile de trimitere și de recepționare de mesaje, așadar nu trebuie să apelați MPI_Recv.
Semnătura funcției este următoarea:
`int MPI_Bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator)`, unde:

- data (↓ + ↑) - reprezintă datele care sunt transmise către toate procesele. Acest parametru este de tip - input pentru procesul cu identificatorul root și este de tip output pentru restul proceselor.
- count (↓) - dimensiunea datelor trimise
- datatype (↓)- tipul datelor trimise
- root (↓) - rangul / identificatorului procesului sursă, care trimite datele către toate procesele din - - comunicator, inclusiv lui însuși
- tag (↓) - identificator al mesajului
- communicator (↓) - comunicatorul în cadrul căruia se face trimiterea datelor către toate procesele din cadrul acestuia
 
### MPI_Scatter
MPI_Scatter este o funcție prin care un proces împarte un array pe bucăți egale ca dimensiuni, unde fiecare bucată revine, în ordine, fiecărui proces, și le trimite tuturor proceselor din comunicator, inclusiv lui însuși.

Semnătura funcției este următoarea: `int MPI_Scatter(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator)`, unde:

- send_data (↓) - reprezintă datele care sunt împărțite și trimise către procesele din comunicator
- send_count (↓) - reprezintă dimensiunea bucății care revine fiecărui proces (de regulă se pune ca fiind dimensiunea_totală / număr_de_procese).
- send_datatype (↓) - tipul datelor trimise către procese
- recv_data (↑) - reprezintă datele care sunt primite și stocate de către procese
- recv_count (↓) - dimensiunea datelor primite (de regulă dimensiunea_totală / număr_de_procese)
- recv_datatype (↓) - tipul datelor primite de către procese (de regulă este același cu send_datatype)
- root (↓) - identificatorul procesului care împarte datele și care le trimite către procesele din - - - - comunicator, inclusiv lui însuși
- communicator (↓) - comunicatorul din care fac parte procesele (de regulă MPI_COMM_WORLD)


### MPI_Gather
MPI_Gather este o funcție care reprezintă inversul lui MPI_Scatter, în sensul că un proces primește elemente de la fiecare proces din comunicator, inclusiv de la el însuși, și le unifică într-o singură colecție.

Semnătura funcției este următoarea: `int MPI_Gather(void* send_data, int send_count, MPI_Datatype send_datatype, void* recv_data, int recv_count, MPI_Datatype recv_datatype, int root, MPI_Comm communicator)`, unde:

- send_data (↓) - reprezintă datele care trimise de fiecare proces către procesul cu id-ul root
- send_count (↓) - reprezintă dimensiunea bucății trimisă de fiecare proces (de regulă se pune ca fiind - dimensiunea_totală / număr_de_procese).
- send_datatype (↓) - tipul datelor trimise de către procese
- recv_data (↑) - reprezintă datele care sunt primite și stocate de către procesul root
- recv_count (↓) - dimensiunea datelor primite (de regulă dimensiunea_totală / număr_de_procese)
- recv_datatype (↓) - tipul datelor primite de către procesul root (de regulă este același cu send_datatype)
- root (↓) - identificatorul procesului care primește datele (inclusiv de la el însuși)
- communicator (↓) - comunicatorul din care fac parte procesele (de regulă MPI_COMM_WORLD)

### Algoritmul inel
Algoritmul inel este un algoritm de tip undă, care funcționează pe topologii de sub formă de inel, unde fiecare nod proces are un vecin dedicat (Urm / next). În cadrul topologiilor inel, transmisia datelor se face folosind adresarea prin direcție, unde mai precis un nod proces trimite datele către vecinul său (Urm), astfel în cadrul topologiei formându-se un ciclu Hamiltonian.

În cadrul algoritmului, avem un nod proces inițiator, care trimite un jeton către vecinul său, care este pasat de fiecare proces către vecinul său de-a lungul ciclului, până când jetonul ajunge înapoi până la nodul proces inițiator.

Pseudocod:

```c
chan token[1..n] (tok_type tok) //  canalul de comunicatie

// initiator
process P[i] {
    tok_type tok;
    send token[next](tok); // trimite token-ul la urmatorul nod
    receive token[i](tok); // primeste token-ul de la nodul precedent
    decizie;
}

// neinitiator
process P[k, k = 1..n, k != n] {
    tok_type tok;
    receive token[k](tok); // primeste token-ul de la nodul precedent
    send token[next](tok); // trimite token-ul la urmatorul nod
}
```