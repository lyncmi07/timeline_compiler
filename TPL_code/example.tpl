class Person extends Entity {
  
  _construct(int birthYear, int deathYear, string nameP) {
    name = nameP;
    startYear = birthYear;
    endYear = deathYear;
  }
}

Person zac = new Person(1950, 2030,"Zac")[];
Person michael = new Person(1996, 2070, "Michael")[zac];
