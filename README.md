# OS Project : *License to kill*
## Mathieu TIMMERMAN, Félix DAUNE, Yasser FARESS et Florian PICART

---
#### Project description
A foreign spy network is operating in a city.
This network is composed of source agents ﴾espies﴿ in charge of retrieving information from "sensitive" companies installed in the city, as well as a case officer in charge of collecting information from his source agents, and then transmitting it to his superiors.

We wish to run a simulation composed of separate processes that implement the recovery of information by this spy network, as well as the fight against it by a counterintelligence officer stationed in the city.


---

### Compilation
To compile the whole program, place yourself in the project folder.\
And, in the Terminal, use the command: `$ make`.


### Running the program
To run the program, just use the command: `$ ./spy_simulation 2> error.log`

---

### Testing functionalities
To test the enemy country receiving the messages, you must compile the whole project (with `$ make`) and then `$ make test`.
Then, run the sub-program: `$ ./monitor` and, in an other terminal, run `$ ./test_ennemy`. A message should appear in the monitor.