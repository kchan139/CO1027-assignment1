#include "knight.h"

enum Events { Bowser_surrender = 0,  meet_MadBear     = 1,  meet_Bandits   = 2,  meet_LordLupin  = 3, 
              meet_Elf         = 4,  meet_Troll       = 5,  meet_Shaman    = 6,  meet_SirenVajsh = 7, 
              loot_MushMario   = 11, loot_MushFib     = 12, loot_MushGhost = 13, loot_Remedy     = 15,
              loot_MaidenKiss  = 16, loot_PhoenixDown = 17, meet_Merlin    = 18, meet_Asclepius  = 19,
              meet_Bowser      = 99 };

enum MaxStats { maxLevel = 10, maxPotion = 99, KingArthurHP = 999 };

const float BaseDamage[] = { 0.0, 1.0, 1.5, 4.5, 7.5, 9.5 };

bool tinyEffect       = false, frogEffect          = false, loseToBowser = false,
     meetMerlinBefore = false, meetAsclepiusBefore = false;

int tinyCount   = 0, frogCount = 0, 
    levelBefore = 0, MAX_HP    = 0;

void display (int HP, int level, int remedy, int maidenkiss, int phoenixdown, int rescue) 
{
    cout << "HP="            << HP
         << ", level="       << level
         << ", remedy="      << remedy
         << ", maidenkiss="  << maidenkiss
         << ", phoenixdown=" << phoenixdown
         << ", rescue="      << rescue << endl;
}

void adventureToKoopa (string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue) 
{
    rescue = -1;
    readStats (file_input, HP, level, remedy, maidenkiss, phoenixdown, rescue);
    readEvent (file_input, HP, level, remedy, maidenkiss, phoenixdown, rescue);
    if (HP > 0 && !loseToBowser) rescue = 1;
    display (HP, level, remedy, maidenkiss, phoenixdown, rescue);
}

// Read knight's stats from the 1st line.
void readStats (string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue)
{
    ifstream inputFile;
    inputFile.open (file_input, ifstream::in);
    if (inputFile.is_open())
        inputFile >> HP >> level >> remedy >> maidenkiss >> phoenixdown;
    MAX_HP = HP;
    inputFile.close();
}

// Read the events from the 2nd line.
void readEvent (string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue)
{
    ifstream inputFile (file_input);
    if (inputFile.fail())
        return;

    string line;
    int targetLine = 2, currentLine = 1;
    while (getline(inputFile, line))
    {
        if (currentLine == targetLine) 
        {
            istringstream iss(line);
            int eventID = 0, i = 1;
            while (iss >> eventID)  // read integers from file
            {
                eventUpdate (file_input, HP, level, remedy, maidenkiss, phoenixdown, rescue, eventID, i);
                if (rescue != -1 || iss.peek() == EOF || iss.peek() == '\r' || iss.peek() == '\n') 
                    break;  
                display(HP, level, remedy, maidenkiss, phoenixdown, rescue);
                i++;
            }
            break;
        }
        currentLine++;
    }
    inputFile.close();
}

// Read needed filenames from the 3rd line
string readNeededFile (string file_input, int eventID)
{
    ifstream inputFile(file_input);
    if (inputFile.is_open())
    {
        int targetLine = 3, currentLine   = 1;
        int mushGhost  = 0, asclepiusPack = 1, merlinPack = 2;

        string filename[3];
        string line;
        while (getline(inputFile, line)) 
        {
            if (currentLine == targetLine) 
            {
                istringstream iss(line);
                int i = 0;
                while (getline(iss, filename[i], ',') && i < 3)  i++;
                break;
            }
            currentLine++;
        }
        inputFile.close();
        if (eventID > loot_MushGhost*10)    
            return filename[mushGhost].c_str();
        if (eventID == meet_Asclepius)
            return filename[asclepiusPack].c_str();
        if (eventID == meet_Merlin)
            return filename[merlinPack].c_str();
    }
    inputFile.close();
    return "";
}

void eventUpdate (string file_input, int & HP, int & level, int & remedy, int & maidenkiss, int & phoenixdown, int & rescue, int eventID, int i)
{   
    if (eventID == Bowser_surrender)    {rescue = 1;    return;}
    // loot remedy, maidenkiss, phoenixdown
    if (eventID == loot_Remedy && remedy < maxPotion)               remedy++;
    if (eventID == loot_MaidenKiss && maidenkiss < maxPotion)       maidenkiss++;
    if (eventID == loot_PhoenixDown && phoenixdown < maxPotion)     phoenixdown++;

    if (eventID == meet_Shaman && !tinyEffect && !frogEffect)
    {
        int levelO = (i > 6)? ( (i%10 > 5)? i%10 : 5) : i%10;

        if (level > levelO || MAX_HP == KingArthurHP || isPrime(MAX_HP))
            (level > maxLevel - 2)? level = maxLevel : level += 2;
        else if (level < levelO)
        {
            if (remedy) remedy--;
            else
            {
                tinyEffect = true;
                tinyCount  = 0;
                (HP/5 < 1)? HP = 1 : HP /= 5;
            }
        }
    }
    if (eventID == meet_SirenVajsh && !tinyEffect && !frogEffect)
    {
        int levelO = (i > 6)? ( (i%10 > 5)? i%10 : 5) : i%10;

        if (level > levelO || HP == KingArthurHP || isPrime(MAX_HP))
            (level > maxLevel - 2)? level = maxLevel : level += 2;
        else if (level < levelO)
        {
            if (maidenkiss) maidenkiss--;
            else
            {
                frogEffect  = true;
                frogCount   = 0;
                levelBefore = level;
                level       = 1;
            }
        }
    }
    if (eventID == meet_Bowser)
    {
        if (MAX_HP == KingArthurHP || level == maxLevel)
            level = maxLevel;
        else if (isPrime(MAX_HP) && level >= 8)
            level = maxLevel;
        else {rescue = 0; loseToBowser = true; return;}
    }

    // normal encounters
    if (eventID > Bowser_surrender && eventID < meet_Shaman)
    {
        int levelO = (i > 6)? ( ( (i%10) > 5)? (i%10) : 5) : (i%10);

        if (level > levelO || MAX_HP == KingArthurHP || isPrime(MAX_HP))
            (level > maxLevel - 1)? level : level++;
        else if (level < levelO)
        {
            int dmg = BaseDamage[eventID] * levelO * 10;
            HP     -= dmg;
        }
    }
    
    if (tinyEffect)
    {
        tinyCount++;
        if (tinyCount > 3 && HP > 0)
        {
            (HP*5 < MAX_HP)? HP *= 5 : HP = MAX_HP;
            tinyEffect = false;
        }
        if (remedy && HP > 0)
        {
            (HP*5 < MAX_HP)? HP *= 5 : HP = MAX_HP;
            remedy--;
            tinyEffect = false;
        }
    }
    if (frogEffect)
    {
        frogCount++;
        if (frogCount > 3)
        {
            level      = 1;
            frogEffect = false;
        }
        if (maidenkiss)
        {
            level = levelBefore;
            maidenkiss--;
            frogEffect = false;
        }
    }
    
    if (eventID == loot_MushMario)
    {
        int marioBoost = (mushMario(((level + phoenixdown)%5 + 1) * 3) % 100);
        (HP + marioBoost > MAX_HP) ? HP = MAX_HP : HP += marioBoost;
        for (int i = HP + 1; i <= MAX_HP; i++)
        {
            if (isPrime(i) || i == MAX_HP)
                {HP = i; break;}
        }
    }
    if (eventID == loot_MushFib && HP > 1)
    {
        int prev = 0, curr = 1;
        while (curr < HP) 
        {
            int temp = curr;
            curr    += prev;
            prev     = temp;
        }
        HP = prev;
    }
    if (eventID > loot_MushGhost * 10)
    {
        string fileNeeded = readNeededFile(file_input, eventID);
        readMushGhost (fileNeeded, HP, phoenixdown, eventID);
    }

    if (eventID == meet_Merlin && !meetMerlinBefore)
    {
        meetMerlinBefore = true;
        string fileNeeded = readNeededFile(file_input, eventID);
        readMerlinPack (fileNeeded, HP);
    }
    if (eventID == meet_Asclepius && !meetAsclepiusBefore)
    {
        meetAsclepiusBefore = true;
        string fileNeeded   = readNeededFile(file_input, eventID);
        readAsclepiusPack (fileNeeded, remedy, maidenkiss, phoenixdown);
    } 
    
    // update HP and rescue status
    if (HP <= 0)
    {
        if (phoenixdown < 1)    rescue = 0;
        else 
        {
            phoenixdown--;    HP = MAX_HP;
            tinyEffect   = false; frogEffect = false;
        }
    }
}

void readMushGhost (string file_input, int & HP, int & phoenixdown, int eventID)
{
    ifstream inputFile(file_input);
    if (inputFile.fail())
        return;

    int n2 = 0;
    inputFile >> n2;
    
    int* arr = new int[n2];
    string line;
    getline(inputFile, line); // consume the first line
    getline(inputFile, line); // read the second line

    istringstream iss(line);
    char comma;
    for (int i = 0; i < n2; i++) 
    {
        iss >> arr[i];
        iss >> comma;
    }
    inputFile.close();
    
    // duplicate arr
    int* arrCopy = new int[n2];
    for (int i = 0; i < n2; i++)
        arrCopy[i] = arr[i];

    for (int i = 0; i < n2; i++) 
    {
        if (arrCopy[i] < 0) arrCopy[i] = -1 * arrCopy[i];
        arrCopy[i] = (17 * arrCopy[i] + 9) % 257;
    }

    // updating HP
    string s_eventID = to_string(eventID);
    for (int i = 2; i < s_eventID.length(); i++)
    {
        if      (s_eventID[i] == '1')
            HP -= mushGhost_type1(n2, arr);
        else if (s_eventID[i] == '2')
            HP -= mushGhost_type2(n2, arr);
        else if (s_eventID[i] == '3')
            HP -= mushGhost_type3(n2, arrCopy);
        else if (s_eventID[i] == '4')
            HP -= mushGhost_type4(n2, arrCopy);

        if (HP <= 0)
        {
            if (phoenixdown > 0)
            {
                HP = MAX_HP;
                phoenixdown--;
            }
            else break;
        }
        if (HP > MAX_HP) HP = MAX_HP;
    }
    delete[] arr;
    delete[] arrCopy;
}

void readMerlinPack (string file_input, int & HP)
{
    ifstream inputFile(file_input);
    if (inputFile.fail())
        return;

    int n9 = 0;
    inputFile >> n9;
    for (int i = 0; i < n9; i++)
    {
        string item;
        inputFile >> item;

        if (item.find("merlin") != string::npos ||
            item.find("Merlin") != string::npos)
            (HP + 3 > MAX_HP)? HP = MAX_HP: HP += 3;
        else if ((item.find("m") != string::npos &&
                  item.find("e") != string::npos &&
                  item.find("r") != string::npos &&
                  item.find("l") != string::npos &&
                  item.find("i") != string::npos &&
                  item.find("n") != string::npos) ||
                 (item.find("M") != string::npos &&
                  item.find("E") != string::npos &&
                  item.find("R") != string::npos &&
                  item.find("L") != string::npos &&
                  item.find("I") != string::npos &&
                  item.find("N") != string::npos))
            {
                (HP + 2 > MAX_HP)? HP = MAX_HP: HP += 2;
            }
    }
    inputFile.close();
}

void readAsclepiusPack (string file_input, int & remedy, int & maidenkiss, int & phoenixdown)
{
    ifstream inputFile(file_input);
    if (inputFile.fail())
        return;

    int r1 = 0, c1 = 0;
    inputFile >> r1 >> c1;
    int potion[r1][c1];
    for (int r = 0; r < r1; r++)
    {
        for (int c = 0; c < c1; c++)
            inputFile >> potion[r][c];
    }
    inputFile.close();

    for (int r = 0; r < r1; r++)
    {
        int potionCount = 0;
        for (int c = 0; c < c1; c++)
        {
            if (potionCount > 2) break;

            if (potion[r][c] == loot_Remedy+1)
            {
                potionCount++; 
                if (remedy < maxPotion) remedy++;
            }
            if (potion[r][c] == loot_MaidenKiss+1)
            {
                potionCount++; 
                if (maidenkiss < maxPotion) maidenkiss++;
            }
            if (potion[r][c] == loot_PhoenixDown+1)
            {
                potionCount++; 
                if (phoenixdown < maxPotion) phoenixdown++;
            }
        }
    }
}

int mushGhost_type1(int n2, int arr[])
{
    int maxi = 0, mini = 0;
    for (int i = 0; i < n2; i++)
    {
        if (arr[i] >= arr[maxi]) maxi = i;
        if (arr[i] <= arr[mini]) mini = i;
    }
    return (maxi + mini);
}

int mushGhost_type2(int n2, int arr[])
{
    int  mtx, mti, 
         condition = 0;
    bool top       = false;

    if (arr[0] < arr[1])
    {
        for (int i = 1; i < n2; i++)
        {
            if (!top && arr[i] < arr[i - 1])
            {
                top = true;
                mtx = arr[i - 1];
                mti = i - 1;
            }
            else if (top && arr[i] < arr[i - 1])
                continue;
            else if (!top && arr[i] > arr[i - 1])
                {mtx = arr[i];  mti = i;}
            else
                return ((-2) + (-3));
        }
    }
    else if (arr[0] > arr[1])
    {
        for (int i = 1; i < n2; i++) 
            if (arr[i] < arr[i - 1]) condition++;
        if (!(condition - n2 - 1))
            {mtx = arr[0];  mti = 0;}
        else 
            return ((-2) + (-3));
    }
    return mtx + mti;
}

int mushGhost_type3(int n2, int arr[])
{
    int maxi2 = 0, mini2 = 0;
    for (int i = 0; i < n2; i++)
    {
        if (arr[i] > arr[maxi2]) maxi2 = i;
        if (arr[i] < arr[mini2]) mini2 = i;
    }
    return (maxi2 + mini2);
}

int mushGhost_type4(int n2, int arrCopy[])
{
    int max2_3x = -5,       max2_3i = -7;
    int max1    = INT8_MIN, max2    = INT8_MAX;

    for (int i = 0; i < min(3, n2); i++) 
    {
        if (arrCopy[i] > max1)  {max2 = max1;   max1 = arrCopy[i];} 
        else if (arrCopy[i] > max2 && arrCopy[i] < max1)
            max2 = arrCopy[i];
    }
    if (max2 == INT8_MIN)    {max2_3x = -5;  max2_3i = -7;}
    else 
    {
        for (int i = 0; i < min(3, n2); i++) 
        {
            if (arrCopy[i] == max2) 
            {
                max2_3x = max2;
                max2_3i = i;
                return (max2_3x + max2_3i);
            }
        }
    }
    return (max2_3x + max2_3i);
}

int mushMario(int n1)
{
    int s1 = 0;
    for (int i = 0; i < n1; i++)
        s1 += (99 - i*2);
    return s1;
}

bool isPrime(int x)
{
    if (x < 2) return false;
    for (int i = 2; i < x; i++)
        if (!(x % i)) return false;
    return true;
}
