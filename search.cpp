/**
  Shirley Li
  CS106B
  Assignment 2
  Creates a search engine that allows the user to input queries into the console and prints out the number
  of matching results and the webpages containing said query.
  */
#include "testing/SimpleTest.h"
#include "map.h"
#include "set.h"
#include <string>
#include <iostream>
#include "filelib.h"
#include <fstream>
#include "simpio.h"
#include "strlib.h"
#include "search.h"
using namespace std;

/**
 * @brief cleanToken
 *      takes in a word and trims the punctuation
 * @param token
 *      the word inputted
 * @return token
 *      the trimmed word
 */
string cleanToken(string token) { //5595 is now a cursed number for me.

    bool hasAlpha = false;
    int total = token.length();
    for (int count = 0; count < total; count++) {
        if (isalpha(token[count])) {
            hasAlpha = true;
        }
    }
    if (!hasAlpha) {
        return "";
    }
    else {
        //cout << token << endl;
        int frontIdx = -1;      //since I'm matching the for-each iterations with frontIdx, I need to keep my idx corresponding to the idx of the character
        for (char ch : token) { //in this case, I start with -1 so I can get to idx = 0 when the character at idx 0 is read
            if (ispunct(ch)) {
                frontIdx++;
            }
            else {
                break;      //no need to go through entire string if we just want the first few bits
            }
        }
        token = token.substr(frontIdx + 1);  //need to first substring token so backIdx will be accurate
        int backIdx = token.length() - 1;    //I tried substringing both the front and back ends out at the same time once; it got all weird. Doing them one at a time is just easier.
        while (ispunct(token[backIdx])) {
            backIdx--;
        }
        token = token.substr(0, backIdx + 1);

    }
    token = toLowerCase(token);
    return token;
}

/**
 * @brief readDocs
 *      reads the file being inputted
 * @param dbfile
 *      the file
 * @return docs
 *      the map that keys the url to the contents of the webpages
 */
Map<string, Set<string>> readDocs(string dbfile) {
    Map<string, Set<string>> docs;

    ifstream in;
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);
    Vector<string> lines;
    readEntireFile(in, lines);

    //cout << lines << endl;
    string url;

    for (string line : lines) {

        Set<string> cleanWords; //won't need to worry about duplicates

        Vector<string> separatedLines = stringSplit(line, " ");

        if (separatedLines.size() == 1) { //all the urls are on separate lines
            url = separatedLines[0];      //saving the website url for later (when we finish adding all the applicable words to cleanWords)
        }

        else {
            for (string word : separatedLines) {
                string cleanWord = cleanToken(word);

                if (cleanWord != "") {  //don't want empty strings
                    cleanWords.add(cleanWord);
                }
            }

            docs.put(url, cleanWords); //putting this here b/c it's still within url's scope
        }
    }

    //cout << docs << endl;
    return docs;
}

/**
 * @brief buildIndex
 *      make the words the keys and the website urls the values
 * @param docs
 *      the map that has the original urls and words
 * @return index
 *      the inverted map
 */
Map<string, Set<string>> buildIndex(Map<string, Set<string>>& docs) {
    Map<string, Set<string>> index;

    Vector<string> keys = docs.keys(); //all the website urls
    Vector<Set<string>> allValues = docs.values(); //all the words

    for (Set<string> setOfValuesForOneWebsite : allValues) { //for every Set<string> in the total accumulation of Set<string>>s

        for (string word : setOfValuesForOneWebsite) { //get a specific word in the set
            Set<string> temp;                          //making a temporary set here to save all the urls of this one word;
                                                       //chose to continuously create new Set<string>>s instead of iterating
                                                       //through them to remove the values inputted b/c it's easier this way. For me at least; Idk memory-wise.
            for (string key : keys) {   //checks all the website urls

                if (docs[key].contains(word)) { //if docs--at that website url key--contains the word as a value

                    temp.add(key);

                }
            }

            index.put(word, temp);  //Idk how to optimize this better; this is a crazy O(n^3) program. Should I have used breadth-first?
        }
    }

    return index;
}

/**
 * @brief findQueryMatches
 *      finds the matches for the query entered
 * @param index
 *      the built index that we're searching in
 * @param query
 *      the query that is being inputted
 * @return result
 *      a set with all the words that match with query
 */
Set<string> findQueryMatches(Map<string, Set<string>>& index, string query) {
    Set<string> result;
    Vector<string> q = stringSplit(query, " ");

    if (q.size() == 1) {
        string word = cleanToken(q[0]); //the key
        for (string keyWord : index) {  //for a single word, I just need to combine the two sets
            if (keyWord == word) {
                result += index.get(keyWord);
            }
        }
    }
    else {

        for (string word : q) { //one word from query

            if (word[0] == '+') {  //no need for any + or - checking for first word in query or at the end of words since cleanToken will just clean them all up
                word = cleanToken(word);
                result = result * index.get(word);
            }
            else if (word[0] == '-') {
                word = cleanToken(word);
                result -= index.get(word);
            }
            else {
                word = cleanToken(word);
                result = result + index.get(word);

            }

        }

    }
    //cout << result << endl;
    return result;
}

/**
 * @brief searchEngine
 *      searches through the built index when the user inputs search words
 * @param dbfile
 *      the file that is being used as the database
 */
void searchEngine(string dbfile) {
    Map<string, Set<string>> database = readDocs(dbfile);

    cout << "Stand by while building index..." << endl;

    Vector<string> keys = database.keys();
    database = buildIndex(database);    //placed the buildIndex line here so I could just call keys() and not have to code an iteration of all the values myself haha...
    Vector<string> uniqueTerms = database.keys();

    cout << "Indexed " << keys.size() << " pages containing "
         << uniqueTerms.size() << " unique terms." << endl;
    cout << endl;

    while (true) {
        string input = getLine("Enter query sentence (RETURN/ENTER to quit):");
        if (input == "") {
            cout << endl;
            cout << "All done!" << endl;
            break;
        }
        else {
            input = toLowerCase(input);  //lower cases the input since I'm searching in the next line
            Set<string> searchResults = findQueryMatches(database, input);
            cout << "Found " << searchResults.size() << " matching pages" << endl;
            cout << searchResults << endl;
            cout << endl;
        }
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("cleanToken on tokens with no punctuation") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
}

PROVIDED_TEST("cleanToken on tokens with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

PROVIDED_TEST("cleanToken on non-word tokens"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

PROVIDED_TEST("readDocs from tiny.txt, contains 4 documents") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    EXPECT_EQUAL(docs.size(), 4);
}

PROVIDED_TEST("readDocs from tiny.txt, suess has 5 unique words and includes lowercase fish") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Set<string> seuss = docs["www.dr.seuss.net"];
    EXPECT_EQUAL(seuss.size(), 5);
    EXPECT(seuss.contains("fish"));
    EXPECT(!seuss.contains("Fish"));
}

PROVIDED_TEST("buildIndex from tiny.txt, 20 unique tokens overall") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    EXPECT_EQUAL(index.size(), 20);
    EXPECT(index.containsKey("fish"));
    EXPECT(!index.containsKey("@"));
}

PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 3);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}

STUDENT_TEST("readDocs empty file") {
    Map<string, Set<string>> docs = readDocs("res/tiny_copy.txt");
    EXPECT_EQUAL(docs.size(), 0);
}

STUDENT_TEST("buildIndex with website text") {
    Map<string, Set<string>> docs = readDocs("res/website.txt");
    Map<string, Set<string>> index = buildIndex(docs);
    EXPECT_EQUAL(index.size(), 5595);
}

STUDENT_TEST("findQueryMatches multiple plus signs and minus signs") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);

    Set<string> matchesRedAndFish = findQueryMatches(index, "red ++--fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);


}

STUDENT_TEST("findQueryMatches cancelling out previous input") {
    Map<string, Set<string>> docs = readDocs("res/tiny.txt");
    Map<string, Set<string>> index = buildIndex(docs);

    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red +fish -bread");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);


}

STUDENT_TEST("readDocs with website.txt") { //just checking if it reads; I had some issues with this early on
    Map<string, Set<string>> docs = readDocs("res/website.txt");

}
