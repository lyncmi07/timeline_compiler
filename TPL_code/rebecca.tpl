class Person extends Entity {
  _construct(string nameP, int birthYear, int deathYear) {
    name = nameP;
    startYear = birthYear;
    endYear = deathYear;
  }
}

class MurderEvent extends Event {
  Person murderer;
  Person victim;
  
  _construct(string nameP, Person murdererP, Person victimP, int eventYearP) {
    murderer = murdererP;
    victim = victimP;
    name = nameP;
    eventYear = eventYearP;
  }
  
  empty onEvent(void) {
    victim.endYear = eventYear;
  }
}

class RevengeMurder extends Event {
  Person murderer;
  int originalVictim;
  int victim;
  
  
  _construct(string nameP, Person murdererP, int originalVictimP, int eventYearP) {
    murderer = murdererP;
    originalVictim = originalVictimP;
    eventYear = eventYearP;
    name = nameP;
  }
  
  empty onEvent(void) {
    murderer.endYear = originalVictim;
  }
}

Person jenny = new Person("Jenny", 1900,1950) [];
Person ellie = new Person("Ellie", 1890,1970) [];
Person clare = new Person("Clare", 1920,2000) [];
Person becca = new Person("Becca", 1880,1960) [];

MurderEvent beccaMurder1 = new MurderEvent("Becca's murder by jenny",jenny, becca, 1925) [jenny, becca];
MurderEvent beccaMurder2 = new MurderEvent("Becca's murder by ellie",ellie, becca, 1940) [ellie, becca];

MurderEvent actualEvent = GetEventContainingDataValue("MurderEvent", "victim", becca);
<MurderEvent actualEvent = beccaMurder1;>
MurderEvent beccaRevenge = new MurderEvent("The revenge", clare, actualEvent.murderer, 1945)[clare, actualEvent.murderer];





