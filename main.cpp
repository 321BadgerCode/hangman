// Badger
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <cstring>
#include <sstream>

using namespace std;

#ifdef _WIN32
	#include <windows.h>

	void clearScreen() {
		system("cls");
	}
#else
	#include <unistd.h>

	void clearScreen() {
		system("clear");
	}
#endif

vector<string> fruits = {"apple", "banana", "orange", "grape", "pear", "strawberry", "blueberry", "raspberry", "blackberry", "watermelon", "cantaloupe", "honeydew", "kiwi", "pineapple", "mango"};
vector<string> landmarks = {"eiffel tower", "big ben", "statue of liberty", "golden gate bridge", "colosseum", "pyramids", "great wall of china", "taj mahal", "christ the redeemer", "machu picchu", "stonehenge", "moai statues", "sphinx", "mount rushmore"};
vector<string> fastFood = {"mcdonalds", "burger king", "wendys", "taco bell", "kfc", "chick fil a", "subway", "starbucks", "dunkin donuts", "pizza hut", "dominos", "papa johns", "little caesars", "chipotle", "panera bread"};
vector<string> words;

vector<string> append(vector<string> v1[],int size=1){
	vector<string> v2;
	for(int i=0;i<size;i++){
		for(int j=0;j<v1[i].size();j++){
			v2.push_back(v1[i][j]);
		}
	}
	return v2;
}

string capitalize(string str){
	stringstream ss(str);
	string word;
	string ans="";
	while(ss>>word){
		word[0]=toupper(word[0]);
		ans+=word+" ";
	}
	return ans;
}

string generateWord() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dis(0, words.size() - 1);
	return words[dis(gen)];
}

bool hasLetter(string word, char letter) {
	for (int i = 0; i < word.length(); i++) {
		if (word[i] == letter) {
			return true;
		}
	}
	return false;
}

int getLength(char* arr){
	int i=0;
	while(arr[i]!='\0'){i++;}
	return i;
}

string getWord(string word, char* guessedLetters) {
	string maskedWord = "";

	for (int i = 0; i < word.length(); i++) {
		bool isGuessed = false;
		for (int j = 0; j < strlen(guessedLetters); j++) {
			if (word[i] == guessedLetters[j]) {
				isGuessed = true;
				break;
			}
		}

		if (isGuessed) {
			maskedWord += word[i];
		} else if(word[i]==' '){
				maskedWord += " ";
		} else {
			maskedWord += "_";
		}
	}

	return maskedWord;
}

string getPlatform(string hangman){
	string ans="";
	for (int i = 0; i < hangman.length(); i++) {
		ans+=hangman[i];
	}
	return ans;
}

string getHangman(int numWrongGuesses) {
	string ans="";
	switch (numWrongGuesses) {
		case 0:
			ans="";
			break;
		case 1:
			ans="O";
			break;
		case 2:
			ans="O\n|";
			break;
		case 3:
			ans=" O\n/|";
			break;
		case 4:
			ans=" O\n/|\\";
			break;
		case 5:
			ans=" O\n/|\\\n/";
			break;
		case 6:
			ans=" O\n/|\\\n/ \\";
			break;
	}
	return getPlatform(ans);
}

int main(int argc,char** argv) {
	words=append(new vector<string>[3]{fruits,landmarks,fastFood},3);
	string word = generateWord();
	bool ownWord=false;
	bool verbose=false;
	for(int i=1;i<argc;i++){
		if((string)argv[i]=="-h"){
			cout<<"Usage: "<<argv[0]<<" [-w word] [-t word type] [-h] [-v] [-i]"<<endl;
			cout<<"-w word: Sets the word to guess to \"word\""<<endl;
			cout<<"-t: Sets the word type to \"word type\"\n\t* \"fruits\": Sets the word type to fruits\n\t* \"landmarks\": Sets the word type to landmarks\n\t* \"fast food\": Sets the word type to fast food"<<endl;
			cout<<"-h: Displays this help message"<<endl;
			cout<<"-v: Displays the word to guess"<<endl;
			cout<<"-i: Displays information about the program"<<endl;
			return 0;
		}
		else if((string)argv[i]=="-w"){
			i++;
			word=(string)argv[i];
			ownWord=true;
		}
		else if((string)argv[i]=="-t"){
			if(ownWord){
				cout<<"Cannot set word type when word is set using \"-w\"!"<<endl;
				return 1;
			}
			i++;
			if((string)argv[i]=="fruits"){
				words=append(&fruits);
			}
			else if((string)argv[i]=="landmarks"){
				words=append(&landmarks);
			}
			else if((string)argv[i]=="fast food"){
				words=append(&fastFood);
			}
			else{
				cout<<"\""<<(string)argv[i]<<"\" is not a valid word type!"<<endl;
				return 1;
			}
			word=generateWord();
		}
		else if((string)argv[i]=="-v"){
			verbose=true;
		}
		else if((string)argv[i]=="-i"){
			cout<<"Author: Badger"<<endl;
			cout<<"Version: 1.0.0"<<endl;
			cout<<"License: MIT"<<endl;
			return 0;
		}
	}
	if(verbose){
		cout<<"Verbose word: "<<capitalize(word)<<endl;
	}
	char guessedLetters[26];
	int numGuessedLetters = 0;
	int numWrongGuesses = 0;
	string guessedWord = "";
	while (numWrongGuesses < 6) {
		cout << getHangman(numWrongGuesses) << endl;
		guessedWord=getWord(word,guessedLetters);
		cout << "Word: \"" << guessedWord << "\"" << endl;
		cout << "Guessed Letters: ";
		int len=numGuessedLetters+numWrongGuesses;
		for (int i = 0; i < len; i++) {
			string clr=(hasLetter(word,guessedLetters[i]))?"\033[38;5;10m":"\033[38;5;9m";
			string end=(i==len-1)?"":",";
			cout << clr << guessedLetters[i] << "\033[0m" << end;
		}
		cout << endl;
		if(guessedWord==word){break;}
input:
		cout << "Guess a letter: ";
		char guess;
		cin.clear();
		cin >> guess;
		if(guess>='A'&&guess<='Z'){
			guess+=32;
		}
		else if(guess<'a'||guess>'z'){
			cout<<"\""+string(1,guess)+"\" is not a letter!"<<endl;
			goto input;
		}
		if(hasLetter(guessedLetters,guess)){
			cout<<"\""+string(1,guess)+"\" has already been guessed!"<<endl;
			goto input;
		}
		guessedLetters[len] = guess;
		if (hasLetter(word, guess)) {
			numGuessedLetters++;
		}
		else {
			numWrongGuesses++;
		}
		clearScreen();
	}
	cout << ((numWrongGuesses<6)?"You win!":"You lose!") << endl;
	cout << "The word was: " << capitalize(word) << endl;
	return 0;
}