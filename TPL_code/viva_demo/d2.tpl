#include "./Person.tpl"
#include "./Company.tpl"
#include "./GreatIdea.tpl"
#include "./Product.tpl"

Person steveJobs = new Person(1955, 2011, "Steve Jobs")[];
Company apple = new Company(1976, 2080, "Apple Inc.")[steveJobs];
Person billGates = new Person(1955, 2030, "Bill Gates")[];
Company microsoft = new Company(1975, 2080, "Microsoft Corp.")[billGates];
