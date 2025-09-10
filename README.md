Functionalitati adaugate in plus pentru marire:

tabla de joc - avea cateva cazuri specifice in care ori nu se extindea bine, ori nu se fixa bine, ori se puteau juca carti pe pozitii "interzise"(pozitii care nu aveau carti adiacente)
             - am reparat-o complet si adaptat-o atat pentru modul Training (3x3), cat si pentru celelalte moduri (4x4)

regex - verifica numele introdus de user in Main Menu, acesta nu are voie sa foloseasca cuvinte obscene (sunt mai multe cuvinte obscene introduse intr-un fisier)

lambdas - desi aveam cateva functii lambda folosite, am adaugat mai multe, folosite pentru semnale Qt si callback-uri

smart pointers - am folosit std::unique_ptr pentru management pentru mai multe ferestre, dar si pentru BoardView (UI-ul tablei de joc)
               - acum in loc sa declar ferestre cu *NumeFereastra, folosesc std::unique_ptr pentru management automat de memorie
               - pentru structura de date pentru BoardView am folosit std::vector<std::vector<std::unique_ptr<QPushButton>>>

templates - majoritar folosite in clasa Board, pentru operatii pe tabla de joc
Ex:
          - template<typename Operation> void forEachPosition(Operation op): parcurge toata tabla si aplica o operatie pe fiecare pozitie
          - template<typename Predicate> std::vector<Position> findPositions(Predicate pred): cauta toate pozitiile care indeplinesc o conditie

ranges - cateva adaugate pentru operatii pe tabla de joc
