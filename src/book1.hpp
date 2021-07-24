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

        Engine::Destination Continue(Party::Base &party)
        {
            return destination;
        }

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
                    Choices.push_back(Choice::Base("Seduce the Guards (Team: Charisma 3+, Successes: 2)", {Book::Type::BOOK1, 364}, {Book::Type::BOOK1, 588}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 3, 2));
                }
                else
                {
                    Choices.push_back(Choice::Base("Seduce the Guards (Team: Charisma 3+, Successes: 2)", {Book::Type::BOOK1, 364}, {Book::Type::BOOK1, 588}, Team::Type::DISTRACTION, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 3, 2));
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
            if (Engine::VERIFY_EQUIPMENT(party.Party, {Equipment::Type::TITHE_REPORT}))
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
            Engine::GAIN_HEALTH(party.Party, -4);
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

            if (Engine::VERIFY_EQUIPMENT(party.Party, {Equipment::Type::PRYBAR}))
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
                temp_string = std::string(party.Party[selection[0]].Name) + " LOSES 1 Health Point.";

                Bye = temp_string.c_str();

                Engine::GAIN_HEALTH(party.Party[selection[0]], -1);
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

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::CLIFFTOP);
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

            Text = "You are steering along the jungle peninsula, across waters known as Cape Porta. It is a famous lair of pirates and other ne'er-do-wells.";

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::CAPE_PORTA);

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

            Engine::SET_LOCATION(party, Location::Type::SALTDAD);
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
            Choices.push_back(Choice::Base("Bribe the Mercenaries (Team: Charisma 6+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 6, 7));

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
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team: Charisma 5+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 5, 7));
            }
            else if (Engine::VERIFY_CODES(party, {Codes::Type::BRIBE_GOLD_BULLION2}))
            {
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team: Charisma 4+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 4, 7));
            }
            else if (Engine::VERIFY_CODES(party, {Codes::Type::BRIBE_GOLD_BULLION2}))
            {
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team: Charisma 2+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 2, 7));
            }
            else
            {
                Choices.push_back(Choice::Base("Bribe the Mercenaries (Team: Charisma 6+, Successes: 7)", {Book::Type::BOOK1, 122}, {Book::Type::BOOK1, 832}, Team::Type::MERCENARY, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 6, 7));
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

            if (!Engine::HAS_STATUS(party.Party, Character::Status::RITUAL_SCARRING))
            {
                PreText += "They bombard you with questions about the faith, to determine your worthiness.";

                Choices.push_back(Choice::Base("Answer Cursite zealots (Team: Lore 4+, Successes: 4)", {Book::Type::BOOK1, 657}, {Book::Type::BOOK1, -28}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::LORE}, 4, 4));
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

            Text = "The harbour of Cursus is long and shallow. The jetties reach far out beyond the coast, each wide enough for a pair of dragonyaks to drag the cargoes of ships to the warehouses on the beach.\n\nYou may buy a ship here, if you have enough money. The largest vessel for sale is the CURSITE WAR GALLEY, a coastal-hugging warship which carries little cargo, but is the only vessel that can even hope to compete with the more advanced vessels of Royce and Drakehallow. A smaller, but more efficient, single-masted COG is also available. The WAR GALLEY will cost 1500 silver coins, and the COG 800 silver coins. When you buy a ship note that it is currently docked in Cursus harbour.\n\nYou can repair any ship you have in Cursus harbour. Each Health point you restore costs 50 silver coins. You can restore your ship up to its starting Health value. You can sell a COG here for 250 silver coins, a CURSITE WAR GALLEY for 500 silver coins and a CARRACK for 1500 silver coins. You cannot sell other kinds of ship here.\n\nYou can buy cargo for your ship here, as well. These goods are too large for your characters to carry, but they can be loaded onto any ship you have in Cursus harbour. You cannot buy more cargo units than you can store in your ship (for instance, a Cog can carry 2 Cargo Units). You can also sell any goods you are carrying in your ships for the same price as well:\n\nSALT: 600 silver coins\nCROPS: 500 silver coins\nSTEEL: 3000 silver coins\nSPICES: 3300 silver coins\nWINE: 2100 silver coins\nSLAVES: 1000 silver coins";

            Choices.clear();
            Choices.push_back(Choice::Base("Return to the city centre", {Book::Type::BOOK1, 340}));
            Choices.push_back(Choice::Base("Set sail on one of your ships", {Book::Type::BOOK1, 499}));

            Controls = Story::Controls::HARBOUR;
        }

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::CURSUS);

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
            Choices.push_back(Choice::Base("Order the caravan guards (Team: Charisma 5+, Successes: 3)", {Book::Type::BOOK1, 542}, {Book::Type::BOOK1, 451}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 5, 3));

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
                if (!Engine::VERIFY_EQUIPMENT(party.Party, {Equipment::Type::SCROLL_OF_RAGE}))
                {
                    PreText += "\n\nYou are surprised to see the SCROLL OF RAGE back on the shelf, in the same place you found it last time. It seems that the scroll teleports back here each time it is used.";
                }
                else
                {
                    PreText += "\n\nYou find nothing else of interest here.";
                }
            }

            if (!Engine::VERIFY_EQUIPMENT(party.Party, {Equipment::Type::SCROLL_OF_RAGE}))
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
            Engine::GAIN_HEALTH(party.Party, Team::Type::DISTRACTION, -1);

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
        std::string choice_string = "";

        Story057()
        {
            BookID = Book::Type::BOOK1;

            ID = 57;

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            PreText = "The goblin's grasp of your language is rather poor, but you endeavour to convince them that you are the enemies of their enslavers.";

            auto difficulty = 5;

            if (Engine::VERIFY_CODES(party, {Codes::Type::SPEAK_BANDO}))
            {
                PreText += "\n\nHowever, you speak Bando. Oddly, the goblins are quite familiar with this language.";

                difficulty = 4;
            }

            Choices.clear();

            choice_string = "Parlay with the goblin (Team: Charisma " + std::to_string(difficulty) + "+, Successes: 3)";

            Choices.push_back(Choice::Base(choice_string.c_str(), {Book::Type::BOOK1, 828}, {Book::Type::BOOK1, 389}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, difficulty, 3));

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
            Choices.push_back(Choice::Base("Hide from the guards (Team: Stealth 4+, Successes: 4)", {Book::Type::BOOK1, 698}, {Book::Type::BOOK1, 758}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::STEALTH}, 4, 4));

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

        Engine::Destination Continue(Party::Base &party)
        {
            return destination;
        }

        void AfterCombat(Party::Base &party, Engine::Combat result)
        {
            if (result == Engine::Combat::VICTORY)
            {
                destination = {Book::Type::BOOK1, 391};
            }
            else if (Engine::COUNT(party.Party) > 0)
            {
                for (auto i = 0; i < party.Party.size(); i++)
                {
                    if (party.Party[i].Team == Team::Type::CHASE)
                    {
                        party.Party[i].Equipment.clear();
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
            if (party.LastSelected >= 0 && party.LastSelected < party.Party.size())
            {
                if (party.Party[party.LastSelected].Type == Character::Type::BRASH)
                {
                    return {Book::Type::BOOK1, 4};
                }
                else if (party.Party[party.LastSelected].Type == Character::Type::TASHA)
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

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::SALTDAD);
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
            Choices.push_back(Choice::Base("Save your teammate (Team: Survival 4+, Successes: 5)", {Book::Type::BOOK1, 586}, {Book::Type::BOOK1, 646}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 5));

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
            Choices.push_back(Choice::Base("Navigate the spider's nest (Team: Survival 4+, Successes: 5)", {Book::Type::BOOK1, 461}, {Book::Type::BOOK1, 260}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 5));

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

            Text = "Longport Bay is a county in the kingdom of Royce. It is famed for its steelwork, good agricultural land and the dreadful rivalry of its noble houses. The county is fiercely feudal, with commoners unable to travel freely without a lord's consent. It is also famous for its jousts and fairs, and in good years the locals can grow fat on the rich foodstuffs served at these frequent celebrations. According to the scroll, the county is currently ruled by House Bailey, who were once mere stewards of their rivals House Dayne. That house has seen something of a fall in fortunes lately, and now stand in lower esteem than the Baileys. However, the scroll warns you that all sorts could have changed since the time it was written, as the politics in Longport Bay is both bloody and unpredictable.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::LONGPORT_BAY);
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

            if (party.LastSelected >= 0 && party.LastSelected < party.Party.size())
            {
                for (auto i = 0; i < party.Party[party.LastSelected].Equipment.size(); i++)
                {
                    auto item = party.Party[party.LastSelected].Equipment[i];

                    if (item.Class != Equipment::Class::WEAPON && item.Class != Equipment::Class::ARMOUR)
                    {
                        Take.push_back(item);
                    }
                }

                Limit = Take.size();

                party.Party.erase(party.Party.begin() + party.LastSelected);

                party.Current = -1;

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
            Choices.push_back(Choice::Base("Arrange an audience (Team: Charisma 5+, Successes: 5)", {Book::Type::BOOK1, 134}, {Book::Type::BOOK1, 775}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 5, 5));

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
            Choices.push_back(Choice::Base("Climb the cliffside (Team: Survival 5+, Successes: 4)", {Book::Type::BOOK1, 409}, {Book::Type::BOOK1, -87002}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 5, 4));

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
            if (Engine::COUNT(party.Party) > 1)
            {
                PreText = "One of your party members loses their grip halfway up and plunges into the sea. They are never seen again.";
            }
            else
            {
                PreText = "You lose your grip halfway up and plunge into the sea. You are never seen again.";

                party.Party.clear();

                party.Current = -1;

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
            if (party.LastSelected >= 0 && party.LastSelected < party.Party.size())
            {
                party.Party[party.LastSelected].Equipment.clear();

                party.Party.erase(party.Party.begin() + party.LastSelected);

                party.Current = -1;

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

            Text = "Your return trip to Lhasbreath is mercifully uneventful, and Sir Lawrence Ross lays on a grand banquet upon your arrival. You spend several days with the Ross's, recovering your strength and discussing your adventures.\n\nWhen Emlyn reports her findings, the news is rather disappointing. \"We have recovered many pretty blooms,\" she admits. \"Certainly, the fragrances are quite striking... however, there are no medicinal or culinary plants, as far as I can tell. Nothing sellable.\"\n\n\"Curses!\" grumbles Sir Lawrence. \"This expedition is turning into an expensive failure.\"\n\nStill, Sir Lawrence is good to his word and presents you with a purse of 150 silver coins. Offering your consolations over the failed mission, you depart into the city.\n\nAlthough the expedition did not go perfectly you have learned a lot.\n\nYou gained the code A85.";

            Choices.clear();
            Choices.push_back(Choice::Base("Choose a party member to gain 1 point of SURVIVAL", {Book::Type::BOOK1, 775}, Choice::Type::ROLL_FOR_ATTRIBUTE_INCREASE, {Attribute::Type::SURVIVAL}, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::LHASBREATH);

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
            Choices.push_back(Choice::Base("Force open the door (Individual check: Stealth 5+, Successes: 3)", {Book::Type::BOOK1, 487}, {Book::Type::BOOK1, 724}, Choice::Type::CHARACTER_ATTRIBUTES, {Attribute::Type::STEALTH}, 5, 3));

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
            if (Engine::COUNT(party.Party, Team::Type::MERCENARY) > 0)
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
            Choices.push_back(Choice::Base("Select which party member is in FRONT", {Book::Type::BOOK1, -98}, Choice::Type::SET_STATUS, {Character::Status::FRONT, Character::Status::BACK}, 0));
            Choices.push_back(Choice::Base("Select which party member is at the BACK", {Book::Type::BOOK1, -98}, Choice::Type::SET_STATUS, {Character::Status::FRONT, Character::Status::BACK}, 1));
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
            Choices.push_back(Choice::Base("Navigate the rapids (Team: Survival 5+, Successes: 3)", {Book::Type::BOOK1, 54}, {Book::Type::BOOK1, 337}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 5, 3));

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
            for (auto i = 0; i < party.Party.size(); i++)
            {
                auto non_taxable = std::vector<Equipment::Base>();

                for (auto j = 0; j < party.Party[i].Equipment.size(); j++)
                {
                    if (party.Party[i].Equipment[j].Class == Equipment::Class::SHIELD || party.Party[i].Equipment[j].Attribute == Attribute::Type::NONE || party.Party[i].Equipment[j].Modifier < 2)
                    {
                        non_taxable.push_back(party.Party[i].Equipment[j]);
                    }
                }

                if (non_taxable.size() > 0)
                {
                    party.Party[i].Equipment = non_taxable;
                }
                else
                {
                    party.Party[i].Equipment.clear();
                }
            }

            Engine::GAIN_HEALTH(party.Party, -1);
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

            if (!Engine::VERIFY_EQUIPMENT(party.Party, {Equipment::Type::RING_OF_THE_PATRIARCH}))
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

            if (character >= 0 && character < party.Party.size())
            {
                previousTeam = party.Party[character].Team;

                Engine::SET_TEAM(party.Party[character], Team::Type::SAR_JESSICA_DAYNE);

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
                if (character >= 0 && character < party.Party.size())
                {
                    Engine::SET_TEAM(party.Party[character], previousTeam);
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

            Text = "Your new crew cheer as you come aboard. The ship is large and shabbily built -- but perhaps you can make something of it?\n\n[HULK] Fighting: 3, Health: 5, Cargo: 3 units\n\nNote: Your new ship is in Mordian harbour. What will you do now?";

            Choices.clear();
            Choices.push_back(Choice::Base("Depart in your new ship", {Book::Type::BOOK1, 851}, Choice::Type::SHIP, Location::Type::MORDIAN));
            Choices.push_back(Choice::Base("Enter the wide tunnel", {Book::Type::BOOK1, 593}));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::MORDIAN);

            party.Fleet.push_back(Ship::Base("HULK", Ship::Type::HULK, Location::Type::MORDIAN, 3, 5, 3));
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
            Choices.push_back(Choice::Base("Navigate the Withered Steppes (Team: Survival 4+, Successes: 3)", {Book::Type::BOOK1, -115001}, {Book::Type::BOOK1, -115002}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::SURVIVAL}, 4, 3));

            Controls = Story::Controls::STANDARD;
        }

        void SkillCheck(Party::Base &party, bool outcome, std::vector<int> selection)
        {
            if (!outcome)
            {
                Bye = "Each party member loses 1 Health point as your rations dwindle.";

                Engine::GAIN_HEALTH(party.Party, -1);
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
                Choices.push_back(Choice::Base("Rob the upper floor (Team: Stealth 4+, Successes: 5)", {Book::Type::BOOK1, 22}, {Book::Type::BOOK1, 529}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::STEALTH}, 4, 5));
            }
            else
            {
                Choices.push_back(Choice::Base("Rob the upper floor (Team: Stealth 4+, Successes: 5)", {Book::Type::BOOK1, 22}, {Book::Type::BOOK1, 529}, Team::Type::WALL_CLIMBING, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::STEALTH}, 4, 5));
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
            Engine::GAIN_HEALTH(party.Party, -1);
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

            Text = "Clifftop is currently under the governorship of Ayleta, one of the Everchild's generals. She welcomes you into the hall, and details to you her various plans for the improvement of the city. \"I want to create more permanent structures for the poor, and to reduce the amount of beatings the overseers issue to their farmworkers,\" she says. \"Progress is slow. The population is uneducated and tend to misbehave when they are not being brutalised. It may take many generations to change Clifftop for the better.\"\n\nIt seems a hard task, and not one suited to the abilities of adventurers.";

            Bye = "Wishing her well, you depart the hall.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Engine::SET_LOCATION(party, Location::Type::CLIFFTOP);
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
                Choices.push_back(Choice::Base("Repair the calendar (Team: Lore 5+, Successes: 3)", {Book::Type::BOOK1, -128001}, {Book::Type::BOOK1, 75}, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::LORE}, 5, 3));
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

            Choices.push_back(Choice::Base("Secure the cogwheel (Individual: Survival 4+, Successes: 3)", {Book::Type::BOOK1, 745}, {Book::Type::BOOK1, 549}, Choice::Type::LAST_INDIVIDUAL_CHECK, {Attribute::Type::SURVIVAL}, 4, 3));

            Controls = Story::Controls::STANDARD;
        }
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

    void InitializeStories()
    {
        Book1::Stories = {
            &event018, &event027, &event028, &event044, &event067, &event073, &event076, &event078, &e087_001, &e087_002,
            &e087_003, &event089, &event098, &event102, &e115_001, &e115_002, &e128_001, &e128_002,
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
            &story120, &story121, &story122, &story123, &story124, &story125, &story126, &story127, &story128, &story129};
    }
}
#endif
