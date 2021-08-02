#ifndef __BOOK1__HPP__
#define __BOOK1__HPP__

#include "story.hpp"

namespace Book1
{
    auto Stories = std::vector<Story::Base *>();

    class Story001 : public Story::Base
    {
    public:
        Story001()
        {
            BookID = Book::Type::BOOK1;

            ID = 1;

            Image = "images/legendary-kingdoms-logo.png";

            Text = "You stir into consciousness. Heat and blazing sunlight... the slow rumble of a wagon... heavy chains upon your limbs. You groan, your mouth dry, your stomach empty. As your eyes focus you can see your companions, fellow survivors of the brutal pirate attack, crushed together with you upon the floor of a rolling wagon. You remember little since then, except vowing to your newfound friends that you would stick together, come what may. You are relieved to see that they are still alive, but where are you?\n\nGazing through the bars of the wagon, your eyes focus on a blazing yellow-grey desert. To your left, filling the sky, are the Stonewalls -- a massive and impenetrable mountain range that divides the barbarian kingdoms of the south from the antique and mysterious lands of Drakehallow. But which side of the range are you? The answer becomes immediately apparent as you look beyond the smelly dragonyaks which are ponderously hauling your cage. A vast, crumbling city of minarets and run-down adobe buildings fills your view. The sting in the air and on your lips tells you all you need to know. This is Saltdad, cruel throne of the tyrant Iron King, one of half a dozen tin-pot dictators in the barbaric Valley of Bones. You are far from civilised lands here...\n\nThin-faced slavers with spears rattle the sides of your cage, barking at you to get down. You slump on your hindquarters. For the moment there is nothing you can do with you and your companions in chains. You swear you will regain your freedom and start your lives anew. But first, you must survive the horrors of slavery.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::Type::STARTED_IN_BOOK1});
        }

        Engine::Destination Continue(Party::Base &party)
        {
            return {Book::Type::BOOK1, 641};
        }
    };

    class Story002 : public Story::Base
    {
    public:
        Engine::Destination destination;

        Story002()
        {
            BookID = Book::Type::BOOK1;

            ID = 2;

            Text = "The orcs snarl as they see you, lurching to attack.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            destination = {Book::Type::BOOK1, 478};

            Bye = NULL;

            CanFlee = true;

            FleeRound = 0;

            Monsters = {
                Monster::Base("Orc", 6, 4, 4, 8, 0),
                Monster::Base("Orc", 4, 4, 3, 9, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return destination; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::FLEE)
            {
                Bye = "The orcs chase you out of the dungeon.";

                destination = {Book::Type::BOOK1, 515};
            }
            else
            {
                destination = {Book::Type::BOOK1, 478};
            }
        }
    };

    class Story003 : public Story::Base
    {
    public:
        Story003()
        {
            BookID = Book::Type::BOOK1;

            ID = 3;

            Text = "The treasure chest contains 200 silver coins and a BLUESTONE. Take what you wish.\n\nYou gained the code A45.";

            Bye = "Not wishing to tarry longer, you depart the chamber and return to the crossroads.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::BLUESTONE};

            Limit = 1;

            Engine::GAIN_MONEY(party, 200);

            Engine::GET_CODES(party, {Codes::A(45)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 46}; }
    };

    class Story004 : public Story::Base
    {
    public:
        Story004()
        {
            BookID = Book::Type::BOOK1;

            ID = 4;

            Text = "You swoop Amelia up into your arms just before she reaches the wine barrel. She shrieks in delight and throws an arm around your neck.\n\n\"Is it my prinsh charming?\" she slurs, nuzzling into your chest.\n\n\"Well... if by prince you meant castle pot-washer, then yes,\" you smile, carrying her over to her quarters in the palace.\n\n\"Tell me something,\" says Amelia dreamily, between snoozes, as you bear her through the palace. \"Why are you so charming to everyone except me?\"\n\n\"Charming is another word for lying,\" you say in a rare moment of honesty. \"I'd never lie to you, Amelia...\"\n\nHer snores cut off a potentially romantic moment. Tucking her into bed you go to join the rest of your companions.\n\nAmelia has gained a HEART for Brash. Brash also gains a HEART for Amelia.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_HEARTS(party, Character::Type::AMELIA_PASS_DAYNE, Character::Type::BRASH, 1);
            Engine::GAIN_HEARTS(party, Character::Type::BRASH, Character::Type::AMELIA_PASS_DAYNE, 1);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 280}; }
    };

    class Story005 : public Story::Base
    {
    public:
        Story005()
        {
            BookID = Book::Type::BOOK1;

            ID = 5;

            Text = "You meet up with the lead scholar, Emlyn Pass-Ross, who is already loading supplies onto the mules. She is joined by three other young scholars who look fresh out of university. They don't look the type who could handle themselves in a dangerous jungle.\n\n\"We'll follow our established route into the jungle, and then branch off south once we reach the checkpoint,\" says Emlyn. \"We've discovered all sorts of interesting plants, but so far surprisingly little that is edible.\"\n\n\"What kind of dangers will we face?\" you ask.\n\n\"Most of the jungle beasts leave us alone... to be honest, the monkeys are the most dangerous things you'll find. They travel in huge packs. They're intelligent, thieving sorts. You should watch out for them.\"\n\nSir Lawrence emerges from his tent to kiss his daughter goodbye and to wish you luck. Soon you have emerged from the wooden gates of Lhasbreath and are making your way towards the wall of trees that is the Lhasbreath jungle. Within the canopy of the jungle it is hot and humid. The ground is moist and seems to be the nest of thousands of tiny, biting insects. It must have rained recently, for the path ahead is flooded. Emlyn curses. She intended to take that route through the jungle to reach the checkpoint.";

            Choices.clear();
            Choices.push_back(Choice::Base("Follow the flooded path anyway", {Book::Type::BOOK1, 609}));
            Choices.push_back(Choice::Base("Take the expedition off the path and lead them through the jungle", {Book::Type::BOOK1, 863}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story006 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story006()
        {
            BookID = Book::Type::BOOK1;

            ID = 6;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You have already attempted to find a silver idol in the Caves of Uranu.";

            if (Engine::VERIFY_CODES(party, {Codes::Type::SILVER_IDOL}))
            {
                PreText += " You were successful and you do not need to look again.";
            }
            else
            {
                PreText += " You failed, you will not find another here.";
            }

            PreText += " Either way, you must choose again.";

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            return {Book::Type::BOOK1, 346};
        }
    };

    class Story007 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story007()
        {
            BookID = Book::Type::BOOK1;

            ID = 7;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            auto jessica = Engine::IN_TEAM(party, Character::Type::SAR_JESSICA_DAYNE, Team::Type::DISTRACTION);
            auto akihiro = Engine::IN_TEAM(party, Character::Type::AKIHIRO_OF_CHALICE, Team::Type::DISTRACTION);

            if (jessica || akihiro)
            {
                if (jessica && akihiro)
                {
                    PreText = "Sar Jessica Dayne and Akihiro refuse";
                }
                else if (jessica)
                {
                    PreText = "Sar Jessica Dayne refuses";
                }
                else
                {
                    PreText = "Akihiro refuses";
                }

                PreText += " to comply. ";

                if (jessica && akihiro)
                {
                    PreText = "Their";
                }
                else if (jessica)
                {
                    PreText = "Sar Jessica Dayne's";
                }
                else
                {
                    PreText = "Akihiro's";
                }

                PreText += " sense of personal honour ";

                if (jessica && akihiro)
                {
                    PreText = "are";
                }
                else
                {
                    PreText = "is";
                }

                PreText += " too strict to attempt such a strategy.";
            }
            else
            {
                PreText = "The guards have been looking at you with hungry eyes recently. You sidle up to the gate-house guards, suggesting all sorts of pleasures could await them in exchange for a few paltry rations.";

                if (Engine::VERIFY_CODES(party, {Codes::Type::WEAPON_AND_DISTRACTION}))
                {
                    Choices.push_back(Choice::Base("Seduce the Guards (Team check: Charisma 3+, Successes: 2)", {Book::Type::BOOK1, 364}, {Book::Type::BOOK1, 588}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 3, 2));
                }
                else
                {
                    Choices.push_back(Choice::Base("Seduce the Guards (Team check: Charisma 3+, Successes: 2)", {Book::Type::BOOK1, 364}, {Book::Type::BOOK1, 588}, Team::Type::DISTRACTION, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 3, 2));
                }
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            return {Book::Type::BOOK1, 169};
        }
    };

    class Story008 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story008()
        {
            BookID = Book::Type::BOOK1;

            ID = 8;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            PreText = "You cast your eye over the book. It seems to detail instructions about how to prepare skeletons for animation and control.";

            if (!Engine::VERIFY_CODES(party, {Codes::A(75)}))
            {
                PreText += "\n\nIt looks like the hard work has been done for you. Four skeletons have been laid out and prepared in exactly the right way for animation. The book claims you will even be able to have the skeletons fight for you, though they lack the wit for more complex tasks.";

                Choices.push_back(Choice::Base("Invoke the ritual", {Book::Type::BOOK1, 419}));
                Choices.push_back(Choice::Base("Return to the crossroads", {Book::Type::BOOK1, 310}));
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            return {Book::Type::BOOK1, 419};
        }
    };

    class Story009 : public Story::Base
    {
    public:
        Story009()
        {
            BookID = Book::Type::BOOK1;

            ID = 9;

            Text = "With a high kick you send the final monk sprawling. The other monks in the temple lower their eyes and return to their duties, unwilling to block your progress further. You retrieve your weapon and march smartly into the temple.\n\nHonnu is waiting for you, cross-legged on the floor. You join him in meditation for an hour or so, your companions venturing elsewhere in the city now they can see you are safe. \"It is rare for a supplicant to be granted the trials of perfection twice, but you are an unusual man, Akihiro. You must know that your father never returned from the trials. But I sense in you a steel he never had.\"\n\n\"Thank you, Honnu,\" you say, bowing your head.\n\n\"There would be little point in having you study at the temple, you are already skilled in your craft,\" shrugs Honnu. \"Instead I shall send you straight onto your tasks. Two must be completed before you are deemed worthy. It is traditional to send an applicant to retrieve some trifling artefact as proof of dedication; a tedious matter, since the only item of true worth is enlightenment. Therefore, I bid you find me a SILVER IDOL. Find yourself, Akihiro, whilst you are at it.\"\n\n\"Where can I find a SILVER IDOL?\" you ask unsurely.\n\n\"I'm sure more than one exists, but a man of open mind and willing to embrace his destiny might find one in the caves of Uranu,\" suggests Honnu. \"That place has tested, and slain, many a kensai aspirant. Go wisely, Akihiro. I shall see you soon.\"\n\nYou gained the code A50.";

            Bye = "You rise, bow, and go to re-join your companions.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(50)});

            Engine::REJOIN(party);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story010 : public Story::Base
    {
    public:
        Story010()
        {
            BookID = Book::Type::BOOK1;

            ID = 10;

            Text = "\"You should send the man to the Church of Cursus and have priests ritually scar him,\" says Ti'quon coldly.\n\n\"That way he shall be both punished and spiritually improved.\"\n\n\"Yes!\" hisses the King. \"The scarring is most painful. Send this dog to the church!\"\n\nThe poor man is dragged away, and the king retires with his councillors to discuss other business. You take the opportunity to slip away from the hall.\n\n\"A rather harsh punishment, ritual scarring,\" you mutter to Ti'quon as you exit the building.\n\n\"True enough,\" admits Ti'quon. \"Except that I noticed the poor man had already been scarred by the priests, and it is forbidden to perform the ritual twice. Getting him out of the sight of the mad king seemed to be the most important thing.\"\n\nYou shake your head, smiling.\n\nYou gained the code A34.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(34)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 19}; }
    };

    class Story011 : public Story::Base
    {
    public:
        Story011()
        {
            BookID = Book::Type::BOOK1;

            ID = 11;

            Text = "You carefully begin to sneak out of the chamber, but you are out of luck. The orcs spot you as you attempt to climb the stairs, and leap to a savage attack.\n\nThe slaves cannot help you in this battle, as their chains are too short to attack the orcs now they are at the back of the chamber fighting you.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 213}; }
    };

    class Story012 : public Story::Base
    {
    public:
        Story012()
        {
            BookID = Book::Type::BOOK1;

            ID = 12;

            Text = "You are making your way across the Westroad, an ancient highway built-up to resist the burying sands of the Valley. Travellers and soldiers pass intermittently, making their way between the cities of Saltdad and Clifftop.";

            Choices.clear();
            Choices.push_back(Choice::Base("Random Event", {{2, NULL, {Book::Type::BOOK1, 466}}, {3, NULL, {Book::Type::BOOK1, 397}}, {6, NULL, {Book::Type::BOOK1, 569}}}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story013 : public Story::Base
    {
    public:
        Story013()
        {
            BookID = Book::Type::BOOK1;

            ID = 13;

            Text = "What do you wish to do now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Board your ship", {Book::Type::BOOK1, 851}, Choice::Type::SHIP));
            Choices.push_back(Choice::Base("Enter the wide tunnel", {Book::Type::BOOK1, 593}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story014 : public Story::Base
    {
    public:
        Story014()
        {
            BookID = Book::Type::BOOK1;

            ID = 14;

            Text = "You have returned to the Tumblestones, in the hope of discovering more ancient secrets. Alas, a recent sandstorm has buried the exposed stones, with only the stern face of the old king left unburied, to stare across his ruined empire for all eternity.\n\nDisappointed, you leave.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 453}; }
    };

    class Story015 : public Story::Base
    {
    public:
        Story015()
        {
            BookID = Book::Type::BOOK1;

            ID = 15;

            Text = "There are few valuables amongst the goblins, though you manage to scavenge 15 silver coins from the entire horde. Pleased to have survived you make your way onwards.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_MONEY(party, 15);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 395}; }
    };

    class Story016 : public Story::Base
    {
    public:
        Story016()
        {
            BookID = Book::Type::BOOK1;

            ID = 16;

            Text = "You have chosen to battle one of the dark knights of Abraxas. Good luck!\n\nNote: On any round when Skallos does not take damage, he will unleash a roar of black magic. Each party member must lose 1 point of Health, and Skallos will restore 4 points of Health, up to his maximum of 20 Health points.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Skallos", Monster::Type::SKALLOS, 9, 4, 5, 20, 0),
                Monster::Base("Green Skeletons", 8, 4, 3, 30, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 811}; }
    };

    class Story017 : public Story::Base
    {
    public:
        Story017()
        {
            BookID = Book::Type::BOOK1;

            ID = 17;

            Text = "The Iron King is most displeased to see you. He clicks his fingers and soon you have been dragged away to the dungeons for a gruesome session of torture. Each party member LOSES 4 Health points. Afterwards, the king has the guards dispose of you as they see fit.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::TITHE_REPORT}))
            {
                return {Book::Type::BOOK1, 246};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_HEALTH(party, -4);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 55}; }
    };

    class Story018 : public Story::Base
    {
    public:
        Story018()
        {
            BookID = Book::Type::BOOK1;

            ID = 18;

            Text = "The door in front of you is stuck. To get it open you will have to force it.";

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(57)}))
            {
                return {Book::Type::BOOK1, 350};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Bye = NULL;

            temp_string = "";

            Choices.clear();

            if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::PRYBAR}))
            {
                Choices.push_back(Choice::Base("Force open the door (Individual check: Fighting 4+, Successes: 3)", {Book::Type::BOOK1, 350}, {Book::Type::BOOK1, -18}, {Attribute::Type::FIGHTING}, 4, 3));
            }
            else
            {
                Choices.push_back(Choice::Base("Force open the door (Individual check: Fighting 5+, Successes: 3)", {Book::Type::BOOK1, 350}, {Book::Type::BOOK1, -18}, {Attribute::Type::FIGHTING}, 5, 3));
            }

            Choices.push_back(Choice::Base("Consider other exits from the room", {Book::Type::BOOK1, 494}));
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (outcome)
            {
                Bye = "The door pops open. You gain the code A57.\n\nYou can explore the corridor beyond.";

                Engine::GET_CODES(party, {Codes::A(57)});
            }
            else
            {
                temp_string = std::string(party.Members[selection[0]].Name) + " LOSES 1 Health Point.";

                Bye = temp_string.c_str();

                Engine::GAIN_HEALTH(party.Members[selection[0]], -1);
            }
        }
    };

    // Dummy event to trigger Bye message in BOOK1:018
    class Event018 : public Story::Base
    {
    public:
        Event018()
        {
            BookID = Book::Type::BOOK1;

            ID = -18;

            DisplayID = 18;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party) { return {Book::Type::BOOK1, 18}; }
    };

    class Story019 : public Story::Base
    {
    public:
        Story019()
        {
            BookID = Book::Type::BOOK1;

            ID = 19;

            Location = Location::Type::CLIFFTOP;

            IsCity = true;

            Image = "images/book1/dreadful_city.png";

            Text = "You are in a dreadful city of poor mud-brick and grass hovels, surrounded by weed-filled cropland. Even the most impressive buildings are little more than timber halls, sagging under the weight of their own roofs. The local peasants are care-worn and well whipped, scarcely better dressed than slaves. The wealthier members of the town dress less well than a Saltdad commoner. The dissonant bells of the church of Cursus clang across town, followed by dirge-like processions of rag-wearing choristers singing hymns of dull praise to their uncaring deity.\n\nThe whole rotten town is perched atop a cliff some fifty or so feet tall, and yet this is a port city. A gigantic cargo crane leers over a thin and precarious path to the wooden harbour below, where at least some signs of life and love can be heard from the cluster of taverns that serve the sailors.";

            Choices.clear();
            Choices.push_back(Choice::Base("Visit the king's hall", {Book::Type::BOOK1, 557}));
            Choices.push_back(Choice::Base("Go to the marketplace", {Book::Type::BOOK1, 759}));
            Choices.push_back(Choice::Base("Go to the slave market", {Book::Type::BOOK1, 152}));
            Choices.push_back(Choice::Base("Visit the harbourside taverns", {Book::Type::BOOK1, 505}));
            Choices.push_back(Choice::Base("Go to the Church of Cursus", {Book::Type::BOOK1, 207}));
            Choices.push_back(Choice::Base("Head to the dockyard", {Book::Type::BOOK1, 703}));
            Choices.push_back(Choice::Base("Explore the city", {Book::Type::BOOK1, 354}));
            Choices.push_back(Choice::Base("Leave the city east, along the road", {Book::Type::BOOK1, 12}));
            Choices.push_back(Choice::Base("Leave the city south, into the desert", {Book::Type::BOOK1, 725}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story020 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story020()
        {
            BookID = Book::Type::BOOK1;

            ID = 20;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "The passageway opens up into a long hall, lined with stamped bronze plates. Torches burn in brackets along the walls, illuminating hideous murals of serpents devouring animals, trees and rocks. You see daylight ahead and come upon a chamber whose roof is open to the blazing sun. It is another temple chamber with black altars and fine silverwork dominating the room.\n\nRising from his position in front of the high altar, a snakeman priest in flowing vestal robes turns to gaze at you with piercing eyes.\n\n\"The heretic slaves return!\" he hisses. \"Do not think that you can seek repentance from the lord of judgement now! Once a traitor, always a traitor!\"";

            Monsters.clear();

            CanFlee = false;

            if (!Engine::VERIFY_CODES(party, {Codes::A(41)}))
            {
                PreText += "\n\n\"We have betrayed no one!\" you reply. \"You have trapped us here. Allow us to leave and we will do you no harm.\"\n\n\"Liars! You have stolen our god!\" spits the snakeman priest. \"Our breed faulters and dies. Now, you shall die with us!\" The snakeman hisses foul words of sorcery. Suddenly the temple walls begin to crack, and blocks of stone rain down upon you. Even as the temple collapses the snakeman priest advances towards you, brandishing his staff.";

                Monsters = {Monster::Base("Snakeman Priest", Monster::Type::SNAKEMAN_PRIEST, 4, 4, 3, 16, 0)};
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(41)}))
            {
                return {Book::Type::BOOK1, 714};
            }
            else
            {
                return {Book::Type::BOOK1, 881};
            }
        }
    };

    class Story021 : public Story::Base
    {
    public:
        Story021()
        {
            BookID = Book::Type::BOOK1;

            ID = 21;

            Location = Location::Type::CAPE_PORTA;

            Text = "You are steering along the jungle peninsula, across waters known as Cape Porta. It is a famous lair of pirates and other ne'er-do-wells.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::Random.UniformIntDistribution(1, 6);

            auto result = Engine::Random.NextInt();

            Choices.clear();

            if (result != 1)
            {
                Choices.push_back(Choice::Base("Sail north towards Clifftop", {Book::Type::BOOK1, 82}));
                Choices.push_back(Choice::Base("Sail east towards Cursus", {Book::Type::BOOK1, 814}));
                Choices.push_back(Choice::Base("(The Savage Lands) Sail west towards the Savage Lands", {Book::Type::BOOK5, 75}));
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 429}; }
    };

    class Story022 : public Story::Base
    {
    public:
        Story022()
        {
            BookID = Book::Type::BOOK1;

            ID = 22;

            Text = "Avoiding Chellar's room, you still manage to secure a pretty horde of items. You may take any or all of the following: JEWELLERY BOX, CALLIGRAPHY INK, SILVER IDOL, GOLD PORTRAIT, BRONZE SCORPION. You emerge with Brekken back onto the landing. You decide, with a mutual nod, that you have pushed your luck far enough in the house and make your way back out of the windows and into the streets of the city.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::JEWELLERY_BOX, Equipment::CALLIGRAPHY_INK, Equipment::SILVER_IDOL, Equipment::GOLD_PORTRAIT, Equipment::BRONZE_SCORPION};

            Limit = 5;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 833}; }
    };

    class Story023 : public Story::Base
    {
    public:
        Story023()
        {
            BookID = Book::Type::BOOK1;

            ID = 23;

            Text = "You emerge into a short corridor running west to east. To the west you can make out a crossroads. To the east a flight of stairs leads upwards. There is a door in the south wall.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go west", {Book::Type::BOOK1, 310}));
            Choices.push_back(Choice::Base("Go east, up the stairs", {Book::Type::BOOK1, 437}));
            Choices.push_back(Choice::Base("Go through the door in the south wall", {Book::Type::BOOK1, 604}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story024 : public Story::Base
    {
    public:
        Story024()
        {
            BookID = Book::Type::BOOK1;

            ID = 24;

            Text = "You have entered a chamber which is well furnished with flags and banners. It has the feel of a Chalice temple, although there is no sign that anyone actually worships here. A statue of a kneeling monk sits at the far end of the room. There are two exits from the chamber, each of them leading to a tunnel with stairs ascending.\n\nYou gain the code A77.\n\nSuddenly you see a light coming down the left-hand stairwell. Something is coming... but it is walking softly, as you cannot make out the sound of footsteps.";

            Choices.clear();
            Choices.push_back(Choice::Base("Hide behind the statue", {Book::Type::BOOK1, 782}));
            Choices.push_back(Choice::Base("Confront whoever is coming down the stairs", {Book::Type::BOOK1, 757}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(77)});
        }
    };

    class Story025 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story025()
        {
            BookID = Book::Type::BOOK1;

            ID = 25;

            Location = Location::Type::SALTDAD;

            Image = "images/book1/salt_mines.png";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You stand in the shadow of the Stonewalls, the finger-like crags soaring high into the burning sky. Cut into the roots of a tall mountain are the infamous salt mines of Saltdad. Dozens of warehouses are scattered around the entrance of the mine, with workers boiling away the salt from rock with water drawn from the Cold River to the east. Below your feet, and deep into the mountain, you know that thousands of slaves toil in the grim darkness.\n\nThe mine guards bring you to the owners, hard-faced men with unflinching stares.";

            Choices.clear();

            if (!Engine::VERIFY_CODES(party, {Codes::A(1)}))
            {
                PreText += "\n\n\"I suppose you are here about the troglodytes,\" sighs Clavod, the mine master. \"We are losing so many slaves it starts to become worth protecting them after a while. The deal is 25 silver for each troglodyte head you bring me. Take it or leave it.\"\n\nClavod sees your blank expression. \"Unless you came to arrange salt transportation, of course,\" he says hastily. \"We can transport salt to ships moored in Clifftop or Cursus.\"";

                Choices.push_back(Choice::Base("Accept the mission to hunt the troglodytes", {Book::Type::BOOK1, 681}));
                Choices.push_back(Choice::Base("Sell him some TROGLODYTE HEADS", {Book::Type::BOOK1, 233}, {Equipment::TROGLODYTE_HEAD}));
                Choices.push_back(Choice::Base("Express interest in buying salt for your ships", {Book::Type::BOOK1, 575}));
                Choices.push_back(Choice::Base("Apologise for wasting the mine master's time and leave", {Book::Type::BOOK1, 722}));
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 532}; }
    };

    class Story026 : public Story::Base
    {
    public:
        Story026()
        {
            BookID = Book::Type::BOOK1;

            ID = 26;

            Text = "You pour the liquid metal into the well and cross your fingers. Soon, the runes around the circle blaze into light, dousing you with brilliant coloured energies. When the glow finally wears off you feel stronger than before.";

            Bye = "Feeling more vigorous than ever, you return to the crossroads.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member and increase their maximum, unwounded Health score by 1 point", {Book::Type::BOOK1, 566}, Choice::Type::ADD_MAX_HEALTH, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::QUICKSILVER});
        }
    };

    class Story027 : public Story::Base
    {
    public:
        Story027()
        {
            BookID = Book::Type::BOOK1;

            ID = 27;

            Text = "The Everchild presents you with a bar of GOLD BULLION to help convince the mercenaries to change sides. Ayleta has been in private communication with the mercenaries for some time and has already negotiated a safe path between the mercenary encampment and the Everchild's forces.\n\nYou are received coolly by the unit captain, who cautiously welcomes you into his tent with his officers and bodyguards around him. You state your opinion that the Everchild's victory in the coming conflict is all but certain, and that she is willing to let mercenary companies that co-operate with her depart safely. Matters soon turn to money, with the mercenary captain suggesting that quite a lot of additional money will be coming his way should the patriarch win the battle, and Saltdad and other cities need to be supressed. He is also worried about his reputation if he plays turncoat without fighting even a single battle for his employer.\n\nOnly party members assigned to the MERCENARY TEAM may assist with this skill check.\n\nNote: If you give the mercenaries a bar of GOLD BULLION, the DC of this check becomes 5+. If you give them two bars, the DC becomes 4+. If you give them three bars the DC becomes 2+.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();
            Choices.push_back(Choice::Base("Give the mercenaries a GOLD BULLION bar", {Book::Type::BOOK1, -27}, Choice::Type::BRIBE_CODEWORD, {Equipment::GOLD_BULLION}, {Codes::Type::BRIBE_GOLD_BULLION1}, 1));
            Choices.push_back(Choice::Base("Give the mercenaries two GOLD BULLION bars", {Book::Type::BOOK1, -27}, Choice::Type::BRIBE_CODEWORD, {Equipment::GOLD_BULLION}, {Codes::Type::BRIBE_GOLD_BULLION2}, 2));
            Choices.push_back(Choice::Base("Give the mercenaries three GOLD BULLION bars", {Book::Type::BOOK1, -27}, Choice::Type::BRIBE_CODEWORD, {Equipment::GOLD_BULLION}, {Codes::Type::BRIBE_GOLD_BULLION2}, 3));
            Choices.push_back(Choice::Base("Bribe the Mercenaries (Team check: Charisma 6+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 6, 7));

            Take = {Equipment::GOLD_BULLION};

            Limit = 1;
        }
    };

    class Event027 : public Story::Base
    {
    public:
        Event027()
        {
            BookID = Book::Type::BOOK1;

            ID = -27;

            DisplayID = 27;

            Text = "Only party members assigned to the MERCENARY TEAM may assist with this skill check.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            if (Engine::VERIFY_CODES(party, {Codes::Type::BRIBE_GOLD_BULLION1}))
            {
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team check: Charisma 5+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 5, 7));
            }
            else if (Engine::VERIFY_CODES(party, {Codes::Type::BRIBE_GOLD_BULLION2}))
            {
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team check: Charisma 4+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 4, 7));
            }
            else if (Engine::VERIFY_CODES(party, {Codes::Type::BRIBE_GOLD_BULLION2}))
            {
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team check: Charisma 2+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 2, 7));
            }
            else
            {
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team check: Charisma 6+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 6, 7));
            }
        }
    };

    class Story028 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story028()
        {
            BookID = Book::Type::BOOK1;

            ID = 28;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Bye = NULL;

            PreText = "You come across a group of zealots from Cursus, sweeping the land for any heretics or unbelievers they can find. ";

            Choices.clear();

            if (!Engine::HAS_STATUS(party, Character::Status::RITUAL_SCARRING))
            {
                PreText += "They bombard you with questions about the faith, to determine your worthiness.";

                Choices.push_back(Choice::Base("Answer Cursite zealots (Team check: Lore 4+, Successes: 4)", {Book::Type::BOOK1, 657}, {Book::Type::BOOK1, -28}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::LORE}, 4, 4));
            }
            else
            {
                PreText += "The zealots let you pass.";
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 657}; }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (outcome)
            {
                Bye = "The zealots reluctantly let you go.";
            }
        }
    };

    class Event028 : public Story::Base
    {
    public:
        Event028()
        {
            BookID = Book::Type::BOOK1;

            ID = -28;

            DisplayID = 28;

            Text = "Your heretical ignorance drives them into a killer frenzy and you must fight.\n\nNote: The zealot healer will only attack if he is the last opponent standing. Otherwise he will heal each zealot 2 Health points. These additional points can take them over their starting health.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Zealot", Monster::Type::ZEALOT, 3, 4, 4, 10, 0),
                Monster::Base("Zealot", Monster::Type::ZEALOT, 4, 4, 4, 11, 0),
                Monster::Base("Zealot", Monster::Type::ZEALOT, 3, 4, 4, 10, 0),
                Monster::Base("Zealot Healer", Monster::Type::ZEALOT_HEALER, 3, 4, 4, 15, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 63}; }
    };

    class Story029 : public Story::Base
    {
    public:
        Story029()
        {
            BookID = Book::Type::BOOK1;

            ID = 29;

            Text = "You curse as the thief fades from view. You cannot face the owner of the house for shame, and instead slip away into the city.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story030 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story030()
        {
            BookID = Book::Type::BOOK1;

            ID = 30;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You sit down with the nomad chief and share a pipe of smoking weed with him. He is a jovial fellow, unaffected by the woes of a nomadic life.";

            Choices.clear();

            if (!Engine::VERIFY_CODES(party, {Codes::A(1)}) || Engine::VERIFY_CODES(party, {Codes::A(37)}))
            {
                PreText += "\n\nYou have nothing of importance to discuss with him.";
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (!Engine::VERIFY_CODES(party, {Codes::A(1)}) || Engine::VERIFY_CODES(party, {Codes::A(37)}))
            {
                return {Book::Type::BOOK1, 473};
            }
            else
            {
                return {Book::Type::BOOK1, 737};
            }
        }
    };

    class Story031 : public Story::Base
    {
    public:
        Story031()
        {
            BookID = Book::Type::BOOK1;

            ID = 31;

            Text = "As you approach the door you do not see the orcs from the nearby guardroom closing in behind you.\n\nNote: The orcs get the first combat turn, not you.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(56)}))
            {
                return {Book::Type::BOOK1, 788};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::Type::LAST_IN_COMBAT});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 2}; }
    };

    class Story032 : public Story::Base
    {
    public:
        Story032()
        {
            BookID = Book::Type::BOOK1;

            ID = 32;

            Text = "You quietly utter the correct counter spells to render the arcane wards passive. Slipping inside you can see Descantos himself, an elderly wizard, in deep meditation -- no doubt preparing his spells for tomorrow's conflict.";

            Choices.clear();
            Choices.push_back(Choice::Base("Sneak up to the wizard and slit his throat", {Book::Type::BOOK1, 336}));
            Choices.push_back(Choice::Base("Overwhelm and stab the wizard to death in a sudden rush", {Book::Type::BOOK1, 877}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story033 : public Story::Base
    {
    public:
        Story033()
        {
            BookID = Book::Type::BOOK1;

            ID = 33;

            Location = Location::Type::CURSUS;

            Text = "The harbour of Cursus is long and shallow. The jetties reach far out beyond the coast, each wide enough for a pair of dragonyaks to drag the cargoes of ships to the warehouses on the beach.\n\nYou may buy a ship here, if you have enough money. The largest vessel for sale is the CURSITE WAR GALLEY, a coastal-hugging warship which carries little cargo, but is the only vessel that can even hope to compete with the more advanced vessels of Royce and Drakehallow. A smaller, but more efficient, single-masted COG is also available. The WAR GALLEY will cost 1500 silver coins, and the COG 800 silver coins. When you buy a ship note that it is currently docked in Cursus harbour.\n\nYou can repair any ship you have in Cursus harbour. Each Health point you restore costs 50 silver coins. You can restore your ship up to its starting Health value. You can sell a COG here for 250 silver coins, a CURSITE WAR GALLEY for 500 silver coins and a CARRACK for 1500 silver coins. You cannot sell other kinds of ship here.\n\nYou can buy cargo for your ship here, as well. These goods are too large for your characters to carry, but they can be loaded onto any ship you have in Cursus harbour. You cannot buy more cargo units than you can store in your ship (for instance, a Cog can carry 2 Cargo Units). You can also sell any goods you are carrying in your ships for the same price as well:\n\nSALT: 600 silver coins\nCROPS: 500 silver coins\nSTEEL: 3000 silver coins\nSPICES: 3300 silver coins\nWINE: 2100 silver coins\nSLAVES: 1000 silver coins";

            Choices.clear();
            Choices.push_back(Choice::Base("Return to the city centre", {Book::Type::BOOK1, 340}));
            Choices.push_back(Choice::Base("Set sail on one of your ships", {Book::Type::BOOK1, 499}));

            Controls = Story::Controls::HARBOUR;
        }

        void Event(Party::Base &party)
        {
            // TODO: Carrack Stats
            Ships = {
                {Ship::Base("COG", Ship::Type::COG, Location::Type::CURSUS, 4, 7, 2), 800, 250},
                {Ship::Base("CURSITE WAR GALLEY", Ship::Type::CURSITE_WAR_GALLEY, Location::Type::CURSUS, 6, 9, 1), 1500, 500},
                {Ship::Base("CARRACK", Ship::Type::CARRACK, Location::Type::CURSUS, 6, 9, 1), -1, 1500}};

            Cargo = {
                {Cargo::Type::SALT, 600, 600},
                {Cargo::Type::CROPS, 500, 500},
                {Cargo::Type::STEEL, 3000, 3000},
                {Cargo::Type::SPICES, 3300, 3300},
                {Cargo::Type::WINE, 2100, 2100},
                {Cargo::Type::SLAVES, 1000, 1000}

            };

            ShipRepairPrice = 50;
        }
    };

    class Story034 : public Story::Base
    {
    public:
        Story034()
        {
            BookID = Book::Type::BOOK1;

            ID = 34;

            Text = "You return to the large burial chamber. The ghouls are long gone, and there is no more treasure to be found here. If you wish you can scribe the spell on the bronze tablet into your spellbook, if you have had reason to remove it:\n\nWisdom (Adventure)\n\nYour mind awakens with ancient knowledge. Cast this spell when you have failed a Lore check. You gain 3 automatic successes for this check only.\n\nRecharge: 50 silver.";

            Bye = "You return to the crossroads.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Spells = {Spells::WISDOM};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 46}; }
    };

    class Story035 : public Story::Base
    {
    public:
        Story035()
        {
            BookID = Book::Type::BOOK1;

            ID = 35;

            Text = "\"You say the captain wishes to see us? And he mentioned a promotion?\" presses the guard.\n\n\"Apparently his current lieutenant is not up to scratch,\" shrugs Brash. \"He's asked to see other guards as well. I imagine the first to see the captain will probably get the promotion.\"\n\nThe guards push past you in their eagerness to get to the captain's office.\n\nNote: Your DISTRACTION TEAM can join up with the WEAPON's TEAM for all future checks.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::Type::WEAPON_AND_DISTRACTION});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 456}; }
    };

    class Story036 : public Story::Base
    {
    public:
        Story036()
        {
            BookID = Book::Type::BOOK1;

            ID = 36;

            Text = "You are in the ogre's bedroom. It would be too unpleasant (not to mention unhygienic) to rest here yourselves, so you gingerly make your way back outside.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 310}; }
    };

    class Story037 : public Story::Base
    {
    public:
        Story037()
        {
            BookID = Book::Type::BOOK1;

            ID = 37;

            Text = "The Lhasbreath barbarian suddenly jolts, a blade sticking out of his chest. He falls away, to reveal a smirking Tash, her crude sword dripping with blood.\n\n\"Thanks, Tash,\" smiles Jessica, helped to her feet by the clever thief. Sar Jessica won't soon forget this.\n\nJessica has gained a heart for Tasha. Tasha also gains a heart for Sar Jessica.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_HEARTS(party, Character::Type::SAR_JESSICA_DAYNE, Character::Type::TASHA, 1);
            Engine::GAIN_HEARTS(party, Character::Type::TASHA, Character::Type::SAR_JESSICA_DAYNE, 1);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 753}; }
    };

    class Story038 : public Story::Base
    {
    public:
        Story038()
        {
            BookID = Book::Type::BOOK1;

            ID = 38;

            Text = "You kill the orc in a single stroke. He keels over, the book slipping from his grasp.\n\nYou gain the code A71.";

            Choices.clear();
            Choices.push_back(Choice::Base("Examine the black book he was reading", {Book::Type::BOOK1, 119}));
            Choices.push_back(Choice::Base("Close the door and go somewhere else", {Book::Type::BOOK1, 821}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(71)});
        }
    };

    class Story039 : public Story::Base
    {
    public:
        Story039()
        {
            BookID = Book::Type::BOOK1;

            ID = 39;

            Text = "You join the disorganised caravan guards, barking orders at them to form a defensive line and protect their archers.";

            Choices.clear();
            Choices.push_back(Choice::Base("Order the caravan guards (Team check: Charisma 5+, Successes: 3)", {Book::Type::BOOK1, 542}, {Book::Type::BOOK1, 451}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 5, 3));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story040 : public Story::Base
    {
    public:
        Story040()
        {
            BookID = Book::Type::BOOK1;

            ID = 40;

            Text = "With a dramatic flourish you unroll the DRAGON HEAD from its wrappings. King Scarrenden stands in amazement, and a gasp echoes across the chamber. His councillors flock round the head, to confirm its origins.\n\n\"This is indeed impressive,\" agrees the king. \"I set you a challenge and you have overcome it. Therefore, I shall be as good as my word. I shall send the Everchild my finest berserkers to aid her struggle. She also has the loyalty of my city. May she be victorious in her endeavours, and may her glory be ours!\"\n\nThere is a mixed cheer from the hall. Not all are pleased that King Scarrenden has submitted his city to the rule of child. However, even here the Everchild's fame is evident, and the enthusiasm of some of the warriors cannot be mistaken.\n\nYou may add the following soldiers to the Luutanesh barracks:\n\n[Lhasbreath Berserkers]: Strength 5, Morale 2\n\nThese berserkers are strong but flighty troops, who will fight for you as long as the going is good.\n\nGaining King Scarrenden's support has been quite an endeavour.\n\nYou gained the code A16.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to gain 1 point of SURVIVAL", {Book::Type::BOOK1, 775}, Choice::Type::ROLL_FOR_ATTRIBUTE_INCREASE, {Attribute::Type::SURVIVAL}, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::DRAGON_HEAD});

            auto location = Location::Type::LUUTANESH;

            if (Engine::VERIFY_CODES(party, {Codes::Type::QUEEN_IN_SALTDAD}))
            {
                location = Location::Type::SALTDAD;
            }

            Army = {Army::Base("Lhasbreath Berserkers", Army::Type::LHASBREATH_BERSERKERS, location, 5, 2)};

            Engine::GET_CODES(party, {Codes::A(16)});
        }
    };

    class Story041 : public Story::Base
    {
    public:
        Story041()
        {
            BookID = Book::Type::BOOK1;

            ID = 41;

            Text = "Dazzling sunlight almost blinds you as you emerge into daylight. You are in a sandy, secluded alleyway, the sounds of marketplace hawkers reaching your ears. You quickly help the Everchild out of the drains, hiding her behind some tall baskets as you check for the guards.\n\n\"We are free!\" she gasps. \"I feared I might die in that terrible place.\"\n\n\"What will you do now?\" you ask. \"You cannot stay in the city...\"\n\n\"No -- here we shall divide for safety,\" she agrees. \"Che Long can look after me from here on in. I cannot thank you enough for your safe delivery of myself and my followers. I can give you no reward, and you owe me nothing, but...\"\n\nYou silence her as you duck behind the baskets. A guard patrol of men in gleaming bronze armour clank past, wickedly shaped halberds in their grasp. \"The Bronzeguard,\" hisses Che Long. \"The elite warriors and boot lickers of the Iron King. Their amour is supposedly enchanted, dug up from the palace vaults by the Iron King's servants.\"\n\n\"They are still men underneath,\" you growl.\n\n\"If you hate tyranny and would see justice restored, come and find me in the Cold River Inn, in Luutanesh,\" implores the Everchild. \"The path to freedom is long and difficult, but together we can find a way to end the bloody reign of the Iron King forever.\"\n\nThe coast is clear, and Che Long quickly bows to you. \"We must go before the general alarm is raised. You have my thanks. Come majesty.\"\n\nAt that the Everchild dons a cowl over her head and slips into the crowds with her followers. \n\nYour experiences in the arena have taught you much. You may raise one skill (Fighting, Stealth, Lore, etc) by one point for each of your characters.\n\nYou gain the code A6.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            if (Engine::VERIFY_CODES(party, {Codes::Type::STARTED_IN_BOOK1}))
            {
                Choices.push_back(Choice::Base("Each of your characters raises one skill by one point", {Book::Type::BOOK1, 313}, Choice::Type::PARTY_RAISE_ATTRIBUTE, 1));
            }
            else
            {
                Choices.push_back(Choice::Base("Each of your characters raises maximum health points by one point", {Book::Type::BOOK1, 313}, Choice::Type::PARTY_RAISE_HEALTH, 1));
            }

            Engine::GET_CODES(party, {Codes::A(6)});
        }
    };

    class Story042 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story042()
        {
            BookID = Book::Type::BOOK1;

            ID = 42;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You return to the small reading room, mouldering books scattered across the tiny library.";

            if (!Engine::VERIFY_CODES(party, {Codes::Type::FIRST_TIME_SCROLL_OF_RAGE}))
            {
                PreText += "\n\nYou find a SCROLL OF RAGE here.";

                Engine::GET_CODES(party, {Codes::Type::FIRST_TIME_SCROLL_OF_RAGE});
            }
            else
            {
                if (!Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::SCROLL_OF_RAGE}))
                {
                    PreText += "\n\nYou are surprised to see the SCROLL OF RAGE back on the shelf, in the same place you found it last time. It seems that the scroll teleports back here each time it is used.";
                }
                else
                {
                    PreText += "\n\nYou find nothing else of interest here.";
                }
            }

            if (!Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::SCROLL_OF_RAGE}))
            {
                Take = {Equipment::SCROLL_OF_RAGE};

                Limit = 1;

                PreText += "\n\nYou can take the SCROLL OF RAGE. ";
            }
            else
            {
                Take = {};

                Limit = 0;
            }

            PreText += "You cannot have more than one SCROLL OF RAGE equipped at any given time. You can use it in combat to increase the Fighting score of each of your party members by 1 point until the end of the combat. After one use it will teleport back to the bookshelf in this room.";

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 494}; }
    };

    class Story043 : public Story::Base
    {
    public:
        Story043()
        {
            BookID = Book::Type::BOOK1;

            ID = 43;

            Text = "Janus is overjoyed to see the LOCKET. You hand it over to him and he cradles it in his hands. \"Now I can remember Alidale always!\" he sighs. He opens the LOCKET to have a look inside, when suddenly there is a flash and a profusion of smoke.\n\nStepping out of the smoke comes a young woman, a wizard by the looks of her, coughing and waving the smoke away from her face. \"Alidale?\" gasps Janus. \"Why! You don't look a day older than when you left for that temple!\"\n\n\"Janus!\" she cries overjoyed. \"You... do look a bit older, if I'm honest. I must have been in that locket for a long time.\"\n\nAlidale explains that she was surrounded by hideous serpent men and was forced to transport herself inside her locket for protection. The snake men assumed she had teleported away, but in fact she has been trapped in the locket for all these years. She never expected a rescue would take forty summers. She has little to reward you with but shares with you some of her many discoveries and insights into the ancient world that she has uncovered in her travels.\n\nYou gained the code A29.";

            Bye = "Bidding the odd couple farewell, you resume your journey.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to gain 1 point of LORE", {Book::Type::BOOK1, 614}, Choice::Type::RAISE_ATTRIBUTE_SCORE, {Attribute::Type::LORE}, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::BRONZE_LOCKET});

            Engine::GET_CODES(party, {Codes::A(29)});
        }
    };

    class Story044 : public Story::Base
    {
    public:
        Story044()
        {
            BookID = Book::Type::BOOK1;

            ID = 44;

            Text = "Tackling the room will come in two stages. Firstly, you must attempt to figure out the safest route. By examining the positions of the orc bodies you might be able to figure out which parts of the room are safe and which parts are deadly traps. Secondly, someone must attempt to carefully sneak through the room.\n\nNote: You cannot fail this test.";

            Choices.clear();
            Choices.push_back(Choice::Base("Discern the safe route (Team check: Lore 5+, Successes: Special)", {Book::Type::BOOK1, -44}, {Book::Type::BOOK1, -44}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::LORE}, 5, 0));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event044 : public Story::Base
    {
    public:
        Event044()
        {
            BookID = Book::Type::BOOK1;

            ID = -44;

            DisplayID = 44;

            Text = "Next, pick someone to try and sneak through the room.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            int success = 7 - party.RecentSuccesses;

            if (success < 0)
            {
                success = 0;
            }

            temp_string = "Sneak through the room (Individual check: Stealth 4+, Successes: ";

            if (success > 0)
            {
                temp_string += std::to_string(success);
            }
            else
            {
                temp_string += "Guaranteed";
            }

            temp_string += ")";

            Choices.clear();

            Choices.push_back(Choice::Base(temp_string.c_str(), {Book::Type::BOOK1, 385}, {Book::Type::BOOK1, 769}, {Attribute::Type::STEALTH}, 4, success));
        }
    };

    class Story045 : public Story::Base
    {
    public:
        Story045()
        {
            BookID = Book::Type::BOOK1;

            ID = 45;

            Text = "Timing it just right, you begin to shove one another in the water queue. To make it convincing, you start to exchange a few hard blows, attempting to drag other slaves in the queue into your rumble.\n\nEach party member involved in the fight LOSES 1 Health point.\n\nNote: Only party members assigned to the DISTRACTION TEAM can contribute to the check.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_HEALTH(party, Team::Type::DISTRACTION, -1);

            Choices.clear();

            if (Engine::VERIFY_CODES(party, {Codes::Type::WEAPON_AND_DISTRACTION}))
            {
                Choices.push_back(Choice::Base("Stage a convincing fight (Team check: Fighting 4+, Successes: 3)", {Book::Type::BOOK1, 837}, {Book::Type::BOOK1, 762}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::FIGHTING}, 4, 3));
            }
            else
            {
                Choices.push_back(Choice::Base("Stage a convincing fight (Team check: Fighting 4+, Successes: 3)", {Book::Type::BOOK1, 837}, {Book::Type::BOOK1, 762}, Team::Type::DISTRACTION, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::FIGHTING}, 4, 3));
            }
        }
    };

    class Story046 : public Story::Base
    {
    public:
        Story046()
        {
            BookID = Book::Type::BOOK1;

            ID = 46;

            Text = "You have returned to the crossroads. Above you daylight streams through the exit shaft.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go northwest", {Book::Type::BOOK1, 761}));
            Choices.push_back(Choice::Base("Go northeast", {Book::Type::BOOK1, 801}));
            Choices.push_back(Choice::Base("Go southeast", {Book::Type::BOOK1, 869}));
            Choices.push_back(Choice::Base("Go southwest", {Book::Type::BOOK1, 217}));
            Choices.push_back(Choice::Base("Climb back up to the surface", {Book::Type::BOOK1, 426}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story047 : public Story::Base
    {
    public:
        Story047()
        {
            BookID = Book::Type::BOOK1;

            ID = 47;

            Text = "You gather round and attempt to pull apart the barrier, but despite its crude appearance it is actually very well built. Even sledgehammers and mules couldn't pull this barricade down from this side. Is this as far as the orcs have penetrated into the complex?\n\nGrumbling with irritation, you head back east towards the small room you came from.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 821}; }
    };

    class Story048 : public Story::Base
    {
    public:
        Story048()
        {
            BookID = Book::Type::BOOK1;

            ID = 48;

            Text = "The enemy are too numerous. Horns are sounded, and your forces begin a general retreat. Fortunately, you have the open desert to scatter into and a rearward base to reform at, but casualties are almost inevitable.\n\nThe queen is appalled at her loss but marshals her forces with a mother's tenderness. \"This is not the end,\" she vows. \"We must rebuild our forces and gather new allies. Clifftop isn't going anywhere, and neither am I.\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Retreat to Saltdad", {Book::Type::BOOK1, 620}, Choice::Type::RETREAT, Location::Type::SALTDAD, 2));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story049 : public Story::Base
    {
    public:
        Story049()
        {
            BookID = Book::Type::BOOK1;

            ID = 49;

            Text = "Your return trip to Lhasbreath is mercifully uneventful, and Sir Lawrence Ross lays on a grand banquet upon your arrival. You spend several days with the Ross's, recovering your strength and discussing your adventures.\n\nAll your party members are RESTORED to MAXIMUM Health.\n\nWhen Emlyn reports her findings, the news is good. \"There are several herbs that appear to have medicinal qualities,\" she smiles. \"Additionally, one of the flowers is producing a sweet pepper which seems edible and delicious. If we can find a source of these in the wild, House Ross can enter the spice market.\"\n\nSir Lawrence is overjoyed with these results. You receive 600 silver coins as a reward. In addition, he presents you with a SEAL OF HOUSE ROSS. \"Should you ever travel to Strongstone, present this to the castle steward, Wyrran Glenpetre. You will be treated as an honoured guest.\"\n\nThis has been an amazing adventure.\n\nYou gained the code A85.";

            Bye = "Thanking Sir Lawrence and Emlyn for their generosity, you depart into the city.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to gain 1 point of SURVIVAL", {Book::Type::BOOK1, 775}, Choice::Type::RAISE_ATTRIBUTE_SCORE, {Attribute::Type::SURVIVAL}, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::SEAL_OF_HOUSE_ROSS};

            Limit = 1;

            Engine::GAIN_MONEY(party, 600);

            Engine::REST(party);

            Engine::GET_CODES(party, {Codes::A(85)});
        }
    };

    class Story050 : public Story::Base
    {
    public:
        Story050()
        {
            BookID = Book::Type::BOOK1;

            ID = 50;

            Text = "You manage to translate the brightly painted calendar, to the delight of Kopu. He is impressed with your work and presents you with a BLUESTONE as a reward. You ask what the stone is for.\n\n\"It is sacred to our priesthood,\" admits Kopu. \"You can exchange it for blessings at our temples.\"\n\n\"I thought you said Kalu had no temples anymore?\" you press.\n\n\"Well,\" he says. \"It's the thought that counts.\"\n\nGrumbling somewhat, you get some sleep whilst you wait out the storm.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::BLUESTONE};

            Limit = 1;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 585}; }
    };

    class Story051 : public Story::Base
    {
    public:
        Story051()
        {
            BookID = Book::Type::BOOK1;

            ID = 51;

            Text = "Where will you travel next?";

            Choices.clear();
            Choices.push_back(Choice::Base("North, to the Granite Hills", {Book::Type::BOOK1, 275}));
            Choices.push_back(Choice::Base("East, into the desert", {Book::Type::BOOK1, 202}));
            Choices.push_back(Choice::Base("South, to the Mordain ruins", {Book::Type::BOOK1, 515}));
            Choices.push_back(Choice::Base("West, across the desert towards Lhasbreath", {Book::Type::BOOK1, 752}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story052 : public Story::Base
    {
    public:
        Story052()
        {
            BookID = Book::Type::BOOK1;

            ID = 52;

            Text = "Your ship is boarded! Dozens of the Bando swamp the decks, their poisoned blow darts and spears making short work of any opposition. Surrounded, and realising the futility of your position, you surrender.\n\nYou are stripped and bound, along with the rest of your surviving crew. Your ship is looted and then sunk.\n\nAfter spending a few days with the Bando in cramped bamboo cages, the mercenary tribesmen sell you off to a passing slaver ship in return for a few gemstones and some curious-looking fabrics.";

            Choices.clear();
            Choices.push_back(Choice::Base("See where you are eventually sold", {{2, "You are sold to the Saltdad Arena", {Book::Type::BOOK1, 631}}, {4, "You are sold to the taskmasters of Clifftop", {Book::Type::BOOK1, 866}}, {6, "You are sold to the priests of Cursus", {Book::Type::BOOK1, 531}}}, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            party.Money = 0;

            Engine::SINK_SHIP(party);

            Engine::LOSE_ALL(party);
        }
    };

    class Story053 : public Story::Base
    {
    public:
        Story053()
        {
            BookID = Book::Type::BOOK1;

            ID = 53;

            Text = "There is no point in risking everyone at once.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to cross the beam first", {Book::Type::BOOK1, 208}, Choice::Type::CHOOSE_PARTY_MEMBER));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story054 : public Story::Base
    {
    public:
        Story054()
        {
            BookID = Book::Type::BOOK1;

            ID = 54;

            Text = "There are a number of close shaves, and even minor impacts, but the raft holds steady as you correct its course through the sloping rapids. Eventually the waters calm, and you ride the current downstream at a good speed. Looking at the densely packed jungle on either side you are glad to your soul that you didn't have to hack your way through all that vegetation. Eventually the jungle clears somewhat, and a wide, muddy bank comes into view. Emlyn thinks you have gone far enough, and you beach the raft onto the bank.\n\n\"Skilfully done,\" smiles Emlyn as you help her and the other scholars to shore. \"Let's explore the unknown, shall we?\"";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 763}; }
    };

    class Story055 : public Story::Base
    {
    public:
        Story055()
        {
            BookID = Book::Type::BOOK1;

            ID = 55;

            Text = "The guards strip you of your money and equipment. Jail space is at a premium, so you are sold into slavery.";

            Choices.clear();
            Choices.push_back(Choice::Base("See your fate", {{2, "You are sold to the Saltdad Arena", {Book::Type::BOOK1, 631}}, {4, "You are sold to the taskmasters of Clifftop", {Book::Type::BOOK1, 866}}, {6, "You are sold to the priests of Cursus", {Book::Type::BOOK1, 531}}}, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            party.Money = 0;

            Engine::LOSE_ALL(party);
        }
    };

    class Story056 : public Story::Base
    {
    public:
        Story056()
        {
            BookID = Book::Type::BOOK1;

            ID = 56;

            Text = "You kiss the burning, rich-yellow sands as you emerge from the shadow lands. You are free from the tyranny of the monochrome world.\n\nYou gained the code A23.";

            Bye = "You head north, directly away from the Blackwall.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to gain 1 point of SURVIVAL", {Book::Type::BOOK1, 511}, Choice::Type::RAISE_ATTRIBUTE_SCORE, {Attribute::Type::SURVIVAL}, 1));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(23)}))
            {
                return {Book::Type::BOOK1, 244};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(23)});
        }
    };

    class Story057 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story057()
        {
            BookID = Book::Type::BOOK1;

            ID = 57;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Bye = NULL;

            PreText = "The goblin's grasp of your language is rather poor, but you endeavour to convince them that you are the enemies of their enslavers.";

            auto difficulty = 5;

            if (Engine::VERIFY_CODES(party, {Codes::Type::SPEAK_BANDO}))
            {
                PreText += "\n\nHowever, you speak Bando. Oddly, the goblins are quite familiar with this language.";

                difficulty = 4;
            }

            Choices.clear();

            temp_string = "Parlay with the goblin (Team check: Charisma " + std::to_string(difficulty) + "+, Successes: 3)";

            Choices.push_back(Choice::Base(temp_string.c_str(), {Book::Type::BOOK1, 828}, {Book::Type::BOOK1, 389}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, difficulty, 3));

            Text = PreText.c_str();
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (!outcome)
            {
                Bye = "The goblins get hostile.";
            }
        }
    };

    class Story058 : public Story::Base
    {
    public:
        Story058()
        {
            BookID = Book::Type::BOOK1;

            ID = 58;

            Text = "As you are pouring the POISON into the cauldron, the head chef catches your hand. \"What's that?\" she demands sharply. You drop the VIAL and bolt away, even as the chefs yell for the guards. You make a dash for the walls, hoping to be able to leap over the low barricades of the compound and into freedom. You make it to one of the squat towers that line the walls, but alas, three Bronzeguard soldiers stand alert in the room. Barricading the door behind you, the team grimly draw their weapons and prepare to fight!";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::VIAL_OF_POISON});

            CanFlee = false;

            Monsters = {
                Monster::Base("Bronzeguard", 8, 4, 5, 12, 0),
                Monster::Base("Bronzeguard", 9, 4, 5, 10, 0),
                Monster::Base("Bronzeguard", 7, 4, 5, 11, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 623}; }
    };

    class Story059 : public Story::Base
    {
    public:
        Story059()
        {
            BookID = Book::Type::BOOK1;

            ID = 59;

            Text = "You knock the guards down into the dirt and surge forwards. There is nowhere to hide but in the city itself so you dash into the streets whilst the guards roar behind you.";

            Choices.clear();
            Choices.push_back(Choice::Base("Hide from the guards (Team check: Stealth 4+, Successes: 4)", {Book::Type::BOOK1, 698}, {Book::Type::BOOK1, 758}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::STEALTH}, 4, 4));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story060 : public Story::Base
    {
    public:
        Engine::Destination destination;

        Story060()
        {
            BookID = Book::Type::BOOK1;

            ID = 60;

            Text = "The thief is almost in grabbing range. He turns a corner only to find himself in a dead-end alley. He curses and draws his sword. \"You've bitten off more than you can chew, foreigners!\" he spits.\n\nNote: Only party members still in the chase can fight in this battle.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Team = Team::Type::CHASE;

            destination = {Book::Type::BOOK1, 391};

            Bye = NULL;

            CanFlee = false;

            Monsters = {Monster::Base("Thief of Royce", 5, 4, 4, 14, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return destination; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                destination = {Book::Type::BOOK1, 391};
            }
            else if (Engine::COUNT(party) > 0)
            {
                for (auto i = 0; i < party.Members.size(); i++)
                {
                    if (party.Members[i].Team == Team::Type::CHASE)
                    {
                        party.Members[i].Equipment.clear();
                    }
                }

                Bye = "The team you sent to chase the thief are all dead. The thief has taken all of their possessions. The remaining party members are mourning the loss.";

                destination = {Book::Type::BOOK1, 450};
            }
        }
    };

    class Story061 : public Story::Base
    {
    public:
        Story061()
        {
            BookID = Book::Type::BOOK1;

            ID = 61;

            Text = "You carry Amelia to bed after a few feeble protestations. Once she is safely tucked-in the rest of the party retire as well.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (party.LastSelected >= 0 && party.LastSelected < party.Members.size())
            {
                if (party.Members[party.LastSelected].Type == Character::Type::BRASH)
                {
                    return {Book::Type::BOOK1, 4};
                }
                else if (party.Members[party.LastSelected].Type == Character::Type::TASHA)
                {
                    return {Book::Type::BOOK1, 870};
                }
                else
                {
                    return {Book::Type::NONE, -1};
                }
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 280}; }
    };

    class Story062 : public Story::Base
    {
    public:
        Story062()
        {
            BookID = Book::Type::BOOK1;

            ID = 62;

            Text = "That night, you stroll along the terrace, the night a canopy of stars, reflected in the watery rice paddies that surround the house. You need some fresh air. Being treated as a hero all day and night can be exhausting.\n\nYou spy Brash leaning back on a chair, his feet up on the railings, a cup of rice wine in hand. He stares out over the still landscape.\n\n\"Silver for your thoughts,\" you say, causing Brash to yelp in surprise, fall off his chair and crash to ground.\n\n\"Ah! Akihiro! Always catching me at my best!\" Brash says sheepishly. You offer him a hand up, the fair lad accepting, pulled almost into an embrace. He blushes and backs away.\n\n\"Not like you to lose your tongue, my friend,\" you smile. \"You've been avoiding me, I sense.\"\n\n\"A bit,\" he says. \"I just don't know what to do with myself these days...\"\n\n\"How so?\" you ask. When he doesn't respond you flick him a silver coin, which he catches with his quick reflexes. He looks at it in his hand and realises he has been fooled.\n\n\"I've been thinking about you,\" he admits. \"Quite a bit. I don't know what to do about it. The kind of feelings I have aren't allowed in Royce.\"\n\n\"They are not allowed anywhere,\" you say, joining him at the railings. \"There are many men who would rather kill than love. And more who would kill to stop love, no matter how earnest. I will never hurry you, Brash, but I do not accept uncertainty. If you feel for me what I feel for you, you must be sure. Once done, we cannot undo it.\"\n\nYou are disturbed by a creak on the floorboards. You turn to see your sister arriving on the terrace. \"Come, Akihiro, you are asked for! Do not keep the people from their hero!\"\n\n\"The price of fame,\" you smile. \"Come, Brash. Walking out on a party is not done.\"\n\nYou offer him your hand, and he accepts, squeezing before releasing.\n\nAkihiro has gained a HEART for Brash. Brash also gains a HEART for Akihiro.\n\nYou gained the code A98.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(98)});

            Engine::GAIN_HEARTS(party, Character::Type::AKIHIRO_OF_CHALICE, Character::Type::BRASH, 1);
            Engine::GAIN_HEARTS(party, Character::Type::BRASH, Character::Type::AKIHIRO_OF_CHALICE, 1);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story063 : public Story::Base
    {
    public:
        Story063()
        {
            BookID = Book::Type::BOOK1;

            ID = 63;

            Text = "The zealots had a few minor talismans and some gaudy jewellery you sell at a marketplace for 25 silver coins. It seems a paltry reward for such a battle.\n\nWhere will you travel now?";

            Choices.clear();
            Choices.push_back(Choice::Base("North, towards the Temple of the Unbroken", {Book::Type::BOOK1, 535}));
            Choices.push_back(Choice::Base("East, towards the city of Cursus", {Book::Type::BOOK1, 340}));
            Choices.push_back(Choice::Base("West, towards the Mordain Ruins", {Book::Type::BOOK1, 515}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_MONEY(party, 25);
        }
    };

    class Story064 : public Story::Base
    {
    public:
        Story064()
        {
            BookID = Book::Type::BOOK1;

            ID = 64;

            Text = "With the ascendancy of the Everchild the streets have become safer and justice more accessible to the common people. Good news for the city, but it makes for a dull walk through the ramshackle town.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 75}; }
    };

    class Story065 : public Story::Base
    {
    public:
        Story065()
        {
            BookID = Book::Type::BOOK1;

            ID = 65;

            Text = "The party member reading the book is standing immobile, lost in the mystic words of the cursed book.\n\nWhat does the rest of the party do?";

            Choices.clear();
            Choices.push_back(Choice::Base("Snatch the book from their grasp", {Book::Type::BOOK1, 771}));
            Choices.push_back(Choice::Base("Abandon the party member to their madness", {Book::Type::BOOK1, 84}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story066 : public Story::Base
    {
    public:
        Story066()
        {
            BookID = Book::Type::BOOK1;

            ID = 66;

            Text = "You spend several days in the handsomely appointed Cold River Inn.\n\nNote: For every 5 silver coins you spend, each party member can RECOVER 1 Health point.\n\nSpell casters can also spend silver here to RECHARGE their spells, purchasing components in the nearby marketplace and going into meditation in the privacy of their rooms.";

            RestPrice = 5;

            Choices.clear();
            Choices.push_back(Choice::Base("Return to the Everchild's hideout", {Book::Type::BOOK1, 263}));
            Choices.push_back(Choice::Base("Leave Luutanesh", {Book::Type::BOOK1, 614}));

            Controls = Story::Controls::INN;
        }
    };

    class Story067 : public Story::Base
    {
    public:
        Story067()
        {
            BookID = Book::Type::BOOK1;

            ID = 67;

            Text = "You descend into the basement, carved out of cliff rock, and decorated with thick carpets and ever-burning candlesticks. Eventually you emerge into an oval chamber. To the south an archway leads to another circular room filled with strange, glowing runes. In the north wall is a strong, round steel door. It appears to have no handle or lock upon it. Perhaps this is the fabled vault of Unbraaki?\n\nOn either side of the vault are bookcases which are stacked with blank paper. Your entrance has created a draught, and a single blank sheet tumbles from the stacks to drift to the floor. You watch with growing nervousness as another sheet floats free and then another and another. The sheets of paper fold themselves rapidly, joining together and folding and refolding into shapes. Soon, a man-like figure has formed, followed by another and another. A paper tiger is folded into completion, as well as a fierce dragon. As one, the paper figures turn, swirls of sheets whipping around them like a snowstorm. They advance towards you.\n\nNotes: You cannot make armour saves against the attacks of these paper opponents.";

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(92)}))
            {
                return {Book::Type::BOOK1, 803};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            if (Engine::HAS_SPELL_ANY(party, {Spells::Type::ORB_OF_ANNIHILATION, Spells::Type::MAELSTROM}))
            {
                Choices.push_back(Choice::Base("Cast Orb of Annihilation or Maelstrom", {Book::Type::BOOK1, 121}));
                Choices.push_back(Choice::Base("Defend yourselves against these weird opponents", {Book::Type::BOOK1, -67}));
            }
            else
            {
                Monsters = {
                    Monster::Base("Paper Golems", Monster::Type::PAPER, 4, 4, 4, 10, 0),
                    Monster::Base("Paper Tiger", Monster::Type::PAPER, 5, 4, 4, 6, 0),
                    Monster::Base("Paper Dragon", Monster::Type::PAPER, 5, 4, 4, 6, 0)};

                CanFlee = false;
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 168}; }
    };

    class Event067 : public Story::Base
    {
    public:
        Event067()
        {
            BookID = Book::Type::BOOK1;

            ID = -67;

            DisplayID = 67;

            Text = "Defend yourselves against these weird opponents.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Monsters = {
                Monster::Base("Paper Golems", Monster::Type::PAPER, 4, 4, 4, 10, 0),
                Monster::Base("Paper Tiger", Monster::Type::PAPER, 5, 4, 4, 6, 0),
                Monster::Base("Paper Dragon", Monster::Type::PAPER, 5, 4, 4, 6, 0)};

            CanFlee = false;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 168}; }
    };

    class Story068 : public Story::Base
    {
    public:
        Story068()
        {
            BookID = Book::Type::BOOK1;

            ID = 68;

            Text = "Akini is delighted as you return her aunt to the family home. She thanks you profusely. \"I do not understand why strangers would perform such a kindness for me, but you have my eternal thanks,\" she smiles. She presents you with some FINE BOOTS (Stealth +2). \"These were cast out by my lady because one has a tiny scuff on the heel,\" says Akini. \"I shall never wear noble garments again. Let them be worn by those of noble heart, such as you.\"";

            Bye = "You bow and take your leave.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::FINE_BOOTS2};

            Limit = 1;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 75}; }
    };

    class Story069 : public Story::Base
    {
    public:
        Story069()
        {
            BookID = Book::Type::BOOK1;

            ID = 69;

            Text = "You enter a small alchemical laboratory. A rack where potion bottles are stored is empty, presumably already looted by the orcs. However, two potions seem to be in the process of being brewed, an open book listing the instructions on a table which shares space with many alembics and beakers. There are two doors in this room, one to the west, and another to the east.";

            Choices.clear();
            Choices.push_back(Choice::Base("Attempt to complete the creation of the potions", {Book::Type::BOOK1, 482}));
            Choices.push_back(Choice::Base("Pass through the door in the western wall", {Book::Type::BOOK1, 256}));
            Choices.push_back(Choice::Base("Return to the corridor through the east door", {Book::Type::BOOK1, 755}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(73)}))
            {
                return {Book::Type::BOOK1, 873};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }
    };

    class Story070 : public Story::Base
    {
    public:
        Story070()
        {
            BookID = Book::Type::BOOK1;

            ID = 70;

            Text = "You track down Damned Suzanna through the dusty gulches in Clifftop. Eventually you find her, recovering from the wounds of her beating, sweltering in her open-doored shack. When she sees you her eyes light up, and she eagerly snatches the proffered REGISTRY PAPERS from your hands. She gazes with confusion as she reads.\n\n\"This can't be right!\" she exclaims. \"It says here that my grandfather legally sold his land to Unbraaki fifty years ago. But my father said...\"\n\n\"I'm afraid it was countersigned by the priests of Cursus,\" you say. \"I suppose it's possible that there was a conspiracy against your family, but it's rare for priests to conspire with sorcerers.\"\n\nSuzanna slumps upon her bed. \"Everything my father told me was a lie,\" she groans. \"I'm not the inheritor of a massive estate, I'm just... me.\"\n\n\"That should be good enough,\" you say. \"Wealth rarely comes to those who sit around for it. If you want to make something of yourself, get out there and do it. You're certainly tough enough.\"\n\nShe nods. \"You're right. It's time I stopped waiting for my life to get better; I've got nothing to lose going adventuring. I'll sign on with the next ship in port.\" She presses a GREY TALISMAN into your hand. \"It's not much, but I want you to have this. It was the last thing my father gave me. Time for a new life.\"\n\nYou gained the code A36.";

            Bye = "Waving Damned Suzanna farewell, you make your way on.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::REGISTRY_PAPERS});

            Take = {Equipment::GREY_TALISMAN};

            Limit = 1;

            Engine::GET_CODES(party, {Codes::A(36)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 19}; }
    };

    class Story071 : public Story::Base
    {
    public:
        Story071()
        {
            BookID = Book::Type::BOOK1;

            ID = 71;

            Text = "With a final flourish, you slay your last opponent. Although there are other living slaves in the arena, the battle suddenly stops as the Iron King stands. In a thickly accented voice, the king declares you champions of the arena. The crowd go wild, cheering you heartily as you catch your breath from the intense battle.\n\nAny hopes you had that being made champion would grant you freedom are soon quashed. Instead you are disarmed and led back into the dungeons to nurse your wounds.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_ALL(party);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 608}; }
    };

    class Story072 : public Story::Base
    {
    public:
        Story072()
        {
            BookID = Book::Type::BOOK1;

            ID = 72;

            Text = "You can take up to four SHIELDs (Armour +2) from the fallen snakemen if you wish. Their weapons have unusual grips and are not useable for humans.\n\nYou glance around the temple chamber. Despite the cracks and structural damage the room has been kept in good condition, regularly swept clean of dirt and filth. But why would inhuman monsters care for an old Cursus temple?\n\nYou try lifting the portcullis, but it will not shift an inch. You will have to find another way out. Glancing around the chamber you can see a door in the back wall, and an archway in which a ramp leads upwards into the ziggurat. Which direction would you like to investigate?";

            Choices.clear();
            Choices.push_back(Choice::Base("The door", {Book::Type::BOOK1, 380}));
            Choices.push_back(Choice::Base("The archway", {Book::Type::BOOK1, 806}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::SHIELD2, Equipment::SHIELD2, Equipment::SHIELD2, Equipment::SHIELD2};

            Limit = 4;
        }
    };

    class Story073 : public Story::Base
    {
    public:
        Story073()
        {
            BookID = Book::Type::BOOK1;

            ID = 73;

            Text = "You come upon the Shaded Gate. You feel diminished, as if you were not real.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to lose 1 Health", {Book::Type::BOOK1, -73}, Choice::Type::GAIN_HEALTH, -1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event073 : public Story::Base
    {
    public:
        Event073()
        {
            BookID = Book::Type::BOOK1;

            ID = -73;

            DisplayID = 73;

            Text = "Where will you go?";

            Choices.clear();
            Choices.push_back(Choice::Base("Go through the Shaded Gate", {Book::Type::BOOK1, 56}));
            Choices.push_back(Choice::Base("West", {Book::Type::BOOK1, 273}));
            Choices.push_back(Choice::Base("South", {Book::Type::BOOK1, 886}));
            Choices.push_back(Choice::Base("East", {Book::Type::BOOK1, 537}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story074 : public Story::Base
    {
    public:
        Story074()
        {
            BookID = Book::Type::BOOK1;

            ID = 74;

            Text = "You inform the guard that you wish to see Unbraaki again. You are escorted into his presence, though he seems surprised to see you. \"I have already given you my soldiers,\" he grumbles. \"What more could your queen possibly desire?\"\n\n\"What if we need reinforcements?\" you ask. \"Your soldiers may not be enough to complete the campaign.\"\n\n\"That is no concern of mine,\" snaps Unbraaki coldly. \"You'll get no more from me. I have spent a fortune replacing the soldiers I gave to the Everchild. Send her my regards -- but don't bother me again.\"";

            Bye = "Realising that Unbraaki can be pushed no further, you bow and depart.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 265}; }
    };

    class Story075 : public Story::Base
    {
    public:
        Story075()
        {
            BookID = Book::Type::BOOK1;

            ID = 75;

            Location = Location::Type::SALTDAD;

            IsCity = true;

            Image = "images/book1/saltdad_city.png";

            Text = "Saltdad is a city of tumbledown mud-brick houses clustered into claustrophobic alleyways. Above these ragtag structures stands a half-collapsed palace, a minareted masterpiece that has seen better days. Amidst the squalor there is wealth to be found, for Saltdad produces the major export of the valley -- rock salt, chiselled out of winding mines to the east of the city. The Great Westroad runs through the heart of the city, once a paved highway, now a poorly maintained tumble of stones which nonetheless allows wagons and carts to drag themselves above the desert sands. Where the road cuts through the city stand the marketplaces, inns, slave markets and other services the city is famed for. A constant chatter of voices is heard at all times, for Saltdad never closes its places of business, even in the dead of night.";

            Choices.clear();
            Choices.push_back(Choice::Base("Find an inn", {Book::Type::BOOK1, 172}));
            Choices.push_back(Choice::Base("Go to the marketplace", {Book::Type::BOOK1, 798}));
            Choices.push_back(Choice::Base("Go to the slave market", {Book::Type::BOOK1, 326}));
            Choices.push_back(Choice::Base("Visit the palace", {Book::Type::BOOK1, 633}));
            Choices.push_back(Choice::Base("Head to the barracks of the Bronzeguard", {Book::Type::BOOK1, 390}));
            Choices.push_back(Choice::Base("Track down the local Thieves guild", {Book::Type::BOOK1, 179}));
            Choices.push_back(Choice::Base("Visit the temple of Cursus", {Book::Type::BOOK1, 770}));
            Choices.push_back(Choice::Base("Visit the temple of Kalu", {Book::Type::BOOK1, 382}));
            Choices.push_back(Choice::Base("Explore the city", {Book::Type::BOOK1, 471}));
            Choices.push_back(Choice::Base("Head west along the road", {Book::Type::BOOK1, 511}));
            Choices.push_back(Choice::Base("Head east along the road", {Book::Type::BOOK1, 177}));
            Choices.push_back(Choice::Base("Head south into the desert", {Book::Type::BOOK1, 858}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story076 : public Story::Base
    {
    public:
        Story076()
        {
            BookID = Book::Type::BOOK1;

            ID = 76;

            Text = "This must be Garon the Bloody-mouthed, whom you were sent to find. His father will no doubt grieve to know his fate, but at least it will solve the mystery of his disappearance. You were not asked to return the body, but if you wish to do so, you can take the BARBARIAN BODY. Unfortunately, it takes up five inventory slots and must be carried by a single character.\n\nWhat now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Take the BARBARIAN BODY", {Book::Type::BOOK1, -76}, Choice::Type::GET_EQUIPMENT_CODE, {Equipment::BARBARIAN_BODY}, {Codes::A(86)}, {}));
            Choices.push_back(Choice::Base("Examine the missing south wall", {Book::Type::BOOK1, 645}));
            Choices.push_back(Choice::Base("Leave the room and return to the crossroads", {Book::Type::BOOK1, 566}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event076 : public Story::Base
    {
    public:
        Event076()
        {
            BookID = Book::Type::BOOK1;

            ID = -76;

            DisplayID = 76;

            Text = "What now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Examine the missing south wall", {Book::Type::BOOK1, 645}));
            Choices.push_back(Choice::Base("Leave the room and return to the crossroads", {Book::Type::BOOK1, 566}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story077 : public Story::Base
    {
    public:
        Story077()
        {
            BookID = Book::Type::BOOK1;

            ID = 77;

            Text = "About halfway up the ramp you come to an archway. Through the archway you can see nothing; your lanterns cannot penetrate the darkness. You try experimentally poking a weapon through the archway, but it seems to come out unharmed.";

            Choices.clear();
            Choices.push_back(Choice::Base("Enter the archway", {Book::Type::BOOK1, 497}));
            Choices.push_back(Choice::Base("Keep going upwards", {Book::Type::BOOK1, 93}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story078 : public Story::Base
    {
    public:
        Story078()
        {
            BookID = Book::Type::BOOK1;

            ID = 78;

            Text = "This cave appears to be empty, but for the heaps of sand piled into it. Suddenly one of your party members begins to abruptly sink into the sand.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose the party member sinking into the sand", {Book::Type::BOOK1, -78}, Choice::Type::CHOOSE_PARTY_MEMBER));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event078 : public Story::Base
    {
    public:
        Event078()
        {
            BookID = Book::Type::BOOK1;

            ID = -78;

            DisplayID = 78;

            Text = "This is no normal quicksand, as there is not a drop of moisture in the cave. Nonetheless the hungry earth is pulling them down quickly.";

            Choices.clear();
            Choices.push_back(Choice::Base("Save your teammate (Team check: Survival 4+, Successes: 5)", {Book::Type::BOOK1, 586}, {Book::Type::BOOK1, 646}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 5));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story079 : public Story::Base
    {
    public:
        Story079()
        {
            BookID = Book::Type::BOOK1;

            ID = 79;

            Text = "You wander the merchant sector of the city, expecting to find nothing but lonely stones and whistling wind. You are surprised, therefore, to discover signs of a recent excavation. Near the entrance you see the bodies of several decomposing humans... and the body of a single, diseased-looking orc. Strange. Orcs are not native to this part of the world and normally call the Savage Lands their home. What are they doing all the way out here?\n\nBeyond the bodies an ancient set of steps lead into the earth, the walls of the tunnel made of well-set brick, and the floors of large flagstones.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go down the stairs, into the excavated dungeon", {Book::Type::BOOK1, 350}));
            Choices.push_back(Choice::Base("Go elsewhere in the city", {Book::Type::BOOK1, 515}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story080 : public Story::Base
    {
    public:
        Story080()
        {
            BookID = Book::Type::BOOK1;

            ID = 80;

            Text = "Holding your torches high you begin to make your way into the crevice, carefully watching your feet in the dancing firelight as they crunch near the eggs. A giant spider above you crawls along the ceiling, as if tracking your progress through her nursery.\n\n\"Step where we step,\" you advise the Everchild's followers as you move carefully forwards.";

            Choices.clear();
            Choices.push_back(Choice::Base("Navigate the spider's nest (Team check: Survival 4+, Successes: 5)", {Book::Type::BOOK1, 461}, {Book::Type::BOOK1, 260}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 5));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story081 : public Story::Base
    {
    public:
        Story081()
        {
            BookID = Book::Type::BOOK1;

            ID = 81;

            Location = Location::Type::LONGPORT_BAY;

            Text = "Longport Bay is a county in the kingdom of Royce. It is famed for its steelwork, good agricultural land and the dreadful rivalry of its noble houses. The county is fiercely feudal, with commoners unable to travel freely without a lord's consent. It is also famous for its jousts and fairs, and in good years the locals can grow fat on the rich foodstuffs served at these frequent celebrations. According to the scroll, the county is currently ruled by House Bailey, who were once mere stewards of their rivals House Dayne. That house has seen something of a fall in fortunes lately, and now stand in lower esteem than the Baileys. However, the scroll warns you that all sorts could have changed since the time it was written, as the politics in Longport Bay is both bloody and unpredictable.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 680}; }
    };

    class Story082 : public Story::Base
    {
    public:
        Story082()
        {
            BookID = Book::Type::BOOK1;

            ID = 82;

            Text = "You are sailing along the northwest coast of the Valley of Bones. A steady sea breeze counters what would otherwise be an unbearably hot sun. Great cliffs line the coast. Atop them sits a ramshackle city of crude adobe buildings and assorted ruins. A harbour juts dangerously from the cliff face, moored ships bobbing like corks against the pounding waves. Drenched harbourside taverns sit squalidly amongst the docks.";

            Choices.clear();
            Choices.push_back(Choice::Base("Pull into Clifftop harbour", {Book::Type::BOOK1, 703}));
            Choices.push_back(Choice::Base("(Drakehallow) Sail north, past the Stonewalls", {Book::Type::BOOK6, 420}));
            Choices.push_back(Choice::Base("(Crown and Tower) Sail west into the Bayswater Channel", {Book::Type::BOOK2, 392}));
            Choices.push_back(Choice::Base("Sail south to Cape Porta", {Book::Type::BOOK1, 21}));
            Choices.push_back(Choice::Base("(Code: A32) Consider doing something else", {Book::Type::BOOK1, 643}, {Codes::A(32)}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story083 : public Story::Base
    {
    public:
        Story083()
        {
            BookID = Book::Type::BOOK1;

            ID = 83;

            Text = "You hold out the ENGAGEMENT RING. \"The RING was not sold, it was stolen,\" you insist. \"I found this in the hideout of a group of nefarious thieves. She wants you to have it. She loved you, she was not using you.\"\n\nDulcimer takes the RING, swallowing hard.\n\n\"Is my lord really so poor a judge of character?\" you continue. \"The girl has a noble spirit and an honest temperament. It was not witchcraft but love that was the cause of her deception. Do not damage your family name further with this ridiculous feud with a common house. There is nothing to gain but dishonour.\"\n\nDulcimer's father agrees. \"Now that there is no financial loss to the family this matter should be closed quickly. You may take the girl's aunt back to her family. However, Akini is not to cross our path again.\"\n\nAkini's aunt is released, and you escort her back to her home.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::ENGAGEMENT_RING});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 68}; }
    };

    class Story084 : public Story::Base
    {
    public:
        Story084()
        {
            BookID = Book::Type::BOOK1;

            ID = 84;

            Text = "It is all too obvious what has happened. Your companion is doomed for all time to read that cursed book -- but to disturb them would invoke such a terrible wrath you might not survive. You carefully remove the items and money your companions was carrying. After this you say your sorry farewells, hoping that some part of your friend is still aware deep down inside.\n\nNote: This party member is lost forever. Curiously, if you ever return to this room, the party member will have vanished.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take.clear();

            Limit = 0;

            if (party.LastSelected >= 0 && party.LastSelected < party.Members.size())
            {
                for (auto i = 0; i < party.Members[party.LastSelected].Equipment.size(); i++)
                {
                    auto item = party.Members[party.LastSelected].Equipment[i];

                    if (item.Class != Equipment::Class::WEAPON && item.Class != Equipment::Class::ARMOUR)
                    {
                        Take.push_back(item);
                    }
                }

                Limit = Take.size();

                party.Members.erase(party.Members.begin() + party.LastSelected);

                party.CurrentCharacter = -1;

                party.LastSelected = -1;
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 821}; }
    };

    class Story085 : public Story::Base
    {
    public:
        Story085()
        {
            BookID = Book::Type::BOOK1;

            ID = 85;

            Text = "Only paupers use the rope bridges... as they have a tendency to suddenly break.";

            Choices.clear();
            Choices.push_back(Choice::Base("Random Event", {{1, NULL, {Book::Type::BOOK1, 824}}, {6, NULL, {Book::Type::BOOK1, 519}}}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story086 : public Story::Base
    {
    public:
        Story086()
        {
            BookID = Book::Type::BOOK1;

            ID = 86;

            Text = "You go amongst the king's councillors, trying to find one who might be able to arrange an appointment for you.";

            Choices.clear();
            Choices.push_back(Choice::Base("Arrange an audience (Team check: Charisma 5+, Successes: 5)", {Book::Type::BOOK1, 134}, {Book::Type::BOOK1, 775}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 5, 5));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Bye = NULL;
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (!outcome)
            {
                Bye = "You cannot find anyone to represent you. Eventually it gets dark and the hall is closed to foreigners.";
            }
        }
    };

    class Story087 : public Story::Base
    {
    public:
        Story087()
        {
            BookID = Book::Type::BOOK1;

            ID = 87;

            Text = "Water gushes in through the ripped hole in your ship. Your crew give a great cry and abandon ship, leaping into the frothing waves with a wail. You have scant seconds to prepare yourself to dive into the ocean. Any character wearing a suit of armour or robes discards it. You can carry no more than four items each before leaping into the sea -- any remaining items must be crossed off (there is no time to conjure a magic cabinet).";

            Bye = "You swim the base of the cliff, exhausted and freezing -- but there is nowhere to go but up.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::Type::NO_VAULT_ACCESS});
            Engine::LOSE_ALL(party, Equipment::Class::ARMOUR);
            Engine::LOSE_ALL(party, Equipment::Class::ROBE);

            Choices.clear();

            if (!Engine::VERIFY_EQUIPMENT_LIMIT(party, 4))
            {
                Choices.push_back(Choice::Base("Drop number of items down to 4", {Book::Type::BOOK1, -87001}, Choice::Type::LIMIT_EQUIPMENT, 4));
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, -87001}; }
    };

    class E087_001 : public Story::Base
    {
    public:
        E087_001()
        {
            BookID = Book::Type::BOOK1;

            ID = -87001;

            DisplayID = 87;

            Text = "You attempt to scale the cliffsides leading to the jungle.";

            Choices.clear();
            Choices.push_back(Choice::Base("Climb the cliffside (Team check: Survival 5+, Successes: 4)", {Book::Type::BOOK1, 409}, {Book::Type::BOOK1, -87002}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 5, 4));

            Controls = Story::Controls::STANDARD;
        }
    };

    class E087_002 : public Story::Base
    {
    public:
        std::string PreText = "";

        E087_002()
        {
            BookID = Book::Type::BOOK1;

            ID = -87002;

            DisplayID = 87;

            Choices.clear();
            Choices.push_back(Choice::Base("Choose the party member lost in the climbing attempt", {Book::Type::BOOK1, -87003}, Choice::Type::CHOOSE_PARTY_MEMBER));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            if (Engine::COUNT(party) > 1)
            {
                PreText = "One of your party members loses their grip halfway up and plunges into the sea. They are never seen again.";
            }
            else
            {
                PreText = "You lose your grip halfway up and plunge into the sea. You are never seen again.";

                party.Members.clear();

                party.CurrentCharacter = -1;

                party.LastSelected = -1;
            }

            Text = PreText.c_str();
        }
    };

    class E087_003 : public Story::Base
    {
    public:
        E087_003()
        {
            BookID = Book::Type::BOOK1;

            ID = -87003;

            DisplayID = 87;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (party.LastSelected >= 0 && party.LastSelected < party.Members.size())
            {
                party.Dead.push_back(party.Members[party.LastSelected].Type);

                party.Members[party.LastSelected].Equipment.clear();

                party.Members.erase(party.Members.begin() + party.LastSelected);

                party.CurrentCharacter = -1;

                party.LastSelected = -1;
            }

            return {Book::Type::BOOK1, -87001};
        }
    };

    class Story088 : public Story::Base
    {
    public:
        Story088()
        {
            BookID = Book::Type::BOOK1;

            ID = 88;

            Text = "You dash after Yu Yan, the thief frantically attempting to negotiate the ledge, which you navigate with the skill of a mountain goat. She stumbles, and only just manages to catch the ledge before falling. You dash to her side.\n\n\"Need a hand?\" you ask.\n\n\"Let me guess... you want the damn SILVER IDOL, yes?\"\n\n\"I think I'll need all the treasure; it's only weighing you down,\" you respond smartly.\n\nYu Yan groans, but throws her loot bag up to you. It contains not only the SILVER IDOL, but 200 silver pieces as well. Grinning, you give her a hand up on to the ledge. She grasps the sides of the wall, breathing heavily as you lean casually against the hillside.\n\n\"Make yourself scarce, Yu Yan,\" you say to her. \"Up here, in the hills, this is my domain.\"\n\nShe thanks you for your mercy and shuffles away, dashing over the hillside as soon as she is on safer ground. You make your way back along the ledge, hoping to find the beautiful vista again, but you cannot find it. Perhaps it is the time of night, but the surroundings no longer look so beautiful and peaceful as they were before. You shrug, but you are pleased with your victory.";

            Bye = "You make your way back to the temple in Chalice.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_MONEY(party, 200);

            Take = {Equipment::SILVER_IDOL};

            Limit = 1;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 856}; }
    };

    class Story089 : public Story::Base
    {
    public:
        Story089()
        {
            BookID = Book::Type::BOOK1;

            ID = 89;

            Text = "A great caravan of salt merchants, enroute to Clifftop, offer you their services. Their merchants have various odds and ends you might find useful.\n\nIf you have a ship docked in Clifftop, you can also buy up to three cargo units of SALT from the merchants for 500 silver coins each. They will deliver this cargo straight to your ship provided there is room for it.";

            Choices.clear();
            Choices.push_back(Choice::Base("Buy one cargo unit of SALT", {Book::Type::BOOK1, -89}, Choice::Type::DELIVER, {Cargo::Type::SALT}, Location::Type::CLIFFTOP, 500));
            Choices.push_back(Choice::Base("Buy two cargo units of SALT", {Book::Type::BOOK1, -89}, Choice::Type::DELIVER, {Cargo::Type::SALT, Cargo::Type::SALT}, Location::Type::CLIFFTOP, 1000));
            Choices.push_back(Choice::Base("Buy three cargo units of SALT", {Book::Type::BOOK1, -89}, Choice::Type::DELIVER, {Cargo::Type::SALT, Cargo::Type::SALT, Cargo::Type::SALT}, Location::Type::CLIFFTOP, 1500));
            Choices.push_back(Choice::Base("You have finished your trading", {Book::Type::BOOK1, 569}));

            Controls = Story::Controls::SHOP;
        }

        void Event(Party::Base &party)
        {
            Shop = {
                {Equipment::CRUDE_BLADE, 25, 10},
                {Equipment::MAUL, 50, 20},
                {Equipment::SHIELD2, 50, 25},
                {Equipment::INCENSE, -1, 90},
                {Equipment::BLUESTONE, -1, 45},
                {Equipment::TALISMAN_OF_ST_ELIAS, -1, 150}};
        }
    };

    class Event089 : public Story::Base
    {
    public:
        Event089()
        {
            BookID = Book::Type::BOOK1;

            ID = -89;

            DisplayID = 89;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party) { return {Book::Type::BOOK1, 89}; }
    };

    class Story090 : public Story::Base
    {
    public:
        Story090()
        {
            BookID = Book::Type::BOOK1;

            ID = 90;

            Location = Location::Type::LHASBREATH;

            Text = "Your return trip to Lhasbreath is mercifully uneventful, and Sir Lawrence Ross lays on a grand banquet upon your arrival. You spend several days with the Ross's, recovering your strength and discussing your adventures.\n\nWhen Emlyn reports her findings, the news is rather disappointing. \"We have recovered many pretty blooms,\" she admits. \"Certainly, the fragrances are quite striking... however, there are no medicinal or culinary plants, as far as I can tell. Nothing sellable.\"\n\n\"Curses!\" grumbles Sir Lawrence. \"This expedition is turning into an expensive failure.\"\n\nStill, Sir Lawrence is good to his word and presents you with a purse of 150 silver coins. Offering your consolations over the failed mission, you depart into the city.\n\nAlthough the expedition did not go perfectly you have learned a lot.\n\nYou gained the code A85.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to gain 1 point of SURVIVAL", {Book::Type::BOOK1, 775}, Choice::Type::ROLL_FOR_ATTRIBUTE_INCREASE, {Attribute::Type::SURVIVAL}, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_MONEY(party, 150);

            Engine::REST(party);

            Engine::GET_CODES(party, {Codes::A(85)});
        }
    };

    class Story091 : public Story::Base
    {
    public:
        Story091()
        {
            BookID = Book::Type::BOOK1;

            ID = 91;

            Text = "Climbing through the window silently will take some skill.";

            Choices.clear();
            Choices.push_back(Choice::Base("Force open the door (Individual check: Stealth 5+, Successes: 3)", {Book::Type::BOOK1, 487}, {Book::Type::BOOK1, 724}, Choice::Type::LAST_CHARACTER, {Attribute::Type::STEALTH}, 5, 3));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story092 : public Story::Base
    {
    public:
        Story092()
        {
            BookID = Book::Type::BOOK1;

            ID = 92;

            Text = "You creep up the stairs and reach a long landing. Peering out of the windows you can see Brekken and the wall- climbing team struggling to pry open the windows. With a grin you undo the latch, allowing the wall climbers easy access to the house.\n\nNote: For the rest of the break-in, when you make team checks, you can use the skills and combat abilities of your entire team, not just the wall-climbing team.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::Type::WALL_CLIMBING_AND_DOOR});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 116}; }
    };

    class Story093 : public Story::Base
    {
    public:
        Story093()
        {
            BookID = Book::Type::BOOK1;

            ID = 93;

            Text = "The ramp continues to wind upwards until you reach a small room with statues of hideous snakemen in alcoves along the walls. Here your way divides.";

            Choices.clear();
            Choices.push_back(Choice::Base("Make your way left, down a tunnel with a good deal of hissing coming from it", {Book::Type::BOOK1, 538}));
            Choices.push_back(Choice::Base("Go to the right, where the tunnel narrows alarmingly", {Book::Type::BOOK1, 98}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story094 : public Story::Base
    {
    public:
        Story094()
        {
            BookID = Book::Type::BOOK1;

            ID = 94;

            Text = "You carefully lower yourself into the pit, being careful to avoid the spikes. Moving aside the timbers you check over the body. You find 15 silver coins spilling from a rotten pouch and a still intact DRAGONYAK HORN.\n\nYou gained the code A46.";

            Choices.clear();
            Choices.push_back(Choice::Base("Climb up the far side of the pit", {Book::Type::BOOK1, 303}));
            Choices.push_back(Choice::Base("Climb back up the way you came", {Book::Type::BOOK1, 761}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(46)}))
            {
                return {Book::Type::BOOK1, 624};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::DRAGONYAK_HORN};

            Limit = 1;

            Engine::GAIN_MONEY(party, 15);

            Engine::GET_CODES(party, {Codes::A(46)});
        }
    };

    class Story095 : public Story::Base
    {
    public:
        Story095()
        {
            BookID = Book::Type::BOOK1;

            ID = 95;

            Text = "The assassin falls, his dagger clattering to the ground. You quickly escort the Everchild to a safer location. \"I'm alright! I promise! I knew you would protect me,\" she smiles. \"You always have.\"\n\nThe Everchild looks unflustered and calm as the morning sun illuminates the horizon, and her presence on the field, astride a nomad steed, causes your army to cheer wildly. Confidence is high. Now it is time to see if your preparations for the coming battle have been enough.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 162}; }
    };

    class Story096 : public Story::Base
    {
    public:
        Story096()
        {
            BookID = Book::Type::BOOK1;

            ID = 96;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::COUNT(party, Team::Type::MERCENARY) > 0)
            {
                return {Book::Type::BOOK1, 27};
            }
            else
            {
                return {Book::Type::BOOK1, 545};
            }
        }
    };

    class Story097 : public Story::Base
    {
    public:
        Story097()
        {
            BookID = Book::Type::BOOK1;

            ID = 97;

            Text = "You do not trust idly displayed skeletons these days. You make a point of shattering their skulls and ribcages, casting their bones about and generally demolishing their mortal remains. You feel a little bad about it afterwards, wondering if you have committed some sort of sacrilege, but there is no going back now.\n\nYou gained the code A75.";

            Choices.clear();
            Choices.push_back(Choice::Base("Examine the book", {Book::Type::BOOK1, 8}));
            Choices.push_back(Choice::Base("Return to the crossroads", {Book::Type::BOOK1, 310}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(75)});
        }
    };

    class Story098 : public Story::Base
    {
    public:
        Story098()
        {
            BookID = Book::Type::BOOK1;

            ID = 98;

            Text = "The tunnel is extremely narrow, and you must proceed down it single file. Decide which party member is at the front of the group and which is at the back.";

            Choices.clear();
            Choices.push_back(Choice::Base("Select which party member is in FRONT", {Book::Type::BOOK1, -98}, Choice::Type::SET_STATUS_FROM_LIST, {Character::Status::FRONT, Character::Status::BACK}, 0));
            Choices.push_back(Choice::Base("Select which party member is at the BACK", {Book::Type::BOOK1, -98}, Choice::Type::SET_STATUS_FROM_LIST, {Character::Status::FRONT, Character::Status::BACK}, 1));
            Choices.push_back(Choice::Base("Proceed", {Book::Type::BOOK1, 849}, Choice::Type::HAS_STATUS, {Character::Status::FRONT, Character::Status::BACK}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event098 : public Story::Base
    {
    public:
        Event098()
        {
            BookID = Book::Type::BOOK1;

            ID = -98;

            DisplayID = 98;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party) { return {Book::Type::BOOK1, 98}; }
    };

    class Story099 : public Story::Base
    {
    public:
        Story099()
        {
            BookID = Book::Type::BOOK1;

            ID = 99;

            Text = "The goblins chitter to each other in their native tongue, unsure of what to do. Such indecisive beasts!";

            Choices.clear();
            Choices.push_back(Choice::Base("Keep waiting", {Book::Type::BOOK1, 389}));
            Choices.push_back(Choice::Base("Leave the kitchen through the north door", {Book::Type::BOOK1, 366}));
            Choices.push_back(Choice::Base("Leave the kitchen through the south door", {Book::Type::BOOK1, 736}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story100 : public Story::Base
    {
    public:
        Story100()
        {
            BookID = Book::Type::BOOK1;

            ID = 100;

            Text = "With precise incantations you launch yourself into the ether, towards the Valley of Bones.\n\nWhere do you wish to appear?";

            Choices.clear();
            Choices.push_back(Choice::Base("Chalice", {Book::Type::BOOK1, 450}));
            Choices.push_back(Choice::Base("Clifftop", {Book::Type::BOOK1, 19}));
            Choices.push_back(Choice::Base("Cursus", {Book::Type::BOOK1, 340}));
            Choices.push_back(Choice::Base("Lhasbreath Oasis", {Book::Type::BOOK1, 590}));
            Choices.push_back(Choice::Base("Lhasbreath", {Book::Type::BOOK1, 775}));
            Choices.push_back(Choice::Base("Lhasbreath Jungle", {Book::Type::BOOK1, 370}));
            Choices.push_back(Choice::Base("Luutanesh", {Book::Type::BOOK1, 614}));
            Choices.push_back(Choice::Base("Saltdad", {Book::Type::BOOK1, 75}));
            Choices.push_back(Choice::Base("The Blackwall", {Book::Type::BOOK1, 691}));
            Choices.push_back(Choice::Base("The Caves of Uranu", {Book::Type::BOOK1, 346}));
            Choices.push_back(Choice::Base("The Granite Hills", {Book::Type::BOOK1, 275}));
            Choices.push_back(Choice::Base("The Palace of Unbraaki", {Book::Type::BOOK1, 890}));
            Choices.push_back(Choice::Base("The Ruins of Mordain", {Book::Type::BOOK1, 515}));
            Choices.push_back(Choice::Base("The Salt Mines of Saltdad", {Book::Type::BOOK1, 25}));
            Choices.push_back(Choice::Base("The Salt Plains", {Book::Type::BOOK1, 460}));
            Choices.push_back(Choice::Base("The Temple of the Unbroken", {Book::Type::BOOK1, 535}));
            Choices.push_back(Choice::Base("The Tumblestones", {Book::Type::BOOK1, 137}));
            Choices.push_back(Choice::Base("The Withered Steppes", {Book::Type::BOOK1, 115}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story101 : public Story::Base
    {
    public:
        Story101()
        {
            BookID = Book::Type::BOOK1;

            ID = 101;

            Text = "Spitting on the blood-stained sands, you stride into the central battle, back to back, swords in hand.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Lhasbreth Barbarians", 7, 5, 3, 17, 0),
                Monster::Base("Chalice Swordsmen", 5, 4, 4, 10, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 71}; }
    };

    class Story102 : public Story::Base
    {
    public:
        Story102()
        {
            BookID = Book::Type::BOOK1;

            ID = 102;

            Text = "This KEY has certainly travelled a long way from its lock -- but it fits! You hear a loud clang as you turn the KEY, its triple locks unbolting one by one. The KEY seems to be absorbed into the door mechanism itself and is pulled out of your hand. You step into a vast chamber beyond, filled with urns brimming with treasure. A stone sarcophagus of a mighty king dominates the middle of the enormous room, raised up on a high platform surmountable only by a long set of stairs.\n\nFrom the corners of the chamber you see something terrible. Two giant skeletons, clad in bronze armour, suddenly animate, blazing balefire in their eyes. They heft enormous swords, ten feet long each.\n\nIf you wish you can flee the chamber now -- but you can never return. The door will slam shut behind you.";

            Choices.clear();
            Choices.push_back(Choice::Base("Flee the chamber", {Book::Type::BOOK1, 46}));
            Choices.push_back(Choice::Base("Fight for your treasure", {Book::Type::BOOK1, -102}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::PYRAMIDAL_KEY});
        }
    };

    class Event102 : public Story::Base
    {
    public:
        Event102()
        {
            BookID = Book::Type::BOOK1;

            ID = -102;

            DisplayID = 102;

            Text = "You have chosen to fight for your treasure.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Giant Skeleton", 8, 3, 5, 30, 0),
                Monster::Base("Giant Skeleton", 10, 3, 4, 25, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 178}; }
    };

    class Story103 : public Story::Base
    {
    public:
        Story103()
        {
            BookID = Book::Type::BOOK1;

            ID = 103;

            Text = "You open the cage and the bird immediately flies onto a low branch. You are about to move on when the bird addresses you. \"I'm grateful,\" it says. \"Not many humans would do such a thing without promise of reward.\"\n\n\"Not many birds can talk,\" you say, surprised.\n\n\"All animals can talk -- but rarely in your language,\" admits the bird. \"I can teach you our tongue, if you have any skill in sorcery.\"\n\nIf you want to accept the bird's offer you can learn the spell below if you have any spellcasters in your party:\n\nAnimal Speech (Adventure)\n\nYou are able to communicate using the secret speech of animals.\n\nRecharge: 50 silver.";

            Bye = "You thank the bird and watch as it flies off above the jungle canopy.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Spells = {Spells::ANIMAL_SPEECH};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 618}; }
    };

    class Story104 : public Story::Base
    {
    public:
        Story104()
        {
            BookID = Book::Type::BOOK1;

            ID = 104;

            Text = "You cannot be certain, but the mural appears to be telling some sort of story. An image of a terrible demon, possibly Abraxas from legend, rises from the earth and demolishes a number of cities. From cracks in the ground come snakemen, slithering around the survivors and rounding them up. The humans are turned into slaves, building the snakemen a mighty temple, but soon even the slaves begin to worship at the altars of the temple as well. Here the mural ends abruptly. Make of it what you will.\n\nYou gained the code A41.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(41)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 77}; }
    };

    class Story105 : public Story::Base
    {
    public:
        Story105()
        {
            BookID = Book::Type::BOOK1;

            ID = 105;

            Text = "You make your way down to the riverbank, and eventually find the raft with Emlyn's help, covered over with weeds and bushes. Two long poles can be used to pilot the craft, and soon you are punting down the river, the sun shimmering through the canopy of trees onto the glittering water.\n\nThe raft begins to pick up speed, and as you round a corner you can see why. There are rapids ahead, the water splashing with thick foam against scattered rocks that peek through the surface of the river. If you were in canoes it would be easily navigable. On your raft it will take all your skill to avoid danger.";

            Choices.clear();
            Choices.push_back(Choice::Base("Navigate the rapids (Team check: Survival 5+, Successes: 3)", {Book::Type::BOOK1, 54}, {Book::Type::BOOK1, 337}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 5, 3));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story106 : public Story::Base
    {
    public:
        Story106()
        {
            BookID = Book::Type::BOOK1;

            ID = 106;

            Text = "You pretend to offer aid; anything to get out of sight of the Everchild's rebellious followers. At the first opportunity you inform the guards about the rebel's intentions. A few days later Che Long, Milagros and her followers have vanished from the arena. Word reaches you about a bloody struggle in the cells, but there are few details. Not long after, the arena master releases you under orders of the Iron King. \"His majesty is most pleased with your loyalty; the identity of the rebel leaders has been a well-kept secret. He offers you this reward and asks you to attend him at his palace if you seek further remunerating work.\"\n\nThe arena master presents you with 200 silver pieces.\n\nYou gained the code A4.";

            Bye = "Weighing the price of treachery with a smile, you gleefully leave the arena.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_MONEY(party, 200);

            Engine::GET_CODES(party, {Codes::A(4)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 75}; }
    };

    class Story107 : public Story::Base
    {
    public:
        Story107()
        {
            BookID = Book::Type::BOOK1;

            ID = 107;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::IN_PARTY(party, Character::Type::SAR_JESSICA_DAYNE))
            {
                return {Book::Type::BOOK1, 514};
            }
            else
            {
                return {Book::Type::BOOK1, 753};
            }
        }
    };

    class Story108 : public Story::Base
    {
    public:
        Story108()
        {
            BookID = Book::Type::BOOK1;

            ID = 108;

            Text = "You hear hoofbeats in the distance. Glancing over the top of dune you spot a group of Lhasbreath barbarians, riding on horseback. There are six of them -- quite a formidable group!";

            Choices.clear();
            Choices.push_back(Choice::Base("Attempt to hide from them by partially burying yourselves in the dunes", {Book::Type::BOOK1, 525}));
            Choices.push_back(Choice::Base("Greet them cordially", {Book::Type::BOOK1, 199}));
            Choices.push_back(Choice::Base("Attack them at long distance", {Book::Type::BOOK1, 297}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story109 : public Story::Base
    {
    public:
        Story109()
        {
            BookID = Book::Type::BOOK1;

            ID = 109;

            Text = "There are no exceptions. With dozens of guards at their command the priests strip you of all your taxable items (items that provide a +2 or greater bonus, except for shields). For good measure you are scourged with the whip as well. Each party member must lose 1 Health point.";

            Bye = "Appalled at this daylight robbery you make your way on.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                auto non_taxable = std::vector<Equipment::Base>();

                for (auto j = 0; j < party.Members[i].Equipment.size(); j++)
                {
                    if (party.Members[i].Equipment[j].Class == Equipment::Class::SHIELD || party.Members[i].Equipment[j].Attribute == Attribute::Type::NONE || party.Members[i].Equipment[j].Modifier < 2)
                    {
                        non_taxable.push_back(party.Members[i].Equipment[j]);
                    }
                }

                if (non_taxable.size() > 0)
                {
                    party.Members[i].Equipment = non_taxable;
                }
                else
                {
                    party.Members[i].Equipment.clear();
                }
            }

            Engine::GAIN_HEALTH(party, -1);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 458}; }
    };

    // TODO: Characters with Ritual Scarring who entered the Ziggurat will be in Team Zigurrat until they return to town
    class Story110 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story110()
        {
            BookID = Book::Type::BOOK1;

            ID = 110;

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(100)}))
            {
                return {Book::Type::BOOK1, 823};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            PreText = "The Grand Ziggurat is one of the largest buildings in the valley, dwarfed only by the colossal Saltdad palace. Guarding its steps are the fanatical Cursus Temple Guard, the elite soldiers of the patriarch. They halt you as you begin to climb the steps.\n\n\"Hold there, filthy heathens!\" they spit. \"Only true followers of Cursus are allowed within!\"";

            Choices.clear();

            if (!Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::RING_OF_THE_PATRIARCH}))
            {
                PreText += "\n\nNote: Only party members who have Ritual Scarring (Cursus) are permitted to enter the ziggurat. Those without the scarring must wait outside, and you cannot use their equipment or abilities until you leave the ziggurat.";

                Choices.push_back(Choice::Base("Enter the ziggurat (party members without Ritual Scarring must wait outside)", {Book::Type::BOOK1, 574}, Choice::Type::HAS_STATUS, {Character::Status::RITUAL_SCARRING}));
                Choices.push_back(Choice::Base("Return to the town centre", {Book::Type::BOOK1, 340}));
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 387}; }
    };

    class Story111 : public Story::Base
    {
    public:
        Engine::Destination destination = {};

        Team::Type previousTeam = Team::Type::NONE;

        int character = -1;

        Story111()
        {
            BookID = Book::Type::BOOK1;

            ID = 111;

            Text = "Sar Jessica is the first-born daughter of a baron of Royce. It is impossible for a nobleman such as Dulcimer to refuse the challenge and keep face in court. He emerges from the house, accompanied by his house guards, an Ozury rapier in his hand. His family, an endless procession of cousins, grandparents, brothers and sisters, gather in a circle to watch the duel.\n\nNote: Only Sar Jessica may engage in this battle.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            destination = {Book::Type::BOOK1, 68};

            CanFlee = false;

            Team = Team::Type::SAR_JESSICA_DAYNE;

            character = Engine::FIND_CHARACTER(party, Character::Type::SAR_JESSICA_DAYNE);

            if (character >= 0 && character < party.Members.size())
            {
                previousTeam = party.Members[character].Team;

                Engine::SET_TEAM(party.Members[character]);

                Monsters = {Monster::Base("Dulcimer", 4, 4, 4, 8, 0)};
            }
            else
            {
                Monsters.clear();
            }
        }

        Engine::Destination Continue(Party::Base &party) { return destination; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                if (character >= 0 && character < party.Members.size())
                {
                    Engine::SET_TEAM(party.Members[character], previousTeam);
                }

                Bye = "The family reluctantly release Akini's aunt, bidding you never to darken their doorstep again";

                destination = {Book::Type::BOOK1, 68};
            }
            else
            {
                Bye = "Sar Jessica is slain. You take her equipment before leaving the house in disgrace";

                destination = {Book::Type::BOOK1, 468};
            }
        }
    };

    class Story112 : public Story::Base
    {
    public:
        Story112()
        {
            BookID = Book::Type::BOOK1;

            ID = 112;

            Location = Location::Type::MORDAIN;

            Text = "Your new crew cheer as you come aboard. The ship is large and shabbily built -- but perhaps you can make something of it?\n\n[HULK] Fighting: 3, Health: 5, Cargo: 3 units\n\nNote: Your new ship is in Mordain harbour. What will you do now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Depart in your new ship", {Book::Type::BOOK1, 851}, Choice::Type::SHIP, Location::Type::MORDAIN));
            Choices.push_back(Choice::Base("Enter the wide tunnel", {Book::Type::BOOK1, 593}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            party.Fleet.push_back(Ship::Base("HULK", Ship::Type::HULK, Location::Type::MORDAIN, 3, 5, 3));
        }
    };

    class Story113 : public Story::Base
    {
    public:
        Story113()
        {
            BookID = Book::Type::BOOK1;

            ID = 113;

            Text = "You are travelling along the Northroad that stretches between Cursus and Luutanesh. Armed convoys from the mystical city of Chalice roll by, along with scores of travellers. Watering stops dot the ragged highway, with merchants paying a few silver coins a time to feed their labouring dragonyaks.";

            Choices.clear();
            Choices.push_back(Choice::Base("Random Event", {{1, NULL, {Book::Type::BOOK1, 694}}, {6, NULL, {Book::Type::BOOK1, 477}}}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story114 : public Story::Base
    {
    public:
        Story114()
        {
            BookID = Book::Type::BOOK1;

            ID = 114;

            Text = "You cautiously peek into the room. Before you are four enormous orcs, armed to the teeth. Fortunately all four are dead, victims of a poisoning. It is a coward's weapon; but very effective!";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 669}; }
    };

    class Story115 : public Story::Base
    {
    public:
        Story115()
        {
            BookID = Book::Type::BOOK1;

            ID = 115;

            Image = "images/book1/rocky_desert.png";

            Text = "You are travelling across a stretch of rocky desert, that rolls and twists into great gulches and canyons. Paths have been worn into the stone from hundreds of years of travel by nomads and barbarians who avoid the well-guarded roads that link the cities. It is hot and dry, and getting lost is a distinct possibility.";

            Choices.clear();
            Choices.push_back(Choice::Base("Navigate the Withered Steppes (Team check: Survival 4+, Successes: 3)", {Book::Type::BOOK1, -115001}, {Book::Type::BOOK1, -115002}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 3));

            Controls = Story::Controls::STANDARD;
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (!outcome)
            {
                Bye = "Each party member loses 1 Health point as your rations dwindle.";

                Engine::GAIN_HEALTH(party, -1);
            }
            else
            {
                Bye = NULL;
            }
        }
    };

    class E115_001 : public Story::Base
    {
    public:
        E115_001()
        {
            BookID = Book::Type::BOOK1;

            ID = -115001;

            DisplayID = 115;

            Image = "images/book1/rocky_desert.png";

            Text = "You manage to get your bearings enough to explore your surroundings.";

            Choices.clear();
            Choices.push_back(Choice::Base("Look for a nomad camp", {Book::Type::BOOK1, 253}));
            Choices.push_back(Choice::Base("Explore some of the canyons", {Book::Type::BOOK1, 884}));
            Choices.push_back(Choice::Base("Go west to the Granite Hills", {Book::Type::BOOK1, 275}));
            Choices.push_back(Choice::Base("Go southwest into the deep desert", {Book::Type::BOOK1, 202}));
            Choices.push_back(Choice::Base("Go south to the Temple of the Unbroken", {Book::Type::BOOK1, 535}));
            Choices.push_back(Choice::Base("Go east to the Northroad", {Book::Type::BOOK1, 113}));
            Choices.push_back(Choice::Base("Go northeast to Luutanesh", {Book::Type::BOOK1, 614}));
            Choices.push_back(Choice::Base("Go north to the Westroad", {Book::Type::BOOK1, 177}));
            Choices.push_back(Choice::Base("Go northeast towards Saltdad", {Book::Type::BOOK1, 858}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class E115_002 : public Story::Base
    {
    public:
        E115_002()
        {
            BookID = Book::Type::BOOK1;

            ID = -115002;

            DisplayID = 115;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party) { return {Book::Type::BOOK1, 115}; }
    };

    class Story116 : public Story::Base
    {
    public:
        Story116()
        {
            BookID = Book::Type::BOOK1;

            ID = 116;

            Text = "You creep across the landing, looting rooms as you go.\n\nNote: Only members of the Wall-climbing team may help with the check.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            if (Engine::VERIFY_CODES(party, {Codes::Type::WALL_CLIMBING_AND_DOOR}))
            {
                Choices.push_back(Choice::Base("Rob the upper floor (Team check: Stealth 4+, Successes: 5)", {Book::Type::BOOK1, 22}, {Book::Type::BOOK1, 529}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::STEALTH}, 4, 5));
            }
            else
            {
                Choices.push_back(Choice::Base("Rob the upper floor (Team check: Stealth 4+, Successes: 5)", {Book::Type::BOOK1, 22}, {Book::Type::BOOK1, 529}, Team::Type::WALL_CLIMBING, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::STEALTH}, 4, 5));
            }
        }
    };

    class Story117 : public Story::Base
    {
    public:
        Story117()
        {
            BookID = Book::Type::BOOK1;

            ID = 117;

            Text = "Battle is joined! The ogre lumbers to its feet and attacks. Unbeknownst to you, the ogre's mate, Daisy, is not far away.\n\nNote: After the second round of combat she will join in the fight.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Skullcracker", 8, 4, 4, 12, 0),
                Monster::Base("Daisy", 9, 4, 4, 13, 0, 2)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 431}; }
    };

    class Story118 : public Story::Base
    {
    public:
        Story118()
        {
            BookID = Book::Type::BOOK1;

            ID = 118;

            Text = "You try to elbow your way through the crowd, but by the time you have reached the carcass it has been almost stripped clean. You are forced to gnaw on empty bones, feeding on marrow and almost inedible ligaments. The lack of food begins to drain your spirits.\n\nEach Party Member must LOSES 1 Health point.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_HEALTH(party, -1);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 865}; }
    };

    class Story119 : public Story::Base
    {
    public:
        Story119()
        {
            BookID = Book::Type::BOOK1;

            ID = 119;

            Text = "Pick a party member to read the book.";

            Choices.clear();
            Choices.push_back(Choice::Base("Make your choice", {Book::Type::BOOK1, 839}, Choice::Type::CHOOSE_PARTY_MEMBER));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story120 : public Story::Base
    {
    public:
        Story120()
        {
            BookID = Book::Type::BOOK1;

            ID = 120;

            Text = "After days of wandering you finally reach the banks of a wide and fast-running river. You plunge your faces into the freshwater, drinking your fill. Following the river downstream you come to a shabby city perched atop a massive cliff.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 19}; }
    };

    class Story121 : public Story::Base
    {
    public:
        Story121()
        {
            BookID = Book::Type::BOOK1;

            ID = 121;

            Text = "Concentrating tightly, you unfurl a magical spell of pure fire and roll it towards your flammable enemies. They have no defence against such terrible magic and are instantly consumed with a pitched wail. The smoke and glowing sheets force you back to the stairs for a time, but by the time their cries have subsided there is nothing left of the paper golems but ash.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 168}; }
    };

    class Story122 : public Story::Base
    {
    public:
        Story122()
        {
            BookID = Book::Type::BOOK1;

            ID = 122;

            Text = "After strenuous negotiations the mercenaries finally agree to be discreetly absent during the battle tomorrow. Changing sides altogether would be a step too far for their precious reputations. This will have an enormous impact on the forthcoming battle.\n\nYou gained the code A96.";

            Bye = "Wishing them a speedy journey to their homelands you return triumphant.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(96)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 301}; }
    };

    class Story123 : public Story::Base
    {
    public:
        Story123()
        {
            BookID = Book::Type::BOOK1;

            ID = 123;

            Text = "Gurgling with effort, air bubbles blasting through the water from your mouth and nostrils, you finally wrench the block up high enough to get out. You roll under the block and through the shadow door, the stone block slamming down shut as you do so. You are soaking wet... but alive.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 717}; }
    };

    class Story124 : public Story::Base
    {
    public:
        Story124()
        {
            BookID = Book::Type::BOOK1;

            ID = 124;

            Text = "The command tent contains half a dozen agents; men and women from Lhasbreath and Chalice rub shoulders with freed slaves from Saltdad and Clifftop. You stand by the Everchild's side as she offers commissions.\n\n\"There are three tasks that must be done tonight,\" she says quietly. \"The assassination of Descantos, the hired wizard of Cursus. Convincing the mercenaries to abandon the patriarch's cause. And lastly, seeing to my personal security tonight. Each is important.\"\n\nShe turns to you. \"Which of these missions, my friends, will you undertake?\"\n\nYou must now decide how many missions your party will undertake, and who will go on each mission. Any missions you don't undertake will be performed by the Everchild's other agents -- although they have a slimmer chance of success. You can choose to undertake all three missions if you wish, or just one or two.";

            Choices.clear();
            Choices.push_back(Choice::Base("Decide who will undertake each mission", {Book::Type::BOOK1, 567}, Choice::Type::ASSIGN_TEAMS, {{Team::Type::ASSASSINATION_DESCANTOS, 0, 4}, {Team::Type::MERCENARY, 0, 4}, {Team::Type::EVERCHILD_SECURITY, 0, 4}}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story125 : public Story::Base
    {
    public:
        Story125()
        {
            BookID = Book::Type::BOOK1;

            ID = 125;

            Text = "Sand surrounds you in all directions, though you can make out some hills to the north.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go north", {Book::Type::BOOK1, 275}));
            Choices.push_back(Choice::Base("Go west", {Book::Type::BOOK1, 590}));
            Choices.push_back(Choice::Base("Go south", {Book::Type::BOOK1, 515}));
            Choices.push_back(Choice::Base("Go east", {Book::Type::BOOK1, 535}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story126 : public Story::Base
    {
    public:
        Story126()
        {
            BookID = Book::Type::BOOK1;

            ID = 126;

            Location = Location::Type::CLIFFTOP;

            IsCity = true;

            Text = "Clifftop is currently under the governorship of Ayleta, one of the Everchild's generals. She welcomes you into the hall, and details to you her various plans for the improvement of the city. \"I want to create more permanent structures for the poor, and to reduce the amount of beatings the overseers issue to their farmworkers,\" she says. \"Progress is slow. The population is uneducated and tend to misbehave when they are not being brutalised. It may take many generations to change Clifftop for the better.\"\n\nIt seems a hard task, and not one suited to the abilities of adventurers.";

            Bye = "Wishing her well, you depart the hall.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 19}; }
    };

    class Story127 : public Story::Base
    {
    public:
        Story127()
        {
            BookID = Book::Type::BOOK1;

            ID = 127;

            Text = "The door opens into a small room which contains a bookcase and a broken reading chair. Searching the shelves is an unpleasant experience, as the books are damp and fall to pieces on contact with your fingers. You do manage to find a magic scroll which seems undamaged. It is a SCROLL OF RAGE. You can use it in combat to increase the Fighting score of each of your party members by 1 point until the end of the combat. After one use it will vanish.";

            Bye = "There is nothing else of value in the room, so you exit by the only door.";

            Choices.clear();
            Choices.push_back(Choice::Base("Take the SCROLL OF RAGE", {Book::Type::BOOK1, 494}, Choice::Type::GET_EQUIPMENT_CODE, {Equipment::SCROLL_OF_RAGE}, {Codes::A(59)}, {Codes::Type::FIRST_TIME_SCROLL_OF_RAGE}));
            Choices.push_back(Choice::Base("Leave it alone", {Book::Type::BOOK1, 494}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(59)}))
            {
                return {Book::Type::BOOK1, 42};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }
    };

    class Story128 : public Story::Base
    {
    public:
        Story128()
        {
            BookID = Book::Type::BOOK1;

            ID = 128;

            Text = "The Temple of Kalu is a squat tower of blue stone, built in a jumble-like pattern from the ruins of the old temple. The Everchild has decreed the temple should be rebuilt in defiance of the laws laid down by the patriarch of Cursus.\n\nWithin the temple tiny bells are softly chimed every minute, with parishioners and priests taking it in turns to strike the correct bell according to the time of day. The great calendar, a mosaic and guide to the planting dates of the year, is still incomplete, and requires more BLUESTONES to finish the work.\n\nIf you have any BLUESTONES you may exchange one for a blessing from the priests. If you are blessed, find the party member with the lowest Survival score in the team and increase their Survival by 1 point. You can assist with the repairs to the calendar.\n\nNote: You can only assist the priests with repairs to the calendar once.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            Choices.push_back(Choice::Base("Exchange a BLUESTONE for a blessing", {Book::Type::BOOK1, -128002}, {Equipment::BLUESTONE}));

            if (!Engine::VERIFY_CODES(party, {Codes::Type::REPAIR_CALENDAR_KALU}))
            {
                Choices.push_back(Choice::Base("Repair the calendar (Team check: Lore 5+, Successes: 3)", {Book::Type::BOOK1, -128001}, {Book::Type::BOOK1, 75}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::LORE}, 5, 3));
            }

            Choices.push_back(Choice::Base("Your business here is complete", {Book::Type::BOOK1, 75}));
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            Engine::GET_CODES(party, {Codes::Type::REPAIR_CALENDAR_KALU});

            if (!outcome)
            {
                Bye = "The calendar is damaged, and the angry priests bans you from ever visiting the temple again.";
            }
        }
    };

    class E128_001 : public Story::Base
    {
    public:
        E128_001()
        {
            BookID = Book::Type::BOOK1;

            ID = -128001;

            DisplayID = 128;

            Text = "The calendar is repaired. You receive the blessing of the priests.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a member among your party with the lowest SURVIVAL score", {Book::Type::BOOK1, 75}, Choice::Type::RAISE_LOWEST_ATTRIBUTE, {Attribute::Type::SURVIVAL}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class E128_002 : public Story::Base
    {
    public:
        E128_002()
        {
            BookID = Book::Type::BOOK1;

            ID = -128002;

            DisplayID = 128;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::BLUESTONE});

            return {Book::Type::BOOK1, -128001};
        }
    };

    class Story129 : public Story::Base
    {
    public:
        Story129()
        {
            BookID = Book::Type::BOOK1;

            ID = 129;

            Text = "You have found the great bronze cogwheel, lying almost buried in the wet sand. The wheel is quite heavy, and it will take many attempts to tie a strong knot around it, and in some dreadful currents.\n\nNote: Only the party member who made the last skill check can make the attempt.";

            Choices.clear();

            Choices.push_back(Choice::Base("Secure the cogwheel (Individual check: Survival 4+, Successes: 3)", {Book::Type::BOOK1, 745}, {Book::Type::BOOK1, 549}, Choice::Type::LAST_INDIVIDUAL_CHECK, {Attribute::Type::SURVIVAL}, 4, 3));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story130 : public Story::Base
    {
    public:
        Story130()
        {
            BookID = Book::Type::BOOK1;

            ID = 130;

            Location = Location::Type::LHASBREATH;

            Text = "The barbarians hail you as friends of Lhasbreath.\n\nAfter discussing the health of the queen you bid the barbarians farewell.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 219}; }
    };

    class Story131 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story131()
        {
            BookID = Book::Type::BOOK1;

            ID = 131;

            Bye = "You carry on down the tunnel.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "The RUNESWORD (FIGHTING +3) is a powerful weapon, however it also has the dreadful side effect of slowly leaching the wielder's life. If you carry this sword, the character bearing it loses 1 Health point each time you turn to a new reference number. You can share the sword around the party in order to spread out the damage, if you wish. You can discard the sword at any time to end the curse, but you cannot sell it, since its curse is immediately apparent to anyone who holds it.";

            if (party.LastSelected >= 0 && party.LastSelected < party.Members.size())
            {
                if (Engine::SCORE(party.Members[party.LastSelected], Attribute::Type::HEALTH) > 0)
                {
                    PreText += "\n\n" + std::string(party.Members[party.LastSelected].Name) + " LOSES 1 Health Point.";

                    Engine::GAIN_HEALTH(party.Members[party.LastSelected], -1);
                }
            }

            Take = {Equipment::RUNESWORD3};

            Limit = 1;

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 647}; }
    };

    class Story132 : public Story::Base
    {
    public:
        Story132()
        {
            BookID = Book::Type::BOOK1;

            ID = 132;

            Text = "The only item of interest in the tomb is the SILVER SKULL, which seems to have stopped chanting.\n\nThe sarcophagus itself is empty.";

            Bye = "With nowhere else to go, you return to the crossroads.";

            Choices.clear();
            Choices.push_back(Choice::Base("Take the SILVER SKULL", {Book::Type::BOOK1, 46}, Choice::Type::GET_EQUIPMENT_CODE, {Equipment::SILVER_SKULL}, {Codes::A(47)}, {}));
            Choices.push_back(Choice::Base("Leave it alone", {Book::Type::BOOK1, 46}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story133 : public Story::Base
    {
    public:
        Story133()
        {
            BookID = Book::Type::BOOK1;

            ID = 133;

            Text = "In such a dangerous place battle appears inevitable. You confer with the Everchild, who agrees with your plan to strike first. She raises her hands and unleashes a terrible blast of power that jolts all the giant spiders clinging on the roof. They come tumbling down, but quickly regain their feet to rush to an attack.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Black Spider", 5, 5, 4, 6, 0),
                Monster::Base("Grey Spider", 4, 5, 4, 6, 0),
                Monster::Base("Green Spider", 5, 5, 4, 5, 0),
                Monster::Base("Red Spider", 4, 5, 4, 4, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 790}; }
    };

    class Story134 : public Story::Base
    {
    public:
        Story134()
        {
            BookID = Book::Type::BOOK1;

            ID = 134;

            Text = "You manage to find one fellow, a young and dynamic looking man, who claims he can arrange an appointment for you. However, it will cost 600 silver coins in administrative fees.";

            Choices.clear();
            Choices.push_back(Choice::Base("Pay the administrative fees (600 silver coins)", {Book::Type::BOOK1, 171}, Choice::Type::LOSE_MONEY, 600));
            Choices.push_back(Choice::Base("You have little choice but to leave the hall", {Book::Type::BOOK1, 775}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story135 : public Story::Base
    {
    public:
        Story135()
        {
            BookID = Book::Type::BOOK1;

            ID = 135;

            Text = "Which level of the mines will you explore now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Return to the surface", {Book::Type::BOOK1, 372}));
            Choices.push_back(Choice::Base("The twelfth level", {Book::Type::BOOK1, 739}));
            Choices.push_back(Choice::Base("The thirteenth level", {Book::Type::BOOK1, 388}));
            Choices.push_back(Choice::Base("The fourteenth level", {Book::Type::BOOK1, 597}));
            Choices.push_back(Choice::Base("The lowest level", {Book::Type::BOOK1, 222}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story136 : public Story::Base
    {
    public:
        Story136()
        {
            BookID = Book::Type::BOOK1;

            ID = 136;

            Text = "You complete the finishing touches, grinding the components to the necessary fineness, and sprinkling them with confidence into the brew. Soon you have completed two POTIONS OF INVULNERABILITY. Pleased with your new finds, you store them in your pack.\n\nYou gained the code A73.\n\nNote: When a party member drinks this potion at the start of a combat round, they can ignore any amount of damage they take from the next attack that hits them. The potion wears off after absorbing the first hit, or if the combat ends.";

            Choices.clear();
            Choices.push_back(Choice::Base("Pass through the door in the western wall", {Book::Type::BOOK1, 256}));
            Choices.push_back(Choice::Base("Return to the corridor through the east door", {Book::Type::BOOK1, 755}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::POTION_OF_INVULNERABILITY, Equipment::POTION_OF_INVULNERABILITY};

            Limit = 2;

            Engine::GET_CODES(party, {Codes::A(73)});
        }
    };

    class Story137 : public Story::Base
    {
    public:
        Story137()
        {
            BookID = Book::Type::BOOK1;

            Location = Location::Type::TUMBLESTONES;

            ID = 137;

            Image = "images/book1/statue_wreck.png";

            Text = "The sands whip against your face as you stagger through the desert. You seek shelter against the wreck of a once enormous statue, with only the sombre face of a long-forgotten king still visible in the lashing wind. Taking cover behind the hawkish nose of the stone face, you wait the storm out.\n\nWhen the wind dies down you examine the area. You realise where you are now. These are the Tumblestones, the remains of an ancient civilisation lost to the sands for all time. Historians from distant lands have attempted excavations here, but the heaped sands filled their trenches as fast as they could be dug, and the storms battered away their camps. Not much is left. A scattering of broken stones, perhaps once an enormous carved mural of some kind, lie revealed across the sands, the recent storms having unearthed them. Taking a casual look, you are surprised to see that they are fragments of a spell, carved in enormous letters.\n\nThe broken mural lies in pieces around you. It might be possible to fit the shattered mural together and decipher the spell if you are clever enough to puzzle it out.\n\nYou gained the code A17.";

            Choices.clear();
            Choices.push_back(Choice::Base("Rebuild the spell mural (Team check: Lore 4+, Successes: 4)", {Book::Type::BOOK1, 603}, {Book::Type::BOOK1, 368}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::LORE}, 4, 4));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(17)}))
            {
                return {Book::Type::BOOK1, 14};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(17)});
        }
    };

    class Story138 : public Story::Base
    {
    public:
        Story138()
        {
            BookID = Book::Type::BOOK1;

            ID = 138;

            Text = "You make your way to the Expeditionary Guild but find the area empty. The blue tents of House Ross have been packed away and there is no sign of Sir Lawrence or Emlyn Pass-Ross. Shrugging, you return to the city centre.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 775}; }
    };

    class Story139 : public Story::Base
    {
    public:
        Story139()
        {
            BookID = Book::Type::BOOK1;

            ID = 139;

            Text = "After washing yourselves you sit upon the bank. Amelia has washed her long hair and is beginning to re-braid it, gazing into her own reflection in the oasis. On a whim Tasha offers to help her, and soon she is twisting her hair into tight braids in a style reminiscent of a Splintered Isle buccaneer. It is a moment of tender kindness, which have been rare since you came to the savage Valley of Bones. Amelia accepts Tasha's help, occasionally checking her progress by glancing into the water.\n\n\"You're going to make me look like a pirate!\" Amelia laughs. \"I quite like it.\"\n\n\"We can take it out if you have to meet any fine lords or ladies,\" Tasha says idly, continuing to braid.\n\n\"If it makes me look more like you, I don't care if some noble doesn't like it,\" shrugs Amelia. \"I'm a bastard girl. No one in Royce see's past my name. Not even my own father.\"\n\n\"Strange customs in your land,\" Tasha muses. \n\n\"Perhaps we'll sail to the Splintered Isles, next. That's true freedom. There's no kings or queens there.\"\n\n\"Or laws,\" laughs Amelia. \"Maybe they're better off for it.\"\n\nThis has been a kindly encounter.\n\nTasha has gained a HEART for Amelia Pass-Dayne. Amelia Pass-Dayne also gains a HEART for Tasha.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_HEARTS(party, Character::Type::TASHA, Character::Type::AMELIA_PASS_DAYNE, 1);
            Engine::GAIN_HEARTS(party, Character::Type::AMELIA_PASS_DAYNE, Character::Type::TASHA, 1);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 51}; }
    };

    class Story140 : public Story::Base
    {
    public:
        Story140()
        {
            BookID = Book::Type::BOOK1;

            ID = 140;

            Text = "Although your trip into the shadow world was short, it was oddly disconcerting. You feel pleased to leave it behind. You head north, directly away from the Blackwall.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 858}; }
    };

    class Story141 : public Story::Base
    {
    public:
        Story141()
        {
            BookID = Book::Type::BOOK1;

            ID = 141;

            Text = "\"All right, then,\" nods the ogre. \"I hate them orcies anyway.\"\n\n\"It's madshards. That's what they're after,\" he says simply.\n\n\"What are madshards?\" you ask.\n\n\"You know... madshards,\" says the ogre, performing some kind of indistinct mime. \"Little bits of metal. From that broken talisman, thing? You know. It's what the whole orc uprising is about.\"\n\n\"And what is the orc uprising about?\" you press.\n\n\"Don't ask me,\" shrugs the ogre. \"I was hoping you could tell me.\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Try to convince the ogre to change sides", {Book::Type::BOOK1, 576}));
            Choices.push_back(Choice::Base("Kill the ogre", {Book::Type::BOOK1, 117}));
            Choices.push_back(Choice::Base("Leave the ogre in peace", {Book::Type::BOOK1, 310}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story142 : public Story::Base
    {
    public:
        Story142()
        {
            BookID = Book::Type::BOOK1;

            ID = 142;

            Location = Location::Type::CHALICE;

            IsCity = true;

            Text = "There is no central marketplace in Chalice, but rather the shops and stalls are spread out amongst the well-tended districts. Stern guards wearing fine longswords and ceremonial robes patrol the streets, watching you carefully as you go from shop to shop. The finest weapons in the valley are forged in this place, with techniques that rival those of Longport, although the very best swords are not available for foreigners.";

            Choices.clear();

            Controls = Story::Controls::SHOP;
        }

        void Event(Party::Base &party)
        {
            Shop = {
                {Equipment::CRUDE_BLADE, 25, 10},
                {Equipment::MAUL, -1, 25},
                {Equipment::IRON_SHORTSWORD1, 200, 100},
                {Equipment::IRON_GREATAXE2, -1, 150},
                {Equipment::STEEL_LONGSWORD2, 800, 400},
                {Equipment::STEEL_GREATSWORD3, -1, 450},
                {Equipment::MASTERWORK_BLADE3, -1, 900},
                {Equipment::MASTERWORK_GREATSWORD4, -1, 1250},
                {Equipment::SHIELD2, 50, 25},
                {Equipment::HIDE_ARMOUR1, -1, 30},
                {Equipment::BONE_ARMOUR2, -1, 150},
                {Equipment::BRONZE_ARMOUR4, -1, 2000},
                {Equipment::SOFT_BOOTS1, 500, 250},
                {Equipment::REFERENCE_BOOK1, 500, 250},
                {Equipment::WARM_CLOAK1, -1, 250},
                {Equipment::HANDSOME_BROOCH1, -1, 250},
                {Equipment::PRYBAR, 100, 50},
                {Equipment::INCENSE, -1, 90},
                {Equipment::GOLDWAX_CANDLE, -1, 100},
                {Equipment::ENGAGEMENT_RING, -1, 65},
                {Equipment::VIAL_OF_POISON, -1, 50},
                {Equipment::DRAGONYAK_HORN, -1, 35},
                {Equipment::LIZARD_HIDE, -1, 50}};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story143 : public Story::Base
    {
    public:
        Story143()
        {
            BookID = Book::Type::BOOK1;

            ID = 143;

            Text = "You spend a full month with Che Long training up the Bronzeguard. They are a formidable sight in their gleaming bronze armour and powerful steel weapons. Only the best recruits are permitted to stay in training, and soon you have a unit of the Everchild's fiercest supporters dressed in enchanted armour. The unit's morale is exceptional, and they are sure to make a powerful impression on the battlefield.\n\nYou may add the following unit to the Saltdad garrison:\n\n[Bronzeguard]: Strength 5, Morale 5\n\nYou cannot have more than one unit of [Bronzeguard] in existence at any given time as their equipment is irreplaceable. If the [Bronzeguard] are ever destroyed, you can train new recruits to use their equipment by spending another two GOLD BULLION bars. Feeling nigh indestructible, you return to the palace.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Army = {Army::Base("Bronzeguard", Army::Type::BRONZEGUARD, Location::Type::SALTDAD, 5, 5, true)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 620}; }
    };

    class Story144 : public Story::Base
    {
    public:
        Story144()
        {
            BookID = Book::Type::BOOK1;

            ID = 144;

            Text = "You are mid-way along the south coast of the Valley of Bones. To the south is Viaan Island, occupied by the fierce Bando tribe.";

            Choices.clear();
            Choices.push_back(Choice::Base("Sail into the deep harbour of the Bando village", {Book::Type::BOOK1, 716}));
            Choices.push_back(Choice::Base("Sail west", {Book::Type::BOOK1, 814}));
            Choices.push_back(Choice::Base("Sail east", {Book::Type::BOOK1, 198}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story145 : public Story::Base
    {
    public:
        Story145()
        {
            BookID = Book::Type::BOOK1;

            ID = 145;

            Text = "You manage to recall the correct incantations to dispel darkness of this kind, and suddenly your lanterns illuminate the room. Within you can see strange coffers, some of which have loose piles of coin scattered over them. Opening the coffers you find a good deal more, some 200 silver coins in total, and a set of ornate BONE ARMOUR (Armour +2).";

            Bye = "Unsure if your reward was worth the dreadful battle to get it, you move on.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::BONE_ARMOUR2};

            Limit = 1;

            Engine::GAIN_MONEY(party, 200);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 93}; }
    };

    class Story146 : public Story::Base
    {
    public:
        Story146()
        {
            BookID = Book::Type::BOOK1;

            ID = 146;

            Text = "Somewhere, deep within your subconscious, you realise that something is wrong. The book is constantly building to point, but never satisfying you. For pages and pages you have been utterly unenlightened, nor can you seem break away from reading it. Slowly, painfully, as if in a dream, you grasp the edge of a page and tear it out. The curse is instantly broken, and you continue to rip out pages until your consciousness clears. Your companions are relieved to see you have woken from your reverie. You have been standing immobile for a full ten minutes!";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 695}; }
    };

    class Story147 : public Story::Base
    {
    public:
        Story147()
        {
            BookID = Book::Type::BOOK1;

            ID = 147;

            Text = "With perfect timing you slip into the room, jemmy open the overseer's table drawer and grab the key. You emerge just in time, ducking into a darkened corner as a pair of guards wander by. Once they have passed, you creep back to the gatehouse door, unlocking it and entering.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 611}; }
    };

    class Story148 : public Story::Base
    {
    public:
        Story148()
        {
            BookID = Book::Type::BOOK1;

            ID = 148;

            Text = "Sir Lawrence pours you a bottle of fine Almskirk wine, and bids you sit upon velvet cushioned chairs in his plush marquee. \"We are here to find spices,\" admits Sir Lawrence. \"House Ross has always been more interested in trade than the petty wars of Longport Bay. Let the Dayne's and the Bailey's fight like dogs -- we are interested in the larger world. We have expeditions elsewhere, but this one is all about exploring the jungles that lie to the west of Lhasbreath. If we can find enough interesting herbs or spices it might be profitable enough to set up a trade post or colony.\"\n\nSir Lawrence frowns. \"Unfortunately, we've hit a snag,\" he admits. \"The knights who were going to escort our scholars into the jungle have perished. They engaged in an honour duel with several barbarian locals... and came out of the battle poorly. Until I can find guards I can trust, I cannot send our scholars in. You seem cultured and capable folk. Perhaps you would be willing to act as expedition leaders? There would be an ample reward on offer!\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Accept Sir Lawrence's offer", {Book::Type::BOOK1, 5}));
            Choices.push_back(Choice::Base("You dot not feel ready to risk the jungle just yet", {Book::Type::BOOK1, 775}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story149 : public Story::Base
    {
    public:
        Engine::Destination destination;

        Story149()
        {
            BookID = Book::Type::BOOK1;

            ID = 149;

            Text = "The bull stands seven feet tall at the shoulder.\n\nThis will not be an easy battle.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take.clear();

            Limit = 0;

            CanFlee = false;

            Monsters = {Monster::Base("Dragonyak Bull", 6, 3, 4, 15, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 115}; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                Take = {Equipment::DRAGONYAK_HORN};

                Limit = 1;
            }
        }
    };

    class Story150 : public Story::Base
    {
    public:
        Engine::Destination destination;

        Story150()
        {
            BookID = Book::Type::BOOK1;

            ID = 150;

            Text = "You must fight!";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Snakeman", Monster::Type::SNAKEMAN, 3, 4, 3, 7, 0),
                Monster::Base("Snakeman", Monster::Type::SNAKEMAN, 4, 4, 3, 7, 0),
                Monster::Base("Snakeman", Monster::Type::SNAKEMAN, 4, 4, 3, 5, 0),
                Monster::Base("Snakeman", Monster::Type::SNAKEMAN, 3, 4, 3, 5, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 579}; }
    };

    class Story151 : public Story::Base
    {
    public:
        Story151()
        {
            BookID = Book::Type::BOOK1;

            ID = 151;

            Text = "You open the door into a large chamber with a glowing floor. Strange circles and runes have been carved in the middle, but it is difficult to make out exactly what has been drawn as crawling over the mystic patterns are hundreds of beetles, their shiny black carapaces reflecting the light of your lanterns. Almost as one they stop and crawl round to look at you. The effect is rather eerie. Like a tiny phalanx they begin to advance upon you.";

            Choices.clear();
            Choices.push_back(Choice::Base("Cast a spell to disperse them", {Book::Type::BOOK1, 754}));
            Choices.push_back(Choice::Base("Attempt to crush the beetles underfoot", {Book::Type::BOOK1, 518}));
            Choices.push_back(Choice::Base("Slam the door shut to keep them out and return to the crossroads", {Book::Type::BOOK1, 566}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(61)}))
            {
                return {Book::Type::BOOK1, 685};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }
    };

    class Story152 : public Story::Base
    {
    public:
        Story152()
        {
            BookID = Book::Type::BOOK1;

            ID = 152;

            Location = Location::Type::CLIFFTOP;

            IsCity = true;

            Text = "The slaves of Clifftop are a lucky breed, compared to the citizens. Having paid money to acquire them, most masters are unwilling to whip their slaves to death, unlike the deathly ill peasants who toil in the fields.\n\nNote: If you have less than four party members in your team you may recruit some more from the slave market.\n\nIt costs 200 silver coins to buy another party member, and they start with no equipment. Remember that a party member without a weapon has their Fighting score reduced by 1 point in combat.";

            Choices.clear();

            Controls = Story::Controls::RECRUIT;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 19}; }
    };

    class Story153 : public Story::Base
    {
    public:
        Story153()
        {
            BookID = Book::Type::BOOK1;

            ID = 153;

            Text = "You show Mannus, the keeper of the holy purse, the IRON SCEPTRE -- a symbol that you speak with the Iron King's authority. He takes it and welcomes you, commanding a slave to pour you some wine and close the door. You begin to discuss the Saltdad tithe and convey the Iron King's wishes that it should be modified downwards as a point of principal. You also add that, coincidentally, the Iron King understands that Mannus is poorly compensated for his time in the tax office and that good conscience compels him to see that changed.";

            Choices.clear();
            Choices.push_back(Choice::Base("Bribe the official (Team check: Charisma 4+, Successes: 4)", {Book::Type::BOOK1, 476}, {Book::Type::BOOK1, 560}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 4, 4));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::IRON_SCEPTRE});
        }
    };

    class Story154 : public Story::Base
    {
    public:
        Story154()
        {
            BookID = Book::Type::BOOK1;

            ID = 154;

            Text = "You bid your companions farewell, assuring them you will be alright and that you must face the coming trial alone. Reluctantly they agree to leave you, saying that they will stay in Chalice until your exploration of the caves is complete.\n\nYou wander the hills for many hours, passing many small and empty caves, pondering where exactly you should be going. After a while you sit upon a rock and enter a deep meditation.\n\nNote: Until you return to Chalice you cannot call on any of your other party members to make skill checks, fight battles or carry or transfer equipment. Akihiro is completely alone.\n\nIf Akihiro dies, eventually the rest of the party will realise he isn't coming back and continue their adventure. Any items of equipment he was carrying at the time of his death will also be lost.";

            Choices.clear();
            Choices.push_back(Choice::Base("Meditate on your destiny (Individual check: Lore 4+, Successes: 2)", {Book::Type::BOOK1, 684}, {Book::Type::BOOK1, 510}, {Attribute::Type::LORE}, 4, 2));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(77)}))
            {
                return {Book::Type::BOOK1, 6};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Engine::GO_SOLO(party, Character::Type::AKIHIRO_OF_CHALICE);
        }
    };

    class Story155 : public Story::Base
    {
    public:
        Story155()
        {
            BookID = Book::Type::BOOK1;

            ID = 155;

            Text = "Dulcimer cannot be swayed and throws you out of his house. Akini thanks you for your efforts but is resigned to a grisly death at the hands of her lover. You cannot stay to watch, and leave, shaking your head at the savagery of the Saltdad court.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 75}; }
    };

    class Story156 : public Story::Base
    {
    public:
        Story156()
        {
            BookID = Book::Type::BOOK1;

            ID = 156;

            Text = "The CRIER BIRD should fetch a tidy sum in a big city. Pleased with your find, you make your way out of the jungle.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::CRIER_BIRD};

            Limit = 1;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 618}; }
    };

    class Story157 : public Story::Base
    {
    public:
        Story157()
        {
            BookID = Book::Type::BOOK1;

            ID = 157;

            Text = "The sound of drums and crowds roaring echo in your ears as you are once again lined up in the gatehouse in preparation for your latest bout. Up to four CRUDE BLADES (Fighting +0) can be equipped by your characters. Again, one character may be armed with a SHIELD (Armour +2), and another with a two-handed MAUL* (Fighting +1).\n\nEquip your characters as you wish";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::CRUDE_BLADE, Equipment::CRUDE_BLADE, Equipment::CRUDE_BLADE, Equipment::CRUDE_BLADE, Equipment::SHIELD2, Equipment::MAUL};

            Limit = 6;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 170}; }
    };

    class Story158 : public Story::Base
    {
    public:
        Story158()
        {
            BookID = Book::Type::BOOK1;

            ID = 158;

            Location = Location::Type::LHASBREATH;

            IsCity = true;

            Text = "The market of Lhasbreath is a noisy and vibrant place, based around a stone amphitheatre and bedecked with brightly coloured tents. Monkeys and birds chitter in the cool shadows, darting away as people approach, only to return to steal a loose berry or fruit.";

            Choices.clear();

            Controls = Story::Controls::SHOP;
        }

        void Event(Party::Base &party)
        {
            Shop = {
                {Equipment::CRUDE_BLADE, 25, 10},
                {Equipment::MAUL, 50, 25},
                {Equipment::IRON_SHORTSWORD1, 200, 100},
                {Equipment::IRON_GREATAXE2, 300, 150},
                {Equipment::STEEL_LONGSWORD2, -1, 400},
                {Equipment::STEEL_GREATSWORD3, -1, 450},
                {Equipment::SHIELD2, 50, 20},
                {Equipment::HIDE_ARMOUR1, 70, 35},
                {Equipment::BONE_ARMOUR2, 300, 150},
                {Equipment::BRONZE_ARMOUR4, -1, 2000},
                {Equipment::SOFT_BOOTS1, -1, 250},
                {Equipment::REFERENCE_BOOK1, -1, 250},
                {Equipment::WARM_CLOAK1, 500, 250},
                {Equipment::HANDSOME_BROOCH1, -1, 250},
                {Equipment::PRYBAR, 100, 50},
                {Equipment::INCENSE, -1, 90},
                {Equipment::BLUESTONE, -1, 45},
                {Equipment::GREY_TALISMAN, -1, 100},
                {Equipment::ENGAGEMENT_RING, -1, 65},
                {Equipment::VIAL_OF_POISON, -1, 50},
                {Equipment::DRAGONYAK_HORN, -1, 35},
                {Equipment::LIZARD_HIDE, -1, 25}};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 775}; }
    };

    class Story159 : public Story::Base
    {
    public:
        Story159()
        {
            BookID = Book::Type::BOOK1;

            ID = 159;

            Text = "As you cross a dune you come upon a strange sight. A monkey, just an arms-length tall, lies in the sand, breathing heavily, almost dehydrated. Such beasts are never seen outside the Lhasbreath jungles, and yet here is one apparently wandering the desert like a nomad. It is in a dreadful state.\n\nYou gained the code A42.";

            Choices.clear();
            Choices.push_back(Choice::Base("Give the monkey some water", {Book::Type::BOOK1, 552}));
            Choices.push_back(Choice::Base("Give it a wide berth", {Book::Type::BOOK1, 125}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(42)}))
            {
                return {Book::Type::BOOK1, 257};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(42)});
        }
    };

    class Story160 : public Story::Base
    {
    public:
        Story160()
        {
            BookID = Book::Type::BOOK1;

            ID = 160;

            Text = "You rip the book from the orc's hands. With a speed and bloodthirsty ferocity you would have thought was impossible from the placid creature, the orc rears up and stabs you in the neck with a knife he pulls from his sleeve.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to lose 4 Health", {Book::Type::BOOK1, -160}, Choice::Type::GAIN_HEALTH, -4));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event160 : public Story::Base
    {
    public:
        Event160()
        {
            BookID = Book::Type::BOOK1;

            ID = -160;

            DisplayID = 160;

            Text = "The rest of your party manage to drag the orc away and stab him to death as he violently thrashes. The change over the orc seemed unbelievable. You didn't even realise orcs could read!\n\nYou gained the code A71.";

            Choices.clear();
            Choices.push_back(Choice::Base("Examine the black book he was reading", {Book::Type::BOOK1, 119}));
            Choices.push_back(Choice::Base("Close the door and go somewhere else", {Book::Type::BOOK1, 821}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(71)});
        }
    };

    class Story161 : public Story::Base
    {
    public:
        Story161()
        {
            BookID = Book::Type::BOOK1;

            ID = 161;

            Text = "You make good names for yourselves in the arena, but come to be feared and envied by the professional gladiators. They arrange with the arena master to have you sold on to less heroic duties.";

            Choices.clear();
            Choices.push_back(Choice::Base("See where you end up", {{3, "You are sold to the taskmasters of Clifftop", {Book::Type::BOOK1, 866}}, {6, "You are sold to the priests of Cursus", {Book::Type::BOOK1, 531}}}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story162 : public Story::Base
    {
    public:
        Engine::Destination destination = {Book::Type::BOOK1, 220};

        Story162()
        {
            BookID = Book::Type::BOOK1;

            ID = 162;

            Text = "It is the morning of battle. About you the soldiers of the Everchild ready their weapons and armour, looking down the gentle incline towards the city. You doubt the soldiers of Cursus will oblige you by fighting you on the slopes -- you will have to go down to meet them.\n\nYou may choose a maximum of six units from the Saltdad barracks to commit to this battle.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            destination = {Book::Type::BOOK1, 220};

            EnemyArmy.clear();
            EnemySpells.clear();
            EnemyArmyStatus.clear();

            BattleLocation = Location::Type::SALTDAD;

            if (Engine::VERIFY_CODES(party, {Codes::A(40)}))
            {
                EnemyArmy.push_back(Army::Base("Cursite Infantry", Army::Type::CURSITE_INFANTRY, Location::Type::SALTDAD, Location::BattleField::LEFT_FLANK_FRONT, 4, 4, false));
            }
            else
            {
                EnemyArmy.push_back(Army::Base("Curzite Zealots", Army::Type::CURSITE_ZEALOTS, Location::Type::SALTDAD, Location::BattleField::LEFT_FLANK_FRONT, 4, 5, false));
                EnemyArmy.push_back(Army::Base("Cursite Infantry", Army::Type::CURSITE_INFANTRY, Location::Type::SALTDAD, Location::BattleField::LEFT_FLANK_SUPPORT, 4, 4, false));
            }

            if (!Engine::VERIFY_CODES(party, {Codes::A(95)}))
            {
                EnemySpells = {
                    {Spells::MassCombat::ENFEEBLEMENT_CENTER_FRONT, 0},
                    {Spells::MassCombat::CLINGING_DREAD_LEFT_FRONT, 1},
                    {Spells::MassCombat::ROUT_LEFT_FRONT, 2}};
            }

            if (Engine::VERIFY_CODES(party, {Codes::A(96)}))
            {
                EnemyArmy.push_back(Army::Base("Citizen Archers", Army::Type::CITIZEN_ARCHERS, Location::Type::SALTDAD, Location::BattleField::CENTER_FRONT, 2, 4, false));
                EnemyArmy.push_back(Army::Base("Cursite Riders", Army::Type::CURSITE_RIDERS, Location::Type::SALTDAD, Location::BattleField::RIGHT_FLANK_FRONT, 5, 4, false));
            }
            else
            {
                EnemyArmyStatus.push_back({Army::Status::STRENGTH1, EnemyArmy.size(), 0, 1});

                EnemyArmy.push_back(Army::Base("Mercenary Knights", Army::Type::MERCENARY_KNIGHTS, Location::Type::SALTDAD, Location::BattleField::CENTER_FRONT, 5, 3, false));
                EnemyArmy.push_back(Army::Base("Citizen Archers", Army::Type::CITIZEN_ARCHERS, Location::Type::SALTDAD, Location::BattleField::CENTER_SUPPORT, 2, 4, false));
                EnemyArmy.push_back(Army::Base("Cursite Riders", Army::Type::CURSITE_RIDERS, Location::Type::SALTDAD, Location::BattleField::RIGHT_FLANK_FRONT, 5, 4, false));
                EnemyArmy.push_back(Army::Base("Mercenary Spears", Army::Type::MERCENARY_SPEARS, Location::Type::SALTDAD, Location::BattleField::RIGHT_FLANK_SUPPORT, 3, 2, false));
            }
        }

        Engine::Destination Continue(Party::Base &party) { return destination; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                destination = {Book::Type::BOOK1, 316};
            }
            else
            {
                destination = {Book::Type::BOOK1, 220};
            }
        }
    };

    class Story163 : public Story::Base
    {
    public:
        Story163()
        {
            BookID = Book::Type::BOOK1;

            ID = 163;

            Text = "Swallowing your fear you crawl out of the tent and approach the strange light. You see it coalesce into a familiar figure. \"Mother...\" you say sadly.\n\n\"Aye, Amelia, it is I,\" responds your mother, her ghostly form wavering before you. \"I have travelled far from the Savage Lands to give you this warning. Beware Ashdain, lord of the third circle. He has fallen under the power of the dark one and is not your friend. Accept no quest or task he might give you.\"\n\n\"But... I have never heard of this man...\" you whisper. \"And who is this dark one you speak of...?\"\n\n\"I have said more than I should,\" responds the ghostly figure. \"Even your father, Baldwin, never treated me as wickedly as Ashdain. Find my bones... then you shall find the answers you seek.\"\n\n\"Where shall I find them?\" you ask desperately. \"Mother! Mother!\"\n\nBut it is too late. Your mother's image fades like moonlight and vanishes into the black sky. You are sobbing as you return to the tent, and your companions comfort you as best they can. If your mother is indeed dead, you will swear a vengeance upon her killer... if the ghost can be trusted.\n\nYou gained the code A49";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(49)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 840}; }
    };

    class Story164 : public Story::Base
    {
    public:
        Story164()
        {
            BookID = Book::Type::BOOK1;

            ID = 164;

            Text = "You are creeping through the shadowy compound, hardly daring to breathe as you tiptoe from one piece of cover to the next. Guards are everywhere!";

            Choices.clear();
            Choices.push_back(Choice::Base("Sneak into the kitchen", {Book::Type::BOOK1, 424}));
            Choices.push_back(Choice::Base("Prowl into the armoury", {Book::Type::BOOK1, 353}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story165 : public Story::Base
    {
    public:
        Story165()
        {
            BookID = Book::Type::BOOK1;

            ID = 165;

            Text = "You are in a short corridor that twists to the west. A locked door is set in the nearby western wall. Around the bend the corridor terminates in another door with a large red 'x' painted across it. You try the nearby door but find it securely locked.";

            Choices.clear();
            Choices.push_back(Choice::Base("Unlock the nearby door using a RUSTY KEY", {Book::Type::BOOK1, 896}, {Equipment::RUSTY_KEY}));
            Choices.push_back(Choice::Base("Go through the door with red 'x' on it", {Book::Type::BOOK1, 793}));
            Choices.push_back(Choice::Base("Go back to the long corridor", {Book::Type::BOOK1, 634}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story166 : public Story::Base
    {
    public:
        Story166()
        {
            BookID = Book::Type::BOOK1;

            ID = 166;

            Text = "The sand shifts beneath your feet. Bursting from the desert come three giant desert scorpions, each as large as a war hound. The scuttle forwards, their stingers tensing and claws open to catch you.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(1)}) && !Engine::VERIFY_CODES(party, {Codes::A(22)}))
            {
                return {Book::Type::BOOK1, 636};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        void Event(Party::Base &party)
        {
            Take.clear();

            Limit = 0;

            CanFlee = false;

            Monsters = {
                Monster::Base("Scorpion", 5, 4, 4, 5, 0),
                Monster::Base("Scorpion", 4, 4, 4, 4, 0),
                Monster::Base("Scorpion", 4, 4, 4, 3, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 804}; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                Take = {Equipment::VIAL_OF_POISON};

                Limit = 1;
            }
        }
    };

    class Story167 : public Story::Base
    {
    public:
        Story167()
        {
            BookID = Book::Type::BOOK1;

            ID = 167;

            Text = "You have returned to the funeral chamber of Imopposh the Mad. However, you have already defeated this vile shade and taken his treasures. There is nothing more to be done here.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 115}; }
    };

    class Story168 : public Story::Base
    {
    public:
        Story168()
        {
            BookID = Book::Type::BOOK1;

            ID = 168;

            Text = "With the golems defeated you must decide what to do next.\n\nGain the code A92.";

            Choices.clear();
            Choices.push_back(Choice::Base("Examine the vault door", {Book::Type::BOOK1, 215}));
            Choices.push_back(Choice::Base("Check out the room to the south", {Book::Type::BOOK1, 484}));
            Choices.push_back(Choice::Base("Leave the palace", {Book::Type::BOOK1, 265}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(92)});
        }
    };

    class Story169 : public Story::Base
    {
    public:
        Story169()
        {
            BookID = Book::Type::BOOK1;

            ID = 169;

            Text = "Your distraction team must divert the guard's attention from their posts at the gatehouses. What tactic do you think is best?";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Choices.clear();

            if (Engine::COUNT(party, Team::Type::DISTRACTION) > 1)
            {
                Choices.push_back(Choice::Base("Start a fight amongst yourselves", {Book::Type::BOOK1, 45}));
            }

            Choices.push_back(Choice::Base("Attempt to seduce the guards", {Book::Type::BOOK1, 7}));
            Choices.push_back(Choice::Base("Pretend to be a messenger, and say that their captain needs them urgently", {Book::Type::BOOK1, 875}));
        }
    };

    class Story170 : public Story::Base
    {
    public:
        Story170()
        {
            BookID = Book::Type::BOOK1;

            ID = 170;

            Image = "images/book1/malronac_the_deathengine.png";

            Text = "You step out into broiling heat, the sun scorching the sands under your bare feet. You are the first competitors into the arena and receive a roaring cheer from the crowd as you appear. High above you the Iron King gazes down impassively, Malronac the Deathengine standing unflinching by his side. You look across the heat-swept arena as the far gate is opened. You cannot help but swallow as a pair of shaggy-haired desert lions come snarling out of the gate. Their manes are stained red with the blood of other victims, and they roar at you with filthy yellow teeth. You must fight for your lives.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::Type::NO_COMBAT_SPELLS});

            CanFlee = false;

            Monsters = {
                Monster::Base("Lion", 4, 4, 4, 9, 0),
                Monster::Base("Lion", 3, 4, 4, 10, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 236}; }
    };

    class Story171 : public Story::Base
    {
    public:
        Story171()
        {
            BookID = Book::Type::BOOK1;

            ID = 171;

            Text = "The young man takes you to a small side room and asks you to wait while he organises introductions. You take a seat anxiously, hoping the young man is not some devious con artist who has just run off with your money.";

            Choices.clear();
            Choices.push_back(Choice::Base("Random Event", {{4, NULL, {Book::Type::BOOK1, 234}}, {6, NULL, {Book::Type::BOOK1, 308}}}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story172 : public Story::Base
    {
    public:
        Story172()
        {
            BookID = Book::Type::BOOK1;

            ID = 172;

            Text = "You find lodging at a large inn, The Iron Crown, which serves coffee and warm beer in a courtyard surrounded by rentable rooms. For every 5 silver coins you spend, each party member can recover 1 Health point. Spellcasters can also spend silver here to recharge their spells, purchasing components in the nearby marketplace and going into meditation in the privacy of their rooms.\n\nDuring your stay you ask about the nearby landmarks. \"The Blackwall is a place of ill reputation,\" confides a serving wench. \"No one knows why it was built, or what it is walling off, since it is easy to go around it. Still, those who become obsessed with it tend to disappear, never to be seen again!\" Frightening stuff!";

            RestPrice = 5;

            Choices.clear();

            Controls = Story::Controls::INN;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 75}; }
    };

    class Story173 : public Story::Base
    {
    public:
        Story173()
        {
            BookID = Book::Type::BOOK1;

            ID = 173;

            Text = "You frown. This book is nothing but a pack of well-written lies. The truth in the world can only be found from within, not from the idle rantings of some long-dead scholar. You tear out a number of offending pages, the world coming back into focus again. Your companions are relieved to see you have woken from your reverie. You have been standing immobile for a full ten minutes!";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 695}; }
    };

    class Story174 : public Story::Base
    {
    public:
        Story174()
        {
            BookID = Book::Type::BOOK1;

            ID = 174;

            Text = "The spiders above you merely watch as you carefully shuffle along the thin, crumbling ledge. Midway a part of the ledge flakes away, but you manage to keep your footing until you reach the far end. Soon, you have all crossed, the spiders wary of approaching such large, flame carrying prey. Another chamber beckons on the other side.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 347}; }
    };

    class Story175 : public Story::Base
    {
    public:
        Story175()
        {
            BookID = Book::Type::BOOK1;

            ID = 175;

            Text = "It will take skill and judgement to navigate the deadly shoals around Mordain. The city doesn't have a dire reputation for nothing!";

            Choices.clear();
            Choices.push_back(Choice::Base("Navigate the shoals (Team check: Survival 4+, Successes: 5)", {Book::Type::BOOK1, 662}, {Book::Type::BOOK1, 836}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 5));

            Controls = Story::Controls::STANDARD;
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (outcome)
            {
                Bye = "You manage to steer past the rocks and into the harbour.";

                Engine::SET_LOCATION(party, Location::Type::MORDAIN);
            }
            else
            {
                Bye = NULL;
            }
        }
    };

    class Story176 : public Story::Base
    {
    public:
        Story176()
        {
            BookID = Book::Type::BOOK1;

            ID = 176;

            Text = "\"We are emissaries sent by the Everchild,\" you say confidently. \"She wishes to discuss an important matter with the God King.\"\n\n\"The Everchild died a thousand summers ago,\" says the sister firmly. \"The God King does not deal with the emissaries of ghosts.\"\n\nThis will take some persuasion.";

            Choices.clear();
            Choices.push_back(Choice::Base("Convince the sister (Team check: Charisma 4+, Successes: 5)", {Book::Type::BOOK1, 709}, {Book::Type::BOOK1, 744}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 4, 5));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story177 : public Story::Base
    {
    public:
        Story177()
        {
            BookID = Book::Type::BOOK1;

            ID = 177;

            Text = "You are travelling on the Great Westroad, the western branch of a magnificent highway made in the elder times. The entire road is raised on a bank of earth some twenty feet high, and great sand dunes have piled up over the years on either side. It is busy with traffic, and with flatulent dragonyaks hauling many tons of goods in massive wagons between the great cities of the valley. To the north the Stonewalls act as a barrier between the consuming deserts of the south and the mysterious and powerful nation of Drakehallow.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go west to Saltdad", {Book::Type::BOOK1, 75}));
            Choices.push_back(Choice::Base("Go east to Luutanesh", {Book::Type::BOOK1, 614}));
            Choices.push_back(Choice::Base("Go north to the salt mines", {Book::Type::BOOK1, 25}));
            Choices.push_back(Choice::Base("Go south to the Withered Steppes", {Book::Type::BOOK1, 115}));
            Choices.push_back(Choice::Base("Talk to the caravan drivers", {Book::Type::BOOK1, 781}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story178 : public Story::Base
    {
    public:
        Story178()
        {
            BookID = Book::Type::BOOK1;

            ID = 178;

            Text = "The final giant skeleton topples with a mighty crash. The treasures of the ancients are now yours to seize! There are 2250 silver coins to take. You also find a GOLDWAX CANDLE, a bar of GOLD BULLION and a CLOAK OF PROTECTION (Survival +3). In the coffin of the king, you prise a MAGICAL SHORTSWORD (Fighting +4) from his grasp.";

            Bye = "Pleased with your finds you stride out of the chamber with your treasures, the curious door slamming shut behind you as you leave.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::GOLD_BULLION, Equipment::GOLDWAX_CANDLE, Equipment::CLOAK_OF_PROTECTION3, Equipment::MAGICAL_SHORTSWORD4};

            Limit = 4;

            Engine::GAIN_MONEY(party, 2250);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 46}; }
    };

    class Story179 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story179()
        {
            BookID = Book::Type::BOOK1;

            ID = 179;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "It will be difficult indeed to track down the local thieves' guild without a contact.";

            if (!Engine::VERIFY_CODES(party, {Codes::A(5)}))
            {
                PreText += " Your enquiries are met with nothing but blank stares and snarls.";
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(5)}))
            {
                return {Book::Type::BOOK1, 216};
            }
            else
            {
                return {Book::Type::BOOK1, 75};
            }
        }
    };

    class Story180 : public Story::Base
    {
    public:
        Story180()
        {
            BookID = Book::Type::BOOK1;

            ID = 180;

            Text = "You are in a long corridor, the remains of the barricade you dismantled spread thinly along its length. To the west, stone steps descend further into the complex. To the east is a simple wooden door.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go west, down the steps", {Book::Type::BOOK1, 747}));
            Choices.push_back(Choice::Base("Go east, through the door", {Book::Type::BOOK1, 821}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story181 : public Story::Base
    {
    public:
        Story181()
        {
            BookID = Book::Type::BOOK1;

            ID = 181;

            Text = "The normally straight-laced Amelia has enjoyed rather too much wine this evening. She is visibly tottering over to the wine barrel for more, to the raucous laughter of nearby soldiers.";

            Choices.clear();
            Choices.push_back(Choice::Base("Send a party member to escort her to bed", {Book::Type::BOOK1, 61}, Choice::Type::CHOOSE_PARTY_MEMBER));
            Choices.push_back(Choice::Base("Let the woman enjoy herself for once", {Book::Type::BOOK1, 594}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story182 : public Story::Base
    {
    public:
        Story182()
        {
            BookID = Book::Type::BOOK1;

            ID = 182;

            Location = Location::Type::LHASBREATH;

            IsCity = true;

            Text = "Lhasbreath is a bustling town, full of energy and life. Therefore, it is strange to encounter an older looking man looking glum upon his doorstep. \"My son, Garon the Bloody Mouthed is missing,\" he admits. \"He was adventuring near the cursed ruins of Mordain, but alas I have not seen him since. You are clearly travellers yourselves. If you should find my son, bring him home. You shall know him by his great beard which he paints red in honour of his grandfather.\"";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::BARBARIAN_BODY}))
            {
                return {Book::Type::BOOK1, 249};
            }
            else if (Engine::VERIFY_CODES_ANY(party, {Codes::A(67), Codes::A(86)}))
            {
                return {Book::Type::BOOK1, 331};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(66)}))
            {
                Bye = NULL;

                return {Book::Type::BOOK1, 286};
            }
            else
            {
                Bye = "You promise the old man that you will keep an eye out for him, and leave him to his misery.\n\nYou gained the code A67.";

                Engine::GET_CODES(party, {Codes::A(67)});

                return {Book::Type::BOOK1, 775};
            }
        }
    };

    class Story183 : public Story::Base
    {
    public:
        Story183()
        {
            BookID = Book::Type::BOOK1;

            ID = 183;

            Image = "images/book1/vault_door.png";

            Text = "Dashing from one pillar to the next as each guard turns their back, you manage to creep as stealthily as mice until you are out of sight of the guards. Eventually you reach the back of the hallway and come to vault door itself. It is a colossal slab of black marble. It has no lock, nor handle, and needs none. Its weight alone is designed to ensure it stays closed. It would take half a dozen men to open this door in normal times. Can you manage it yourselves?";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Bye = NULL;

            Choices.clear();

            if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::PRYBAR}))
            {
                Choices.push_back(Choice::Base("Open the vault (Team check: Fighting 5+, Successes: 4)", {Book::Type::BOOK1, 498}, {Book::Type::BOOK1, -183}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::FIGHTING}, 5, 4));
            }
            else
            {
                Choices.push_back(Choice::Base("Open the vault (Team check: Fighting 6+, Successes: 4)", {Book::Type::BOOK1, 498}, {Book::Type::BOOK1, -183}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::FIGHTING}, 6, 4));
            }

            Choices.push_back(Choice::Base("Give up and return to the main floor", {Book::Type::BOOK1, 574}));
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (!outcome)
            {
                Bye = "Each party member loses 1 Health point from the strain.";

                Engine::GAIN_HEALTH(party, -1);
            }
            else
            {
                Bye = NULL;
            }
        }
    };

    // Dummy event to trigger Bye message in BOOK1:0183
    class Event183 : public Story::Base
    {
    public:
        Event183()
        {
            BookID = Book::Type::BOOK1;

            ID = -183;

            DisplayID = 183;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party) { return {Book::Type::BOOK1, 183}; }
    };

    class Story184 : public Story::Base
    {
    public:
        Story184()
        {
            BookID = Book::Type::BOOK1;

            ID = 184;

            Text = "You are almost sad as Yu Yan sprawls to the ground. The woman fought well, and it was a shame such talent should go to waste. You search her, her STONECUTTER SWORD (Fighting +2) is the most interesting possession she has. This weapon inflicts two points of Health damage for each success you score against rocky or stony opponents.\n\nShe also carries 135 silver pieces and, to your delight, a SILVER IDOL. You shake your head in amazement. You almost let the object of your quest slip through your fingers!";

            Bye = "Pleased to have completed your quest so promptly, you make your way out of the cave, and back to the temple in Chalice.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::STONECUTTER_SWORD2, Equipment::SILVER_IDOL};

            Limit = 2;

            Engine::GAIN_MONEY(party, 135);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 856}; }
    };

    class Story185 : public Story::Base
    {
    public:
        Story185()
        {
            BookID = Book::Type::BOOK1;

            ID = 185;

            Text = "You get into a conversation with an old man named Janus about his adventurous wife, Alidale. Apparently Alidale was a great explorer of the Valley in the times before the rise of the Iron King. She had heard of the Temple of the Unbroken, the birthplace of the Cursus religion, and wanted to explore the ruins. Sadly, she never returned. It has been forty years since then, and Janus has never taken another wife. He asks for you to keep an eye out for a bronze locket if you should ever chance to venture into the temple yourself. It was a gift Janus bought his wife when they were courting, and he would like it back to member her by.\n\nYou assure him you will keep your eyes peeled before moving on.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::BRONZE_LOCKET}))
            {
                return {Book::Type::BOOK1, 43};
            }
            else if (Engine::VERIFY_CODES(party, {Codes::A(29)}))
            {
                return {Book::Type::BOOK1, 893};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 614}; }
    };

    class Story186 : public Story::Base
    {
    public:
        Story186()
        {
            BookID = Book::Type::BOOK1;

            ID = 186;

            Text = "A howling storm whips up from the cape to the west. The powerful gusts are blowing you closer and closer to the rocks!";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Bye = NULL;

            Choices.clear();

            if (!Engine::HAS_SPELL(party, {Spells::Type::NATURE_WARD}))
            {
                Choices.push_back(Choice::Base("Navigate the storm (Individual check: Survival 6+, Successes: Special)", {Book::Type::BOOK1, -186}, {Book::Type::BOOK1, -186}, {Attribute::Type::SURVIVAL}, 6, 0));
            }
        }
    };

    class Event186 : public Story::Base
    {
    public:
        std::string PreText = "";

        int damage = 0;

        Event186()
        {
            BookID = Book::Type::BOOK1;

            ID = -186;

            DisplayID = 186;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (party.CurrentShip != -1 && party.CurrentShip >= 0 && party.CurrentShip < party.Fleet.size())
            {
                damage = 8 - party.RecentSuccesses;

                Engine::GAIN_HEALTH(party.Fleet[party.CurrentShip], -damage);

                if (party.Fleet[party.CurrentShip].Health > 0)
                {
                    return {Book::Type::NONE, -1};
                }
                else
                {
                    return {Book::Type::BOOK1, 484};
                }
            }
            else
            {
                return {Book::Type::BOOK1, 484};
            }
        }

        void Event(Party::Base &party)
        {
            PreText = "Your ship suffers " + std::to_string(damage) + " damage.\n\nYou manage to avoid the worst of the dreadful weather, despite some damage to the ship. Relieved at your good fortune you resolutely sail on.";

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 240}; }
    };

    class Story187 : public Story::Base
    {
    public:
        Story187()
        {
            BookID = Book::Type::BOOK1;

            ID = 187;

            Text = "The rogues react with surprise and alarm as you stand up and make your way down the ladder that links the balcony to the ground floor. Evidently they have never received visitors before. You attempt to calm them, letting them know that you mean them no harm.";

            Choices.clear();
            Choices.push_back(Choice::Base("Calm the thieves (Team check: Charisma 4+, Successes: 3)", {Book::Type::BOOK1, 430}, {Book::Type::BOOK1, 266}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 4, 3));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story188 : public Story::Base
    {
    public:
        int attempts = 0;

        Story188()
        {
            BookID = Book::Type::BOOK1;

            ID = 188;

            Text = "You tie a few ropes together and bind them strongly to the most intact parts of the remaining wall, and then begin to abseil down the cliffside.";

            Choices.clear();

            Choices.push_back(Choice::Base("Climb down the cliff side (Team check: Survival 3+, Successes: 4)", {Book::Type::BOOK1, 662}, {Book::Type::BOOK1, -188}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 3, 4));

            Controls = Story::Controls::STANDARD;
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            temp_string = "";

            attempts++;

            if (!outcome)
            {
                if (attempts >= 4)
                {
                    attempts = 0;

                    Bye = "After several attempts, your entire party has made it down, one way or another!";

                    Choices[0].DestinationFailed = {Book::Type::BOOK1, 662};
                }
                else
                {
                    Choices[0].DestinationFailed = {Book::Type::BOOK1, -188};
                }
            }
            else
            {
                attempts = 0;
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 662}; }
    };

    class Event188 : public Story::Base
    {
    public:
        std::string PreText = "";

        Event188()
        {
            BookID = Book::Type::BOOK1;

            ID = -188;

            DisplayID = 188;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            temp_string = "";

            Bye = NULL;

            Choices.clear();

            if (Engine::COUNT(party) > 1)
            {
                PreText = "One of your party members falls about halfway down.";

                Choices.push_back(Choice::Base("Choose the party member who falls halfway down", {Book::Type::BOOK1, 188}, Choice::Type::GAIN_HEALTH, -3));
            }
            else
            {
                PreText = "You fall about halfway down.";

                auto character = Engine::FIRST(party);

                if (character >= 0 && character < party.Members.size())
                {
                    Engine::GAIN_HEALTH(party.Members[character], -3);
                }
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 188}; }
    };

    class Story189 : public Story::Base
    {
    public:
        Story189()
        {
            BookID = Book::Type::BOOK1;

            ID = 189;

            Text = "You meet with the Everchild in the war room at the palace. She and her generals are pouring over a huge map of the valley, with a particular focus on the city of Clifftop. \"Welcome back, my friends,\" she smiles. \"As you can see, we are planning our attack on Clifftop. Due to the relative fertility of the land, and reinforcements from Cursus, Clifftop possesses a large, if undisciplined army. I would feel confident going into battle against them with six units from the Saltdad garrison. Any less than that would be... risky. However, you've been stalwart commanders of my forces, and you got me this city. So tell me.\n\nAre we ready to launch an attack on Clifftop?\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Order the attack", {Book::Type::BOOK1, 610}));
            Choices.push_back(Choice::Base("Wait until you have more soldiers", {Book::Type::BOOK1, 620}));
            Choices.push_back(Choice::Base("Have a private word with the Everchild", {Book::Type::BOOK1, 488}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(100)}))
            {
                return {Book::Type::BOOK1, 421};
            }
            else if (Engine::VERIFY_CODES(party, {Codes::A(33)}))
            {
                return {Book::Type::BOOK1, 540};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }
    };

    class Story190 : public Story::Base
    {
    public:
        Story190()
        {
            BookID = Book::Type::BOOK1;

            ID = 190;

            Text = "At first you are unsure if the rumours are true. Can there really be an Expeditionary Guild in a city as raucous and illiterate as Lhasbreath? You find the 'guild' in the tent city, a collection of gloriously decorated marquees embroidered with the heraldic crest of House Ross (a stag's head on a field of blue and gold). This noble house of Royce is based in the county of Longport Bay, which means they are a long way from home.\n\nYou are soon spotted by the leader of the guild, Sir Lawrence Ross, uncle to Baroness Carissa, the head of the house. He is overjoyed to see some cultured travellers in Lhasbreath and immediately invites you into his tent for a drink.";

            Choices.clear();
            Choices.push_back(Choice::Base("Accept his offer", {Book::Type::BOOK1, 148}));
            Choices.push_back(Choice::Base("Politely decline and return to the city centre", {Book::Type::BOOK1, 775}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(85)}))
            {
                return {Book::Type::BOOK1, 138};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }
    };

    class Story191 : public Story::Base
    {
    public:
        Story191()
        {
            BookID = Book::Type::BOOK1;

            ID = 191;

            Text = "In order to get the weapons, you will need to split up your party. Choose up to three party members to perform a distraction, and up to two party members to steal the weapons. You must have at least one party member performing each mission.";

            Choices.clear();
            Choices.push_back(Choice::Base("Decide who will undertake each task", {Book::Type::BOOK1, 169}, Choice::Type::ASSIGN_TEAMS, {{Team::Type::WEAPONS, 1, 4}, {Team::Type::DISTRACTION, 1, 4}}, 2));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story192 : public Story::Base
    {
    public:
        Story192()
        {
            BookID = Book::Type::BOOK1;

            ID = 192;

            Text = "You believe you have deciphered the clever code in the writing. You find a narrow knothole in the rock and plunge your arm inside. You pull out a magnificent silver chalice, which you can sell in town for 100 silver coins.";

            Bye = "Pleased with your find, you depart.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GAIN_MONEY(party, 100);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 395}; }
    };

    class Story193 : public Story::Base
    {
    public:
        Story193()
        {
            BookID = Book::Type::BOOK1;

            ID = 193;

            Text = "You twist sharply and slip from Tommul's grasp. He crashes to the floor, his companion clumsily tripping over him. With a laugh you quickly dash away, back to the safety of your companions.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 157}; }
    };

    class Story194 : public Story::Base
    {
    public:
        Story194()
        {
            BookID = Book::Type::BOOK1;

            Location = Location::Type::LUUTANESH;

            ID = 194;

            Text = "You stroll away from Luutanesh, the sun climbing the sky as you make your way back to the great road.";

            Choices.clear();
            Choices.push_back(Choice::Base("West, towards Saltdad", {Book::Type::BOOK1, 177}));
            Choices.push_back(Choice::Base("North, to the salt mines", {Book::Type::BOOK1, 25}));
            Choices.push_back(Choice::Base("East, towards Chalice", {Book::Type::BOOK1, 442}));
            Choices.push_back(Choice::Base("South, to the Withered Steppes", {Book::Type::BOOK1, 115}));
            Choices.push_back(Choice::Base("Southeast, along the Northroad to Cursus", {Book::Type::BOOK1, 113}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story195 : public Story::Base
    {
    public:
        Story195()
        {
            BookID = Book::Type::BOOK1;

            ID = 195;

            Text = "You point out that the definition of a religious ceremony and a funeral observance are quite different things in Chalice law. Simply passing by a temple is not a ceremony in itself, and therefore no law is broken. The kensai pause a moment to consult a monk in a nearby temple, but soon realise that you are correct in the most literal sense. In deference to the dead man's life of selfless service to the state they allow the casket to pass.\n\nThe mourners give a most impious cheer before continuing their wake. The monk who advised the kensai congratulates you. \"Young men are good at war, but poor in wisdom,\" he says. \"With the exception of the young people I see before me. Thank you for avoiding bloodshed. Perhaps this gift will aid you in gaining even greater wisdom?\"\n\nThe monk presents you with a block of INCENSE.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::INCENSE};

            Limit = 1;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story196 : public Story::Base
    {
    public:
        Story196()
        {
            BookID = Book::Type::BOOK1;

            ID = 196;

            Text = "The tunnel ends in a door with a black triangle painted on it. Blood leaks from the gap under the door.";

            Choices.clear();
            Choices.push_back(Choice::Base("Open the door carefully", {Book::Type::BOOK1, 578}));
            Choices.push_back(Choice::Base("Return to the crossroads", {Book::Type::BOOK1, 310}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story197 : public Story::Base
    {
    public:
        Story197()
        {
            BookID = Book::Type::BOOK1;

            ID = 197;

            Text = "You stagger from the chasm, pulling the Everchild from the pit as you do so. You have all suffered wounds, and one of the Everchild's followers lies dead in the crevice. \"Saint Elias watch over his spirit,\" mutters the Everchild, as she glances back into the bloody pit. \"We must go on, or all was for naught.\"\n\nYou stagger, wounded, through the only exit.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 347}; }
    };

    class Story198 : public Story::Base
    {
    public:
        Story198()
        {
            BookID = Book::Type::BOOK1;

            ID = 198;

            Text = "Your ship is crashing through the rolling waves of the Azure Deep, the near-boundless ocean that covers the valley's southern coast. To the north are high cliffs, from which blasts of gritty sand, blown from the strong coastal breezes, pour from the cliffsides. It has the appearance of sulphur-yellow rivers, dashing down into the oblivion of the sea.";

            Choices.clear();
            Choices.push_back(Choice::Base("Random Event", {{2, NULL, {Book::Type::BOOK1, 721}}, {6, NULL, {Book::Type::BOOK1, 851}}}, 1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story199 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story199()
        {
            BookID = Book::Type::BOOK1;

            ID = 199;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(16)}))
            {
                PreText = "The barbarians treat you as friends. After exchanging pleasantries they ride on.";
            }
            else
            {
                PreText = "The barbarians have only looting on their minds, not fine words.";
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(16)}))
            {
                return {Book::Type::BOOK1, 840};
            }
            else
            {
                return {Book::Type::BOOK1, 238};
            }
        }
    };

    class Story200 : public Story::Base
    {
    public:
        Story200()
        {
            BookID = Book::Type::BOOK1;

            ID = 200;

            Text = "Battles at sea are very similar to personal combat. Your ship makes the first attack, unless the text tells you otherwise. Make a Fighting check using the Fighting score of your ship. The Defence of ships is always 4+ in ship combat. For every success you score you can reduce the Health of the enemy ship by 1 point. The enemy ship then attacks you in the same way.\n\nIf you have a spellcaster with any Sea Combat spells, they can cast one spell before each attack you make. Only spells marked 'Sea Combat' can be used when ships fight.\n\nThe combat ends when either your ship or the enemy are reduced to 0 or fewer Health points.\n\nGood luck, captain!";

            Choices.clear();

            Controls = Story::Controls::NONE;
        }
    };

    class Story201 : public Story::Base
    {
    public:
        Story201()
        {
            BookID = Book::Type::BOOK1;

            ID = 201;

            Text = "At first you fear you will have to stay in one of the religious hostelries that dot the city. A nightly whipping does not appeal, so fortunately you discover a single inn that caters to foreigners; The Sign of the Sand, built in the harbour district. Though it is built of stone, it reminds you of the kind of traditional inn you might find in Royce, with warm beer and comfortable rooms aplenty. It was built with the permission of the priests a hundred years ago, to encourage sea trade to the city. Its future is constantly in doubt, as hard-line priests detest its presence in the city, but so far economics has trumped religious politics.\n\nFor every 5 silver coins you spend, each party member can recover 1 Health point. Spell casters can also spend silver here to recharge their spells, purchasing components in the nearby marketplace and going into meditation in the privacy of their rooms.\n\nYou get into a discussion with some travellers from Drakehallow, who seem set on exploring the ruins of Mordain. \"There are vast tunnels and dungeons located beneath the surface,\" says one, a wizard from the magical school Animus Mast. \"No one has mapped them all, but a wise explorer will try. It's easy to get turned around in those deathly arcades.\"";

            RestPrice = 5;

            Choices.clear();

            Controls = Story::Controls::INN;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 340}; }
    };

    class Story202 : public Story::Base
    {
    public:
        Story202()
        {
            BookID = Book::Type::BOOK1;

            ID = 202;

            Text = "You are deep in the central deserts of the valley, where it is sometimes years between rainfall and civilisation is all but unknown.";

            Choices.clear();
            Choices.push_back(Choice::Base("Manage your water (Team check: Survival 4+, Successes: 4)", {Book::Type::BOOK1, -202}, {Book::Type::BOOK1, -202}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 4));

            Controls = Story::Controls::STANDARD;
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (outcome)
            {
                Bye = "You manage to conserve water by travelling mostly at night and sheltering from the worst of the sun.";
            }
            else
            {
                Bye = "Each party member loses 1 Health from heatstroke.";

                Engine::GAIN_HEALTH(party, -1);
            }
        }
    };

    class Event202 : public Story::Base
    {
    public:
        Event202()
        {
            BookID = Book::Type::BOOK1;

            ID = -202;

            DisplayID = 202;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party)
        {
            auto result = Engine::ROLL_DICE(1);

            if (result[0] < 3)
            {
                return {Book::Type::BOOK1, 159};
            }
            else
            {
                return {Book::Type::BOOK1, 125};
            }
        }
    };

    class Story203 : public Story::Base
    {
    public:
        Story203()
        {
            BookID = Book::Type::BOOK1;

            ID = 203;

            Text = "Hacking your way through the library you at last manage to break through the swarm of bashing, flapping books. When the last party member has escaped you slam the door shut, the thud of books impacting on the door shuddering through your body. Eventually the noise stops, and you presume the books have returned to their shelves. Hoping you will have no more book-related perils you make your way onwards.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 69}; }
    };

    class Story204 : public Story::Base
    {
    public:
        Story204()
        {
            BookID = Book::Type::BOOK1;

            ID = 204;

            Text = "\"This is a poor show, my lord, for a noble house to exert such effort against a common one,\" you say sadly. \"Has the house of Dulcimer dropped so low that it can only feud with peasants and serving girls? Are they your equals that you summon such a show of force and indignity? Such rustics should be ignored, not dignified with vengeance. Release the girl's aunt before you are made a laughingstock before the king!\" Panicked by your words, Dulcimer quickly agrees to release Akini's aunt, on your agreement that you will never mention this awkwardness in court. You agree and are soon escorting Akini's aunt back home.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 68}; }
    };

    class Story205 : public Story::Base
    {
    public:
        Story205()
        {
            BookID = Book::Type::BOOK1;

            ID = 205;

            Text = "Your decent is relatively straightforward, though you are compelled to light your lanterns to see anything more than the entry shaft. Tunnels lead off in four directions, carved out of the antique sandstone.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go northwest", {Book::Type::BOOK1, 761}));
            Choices.push_back(Choice::Base("Go northeast", {Book::Type::BOOK1, 801}));
            Choices.push_back(Choice::Base("Go southeast", {Book::Type::BOOK1, 869}));
            Choices.push_back(Choice::Base("Go southwest", {Book::Type::BOOK1, 217}));
            Choices.push_back(Choice::Base("Climb back up to the surface", {Book::Type::BOOK1, 426}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story206 : public Story::Base
    {
    public:
        Story206()
        {
            BookID = Book::Type::BOOK1;

            ID = 206;

            Text = "You greet the Bando tribesmen like old friends, and soon there is singing and cheering across the village. As a good will offering you present the Bando with a few surplus tools and ropes from the ship, and you are presented in turn with sweet, intoxicating fruit drinks and a well-crafted SPEAR (Fighting +0).";

            Bye = "Soon you are escorted to see the shaman.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::SPEAR};

            Limit = 1;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 381}; }
    };

    class Story207 : public Story::Base
    {
    public:
        Story207()
        {
            BookID = Book::Type::BOOK1;

            Location = Location::Type::CLIFFTOP;

            IsCity = true;

            ID = 207;

            Text = "The Church of Cursus in Clifftop is unique in the valley, in that it is made of jungle timbers rather than stone. The city is poor, and not even the faithful have the means to pay for the impressive black stone ziggurats more common to Cursus temples. Within the hall you can see flagellated parishioners begging for forgiveness from their distant god, whilst poor priests while away the endless hours with chants they must recite from memory for lack of expensive books to refer to.\n\nThe high priest is eager for donations. If you pay him 1,000 silver coins he will clumsily bless you. Find the party member with the lowest Stealth score in the team, and if their Stealth score is less than 3, increase their Stealth by 1 point. If all your party member's Stealth scores are already 3 or above, the blessing will have no effect.\n\nYou may also receive some RITUAL SCARRING, which are runes sacred to Cursus carved across the chest of the subject. The priests are bound to perform this service to any who ask for it. If one of your party members wants some RITUAL SCARRING, they must lose 1 point of Health permanently.";

            Choices.clear();
            Choices.push_back(Choice::Base("Select party member with lowest Stealth score", {Book::Type::BOOK1, -207}, Choice::Type::RAISEATTRIBUTE_WITH_BLESSING, {Attribute::Type::STEALTH}, 1000, 3, 1));
            Choices.push_back(Choice::Base("Receive some ritual scarring", {Book::Type::BOOK1, -207}, Choice::Type::PAYFORSTATUS_WITH_HEALTH, {Character::Status::RITUAL_SCARRING}, -1));
            Choices.push_back(Choice::Base("You are finished here", {Book::Type::BOOK1, 19}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event207 : public Story::Base
    {
    public:
        Event207()
        {
            BookID = Book::Type::BOOK1;

            ID = -207;

            DisplayID = 207;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party) { return {Book::Type::BOOK1, 207}; }
    };

    class Story208 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story208()
        {
            BookID = Book::Type::BOOK1;

            ID = 208;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You are about halfway across the plank when it suddenly breaks in two. You plunge down into the pit and onto the spikes.";

            if (party.LastSelected >= 0 && party.LastSelected < party.Members.size() && Engine::SCORE(party.Members[party.LastSelected], Attribute::Type::HEALTH) > 0)
            {
                auto result = Engine::ROLL_DICE(1);

                Engine::GAIN_HEALTH(party.Members[party.LastSelected], -result[0]);

                PreText += "\n\n" + std::string(party.Members[party.LastSelected].Name) + " loses " + std::to_string(result[0]) + " Health Point";

                if (result[0] > 1)
                {
                    PreText += "s";
                }

                PreText += ".";
            }

            PreText += "\n\nYou watch as the timber plank which 'broke' above you suddenly snaps back into place. Only now, from this position, can you see that the plank is hinged on either side, and is designed to separate when someone puts their weight on it and then reset immediately afterwards. You curse the ingenuity of the trap maker as your companions quickly come to your rescue, bending down to offer you their hands. You scrabble out of the pit.";

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 761}; }
    };

    class Story209 : public Story::Base
    {
    public:
        Story209()
        {
            BookID = Book::Type::BOOK1;

            ID = 209;

            Text = "You return to the excavated cavern dug out by the invading orcs. The short tunnels lead to nothing but dead ends. After giving the chamber one last look, you make your way back up the stairs.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 634}; }
    };

    class Story210 : public Story::Base
    {
    public:
        Story210()
        {
            BookID = Book::Type::BOOK1;

            ID = 210;

            Image = "images/book1/attic_bunkbeds.png";

            Text = "You are standing in a comfortable attic room which contains dozens of bunkbeds. Monkeys sleep in the beds, blankets tucked tight around themselves. A few monkeys are still up, sitting in small chairs, reading books by candlelight. Tiny wardrobes contain robes, turbans and hats. What kind of madhouse is this? The monkeys resolutely ignore you as you make your way across the floor. It seems as if you are free to explore the house. Where will you go?";

            Choices.clear();
            Choices.push_back(Choice::Base("To Unbraaki's study", {Book::Type::BOOK1, 659}));
            Choices.push_back(Choice::Base("To the library", {Book::Type::BOOK1, 459}));
            Choices.push_back(Choice::Base("To the basement", {Book::Type::BOOK1, 67}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story211 : public Story::Base
    {
    public:
        Story211()
        {
            BookID = Book::Type::BOOK1;

            ID = 211;

            Text = "You are in the room with the open coffin. Shattered skeletons surround you.";

            Choices.clear();
            Choices.push_back(Choice::Base("Go through the north door", {Book::Type::BOOK1, 717}));
            Choices.push_back(Choice::Base("Go through the south door", {Book::Type::BOOK1, 18}));
            Choices.push_back(Choice::Base("Go through the south-west door", {Book::Type::BOOK1, 127}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story212 : public Story::Base
    {
    public:
        Story212()
        {
            BookID = Book::Type::BOOK1;

            ID = 212;

            Text = "Puffing yourselves up, you and your companions put on a brave show before the slaves, shoving them hard and roaring bold threats. This will require a good deal of bravery and force.";

            Choices.clear();
            Choices.push_back(Choice::Base("Threaten greedy slaves (Team check: Fighting 4+, Successes: 4)", {Book::Type::BOOK1, 526}, {Book::Type::BOOK1, 572}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::FIGHTING}, 4, 4));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story213 : public Story::Base
    {
    public:
        Story213()
        {
            BookID = Book::Type::BOOK1;

            ID = 213;

            Text = "You draw your weapons and prepare for a mighty battle. You are pleased to see the slaves, who realise that this is their best chance of escape, turn on their masters with their picks and shovels.\n\nNote: The slaves will assist you in this battle. After all your party members have attacked, the slaves will inflict 1 point of Health damage on each orc, except the orc leader -- they are too terrified to strike him!";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Orc", Monster::Type::ORC, 4, 4, 4, 9, 0),
                Monster::Base("Orc", Monster::Type::ORC, 5, 4, 4, 11, 0),
                Monster::Base("Orc", Monster::Type::ORC, 4, 4, 4, 10, 0),
                Monster::Base("Orc", Monster::Type::ORC, 3, 4, 4, 10, 0),
                Monster::Base("Orc", Monster::Type::ORC, 4, 4, 4, 6, 0),
                Monster::Base("Orc Leader", Monster::Type::ORC_LEADER, 7, 4, 4, 14, 0)};

            Allies = {Allies::Type::SLAVES};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 707}; }
    };

    class Story214 : public Story::Base
    {
    public:
        Story214()
        {
            BookID = Book::Type::BOOK1;

            ID = 214;

            Location = Location::Type::SALTDAD;

            IsCity = true;

            Text = "The Iron King's armies cannot hold off against you. They break and run, scattering into the desert. The Everchild commands the army forward into the city of Saltdad itself. A number of guards and priests of Cursus attempt to stand in your army's way, and the battle dissolves into furious street fight between your forces. Incredibly, despite your overwhelming victory on the field, it looks like the Iron King might be able to hold you off until the desert night freezes your troops away.\n\nThe deadlock has to be broken! Leaving the Everchild in charge of the army you break through the lines to storm the palace itself. Kicking open the door to the throne room you can see the Iron King in his full battle regalia, waiting with his elite guards.\n\n\"Foreign devils!\" he spits, still proudly wearing his jagged crown. \"This city is mine! I'll not hand over the reins of power to a child!\"\n\nHe grimly advances. The final battle for Saltdad has begun!";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters.clear();

            Monsters.push_back(Monster::Base("The Iron King", 6, 4, 4, 15, 0));

            if (!Engine::VERIFY_CODES(party, {Codes::A(13)}))
            {
                Monsters.push_back(Monster::Base("Malronac the Deathengine", 6, 3, 4, 30, 0, 0, 2));
            }

            if (!Engine::VERIFY_CODES(party, {Codes::A(8)}))
            {
                Monsters.push_back(Monster::Base("Bronzeguard", 9, 4, 5, 12, 0));
            }

            Monsters.push_back(Monster::Base("Sgt. Kheller", 5, 4, 4, 7, 0));
            Monsters.push_back(Monster::Base("Sgt. Tacitus", 5, 4, 4, 6, 0));
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 309}; }
    };

    class Story215 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story215()
        {
            BookID = Book::Type::BOOK1;

            ID = 215;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You look over the vault door. As you surmised there appears to be no way to open it. Without a key or a handle, the vault seems inaccessible.";

            Choices.clear();

            if (!Engine::VERIFY_CODES(party, {Codes::A(43)}))
            {
                PreText += " Perhaps you can find an expert to help you with the vault door?\n\nYou gained the code A11.";

                Engine::GET_CODES(party, {Codes::A(11)});

                Choices.push_back(Choice::Base("Check out the room to the south", {Book::Type::BOOK1, 484}));
                Choices.push_back(Choice::Base("Leave the palace", {Book::Type::BOOK1, 265}));
            }

            Text = PreText.c_str();
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(93)}))
            {
                return {Book::Type::BOOK1, 348};
            }
            else if (Engine::VERIFY_CODES(party, {Codes::A(12)}))
            {
                return {Book::Type::BOOK1, 258};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 293}; }
    };

    class Story216 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story216()
        {
            BookID = Book::Type::BOOK1;

            ID = 216;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You make your way under the city to see Brekken, the leader of the thieves' gang you encountered in the tunnels beneath the arena. He is pleased to see you again and celebrates the occasion by opening a bottle of wine.";

            Choices.clear();

            if (!Engine::VERIFY_CODES(party, {Codes::A(10)}))
            {
                PreText += "\n\n\"It was quite a feat, you sneaking out of the Saltdad Arena like that,\" muses Brekken over a glass of white wine. \"It's a shame to waste such talents merely on matters of survival. Perhaps you'd be willing to do a little wealth redistribution? Namely, from the silver stores of a rich man into the pockets of the poor -- in other words, us?\"\n\n\"What's the job?\" you ask neutrally.\n\n\"The city jeweller, Chellar, a former chancellor of the Iron King,\" says Brekken. \"He's as rich as a king himself. We go in, grab everything we can, and get out. Of course, his brother is a sorcerer, so they say the whole place is protected by magical defences. That's where you come in. You help us deal with the magic, and we'll help get through the more mundane protections, like locks and stubborn guards. We split the rewards fifty-fifty. Deal?\"";

                Engine::GET_CODES(party, {Codes::A(11)});

                Choices.push_back(Choice::Base("Agree to the heist", {Book::Type::BOOK1, 617}));
                Choices.push_back(Choice::Base("Politely decline", {Book::Type::BOOK1, 75}));
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 663}; }
    };

    class Story217 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story217()
        {
            BookID = Book::Type::BOOK1;

            ID = 217;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You continue down the tunnel for a while before it ends at a rather grand door with a strange, three-pronged lock set in its centre. The door is solidly constructed and will not budge an inch, nor is the lock something that can be easily picked.";

            Bye = NULL;

            if (!Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::PYRAMIDAL_KEY}))
            {
                Bye = "There is nothing else for you to do here, so you return to the crossroads.";
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::PYRAMIDAL_KEY}))
            {
                return {Book::Type::BOOK1, 102};
            }
            else
            {
                return {Book::Type::BOOK1, 46};
            }
        }
    };

    class Story218 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story218()
        {
            BookID = Book::Type::BOOK1;

            ID = 218;

            Bye = "Sadly, there is now no chance of recovering the idol. Accepting your failure lightly, you decide to return to the Chalice temple and face your fate.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "You sit upon the ledge side, on a small outcropping of rock, crossing your legs and calming your breathing. In the distance Yu Yan looks at you with amazement, before slipping away into the dusk.\n\nYou gaze out into the beautiful landscape, considering the perfect arrangement of the world before you. Sky, water, rock and sand lay in harmony with each other. All is still, with the setting sun casting the sky into scarlet. Soon the stars emerge, one by one, until a celestial sea revolves around your head, adding the heavens into a mix of perfect symmetry. As you gaze upon the sight, abandoning your quest for the idol, the weight of failure seems to lift from you. Your thoughts enter balance, and suddenly your ambitions seem as weightless as the air.";

            if (!Engine::VERIFY_EQUIPMENT(party, {Equipment::Type::PYRAMIDAL_KEY}))
            {
                auto result = Engine::FIND_CHARACTER(party, Character::Type::AKIHIRO_OF_CHALICE);

                if (result >= 0 && result < party.Members.size() && Engine::SCORE(party.Members[result], Attribute::Type::HEALTH) > 0)
                {
                    PreText += "\n\nAkihiro has achieved Enlightenment. Akihiro can gains 1 point of Lore.";

                    Engine::GAIN_SCORE(party.Members[result], Attribute::Type::LORE, 1);
                }
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 856}; }
    };

    class Story219 : public Story::Base
    {
    public:
        Story219()
        {
            BookID = Book::Type::BOOK1;

            ID = 219;

            Text = "Where do you wish to go from here?";

            Choices.clear();
            Choices.push_back(Choice::Base("Continue west along the road", {Book::Type::BOOK1, 12}));
            Choices.push_back(Choice::Base("South, towards the Blackwall", {Book::Type::BOOK1, 691}));
            Choices.push_back(Choice::Base("North-west into The Salt Plains", {Book::Type::BOOK1, 460}));
            Choices.push_back(Choice::Base("East, to Saltdad", {Book::Type::BOOK1, 75}));
            Choices.push_back(Choice::Base("South-west, to the Tumblestones", {Book::Type::BOOK1, 137}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story220 : public Story::Base
    {
    public:
        Story220()
        {
            BookID = Book::Type::BOOK1;

            ID = 220;

            Text = "The odds are impossible! Your forces are overwhelmed. You watch in despair as the Everchild's encampment is overridden by a hundred horsemen. You share her fate, the merciless army of Cursus chasing you down and cutting your throats. The dream of freedom in the valley, and your adventure, is over.";

            Type = Story::Type::DOOM;

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story221 : public Story::Base
    {
    public:
        Story221()
        {
            BookID = Book::Type::BOOK1;

            ID = 221;

            Text = "The whole family have gathered outside onto the terrace as you approach, their neighbours craning their necks to see you. Janu, your brother-in-law, bows deeply as you approach. \"You honour us with your visit, kensai,\" he says honestly. \"My house is yours. It would please me if you and your companions would stay with us during your visit to the city.\"\n\n\"Thank you, Janu,\" you say. \"Your offer is most kind.\" That night the children sit at your feet as you recount tales of your adventures. Your mother and sister glow with pride to see you in your fine robes, your confidence returned.\n\nYou may stay in the house as long as you like. All party members can restore their Health scores to maximum. Spell casters can also spend silver here to recharge their spells, purchasing components in the nearby marketplace and going into meditation in the privacy of their rooms.";

            RestPrice = 0;

            Choices.clear();

            Controls = Story::Controls::INN;
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Engine::IN_PARTY(party, Character::Type::SAR_JESSICA_DAYNE) && Engine::HEARTS(party, Character::Type::SAR_JESSICA_DAYNE, Character::Type::AKIHIRO_OF_CHALICE) > 0)
            {
                return {Book::Type::BOOK1, 607};
            }
            else if (Engine::IN_PARTY(party, Character::Type::BRASH) && Engine::HEARTS(party, Character::Type::BRASH, Character::Type::AKIHIRO_OF_CHALICE) > 0)
            {
                return {Book::Type::BOOK1, 62};
            }
            else
            {
                return {Book::Type::BOOK1, 450};
            }
        }
    };

    class Story222 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story222()
        {
            BookID = Book::Type::BOOK1;

            ID = 222;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "The lift slams onto the surface of the fifteenth level. This is the deepest portion of the mine, long since abandoned by the workers.";

            Choices.clear();

            if (!Engine::VERIFY_CODES(party, {Codes::A(27)}))
            {
                PreText += "\n\nYou creep carefully down the tunnels until they open into a sizable cavern. Curled up into a tight ball, you see the fish-white scales of a terrible salt dragon, sleeping on a pile of human bones and scattered treasure. Its eyes flick open, and it puffs sharply, trying to scare you off.";

                Engine::GET_CODES(party, {Codes::A(11)});

                Choices.push_back(Choice::Base("Fight the dragon", {Book::Type::BOOK1, 787}));
                Choices.push_back(Choice::Base("Carefully back away to the lift", {Book::Type::BOOK1, 135}));
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 344}; }
    };

    class Story223 : public Story::Base
    {
    public:
        Story223()
        {
            BookID = Book::Type::BOOK1;

            ID = 223;

            Text = "Soon it is time for another bloody battle in the arena. Each character is given a CRUDE BLADE (Fighting +0) and pushed out to face the current arena champions.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take.clear();

            for (auto i = 0; i < Engine::COUNT(party); i++)
            {
                Take.push_back(Equipment::CRUDE_BLADE);
            }

            Limit = Take.size();
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, -223}; }
    };

    class Event223 : public Story::Base
    {
    public:
        Event223()
        {
            BookID = Book::Type::BOOK1;

            ID = -223;

            DisplayID = 223;

            Text = "The crowd roar -- they recognise you from your previous battles and know that this should prove an entertaining fight.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Grim Gladiator", 8, 4, 4, 12, 0),
                Monster::Base("Mighty Gladiator", 5, 3, 4, 10, 0),
                Monster::Base("Agile Gladiator", 6, 3, 5, 6, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 161}; }
    };

    class Story224 : public Story::Base
    {
    public:
        Story224()
        {
            BookID = Book::Type::BOOK1;

            ID = 224;

            Text = "Perhaps it would be wiser if only a few people entered the room? Decide who is passing through the shadow door and who is staying behind. You can send your whole party in if you wish, or change your mind and close off the shadow door.";

            Choices.clear();
            Choices.push_back(Choice::Base("Select the party members who will pass through the shadow door", {Book::Type::BOOK1, -224}, Choice::Type::ASSIGN_TEAMS, {{Team::Type::SHADOW_ROOM, 0, 4}}, 0));
            Choices.push_back(Choice::Base("Close the shadow door", {Book::Type::BOOK1, 717}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Event224 : public Story::Base
    {
    public:
        Event224()
        {
            BookID = Book::Type::BOOK1;

            ID = -224;

            DisplayID = 224;

            Choices.clear();

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::COUNT(party, Team::Type::SHADOW_ROOM) > 0)
            {
                return {Book::Type::BOOK1, 903};
            }
            else
            {
                return {Book::Type::BOOK1, 717};
            }
        }
    };

    class Story225 : public Story::Base
    {
    public:
        Story225()
        {
            BookID = Book::Type::BOOK1;

            ID = 225;

            Location = Location::Type::CHALICE;

            IsCity = true;

            Text = "You have a most pleasant stroll through the centre of Chalice. The streets are peaceful and the people orderly. You have a cup of rice wine by the river to relax and watch as a graceful dune-swan and its chicks paddle down waterway.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story226 : public Story::Base
    {
    public:
        Story226()
        {
            BookID = Book::Type::BOOK1;

            ID = 226;

            Text = "Security is much too tight around the archwizard. All the party members assigned to the assassination have been captured. What happens to them now is down to you. If you capture the city of Cursus you can free them from prison. If your army is defeated, the party members will be executed. Either way, the captured party members can take no further part in the battle until rescued.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0 && party.Members[i].Team == Team::Type::ASSASSINATION_DESCANTOS)
                {
                    Engine::GAIN_STATUS(party.Members[i], Character::Status::CAPTURED);
                }
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 96}; }
    };

    class Story227 : public Story::Base
    {
    public:
        Story227()
        {
            BookID = Book::Type::BOOK1;

            ID = 227;

            Text = "The salt mine is only moderately populated. Many of the ex-slaves have deserted mine work for more pleasant pursuits above ground. Those who remain are better dressed and equipped than they were before but look just as unhealthy.\n\nWhich level of the mines will you explore?";

            Choices.clear();
            Choices.push_back(Choice::Base("The twelfth level", {Book::Type::BOOK1, 739}));
            Choices.push_back(Choice::Base("The thirteenth level", {Book::Type::BOOK1, 388}));
            Choices.push_back(Choice::Base("The fourteenth level", {Book::Type::BOOK1, 597}));
            Choices.push_back(Choice::Base("The lowest level", {Book::Type::BOOK1, 222}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story228 : public Story::Base
    {
    public:
        Story228()
        {
            BookID = Book::Type::BOOK1;

            ID = 228;

            Text = "You believe you have deciphered the clever code in the writing. You find a narrow knothole in the rock and plunge your arm inside. You cry out in shock as a savage blade slices into your hand. Pulling it out you are astonished you did not lose the whole thing.";

            Choices.clear();

            Choices.push_back(Choice::Base("Choose a party member to lose 5 Health points and 1 point of Fighting.", {Book::Type::BOOK1, 395}, Choice::Type::GAIN_HEALTH_ATTRIBUTE, {Attribute::Type::FIGHTING}, -5, -1));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story229 : public Story::Base
    {
    public:
        Story229()
        {
            BookID = Book::Type::BOOK1;

            ID = 229;

            Text = "You furrow your brow as you attempt to mar the runes on the window frame.";

            Choices.clear();
            Choices.push_back(Choice::Base("Dispel the runes (Team check: Lore 4+, Successes: 5)", {Book::Type::BOOK1, 791}, {Book::Type::BOOK1, 831}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::LORE}, 4, 5));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story230 : public Story::Base
    {
    public:
        Story230()
        {
            BookID = Book::Type::BOOK1;

            ID = 230;

            Text = "The bookcase contains a number of valuable tomes, including a REFERENCE BOOK (LORE +1). There is also an interesting tome on herb lore. You flick through it briefly, and come upon a reference to the Lhasbreath Jungles, here in the Valley of Bones. \"...it is a well-known fact that the yellow Hygliph Flower is proof against all snakes, even the wicked snakemen in the Temple of the Unbroken, who react poorly to its pungent perfume.\" Interesting.\n\nYou may take the REFERENCE BOOK. You grab a few of the more PRECIOUS TOMES AS well.";

            Choices.clear();
            Choices.push_back(Choice::Base("Snatch the golden candlestick, if you haven't already", {Book::Type::BOOK1, 644}));
            Choices.push_back(Choice::Base("Head upstairs to help the climbing team", {Book::Type::BOOK1, 92}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::REFERENCE_BOOK1, Equipment::PRECIOUS_TOMES};

            Limit = 2;
        }
    };

    class Story231 : public Story::Base
    {
    public:
        Story231()
        {
            BookID = Book::Type::BOOK1;

            ID = 231;

            Text = "Choose a party member to sneak into the chamber.";

            Choices.clear();
            Choices.push_back(Choice::Base("Sneak into the chamber (Individual check: Stealth 5+, Successes: 2)", {Book::Type::BOOK1, 428}, {Book::Type::BOOK1, 799}, {Attribute::Type::STEALTH}, 5, 2));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story232 : public Story::Base
    {
    public:
        Story232()
        {
            BookID = Book::Type::BOOK1;

            ID = 232;

            Text = "To your mixed delight and horror, the [SKELETONS] rise from the dead, their bones clicking and clacking as they stand up straight before you. The [SKELETONS] will be useful allies.\n\nYou gained the code A75.\n\nNote: There are [SKELETONS] for each party member to control. While a party member has a [SKELETON] following them, they gain an additional 2 points to their Fighting skill, which combines with any other bonuses from weapons or other items. Additionally, your [SKELETONS] will bravely take damage for you. Each skeleton has 6 Health points. When a party member takes damage in combat (not from any other source) the skeleton will step in the way and take the damage for that character. They have no armour, and are destroyed when they are reduced to 0 Health. Skeletons cannot regain Health points. Your [SKELETONS] can only exist whilst you are in the Mordain ruins. As soon as you step outside they will crumble to dust.";

            Bye = "Pleased with your new companions, you make your way back to the crossroads.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            for (auto i = 0; i < party.Members.size(); i++)
            {
                if (Engine::SCORE(party.Members[i], Attribute::Type::HEALTH) > 0)
                {
                    Engine::GAIN_FOLLOWERS(party.Members[i], {Follower::Base("Skeleton", Follower::Type::MORDAIN_SKELETONS, 6)});
                }
            }

            Engine::GET_CODES(party, {Codes::A(75)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 310}; }
    };

    class Story233 : public Story::Base
    {
    public:
        std::string PreText = "";

        Story233()
        {
            BookID = Book::Type::BOOK1;

            ID = 233;

            Choices.clear();
            Choices.push_back(Choice::Base("Return to the mine to hunt more troglodytes", {Book::Type::BOOK1, 644}));
            Choices.push_back(Choice::Base("Make your excuses and leave the mine", {Book::Type::BOOK1, 722}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            auto count = Engine::COUNT_EQUIPMENT(party, {Equipment::Type::TROGLODYTE_HEAD});

            Engine::GAIN_MONEY(party, count * 25);

            PreText = "You received " + std::to_string(count * 25) + " silver coins for the TROGLODYTE HEADs.\n\n\"Keep up the good work,\" he enthuses. \"I'm sure the slaves appreciate your valiant efforts.\"";

            Text = PreText.c_str();
        }
    };

    class Story234 : public Story::Base
    {
    public:
        Story234()
        {
            BookID = Book::Type::BOOK1;

            ID = 234;

            Text = "The man is a devious con artist who has just run off with your money. Eventually a surprised looking servant enters the room and sadly tells you that the king has retired to his chambers, and that all foreigners must now leave the hall until morning. Upon interrogation the servant reveals that the man you gave money to had never been seen in the hall until today. Cursing your naivety, you storm out of the hall.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 775}; }
    };

    class Story235 : public Story::Base
    {
    public:
        Story235()
        {
            BookID = Book::Type::BOOK1;

            ID = 235;

            Text = "You invoke your spell, concentrating on hard on your destination. You can feel the portal links that connect to this location. Where will you travel?";

            Choices.clear();
            Choices.push_back(Choice::Base("The Palace of Unbraaki", {Book::Type::BOOK1, 484}));
            Choices.push_back(Choice::Base("(Crown and Tower) The city of Longport", {Book::Type::BOOK2, 321}));
            Choices.push_back(Choice::Base("(Pirates of the Splintered) Thalsia", {Book::Type::BOOK3, 108}));
            Choices.push_back(Choice::Base("(The Gilded Throne) Ozury", {Book::Type::BOOK4, 500}));
            Choices.push_back(Choice::Base("(The Savage Lands) The Hallowed Grove", {Book::Type::BOOK5, 45}));
            Choices.push_back(Choice::Base("(Drakehallow) Dagon", {Book::Type::BOOK6, 230}));
            Choices.push_back(Choice::Base("(Drakehallow) Animus Mast", {Book::Type::BOOK6, 600}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story236 : public Story::Base
    {
    public:
        Story236()
        {
            BookID = Book::Type::BOOK1;

            ID = 236;

            Text = "The cheers from the crowd resound across the arena as the final lion falls dead. You cannot help but stifle a tear at the death of the noble beasts, cruelly plucked from the wild to fight for the entertainment of the savage crowd.\n\nFeeling more bestial than even a lion, you are taken from the arena and disarmed again, eager for another bowl of water to slake your aching thirst. You come upon a sight of even greater cruelty. Poor Milagros is being shouted at and struck by an angry guard, enraged at receiving a splash of soup across his armour. The poor girl is on the ground, in his shadow, nursing a bruise on her cheek as the guard pulls out his whip to punish her further.";

            Choices.clear();
            Choices.push_back(Choice::Base("Dash to her defence", {Book::Type::BOOK1, 812}));
            Choices.push_back(Choice::Base("Keep well out of it", {Book::Type::BOOK1, 319}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_ALL(party, Equipment::Class::ARMOUR);
            Engine::LOSE_ALL(party, Equipment::Class::WEAPON);
        }
    };

    class Story237 : public Story::Base
    {
    public:
        Story237()
        {
            BookID = Book::Type::BOOK1;

            ID = 237;

            Image = "images/book1/spiders_attack.png";

            Text = "A terrible battle begins!\n\nNote: At the end of each combat round, after the spiders have made their attacks, each party member loses 1 Health from being bitten by the swarms of tiny spiders at their feet.";

            Bye = "You manage to survive.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Black Spider", Monster::Type::SPIDER_WITH_SWARM, 5, 5, 4, 7, 0),
                Monster::Base("Grey Spider", Monster::Type::SPIDER_WITH_SWARM, 4, 5, 4, 7, 0),
                Monster::Base("Green Spider", Monster::Type::SPIDER_WITH_SWARM, 5, 5, 4, 6, 0),
                Monster::Base("Red Spider", Monster::Type::SPIDER_WITH_SWARM, 4, 4, 4, 5, 0),
            };
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 197}; }
    };

    class Story238 : public Story::Base
    {
    public:
        Story238()
        {
            BookID = Book::Type::BOOK1;

            ID = 238;

            Text = "You must fend them off!";

            Bye = "You loot 20 silver coins from their bodies before moving on.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            CanFlee = false;

            Monsters = {
                Monster::Base("Lhasbreath Barbarians", 9, 5, 3, 16, 0),
                Monster::Base("Barbarian Leader", 4, 4, 4, 7, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 840}; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                Engine::GAIN_MONEY(party, 20);
            }
        }
    };

    class Story239 : public Story::Base
    {
    public:
        Team::Type previousTeam = Team::Type::NONE;

        Story239()
        {
            BookID = Book::Type::BOOK1;

            ID = 239;

            Text = "You'll not back down in front of a knave like Tommul. You must fight this battle alone.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            previousTeam = Team::Type::NONE;

            CanFlee = false;

            RoundLimit = 3;

            Monsters = {
                Monster::Base("Tommul", Monster::Type::TOMMUL, 3, 5, 4, 7, 0),
                Monster::Base("Brute", 2, 5, 3, 6, 0)};

            party.CurrentCharacter = Engine::FIND_SOLO(party);

            if (party.CurrentCharacter >= 0 && party.CurrentCharacter < party.Members.size())
            {
                previousTeam = party.Members[party.CurrentCharacter].Team;

                Engine::SET_TEAM(party.Members[party.CurrentCharacter]);

                Team = party.Members[party.CurrentCharacter].Team;
            }
            else if (party.LastSelected >= 0 && party.LastSelected < party.Members.size())
            {
                previousTeam = party.Members[party.LastSelected].Team;

                Engine::SET_TEAM(party.Members[party.LastSelected]);

                Team = party.Members[party.LastSelected].Team;
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 157}; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            auto target = -1;

            if (party.CurrentCharacter >= 0 && party.CurrentCharacter < party.Members.size())
            {
                target = party.CurrentCharacter;
            }
            else if (party.LastSelected >= 0 && party.LastSelected < party.Members.size())
            {
                target = party.LastSelected;
            }

            if (target >= 0 && target < party.Members.size())
            {
                party.Members[target].Team = previousTeam;
            }

            if (result == Engine::Combat::EXCEED_LIMIT)
            {
                temp_string = "The guards arrive and pull you away to your cell.";

                if (!Engine::HAS_MONSTER(Monsters, Monster::Type::TOMMUL))
                {
                    temp_string += " You have defeated Tommul.";

                    if (target >= 0 && target < party.Members.size())
                    {
                        auto result = Engine::ROLL_DICE(1);

                        if (result[0] > Engine::SCORE(party.Members[target], Attribute::Type::FIGHTING))
                        {
                            Engine::GAIN_SCORE(party.Members[target], Attribute::Type::FIGHTING, 1);

                            temp_string += "\n\n" + std::string(party.Members[target].Name) + "'s Fighting skill improved by 1.";
                        }
                    }
                }

                Bye = temp_string.c_str();
            }
        }
    };

    class Story240 : public Story::Base
    {
    public:
        Story240()
        {
            BookID = Book::Type::BOOK1;

            ID = 240;

            Text = "You are sailing just south of the Lhasbreath Jungle.";

            Choices.clear();
            Choices.push_back(Choice::Base("Sail west", {Book::Type::BOOK1, 21}));
            Choices.push_back(Choice::Base("Sail east", {Book::Type::BOOK1, 516}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story241 : public Story::Base
    {
    public:
        Story241()
        {
            BookID = Book::Type::BOOK1;

            ID = 241;

            Text = "You report your failure to retrieve the silver idol but speak instead of the revelation you had on the hilltop. \"Excellent!\" claps Honnu. \"Well, I think we can skip he bit about you bringing me the silver idol.\"\n\nYou flinch. \"But surely if I am to pass the test...?\" \"What do I need of a silver idol, anyway?\" snaps Honnu. \"You will not find your kensai spirit stamped on the base of an idol. Let the thief be pleased with herself. Your spiritual reward will last longer than her reward in coin, I promise you that.\"\n\nYou smile at the cunning old man. Clearly there is more to these tests than meets the eye.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 530}; }
    };

    class Story242 : public Story::Base
    {
    public:
        Story242()
        {
            BookID = Book::Type::BOOK1;

            ID = 242;

            Text = "After a short journey the tunnel leads to an old door, long since smashed to pieces. Beyond lie empty racks and armour stands, the chamber having been looted long ago, and a tunnel continues into the darkness beyond. Only one weapon remains, clutched in the hand of a dusty skeleton. The sword is carved of dark blue metal, with spidery runes engraved across its blade.\n\n\"I sense some enchantments, but also a strong curse,\" notes the Everchild. \"I would advise you to leave the blade and continue.\"";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to pick up the blade", {Book::Type::BOOK1, 131}, Choice::Type::CHOOSE_PARTY_MEMBER));
            Choices.push_back(Choice::Base("Leave the sword where it is and continue onwards", {Book::Type::BOOK1, 647}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story243 : public Story::Base
    {
    public:
        Story243()
        {
            BookID = Book::Type::BOOK1;

            ID = 243;

            Text = "You attempt to make your way to the historic harbour of the city, but the way is blocked with piles of rubble. Mordain, like many coastal cities in the Valley of Bones, was raised on a clifftop in order to deter coastal raiders. However, with the curtain wall of the city in ruins, it is impossible now to even approach the cliffside, let alone get a view down the cliffs. Perhaps it would be easier to get there by sea?";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 515}; }
    };

    class Story244 : public Story::Base
    {
    public:
        Story244()
        {
            BookID = Book::Type::BOOK1;

            ID = 244;

            Text = "Once again you leave the labyrinth that is the shadow land. Feeling weak and drained you head north towards civilisation.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 858}; }
    };

    class Story245 : public Story::Base
    {
    public:
        Story245()
        {
            BookID = Book::Type::BOOK1;

            ID = 245;

            Text = "The ogre seems stunned that such thin and puny creatures could possess so much money. \"Beats me why you wander 'round dungeons when you have that kind of dosh lining your pockets, but well'a'day.\"\n\nThe ogre rises to his feet. \"Skullcracker's the name,\" he says. \"I don't do cities, and I don't do boats. Other than that, I'll hang around with you for a bit.\n\nSkullcracker hands you a RUSTY KEY. \"This is the key to the room I'm supposed to be guarding,\" he says. \"Why don't we loot it, instead? Just go north, then east a bit, and you'll find it.\"\n\nYou gained the code A76.\n\nNote: Skullcracker can be treated just like an ordinary party member. He possesses the following skills:\n\nFighting: 8, Stealth: 1, Lore: 1, Survival: 3, Charisma: 2, Health: 12\n\nSkullcracker has a number of special rules as a party member:\n\n* He is lazy and he will not carry any equipment for you, even items that boost his skills. Skullcracker wields a CLUB, but it is nothing special. Should the party ever lose their equipment he will be able to replace the CLUB instantly with a bit of large bone, table leg or other such item. Effectively, Skullcracker is never disarmed.\n\n* He won't enter a city, which includes the following locations: Chalice, Clifftop, Cursus, Lhasbreath, Luutanesh or Saltdad. If you go to these places, Skullcracker will wait outside until you leave. Ogres aren't very popular in civilised places.\n\n* Even though he can't stay in human inns, Skullcracker will recover Health points at the same rate as your other party members when you pay to stay at an inn.\n\n* He won't board a ship. He had a queasy tummy on the orc boat and won't tolerate going aboard another. If you insist on going sailing, he will leave the party.\n\n* His contract with you ends when you travel to another land -- i.e. you begin to explore another book.\n\nAt this point, Skullcracker with take his leave and wander into the desert.";

            Bye = "Thanking your new companion, you return to the crossroads.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            if (!Engine::IN_PARTY(party, Character::Type::SKULLCRACKER))
            {
                party.Members.push_back(Character::SKULLCRACKER);
            }

            Engine::GET_CODES(party, {Codes::A(76)});

            Take = {Equipment::RUSTY_KEY};

            Limit = 1;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 310}; }
    };

    class Story246 : public Story::Base
    {
    public:
        Story246()
        {
            BookID = Book::Type::BOOK1;

            ID = 246;

            Text = "The king nods as he scans over the document with his eyes. \"A good reduction, and a modest bribe, this is well,\" he concedes. \"Though it pains me to sully our court with the intrigues of foreigners. Take your reward and never come in our presence again.\"\n\nThe Iron King waves to one of his councillors, who reluctantly places 350 silver pieces and an IRON GREATAXE* (Fighting +2) into your hands.";

            Bye = "Not wishing to tarry longer, you depart the chamber and return to the crossroads.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::LOSE_EQUIPMENT(party, {Equipment::Type::TITHE_REPORT});

            Take = {Equipment::IRON_GREATAXE2};

            Limit = 1;

            Engine::GAIN_MONEY(party, 350);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 75}; }
    };

    class Story247 : public Story::Base
    {
    public:
        Story247()
        {
            BookID = Book::Type::BOOK1;

            ID = 247;

            Text = "This small room contains a very large ogre. In fact, there is actually bedding down for two of these enormous creatures, but currently only one is here, chewing on the leg bone of a sand lizard. He eyes you lazily.";

            Choices.clear();
            Choices.push_back(Choice::Base("Attempt to speak with the ogre", {Book::Type::BOOK1, 693}));
            Choices.push_back(Choice::Base("Rush to an attack", {Book::Type::BOOK1, 117}));
            Choices.push_back(Choice::Base("Politely excuse yourself and return to the crossroads", {Book::Type::BOOK1, 310}));

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::A(76)}))
            {
                return {Book::Type::BOOK1, 36};
            }
            else
            {
                return {Book::Type::NONE, -1};
            }
        }
    };

    class Story248 : public Story::Base
    {
    public:
        Story248()
        {
            BookID = Book::Type::BOOK1;

            ID = 248;

            Text = "The dragonyak bull bows to you as you speak in the ancient tongue of beasts. \"You are an animal friend,\" he intones. \"Our legends say that it has been an eon since any dragonyak was so addressed. You honour us.\" The dragonyak unleashes a bellowing roar, which is repeated by his herd. One dragonyak immerses herself in the water of the oasis and emerges sometime later with a CLOAK, sodden with water, but miraculously unharmed by the passage of years. The bull insists you take the cloak, for he is aware that man-apes such as yourself favour them.\n\nYou take THE RUGGED CLOAK (Survival +2) so as not to offend the mighty bull and make your way onwards.\n\nYou gained the code A38.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::RUGGED_CLOAK2};

            Limit = 1;

            Engine::GET_CODES(party, {Codes::A(38)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 115}; }
    };

    class Story249 : public Story::Base
    {
    public:
        Story249()
        {
            BookID = Book::Type::BOOK1;

            ID = 249;

            Text = "You seek out the father of Garon the Bloody Mouthed. He is still sitting sadly upon his doorstep when you return, bearing his son's body in your arms. The father gives a great howl and clasps his son's body.\n\n\"It is as I feared,\" he weeps. \"I thank you for returning his body so it may be buried with honour. I was to present my son with his grandfather's magical amulet upon his successful return -- but now I have no sons left to inherit. I wish you to have it. It is what Garon would have wanted.\"\n\nThe old man presents you with an AMULET OF HEALTH (Health +1). It would be churlish to refuse it. You leave the old man to his sorrows.\n\nYou gain the code A86.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::AMULET_OF_HEALTH1};

            Limit = 1;

            Engine::GET_CODES(party, {Codes::A(86)});
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 775}; }
    };

    class Story250 : public Story::Base
    {
    public:
        Story250()
        {
            BookID = Book::Type::BOOK1;

            ID = 250;

            Text = "It's not your city. You shoulder the loot bag and slide back down the rope. Once you are a few neighbourhoods away you stroll along as if nothing had happened.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::HANDSOME_BROOCH1};

            Limit = 1;

            Engine::GAIN_MONEY(party, 200);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story251 : public Story::Base
    {
    public:
        Story251()
        {
            BookID = Book::Type::BOOK1;

            ID = 251;

            Text = "You gained the code A61.\n\nWith the beetles destroyed you brush them away from the mystic circle and examine it. It appears to be some kind of health boosting magic circle with a small indentation in the middle, not unlike an ink well. Indeed, the instructions for its use have been usefully painted upon the walls of the chamber. It appears that if you pour some QUICKSILVER into the central well the circle will activate and imbue you with power.";

            Choices.clear();
            Choices.push_back(Choice::Base("You have some QUICKSILVER and want to try it out", {Book::Type::BOOK1, 26}, {Equipment::QUICKSILVER}));
            Choices.push_back(Choice::Base("There is nothing more to do here: Return to the crossroads", {Book::Type::BOOK1, 566}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::A(61)});
        }
    };

    class Story252 : public Story::Base
    {
    public:
        Story252()
        {
            BookID = Book::Type::BOOK1;

            ID = 252;

            Text = "You are greeted by Honnu and the other monks, and go into meditation with them for a few days. There is nothing more Honnu can teach a fully trained kensai, so after a few days of peace you make your way onwards.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 450}; }
    };

    class Story253 : public Story::Base
    {
    public:
        Story253()
        {
            BookID = Book::Type::BOOK1;

            ID = 253;

            Text = "Although you know that the nomads make their home in the Withered Steppes, their territory covers hundreds of square miles of desert. Finding them will not be easy.";

            Choices.clear();
            Choices.push_back(Choice::Base("Locate the nomads (Team check: Survival 5+, Successes: 3)", {Book::Type::BOOK1, 473}, {Book::Type::BOOK1, 422}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 5, 3));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story254 : public Story::Base
    {
    public:
        Story254()
        {
            BookID = Book::Type::BOOK1;

            ID = 254;

            Text = "You cannot ward the assassin away. He ducks under your blade and stabs the Everchild in the leg. She squeals and kicks him away, and you impale the knave through the back. Casting his body aside you rush to the Everchild. \"It's not bad!\" she assures you, gripping her bleeding thigh. A quick examination of the assassin's dagger tells you otherwise... it has been poisoned.\n\nSoon she in a critical condition, unable to stand, her flesh looking as grey as stone. In her fevered delirium she insists on leading the troops into battle, but this is clearly impossible. You command her to concentrate on getting better, and to focus all her healing energies on herself.\n\nWord soon gets around about the Everchild's condition. Each army unit in the Saltdad barracks must lose 1 point of Morale. You dread the coming of the dawn, but you realise you cannot escape now. You must face what comes with as much courage as you can muster.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::GET_CODES(party, {Codes::Type::EVERCHILD_POISONED});

            Engine::GAIN_MORALE(party, Location::Type::SALTDAD, -1);
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 162}; }
    };

    class Story255 : public Story::Base
    {
    public:
        Story255()
        {
            BookID = Book::Type::BOOK1;

            ID = 255;

            Image = "images/book1/roughly_dressed_man.png";

            Text = "You clamber up to the open window and peek inside. A roughly dressed man is standing in a finely appointed room, hurriedly searching through cupboards and trunks, casting clothes and knick-knacks over his shoulders. A large bag is thrown on the bed, glittering treasures peeking out of the top.";

            Choices.clear();
            Choices.push_back(Choice::Base("Try and sneak up on the man", {Book::Type::BOOK1, 91}));
            Choices.push_back(Choice::Base("Demand the man tell you what he is doing", {Book::Type::BOOK1, 724}));
            Choices.push_back(Choice::Base("Quickly depart before the man sees you", {Book::Type::BOOK1, 450}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story256 : public Story::Base
    {
    public:
        Story256()
        {
            BookID = Book::Type::BOOK1;

            ID = 256;

            Text = "You have entered a small, cramped library, with bookstacks lining the walls and standing in rows before you. Several books are on the floor, apparently hacked apart with swords. Besides this crude vandalism, the books seem in good condition.";

            Choices.clear();
            Choices.push_back(Choice::Base("Examine some of the tomes", {Book::Type::BOOK1, 558}));
            Choices.push_back(Choice::Base("Shut the door and return to the alchemical chamber", {Book::Type::BOOK1, 69}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story257 : public Story::Base
    {
    public:
        Story257()
        {
            BookID = Book::Type::BOOK1;

            ID = 257;

            Text = "Skittering across the desert sands at unbelievable speed come a pair of giant sand lizards. These beasts alternate between lazily sitting in the sun or hunting prey with energetic intensity. These ones are obviously hungry, and scuttle forwards, their mouths as wide as crocodiles.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take.clear();

            Limit = 0;

            CanFlee = false;

            Monsters = {
                Monster::Base("Sand Lizard", 4, 3, 4, 7, 0),
                Monster::Base("Sand Lizard", 4, 3, 4, 9, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 840}; }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                Take = {Equipment::LIZARD_HIDE};

                Limit = 1;
            }
        }
    };

    // TODO: ensure that the bandits get at least 3 of the treasures
    class Story258 : public Story::Base
    {
    public:
        Story258()
        {
            BookID = Book::Type::BOOK1;

            ID = 258;

            Text = "There is a strange sound in the room to the south, and you instinctively draw your weapons. Fortunately, it is just Brekken and the rest of his thieves. You wondered when they would get here!\n\n\"An old friend of ours in Chalice offered us an easy route into the palace,\" explains Brekken. \"We thought it might be easier than sneaking past all those guards.\"\n\n\"Yes... but how did you know when to arrive,\" you ask, mystified.\n\n\"Well... he's quite knowledgeable, is our friend,\" winks Brekken. \"Shall we get on with it?\"\n\nYou nod, and soon Brekken and his gang are surrounding the vault, their hands pouring over it, scratching their beards and muttering to each other. You catch Brekken producing a scroll, which he reads from. Suddenly the door begins to swing open.\n\n\"I suppose your friend gave you that as well!\" you say, indicating the magic scroll which even now is disintegrating into ash.\n\n\"Well -- he moves in mysterious ways, doesn't he?\" grins Brekken.\n\nNote: You may now loot the vault. However, you must give three items from the treasure list to Brekken and his gang. The silver coins count as one item.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 416}; }
    };

    class Story259 : public Story::Base
    {
    public:
        Story259()
        {
            BookID = Book::Type::BOOK1;

            ID = 259;

            Text = "After days of monotony, you finally come upon an arch in the wall. It is the Shaded Gate, though how you knew its name is a mystery to you. Beyond the arch the desert continues... except the land is a shadowy haze, a dark reflection of the real world.";

            Choices.clear();
            Choices.push_back(Choice::Base("Will you enter the Shaded Gate", {Book::Type::BOOK1, 410}));
            Choices.push_back(Choice::Base("Break off from the wall and head north", {Book::Type::BOOK1, 858}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story260 : public Story::Base
    {
    public:
        Story260()
        {
            BookID = Book::Type::BOOK1;

            ID = 260;

            Text = "The flickering torch deceives your eyes, shadows from your companions making it look like your foot is in a safe place. There is an audible crunch as you step upon a juicy egg. Tiny spiderlings emerge in their hundreds. Suddenly the giant spiders move as one, descending upon your companions and the Everchild's followers. Eggs crack open as careless feet release more baby spiders.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 237}; }
    };

    class Story261 : public Story::Base
    {
    public:
        Story261()
        {
            BookID = Book::Type::BOOK1;

            ID = 261;

            Text = "\"I'm glad I found you. I must tell you some sad news,\" you say gravely. \"I met one of your kind in the desert. An apprentice of Unbraaki. Alas, he perished. He did not return to you because he feared Unbraaki would harm you if he found him hiding here. I am sorry to bring you such sad news.\"\n\nThe chattering in the treetops instantly stops, to be replaced by mournful howls. \"Curse you for bringing such dreadful tidings!\" cries one of the monkeys. \"For you speak of my own son! Oh! Misery! Misery!\"\n\nThe monkeys depart solemnly above you, their howls rending your ears. Emlyn looks on in wonder as the monkeys vanish. \"What did you say to them?\" she asks, astonished.\n\n\"I haven't the heart to tell you,\" you say sadly, wiping a tear from your eye.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 740}; }
    };

    class Story262 : public Story::Base
    {
    public:
        Story262()
        {
            BookID = Book::Type::BOOK1;

            ID = 262;

            Text = "Saint Elias is considered the holiest man in the world, a spokesman from the celestial court who has lived on the world for two thousand years. He is a silver elf, a faerie given human form, who rejected his fey heritage to study the majesty of the gods. He resides in a mighty tower called Tier Mundus, the last of the great towers of the elder time (all the rest were destroyed by Abraxas during the desolation). It is believed that he is personally responsible for the banishment of Abraxas into the centre of the earth at the climax of the desolation, and that he is therefore not merely the vicar of the gods, but also a powerful sorcerer in his own right. He still lives today, and grants blessings to pilgrims. Sometimes he advises the world's mightiest rulers, if they will hear his wisdom, but he has always remained neutral in all political affairs.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 680}; }
    };

    class Story263 : public Story::Base
    {
    public:
        Story263()
        {
            BookID = Book::Type::BOOK1;

            Location = Location::Type::LUUTANESH;

            ID = 263;

            Text = "You are in the Everchild's hideout in Luutanesh. Around you generals and captains of her armies swirl in great preparation for the forthcoming battle.";

            Choices.clear();
            Choices.push_back(Choice::Base("Take the opportunity to rest at the inn", {Book::Type::BOOK1, 66}));
            Choices.push_back(Choice::Base("Speak with the Everchild", {Book::Type::BOOK1, 898}));
            Choices.push_back(Choice::Base("Order the attack on Saltdad to begin", {Book::Type::BOOK1, 376}));
            Choices.push_back(Choice::Base("Leave Luutanesh", {Book::Type::BOOK1, 194}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story264 : public Story::Base
    {
    public:
        Story264()
        {
            BookID = Book::Type::BOOK1;

            ID = 264;

            Text = "This will be a fraught battle.\n\nNote: The skeleton archers do not attack in hand-to-hand combat, but will try and shoot party members if they are not kept busy. If, at the end of the combat turn, the skeleton archers have not taken any damage, they unleash their arrows. Choose two party members to lose 2 Health points -- armour cannot reduce this damage.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take.clear();

            Limit = 0;

            CanFlee = false;

            Monsters = {
                Monster::Base("Skeleton Warriros", 9, 4, 4, 12, 0),
                Monster::Base("Skeleton Archers", Monster::Type::SKELETON_ARCHERS, 0, 0, 4, 13, 0)};
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 555}; }
    };

    class Story265 : public Story::Base
    {
    public:
        Story265()
        {
            BookID = Book::Type::BOOK1;

            ID = 265;

            Text = "You are leaving the Palace of Unbraaki. Where will you venture now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Go north, towards Clifftop", {Book::Type::BOOK1, 725}));
            Choices.push_back(Choice::Base("Go east, to the Tumblestones", {Book::Type::BOOK1, 137}));
            Choices.push_back(Choice::Base("Go southeast, to Lhasbreath", {Book::Type::BOOK1, 775}));
            Choices.push_back(Choice::Base("Head south, into the jungle", {Book::Type::BOOK1, 370}));

            Controls = Story::Controls::STANDARD;
        }
    };

    class Story266 : public Story::Base
    {
    public:
        Story266()
        {
            BookID = Book::Type::BOOK1;

            ID = 266;

            Text = "You have only just begun your negotiations when one of the thieves shouts out, \"They're agents of the Iron King! Slay them before they get away!\" The thieves draw their weapons, the Everchild rising with her allies to lend you aid.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 889}; }
    };

    class Story267 : public Story::Base
    {
    public:
        Story267()
        {
            BookID = Book::Type::BOOK1;

            ID = 267;

            Text = "You cannot help but shed a tear as your bloodied companion sprawls to the ground, dead by your hand. They were too dangerous to be left alive, but that doesn't mean your actions will not haunt your dreams for weeks to come. You may take any possessions from the dead party member that you wish. Kicking away the black book, you exit the chamber.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 821}; }
    };

    class Story268 : public Story::Base
    {
    public:
        Story268()
        {
            BookID = Book::Type::BOOK1;

            ID = 268;

            Text = "Through the desert haze, the legions of the Everchild go on the march. Dust rises on the Northroad as the combined forces of the valley descend upon the defiant city of Cursus. The Everchild calls a halt on a raised perch of land where the city and its harbour can be clearly seen. In the blood-red light of the setting sun you can see that the harbour of Cursus is packed with cogs, hulks and carracks of many nations and heraldry. The patriarch has emptied his treasury to gain followers for his last stand. Spies loyal to the Everchild report that the high priest of Cursus might have access to as many as six thousand fighting men.\n\n\"They do not have space for such a large deployment in the field,\" observes Che Long. \"Or, if they did, such numbers would be unmanageable.\"\n\n\"Are those knights? Has the patriarch brought foreign knights here?\" asks the Everchild, peering into the gloom.\n\n\"They will not be able to wear their heavy armour in the desert,\" says Che Long. \"Unlike the Bronzeguard, their armour is not enchanted and does not keep them cool in the sun.\"\n\nLady Ayleta approaches the Everchild and bows. \"My lady, your agents are assembled in the command tent.\"\n\nThe Everchild bids you follow her as she approaches the tent. \"I have many agents who follow me now, but none are as experienced as you. I shall give you priority over which missions you wish to undertake. All are hazardous, so do not stretch yourselves too thinly.\"";
            
            Bye = "Nodding, you enter the tent with the queen.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 124}; }
    };

    class Story269 : public Story::Base
    {
    public:
        Story269()
        {
            BookID = Book::Type::BOOK1;

            ID = 269;

            Text = "As you round the corner of the corridor you come face to face with a hideous female ogre. She hefts her club to attack, when she suddenly sees Skullcracker lollop into view.\n\n\"Oi! What's going on 'ere then?\" she asks, confused.\n\n\"I've ganged up with this lot,\" growls Skullcracker. \"Can't be bothered to work for the orcs anymore.\"\n\n\"What am I supposed to do, then?\" snaps the female ogre.\n\n\"What 'yer like,\" shrugs Skullcracker. \"It would be impertinent of me to suggest a course of action that influences your sense of agency one way or the other. And you smell.\"\n\n\"Git,\" snaps the lady ogre. \"I suppose I'll sod off myself then.\"\n\n\"Good idea,\" grumbles Skullcracker.\n\nYou are almost teary at this sad parting, but eventually Daisy the ogre shambles away.";
            
            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 165}; }
    };

    auto story001 = Story001();
    auto story002 = Story002();
    auto story003 = Story003();
    auto story004 = Story004();
    auto story005 = Story005();
    auto story006 = Story006();
    auto story007 = Story007();
    auto story008 = Story008();
    auto story009 = Story009();
    auto story010 = Story010();
    auto story011 = Story011();
    auto story012 = Story012();
    auto story013 = Story013();
    auto story014 = Story014();
    auto story015 = Story015();
    auto story016 = Story016();
    auto story017 = Story017();
    auto story018 = Story018();
    auto event018 = Event018();
    auto story019 = Story019();
    auto story020 = Story020();
    auto story021 = Story021();
    auto story022 = Story022();
    auto story023 = Story023();
    auto story024 = Story024();
    auto story025 = Story025();
    auto story026 = Story026();
    auto event027 = Event027();
    auto story027 = Story027();
    auto story028 = Story028();
    auto event028 = Event028();
    auto story029 = Story029();
    auto story030 = Story030();
    auto story031 = Story031();
    auto story032 = Story032();
    auto story033 = Story033();
    auto story034 = Story034();
    auto story035 = Story035();
    auto story036 = Story036();
    auto story037 = Story037();
    auto story038 = Story038();
    auto story039 = Story039();
    auto story040 = Story040();
    auto story041 = Story041();
    auto story042 = Story042();
    auto story043 = Story043();
    auto story044 = Story044();
    auto event044 = Event044();
    auto story045 = Story045();
    auto story046 = Story046();
    auto story047 = Story047();
    auto story048 = Story048();
    auto story049 = Story049();
    auto story050 = Story050();
    auto story051 = Story051();
    auto story052 = Story052();
    auto story053 = Story053();
    auto story054 = Story054();
    auto story055 = Story055();
    auto story056 = Story056();
    auto story057 = Story057();
    auto story058 = Story058();
    auto story059 = Story059();
    auto story060 = Story060();
    auto story061 = Story061();
    auto story062 = Story062();
    auto story063 = Story063();
    auto story064 = Story064();
    auto story065 = Story065();
    auto story066 = Story066();
    auto story067 = Story067();
    auto event067 = Event067();
    auto story068 = Story068();
    auto story069 = Story069();
    auto story070 = Story070();
    auto story071 = Story071();
    auto story072 = Story072();
    auto story073 = Story073();
    auto event073 = Event073();
    auto story074 = Story074();
    auto story075 = Story075();
    auto story076 = Story076();
    auto event076 = Event076();
    auto story077 = Story077();
    auto story078 = Story078();
    auto event078 = Event078();
    auto story079 = Story079();
    auto story080 = Story080();
    auto story081 = Story081();
    auto story082 = Story082();
    auto story083 = Story083();
    auto story084 = Story084();
    auto story085 = Story085();
    auto story086 = Story086();
    auto story087 = Story087();
    auto e087_001 = E087_001();
    auto e087_002 = E087_002();
    auto e087_003 = E087_003();
    auto story088 = Story088();
    auto story089 = Story089();
    auto event089 = Event089();
    auto story090 = Story090();
    auto story091 = Story091();
    auto story092 = Story092();
    auto story093 = Story093();
    auto story094 = Story094();
    auto story095 = Story095();
    auto story096 = Story096();
    auto story097 = Story097();
    auto story098 = Story098();
    auto event098 = Event098();
    auto story099 = Story099();
    auto story100 = Story100();
    auto story101 = Story101();
    auto story102 = Story102();
    auto event102 = Event102();
    auto story103 = Story103();
    auto story104 = Story104();
    auto story105 = Story105();
    auto story106 = Story106();
    auto story107 = Story107();
    auto story108 = Story108();
    auto story109 = Story109();
    auto story110 = Story110();
    auto story111 = Story111();
    auto story112 = Story112();
    auto story113 = Story113();
    auto story114 = Story114();
    auto story115 = Story115();
    auto e115_001 = E115_001();
    auto e115_002 = E115_002();
    auto story116 = Story116();
    auto story117 = Story117();
    auto story118 = Story118();
    auto story119 = Story119();
    auto story120 = Story120();
    auto story121 = Story121();
    auto story122 = Story122();
    auto story123 = Story123();
    auto story124 = Story124();
    auto story125 = Story125();
    auto story126 = Story126();
    auto story127 = Story127();
    auto story128 = Story128();
    auto e128_001 = E128_001();
    auto e128_002 = E128_002();
    auto story129 = Story129();
    auto story130 = Story130();
    auto story131 = Story131();
    auto story132 = Story132();
    auto story133 = Story133();
    auto story134 = Story134();
    auto story135 = Story135();
    auto story136 = Story136();
    auto story137 = Story137();
    auto story138 = Story138();
    auto story139 = Story139();
    auto story140 = Story140();
    auto story141 = Story141();
    auto story142 = Story142();
    auto story143 = Story143();
    auto story144 = Story144();
    auto story145 = Story145();
    auto story146 = Story146();
    auto story147 = Story147();
    auto story148 = Story148();
    auto story149 = Story149();
    auto story150 = Story150();
    auto story151 = Story151();
    auto story152 = Story152();
    auto story153 = Story153();
    auto story154 = Story154();
    auto story155 = Story155();
    auto story156 = Story156();
    auto story157 = Story157();
    auto story158 = Story158();
    auto story159 = Story159();
    auto story160 = Story160();
    auto event160 = Event160();
    auto story161 = Story161();
    auto story162 = Story162();
    auto story163 = Story163();
    auto story164 = Story164();
    auto story165 = Story165();
    auto story166 = Story166();
    auto story167 = Story167();
    auto story168 = Story168();
    auto story169 = Story169();
    auto story170 = Story170();
    auto story171 = Story171();
    auto story172 = Story172();
    auto story173 = Story173();
    auto story174 = Story174();
    auto story175 = Story175();
    auto story176 = Story176();
    auto story177 = Story177();
    auto story178 = Story178();
    auto story179 = Story179();
    auto story180 = Story180();
    auto story181 = Story181();
    auto story182 = Story182();
    auto story183 = Story183();
    auto event183 = Event183();
    auto story184 = Story184();
    auto story185 = Story185();
    auto story186 = Story186();
    auto event186 = Event186();
    auto story187 = Story187();
    auto story188 = Story188();
    auto event188 = Event188();
    auto story189 = Story189();
    auto story190 = Story190();
    auto story191 = Story191();
    auto story192 = Story192();
    auto story193 = Story193();
    auto story194 = Story194();
    auto story195 = Story195();
    auto story196 = Story196();
    auto story197 = Story197();
    auto story198 = Story198();
    auto story199 = Story199();
    auto story200 = Story200();
    auto story201 = Story201();
    auto story202 = Story202();
    auto event202 = Event202();
    auto story203 = Story203();
    auto story204 = Story204();
    auto story205 = Story205();
    auto story206 = Story206();
    auto story207 = Story207();
    auto event207 = Event207();
    auto story208 = Story208();
    auto story209 = Story209();
    auto story210 = Story210();
    auto story211 = Story211();
    auto story212 = Story212();
    auto story213 = Story213();
    auto story214 = Story214();
    auto story215 = Story215();
    auto story216 = Story216();
    auto story217 = Story217();
    auto story218 = Story218();
    auto story219 = Story219();
    auto story220 = Story220();
    auto story221 = Story221();
    auto story222 = Story222();
    auto story223 = Story223();
    auto event223 = Event223();
    auto story224 = Story224();
    auto event224 = Event224();
    auto story225 = Story225();
    auto story226 = Story226();
    auto story227 = Story227();
    auto story228 = Story228();
    auto story229 = Story229();
    auto story230 = Story230();
    auto story231 = Story231();
    auto story232 = Story232();
    auto story233 = Story233();
    auto story234 = Story234();
    auto story235 = Story235();
    auto story236 = Story236();
    auto story237 = Story237();
    auto story238 = Story238();
    auto story239 = Story239();
    auto story240 = Story240();
    auto story241 = Story241();
    auto story242 = Story242();
    auto story243 = Story243();
    auto story244 = Story244();
    auto story245 = Story245();
    auto story246 = Story246();
    auto story247 = Story247();
    auto story248 = Story248();
    auto story249 = Story249();
    auto story250 = Story250();
    auto story251 = Story251();
    auto story252 = Story252();
    auto story253 = Story253();
    auto story254 = Story254();
    auto story255 = Story255();
    auto story256 = Story256();
    auto story257 = Story257();
    auto story258 = Story258();
    auto story259 = Story259();
    auto story260 = Story260();
    auto story261 = Story261();
    auto story262 = Story262();
    auto story263 = Story263();
    auto story264 = Story264();
    auto story265 = Story265();
    auto story266 = Story266();
    auto story267 = Story267();
    auto story268 = Story268();
    auto story269 = Story269();

    void InitializeStories()
    {
        Book1::Stories = {
            &event018, &event027, &event028, &event044, &event067, &event073, &event076, &event078, &e087_001, &e087_002,
            &e087_003, &event089, &event098, &event102, &e115_001, &e115_002, &e128_001, &e128_002, &event160, &event183,
            &event186, &event188, &event202, &event207, &event223, &event224,
            &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009,
            &story010, &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019,
            &story020, &story021, &story022, &story023, &story024, &story025, &story026, &story027, &story028, &story029,
            &story030, &story031, &story032, &story033, &story034, &story035, &story036, &story037, &story038, &story039,
            &story040, &story041, &story042, &story043, &story044, &story045, &story046, &story047, &story048, &story049,
            &story050, &story051, &story052, &story053, &story054, &story055, &story056, &story057, &story058, &story059,
            &story060, &story061, &story062, &story063, &story064, &story065, &story066, &story067, &story068, &story069,
            &story070, &story071, &story072, &story073, &story074, &story075, &story076, &story077, &story078, &story079,
            &story080, &story081, &story082, &story083, &story084, &story085, &story086, &story087, &story088, &story089,
            &story090, &story091, &story092, &story093, &story094, &story095, &story096, &story097, &story098, &story099,
            &story100, &story101, &story102, &story103, &story104, &story105, &story106, &story107, &story108, &story109,
            &story110, &story111, &story112, &story113, &story114, &story115, &story116, &story117, &story118, &story119,
            &story120, &story121, &story122, &story123, &story124, &story125, &story126, &story127, &story128, &story129,
            &story130, &story131, &story132, &story133, &story134, &story135, &story136, &story137, &story138, &story139,
            &story140, &story141, &story142, &story143, &story144, &story145, &story146, &story147, &story148, &story149,
            &story150, &story151, &story152, &story153, &story154, &story155, &story156, &story157, &story158, &story159,
            &story160, &story161, &story162, &story163, &story164, &story165, &story166, &story167, &story168, &story169,
            &story170, &story171, &story172, &story173, &story174, &story175, &story176, &story177, &story178, &story179,
            &story180, &story181, &story182, &story183, &story184, &story185, &story186, &story187, &story188, &story189,
            &story190, &story191, &story192, &story193, &story194, &story195, &story196, &story197, &story198, &story199,
            &story200, &story201, &story202, &story203, &story204, &story205, &story206, &story207, &story208, &story209,
            &story210, &story211, &story212, &story213, &story214, &story215, &story216, &story217, &story218, &story219,
            &story220, &story221, &story222, &story223, &story224, &story225, &story226, &story227, &story228, &story229,
            &story230, &story231, &story232, &story233, &story234, &story235, &story236, &story237, &story238, &story239,
            &story240, &story241, &story242, &story243, &story244, &story245, &story246, &story247, &story248, &story249,
            &story250, &story251, &story252, &story253, &story254, &story255, &story256, &story257, &story258, &story259,
            &story260, &story261, &story262, &story263, &story264, &story265, &story266, &story267, &story268, &story269};
    }
}
#endif
