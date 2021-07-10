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
        Story002()
        {
            BookID = Book::Type::BOOK1;

            ID = 2;

            Image = "images/book1/valley-of-bones-logo-bw.png";

            Text = "The orcs snarl as they see you, lurching to attack.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Bye = NULL;

            Combat = Engine::Combat::NONE;

            CanFlee = true;

            Monsters = {
                Monster::Base("Orc", 6, 4, 4, 8, 0),
                Monster::Base("Orc", 4, 4, 3, 9, 0)};
        }

        Engine::Destination Continue(Party::Base &party)
        {
            if (Combat == Engine::Combat::FLEE)
            {
                Bye = "The orcs chase you out of the dungeon.";

                return {Book::Type::BOOK1, 515};
            }
            else
            {
                return {Book::Type::BOOK1, 478};
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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            auto jessica = Engine::FIND_CHARACTER(party.OtherParty, Character::Type::SAR_JESSICA_DAYNE) >= 0;
            auto akihiro = Engine::FIND_CHARACTER(party.OtherParty, Character::Type::AKIHIRO_OF_CHALICE) >= 0;

            if (jessica || akihiro)
            {
                if (jessica && akihiro)
                {
                    PreText = "Sar Jessica Dayne and Akihiro";
                }
                else if (jessica)
                {
                    PreText = "Sar Jessica Dayne";
                }
                else
                {
                    PreText = "Akihiro";
                }

                PreText += "'s sense of personal honour is too strict to attempt such a strategy.";
            }
            else
            {
                PreText = "The guards have been looking at you with hungry eyes recently. You sidle up to the gate-house guards, suggesting all sorts of pleasures could await them in exchange for a few paltry rations.";

                Choices.push_back(Choice::Base("Seduce the Guards (Team check: Charisma 3+, Required Successs: 2)", {Book::Type::BOOK1, 364}, {Book::Type::BOOK1, 588}, party.OtherParty, Choice::Type::TEAM_ATTRIBUTES, {Attribute::Type::CHARISMA}, 3, 2));
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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

            Text = "You have chosen to battle one of the dark knights of Abraxas. Good luck!\n\nNote: On any round when Skallos does not take damage, he will unleash a roar of black magic. Each party member must lose 1 point of Health, and Skallos will restore 4 points of Health, up to his maximum of 20 Health points.";

            Choices.clear();

            Controls = Story::Controls::STANDARD;
        }

        void Event(Party::Base &party)
        {
            Combat = Engine::Combat::NONE;

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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

            Image = "images/book1/valley-of-bones-logo-bw.png";

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
                Choices.push_back(Choice::Base("Force open the door (Individual check: Fighting 4+, Required Successs: 3)", {Book::Type::BOOK1, 350}, {Book::Type::BOOK1, -18}, {Attribute::Type::FIGHTING}, 4, 3));
            }
            else
            {
                Choices.push_back(Choice::Base("Force open the door (Individual check: Fighting 5+, Required Successs: 3)", {Book::Type::BOOK1, 350}, {Book::Type::BOOK1, -18}, {Attribute::Type::FIGHTING}, 5, 3));
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

            Controls = Story::Controls::NONE;
        }

        Engine::Destination Background(Party::Base &party)
        {
            return {Book::Type::BOOK1, 18};
        }
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

    void InitializeStories()
    {
        Book1::Stories = {
            &event018,
            &story001, &story002, &story003, &story004, &story005, &story006, &story007, &story008, &story009,
            &story010, &story011, &story012, &story013, &story014, &story015, &story016, &story017, &story018, &story019};
    }
}
#endif
