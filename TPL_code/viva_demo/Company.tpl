class Company extends Entity {
  _construct(int foundingYear, int bankruptYear, string nameP) {
    startYear = foundingYear;
    endYear = bankruptYear;
    name = nameP;
  }
}
