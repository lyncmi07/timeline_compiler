class War extends Event {

  Dictator starter;

  _construct(int startYearP, string nameP, Dictator starterP) {
    eventYear = startYearP;
    name = nameP;
    starter = starterP;
  }
}

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
  Dictator enemy;
  
  _construct(int birthYear, int deathYear, string nameP, Country countryInChargeP) {
    super(birthYear, deathYear, nameP);
    countryInCharge = countryInChargeP;
  }
  
  empty setEnemy(Dictator enemyP) {
    enemy = enemyP;
  }
  
  empty printName(void) {
    print("The Dictator ");
    println(GetEventContainingDataValue("War", "starter", enemy).name);
  }
  
  string giveFullName(void) {
    return();
  }
}

Country sovietUnion = new Country (1922, 1991, "The Union of Soviet Socialist Republics") [];
Country nationalistSpain = new Country (1936, 1978, "Nationalist Spain") [];
Dictator franco = new Dictator(1892, 1975, "General Franco", nationalistSpain)[];
Dictator stalin = new Dictator(1878, 1953, "Joseph Stalin", sovietUnion)[];
War spanishWar = new War(1950, "Spanish Invasion", stalin)[];
franco.printName();



