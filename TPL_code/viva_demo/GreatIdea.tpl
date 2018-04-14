class GreatIdea extends Event {
  Company owner;
  _construct(int eventYearP, Company ownerP, string nameP) {
    eventYear = eventYearP;
    owner = ownerP;
    name = nameP;
  }
}
