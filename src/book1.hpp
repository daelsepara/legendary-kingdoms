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

            Text = "The treasure chest contains 200 SILVER COINS and a BLUESTONE. Take what you wish.\n\nYou gained the code A45.";

            Bye = "Not wishing to tarry longer, you depart the chamber and return to the crossroads.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Take = {Equipment::BLUESTONE};

            Limit = 1;

            Engine::GAIN_MONEY(party, 200);

            Engine::GET_CODES(party, {Codes::Base(Book::Type::BOOK1, 45)});
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

        Engine::Destination Continue(Party::Base &party)
        {
            return {Book::Type::BOOK1, 280};
        }
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

                Choices.push_back(Choice::Base("Seduce the Guards (Team: Charisma 3+, Successes: 2)", {Book::Type::BOOK1, 364}, {Book::Type::BOOK1, 588}, Team::Type::DISTRACTION, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 3, 2));
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

            if (!Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 75)}))
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
            Engine::GET_CODES(party, {Codes::Base(Book::Type::BOOK1, 50)});

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
            Engine::GET_CODES(party, {Codes::Base(Book::Type::BOOK1, 34)});
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

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Continue(Party::Base &party)
        {
            Engine::Random.UniformIntDistribution(1, 6);

            auto result = Engine::Random.NextInt();

            if (result < 3)
            {
                return {Book::Type::BOOK1, 466};
            }
            else if (result == 3)
            {
                return {Book::Type::BOOK1, 397};
            }
            else
            {
                return {Book::Type::BOOK1, 596};
            }
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

            Text = "There are few valuables amongst the goblins, though you manage to scavenge 15 SILVER COINS from the entire horde. Pleased to have survived you make your way onwards.";

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
            for (auto i = 0; i < party.Party.size(); i++)
            {
                Engine::GAIN_HEALTH(party.Party[i], -4);
            }
        }

        Engine::Destination Continue(Party::Base &party) { return {Book::Type::BOOK1, 55}; }
    };

    class Story018 : public Story::Base
    {
    public:
        std::string temp_text = "";

        Story018()
        {
            BookID = Book::Type::BOOK1;

            ID = 18;

            Text = "The door in front of you is stuck. To get it open you will have to force it.";

            Controls = Story::Controls::STANDARD;
        }

        Engine::Destination Background(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 57)}))
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

            temp_text = "";

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

                Engine::GET_CODES(party, {Codes::Base(Book::Type::BOOK1, 57)});
            }
            else
            {
                temp_text = std::string(party.Party[selection[0]].Name) + " LOSES 1 Health Point.";

                Bye = temp_text.c_str();

                Engine::GAIN_HEALTH(party.Party[selection[0]], -1);
            }
        }
    };

    // Dummy event to trigger Bye message in BOOK1:018
    class Event018 : public Story::Base
    {
    public:
        std::string temp_text = "";

        Event018()
        {
            BookID = Book::Type::BOOK1;

            ID = -18;

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

            if (!Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 41)}))
            {
                PreText += "\n\n\"We have betrayed no one!\" you reply. \"You have trapped us here. Allow us to leave and we will do you no harm.\"\n\n\"Liars! You have stolen our god!\" spits the snakeman priest. \"Our breed faulters and dies. Now, you shall die with us!\" The snakeman hisses foul words of sorcery. Suddenly the temple walls begin to crack, and blocks of stone rain down upon you. Even as the temple collapses the snakeman priest advances towards you, brandishing his staff.";

                Monsters = {Monster::Base("Snakeman Priest", Monster::Type::SNAKEMAN_PRIEST, 4, 4, 3, 16, 0)};
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 41)}))
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
            Engine::Random.UniformIntDistribution(1, 6);

            auto result = Engine::Random.NextInt();

            Choices.clear();

            if (result != 1)
            {
                Choices.push_back(Choice::Base("Sail north towards Clifftop", {Book::Type::BOOK1, 82}));
                Choices.push_back(Choice::Base("Sail east towards Cursus", {Book::Type::BOOK1, 814}));
                Choices.push_back(Choice::Base("Sail west towards THE SAVAGE LANDS", {Book::Type::BOOK5, 75}));
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
            Engine::GET_CODES(party, {Codes::Base(Book::Type::BOOK1, 77)});
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

            if (!Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 1)}))
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
            Choices.push_back(Choice::Base("Choose a party member and increase their maximum, unwounded Health score by 1 point.", {Book::Type::BOOK1, 566}, Choice::Type::ADD_MAX_HEALTH, 1));

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            for (auto i = 0; i < party.Party.size(); i++)
            {
                if (Engine::VERIFY_EQUIPMENT(party.Party[i], {Equipment::Type::QUICKSILVER}))
                {
                    Engine::LOSE_EQUIPMENT(party.Party[i], {Equipment::Type::QUICKSILVER});

                    break;
                }
            }
        }
    };

    class Story027 : public Story::Base
    {
    public:
        Story027()
        {
            BookID = Book::Type::BOOK1;

            ID = 27;

            Text = "The Everchild presents you with a bar of GOLD BULLION to help convince the mercenaries to change sides. Ayleta has been in private communication with the mercenaries for some time and has already negotiated a safe path between the mercenary encampment and the Everchild's forces.\n\nYou are received coolly by the unit captain, who cautiously welcomes you into his tent with his officers and bodyguards around him. You state your opinion that the Everchild's victory in the coming conflict is all but certain, and that she is willing to let mercenary companies that co-operate with her depart safely. Matters soon turn to money, with the mercenary captain suggesting that quite a lot of additional money will be coming his way should the patriarch win the battle, and Saltdad and other cities need to be supressed. He is also worried about his reputation if he plays turncoat without fighting even a single battle for his employer.\n\nOnly party members assigned to the mercenary team may assist with this skill check.\n\nNote: If you give the mercenaries a bar of GOLD BULLION, the DC of this check becomes 5+. If you give them two bars, the DC becomes 4+. If you give them three bars the DC becomes 2+.";

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

            Text = "Only party members assigned to the mercenary team may assist with this skill check.";

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

            if (!Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 1)}) || Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 37)}))
            {
                PreText += "\n\nYou have nothing of importance to discuss with him.";
            }

            Text = PreText.c_str();
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (!Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 1)}) || Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 37)}))
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
            if (Engine::VERIFY_CODES(party, {Codes::Base(Book::Type::BOOK1, 56)}))
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

            Text = "You return to the large burial chamber. The ghouls are long gone, and there is no more treasure to be found here. If you wish you can scribe the spell on the bronze tablet into your spellbook, if you have had reason to remove it:\n\nWisdom (Adventure)\n\nYour mind awakens with ancient knowledge. Cast this spell when you have failed a Lore check. You gain 3 automatic successes for this check only.";

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

            Text = "\"You say the captain wishes to see us? And he mentioned a promotion?\" presses the guard.\n\n\"Apparently his current lieutenant is not up to scratch,\" shrugs Brash. \"He's asked to see other guards as well. I imagine the first to see the captain will probably get the promotion.\"\n\nThe guards push past you in their eagerness to get to the captain's office.\n\nNote: Your distraction team can join up with the weapon's team for all future checks.";

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
            Engine::GET_CODES(party, {Codes::Base(Book::Type::BOOK1, 71)});
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
            Choices.push_back(Choice::Base("Choose a party member to gain 1 point of SURVIVAL", {Book::Type::BOOK1, 775}, Choice::Type::GAIN_ATTRIBUTE_SCORE, {Attribute::Type::SURVIVAL}, 1));

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

            Engine::GET_CODES(party, {Codes::Base(Book::Type::BOOK1, 16)});
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
    auto story100 = Story100();

    void InitializeStories()
    {
        Book1::Stories = {
            &event018, &event027, &event028,
            &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009,
            &story010, &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019,
            &story020, &story021, &story022, &story023, &story024, &story025, &story026, &story027, &story028, &story029,
            &story030, &story031, &story032, &story033, &story034, &story035, &story036, &story037, &story038, &story039,
            &story040,
            &story100};
    }
}
#endif
