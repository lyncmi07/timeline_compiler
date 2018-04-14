class Person extends Animal {

  _construct(string nameP, int birthDate, int deathDate) {
    name = nameP;
    startYear = birthDate;
    endYear = deathDate;
    legNumber = 2;
  }

  empty printout(void) {
    print("Name: ");
    println(name);
    print("Birth Date: ");
    println(startYear);
    print("Death Date: ");
    println(endYear);
    print("Number of legs: ");
    println(legNumber);
  }
}
