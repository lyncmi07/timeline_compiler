#include "./Person.tpl"
#include "./Company.tpl"
#include "./GreatIdea.tpl"
#include "./Product.tpl"

Person steveJobs = new Person(1955, 2011, "Steve Jobs")[];
Company apple = new Company(1976, 2080, "Apple Inc.")[steveJobs];
Person billGates = new Person(1955, 2030, "Bill Gates")[];
Company microsoft = new Company(1975, 2080, "Microsoft Corp.")[billGates];

GreatIdea appleIdea = new GreatIdea(1997, apple, "mp3")[apple, steveJobs];
GreatIdea microsoftIdea = new GreatIdea(1999, microsoft, "mp3")[microsoft, billGates];

GreatIdea bestIdea = GetEventContainingDataValue("GreatIdea", "name", "mp3");

Product bestMusicPlayer = new Product(2001, 2007, bestIdea.owner, "Best mp3")[bestIdea.owner];
