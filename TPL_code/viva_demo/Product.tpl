class Product extends Entity {
  Company owner;
  _construct(int modelYear, int endYearP, Company ownerP, string nameP) {
    startYear = modelYear;
    endYear = endYearP;
    owner = ownerP;
    name = nameP;
  }
}
