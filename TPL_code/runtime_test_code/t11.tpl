class Person extends Entity {
  _construct (int birthYear, int deathYear, string nameP) {
    startYear = birthYear;
    endYear = deathYear;
    name = nameP;
  }
}

class Country extends Entity {
  _construct (int founding, int dissolved, string nameP) {
    startYear = founding;
    endYear = dissolved;
    name = nameP;
  }
}

class Dictator extends Person {
  Country countryInCharge;
  _construct(int birthYear, int deathYear, string nameP, Country countryInChargeP) {
    super(birthYear, deathYear, nameP);
    countryInCharge = countryInChargeP;
  }
}


Country nationalistSpain = new Country (1936, 1978, "Nationalist Spain") [];
Dictator franco = new Dictator(1892, 1975, "General Franco", nationalistSpain)[];
