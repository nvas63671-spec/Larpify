#include <iostream>
#include <string>
#include <stack>
#include <cstdlib>
#include <windows.h>
using namespace std;

const int MAX_SONGS = 12;
stack<string> playlist, shuffled, lastShuffle;
bool isShuffled = false;

void gotoxy(int x, int y)
{
    COORD c = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void printCentered(string text, int y)
{
    gotoxy((80 - (int)text.length()) / 2, y);
    cout << text;
}

void drawHeader()
{
    string note[6] = { "   |\\", "   | \\", "   |", "   |", " _ |", "(_)|" };
    for (int y = 0; y < 25; y++)
    {
        gotoxy(0, y);
        cout << string(79, ' ');
    }
    for (int i = 0; i < 6; i++)
    {
        gotoxy(36, i);
        cout << note[i];
    }
    printCentered("L A R P I F Y", 6);
    printCentered("~ stack powered music ~", 7);
    printCentered(string(40, '='), 8);
}

void pause(int y)
{
    string wait;
    printCentered("Press Enter to continue...", y);
    getline(cin, wait);
}

void showMessage(string text)
{
    printCentered(text, 11);
    pause(13);
}

int toArray(stack<string> s, string a[])
{
    int n = 0;
    while (!s.empty())
    {
        a[n++] = s.top();
        s.pop();
    }
    return n;
}

stack<string> toStack(string a[], int n)
{
    stack<string> s;
    while (n > 0)
        s.push(a[--n]);
    return s;
}

void removeSong(stack<string>& s, string name)
{
    stack<string> temp;
    while (!s.empty())
    {
        if (s.top() != name) temp.push(s.top());
        s.pop();
    }
    while (!temp.empty())
    {
        s.push(temp.top());
        temp.pop();
    }
}

bool songExists(string name)
{
    string a[MAX_SONGS];
    int n = toArray(playlist, a);
    for (int i = 0; i < n; i++)
        if (a[i] == name) return true;
    return false;
}

void addSong()
{
    drawHeader();
    if ((int)playlist.size() >= MAX_SONGS)
    {
        showMessage("Playlist is full!");
        return;
    }
    gotoxy(20, 10);
    cout << "Enter song name: ";
    string name;
    getline(cin, name);
    if (name.empty() || songExists(name))
        showMessage("Song name is empty or already in the playlist.");
    else
    {
        playlist.push(name);
        if (isShuffled) shuffled.push(name);
        showMessage("Added: " + name);
    }
}

void playSong()
{
    drawHeader();
    if (playlist.empty())
    {
        showMessage("The playlist is empty.");
        return;
    }
    stack<string>& active = isShuffled ? shuffled : playlist;
    string name = active.top();
    active.pop();
    if (isShuffled) removeSong(playlist, name);
    if (playlist.empty()) isShuffled = false;
    showMessage("Now playing: " + name);
}

void viewPlaylist()
{
    drawHeader();
    if (playlist.empty())
    {
        showMessage("The playlist is empty.");
        return;
    }
    printCentered(isShuffled ? "--- SHUFFLED order ---" : "--- Original order ---", 9);
    stack<string> copy = isShuffled ? shuffled : playlist;
    for (int i = 1; !copy.empty(); i++)
    {
        gotoxy(25, 9 + i);
        cout << i << ". " << copy.top() << (i == 1 ? "   <- next up" : "");
        copy.pop();
    }
    pause(23);
}

void shuffleSongs()
{
    drawHeader();
    int n = (int)playlist.size();
    if (n < 2)
    {
        showMessage("Need at least 2 songs to shuffle.");
        return;
    }
    string a[MAX_SONGS];
    toArray(playlist, a);
    stack<string> before = lastShuffle.empty() ? playlist : lastShuffle;
    stack<string> result;
    for (int tries = 0; tries < 50; tries++)
    {
        for (int i = n - 1; i > 0; i--)
            swap(a[i], a[rand() % (i + 1)]);
        result = toStack(a, n);
        if (result != before) break;
    }
    if (result == before)
    {
        swap(a[0], a[1]);
        result = toStack(a, n);
    }
    shuffled = lastShuffle = result;
    isShuffled = true;
    showMessage("Playlist shuffled!");
}

void unshuffleSongs()
{
    drawHeader();
    if (!isShuffled)
    {
        showMessage("Already in the original order.");
        return;
    }
    shuffled = stack<string>();
    isShuffled = false;
    showMessage("Original order restored!");
}

void showMenu()
{
    string items[6] = { "[1] Add a song (push)", "[2] Play next song (pop)", "[3] View playlist",
                        "[4] Shuffle", "[5] Unshuffle", "[6] Exit" };
    drawHeader();
    for (int i = 0; i < 6; i++)
    {
        gotoxy(28, 10 + i);
        cout << items[i];
    }
    printCentered("Songs in playlist: " + to_string(playlist.size()), 17);
    gotoxy(28, 19);
    cout << "Choose an option: ";
}

int main()
{
    srand((unsigned int)GetTickCount());
    playlist.push("Machine Love - JamieP");

    bool running = true;
    while (running)
    {
        showMenu();
        string input;
        getline(cin, input);
        char choice = input.empty() ? ' ' : input[0];

        switch (choice)
        {
        case '1': addSong(); break;
        case '2': playSong(); break;
        case '3': viewPlaylist(); break;
        case '4': shuffleSongs(); break;
        case '5': unshuffleSongs(); break;
        case '6': running = false; break;
        default:
            drawHeader();
            showMessage("Please choose 1 to 6.");
        }
    }

    drawHeader();
    printCentered("Thanks for listening to Larpify!", 12);
    gotoxy(0, 24);
    return 0;
}
