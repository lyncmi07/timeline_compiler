class Person extends Entity {
  _construct (int birthYear, int deathYear, string nameP) {
    startYear = birthYear;
    endYear = deathYear;
    name = nameP;
  }
}



class Dictator extends Person {
  string countryInCharge;
  _construct(int birthYear, int deathYear, string nameP, string countryInChargeP) {
    super(birthYear, deathYear, nameP);
    countryInCharge = countryInChargeP;
  }
}

Dictator franco = new Dictator(1892, 1975, "General Franco", "Spain")[];
