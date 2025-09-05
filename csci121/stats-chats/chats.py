#
# CSCI 121 Fall 2022
#
# Project 2, Part 2
#
# chats.py
#
# Process a text and distill statistics about the bi-gram and tri-gram
# word occurrences in the entered text. It does this by using a dictionary
# of words and bi-grams. Each dictionary entry gives the list of words
# in the text that follow that word/bi-gram (and possibly a count of the
# number of times each follower does so).
#
# The code then generates random text based on these statistics.
#
#
# Usage: python3 chats.py
#
# This command processes a series of lines of text, looking for
# contiguous runs of alphabetic characters treating them each as a
# word. For each such word, it keeps a count of the number of its
# occurrences in the text.
#
# To end text entry hit RETURN and then CTRL-d.  It then generates
# a random text that tries to mimic the text it just processed.
#
#
# Alternative usage: python3 chats.py < textfile.txt
#
# The above instead processes the text of the file in 'textfile.txt'.
#
import pprint
import sys
import random

STOPPERS   = [".", "!", "?"]
WHITESPACE = [" ","\n","\r","\t"]

def simplifyWord(word):
    """Returns the given string with only certain chars and lowercase.

    This "simplifies" a word so that it only contains a sequence of
    lower case letters and apostrophes, making uppercase letters
    lowercase, and skipping others.  It returns the "simplified" word.
    If, upon simplifying a word, all the characters are skipped, the 
    function returns None.
    
    In normal use, this would convert a word like "Ain't" into the
    word "ain't" and return it. It also would take a text string like
    "it105%s" and give back the string "its".

    This particular function behavior is somewhat arbitrary, written
    to be "good enough" just to handle the spurious other characters
    that might arise in the kinds of free texts from things like
    Project Gutenberg. Sadly, this also strips out accented characters
    and non-Roman alphabetic characters.
    """

    # Scan the word for a-z or ' characters.
    convertedWord = "";
    for c in word:
        if 'A' <= c and  c <= 'Z':
            c = c.lower()
        if ('a' <= c and c <= 'z') or c == '\'':
            convertedWord += c;

    # If we added any such characters, return that word.
    if len(convertedWord) > 0:
        return convertedWord
    else:
        # Otherwise, return None.
        return None

def readWordsFromInput():
    """Returns the contents of console input as a list of words.

    Process the console input as consisting of lines of words. Return
    a list of all the words along with the strings that are "stoppers." 
    Each non-stopper word in the list will be lowercase consisting only
    of the letters 'a'-'z' and also apostrophe. All other characters are
    stripped from the input. Stopper words are specified by the variable
    STOPPERS.
    """
     
    def spacedAround(text,c):
        """Returns modified text with spaces around any occurrence of c.

        Helper function that replaces any string `text` that has the
        character `c` so that all the occurrences of `c` are replaced
        with the substring " c ".
        """
        
        splits = text.split(c)
        return (" "+c+" ").join(splits)

    def spaceInsteadOf(text,c):
        """Returns modified text with space replacing any c.

        Helper function that replaces any string `text` that has the
        character `c` so that all the occurrences of `c` are replaced
        with a space.
        """
        
        splits = text.split(c)
        return (" ").join(splits)
    
    # Read the text into one (big) string.
    textChars = sys.stdin.read()

    # Add spaces around each "stopper" character.
    for stopper in STOPPERS: 
        textChars = spacedAround(textChars,stopper)
        
    # Replace each whitespace character with a space.
    for character in WHITESPACE: 
        textChars = spaceInsteadOf(textChars,character)

    # Split the text according to whitespace.
    rawWords = textChars.split(" ")

    # Process the raw words, simplifying them in the process by
    # skipping any characters that we don't currently handle.
    # We treat the "end of sentence"/"stopper" words specially,
    # including them in the list as their own strings.
    words = []
    for word in rawWords:
        if word not in STOPPERS:
            word = simplifyWord(word)
        if word is not None:
            words.append(word)
    return words

def train(theWords):
    """Returns a dictionary containing all of the bigrams and trigrams 
    within a text.

    Broken into four sections, it first adds all the stoppers to the 
    dictionary, then all the bigrams, then there is a helper function 
    to create trigrams, and finally it adds the trigrams. 
    """
    
    wordData = {}
    

    # starts by adding each stopper as a key with the first word 
    # of the input list as its value
    for i in STOPPERS: 
        wordData[i] = [theWords[0]]

    # adds each word in list as a dictionary key with a list of 
    # its followers
    j = 0
    followers = []
    # for every word in the word list
    for i in theWords:
        # if the index of the next word is the final one 
        if j + 1 == len(theWords):
            # and if current word is not a stopper
            if i not in STOPPERS:
                # add '.' as one of its values
                wordData[i] = ['.']
        # otherwise,
        else: 
            # if the word is already in the dictionary
            if i in wordData:
                # add the next word to its associated list
                wordData[i].append(theWords[j + 1])
            # otherwise,
            else: 
                # add that word to a new list
                followers.append(theWords[j + 1])
                # create that word as a key in the dictionary with 
                # its own value list
                wordData[i] = followers
        followers = []
        j += 1

    def makeTrigrams(theWords, i, j):
        """Returns a string concantenating two succeeding items in the list
        
        Helper function that will allow the wordData list
        to describe the trigrams that occur in the text. 
        """
        trigram = ""
        # if the second word is less than the length of the word list
        if j <= len(theWords):
            # if the second word is not blank or a final stopper
            if theWords[j] == '':
                return None
            elif theWords[j] in STOPPERS:
                return None
            # make the trigram string
            trigram = theWords[i] + " " + theWords[j]
        return trigram
    
    # goes through each of the trigrams in the text and adds it in 
    # a similar fashion to the bigrams loop
    word1 = 0
    word2 = 1
    for i in theWords: 
        # if the index of the second word is equal to the length of the word list
        if word2 == len(theWords):
            # stop
            break
        # make the start of a trigram
        d = makeTrigrams(theWords, word1, word2)
        # if that start exists
        if d is not None:
            # and if it is already in the dictionary
            if d in wordData: 
                # add the next word to that key
                wordData[d].append(theWords[word1 + 2])
            # otherwise if it is new,
            else: 
                # create that key with its value list
                wordData[d] = [theWords[word1 + 2]]
        word1 += 1
        word2 += 1 

    return wordData

def chat(biTriDict,numLines,lineWidth):
    """Returns a string that has been generated based on the dictionary from train().
    
    Uses a function to make a single line of the auto-generated text. Uses random
    choice method within this function to randomly pick one of the list items from 
    the value of the key in the dictionary. Then formats the text it creates
    based on the contraints inputted as parameters.
    """
    mainText = ""

    # variable to be used by the function below
    oneLine = []

    def createLine(line, lineWidth, biTriDict, w1, w2):
        '''Returns a list that will be turned into a single string line 
        of auto-generated text.
        
        Helper function that loops through the dictionary and gets the 
        next word of the text based on the previous two words and their
        stored bigrams and trigrams. 
        '''

        nextWord = ""
        # variable to keep track of the length of the line, as the actual line
        # is stored as a list
        fakeLine = ""
        
        # while the length of the line is less than the desired width
        while len(fakeLine) <= lineWidth:
            # if there has not been a word created yet
            if w1 + " " + w2 == " ":
                # w1 is a random word after '.', and add that to the line variables
                w1 = random.choice(biTriDict['.'])
                line.append(w1)
                fakeLine += " " + w1
            # if the second word has not been created yet
            if w2 == "":
                # it is a random word based on the first word, add it to the line variables
                w2 = random.choice(biTriDict[w1])
                line.append(w2)
                fakeLine += " " + w2
            # if there is not a trigram based on the previous two words in the dictionary
            if w1 + " " + w2 not in biTriDict: 
                # the next word is based only one w2's bigram,
                nextWord = random.choice(biTriDict[w2])
                # add the next word to the line
                line.append(nextWord)
                fakeLine += " " + nextWord
                # proceed forward by storing the previous words and restarting the process
                w2 = nextWord
                w1 = w2
            # otherwise,
            else:
                # the next word is based on a trigram
                nextWord = random.choice(biTriDict[w1 + " " + w2])
                # follow logic from directly above
                line.append(nextWord)
                fakeLine += " " + nextWord
                w2 = nextWord
                w1 = w2
        return line

    def createLastLine(line, lineWidth, biTriDict, w1, w2):
        '''Returns a list that will be the final line of the auto-generated text.
        
        Helper function that is the exact same function as the createLine function,
        however, it stops once it comes across a stopper. 
        '''
        nextWord = ""
        fakeLine = ""
        while len(fakeLine) <= lineWidth:
            # if w1 or w2 has become a stopper,
            if w1 in STOPPERS or w2 in STOPPERS:
                # stop
                return line
            else: 
                if w1 + " " + w2 == " ":
                    w1 = random.choice(biTriDict['.'])
                    line.append(w1)
                    fakeLine += " " + w1
                if w2 == "":
                    w2 = random.choice(biTriDict[w1])
                    line.append(w2)
                    fakeLine += " " + w2
                if w1 + " " + w2 not in biTriDict: 
                    nextWord = random.choice(biTriDict[w2])
                    line.append(nextWord)
                    fakeLine += " " + nextWord
                    w2 = nextWord
                    w1 = w2
                else:
                    nextWord = random.choice(biTriDict[w1 + " " + w2])
                    line.append(nextWord)
                    fakeLine += " " + nextWord
                    w2 = nextWord
                    w1 = w2
        return line
    
    # resets w1 and w2
    w1 = ''
    w2 = ''
    # counter for the number of lines
    lines = 0
    # while the number of lines has not reached the penultimate line
    while lines < numLines - 1:
        # the previous line is equal to the line that was just generated
        oneLine = createLine(oneLine, lineWidth, biTriDict, w1, w2)
        # w1 is the second to last index of that line and w2 is the last. 
        # this allows for a more coherent generation when the next line
        # starts rather than just initializing w1 and w2 again based on
        # the '.' entry. 
        w1 = oneLine[-2]
        w2 = oneLine[-1]
        # turn the line into a string
        nextLine = ' '.join(oneLine)
        # add that string to the main text and then start a new line
        mainText += nextLine + "\n"
        # reset the line list
        oneLine = []
        lines += 1
    
    # while there has not been a stopper at the end of the line
    while w2 not in STOPPERS: 
        # make the last line
        oneLine = createLastLine(oneLine, lineWidth, biTriDict, w1, w2)
        w2 = oneLine[-1]
        nextLine = ' '.join(oneLine)
        mainText += nextLine + "\n"
        oneLine = []

    # format the text so that there are not unusual spaces between the end of 
    # a sentence and a stopper
    mainText = mainText.replace(" .", ".")
    mainText = mainText.replace(" !", ".")
    mainText = mainText.replace(" ?", ".")

    print(mainText)


def trainBetter(wordList):
    """Returns a dictionary where the value of each key is a list of lists.

    Each list in the list of lists holds the next word in the bigram/trigram,
    as well as a count of how many times it has been seen. The function does 
    this in steps of stoppers, bigrams, and trigrams.
    """

    wordData = {}
    
    # starts by adding each stopper as a key with the first word 
    # of the input list as its value with a count of 1
    for i in STOPPERS: 
        wordData[i] = [[wordList[0], 1]]

    def makeBigramsList(theWords):
        '''Returns a list of all the bigrams in a text.
        
        Helper function that allows for the program to loop through
        the bigrams so as to evaluate how often they appear in the text.
        '''
        bigrams = []
        i = 0
        j = 1
        # create the bigrams, and add them to the returned list
        while j < len(theWords):
            bigram = theWords[i] + ' ' + theWords[j]
            bigrams.append(bigram)
            bigram = ''
            i += 1
            j += 1
        return bigrams

    # loops through the bigrams list and adds them to the dictionary,
    # or adds to their counts within the value list for each key
    bigramsList = makeBigramsList(wordList)
    index = 0
    updated = False
    # for every bigram
    for i in bigramsList:
        # if the first word of the bigram is in the dictionary
        if wordList[index] in wordData: 
            # for each list in the list of lists for that word
            for item in wordData[wordList[index]]:
                # if the bigram's next word is in the sub-list
                if wordList[index + 1] in item: 
                    # increase its count by one
                    item[1] += 1
                    # record that it was updated
                    updated = True
                    break
            # if it wasn't updated, that means it is a new bigram ending
            if updated == False:
                # therefore, add it to that key
                wordData[wordList[index]].append([wordList[index + 1], 1])
        # otherwise, it has not been seen at all
        else: 
            # add the new bigram starter to the dictionary
            wordData[wordList[index]] = ([[wordList[index + 1], 1]])
        index += 1

    def makeTrigrams(theWords, i, j):
        """Returns a string concantenating two succeeding items in the list
        
        Helper function that will allow the wordData list
        to describe the trigrams that occur in the text. This is nearly the
        same one as was in the regular train function.
        """
        trigram = ""
        if j <= len(theWords):
            if theWords[j] == '':
                return None
            trigram = theWords[i] + " " + theWords[j]
        return trigram
    
    # does the same thing as the bigrams loop, but for trigrams.
    # because of this, the next word becomes the index 2 down from
    # the word being evaluated, and also this loop is making the 
    # trigrams as it goes.
    word1 = 0
    word2 = 1
    update = False
    for i in wordList: 
        if word2 == len(wordList):
            break
        d = makeTrigrams(wordList, word1, word2)
        if d is not None:
            if d in wordData: 
                for item in wordData[d]:
                    if wordList[word1 + 2] in item: 
                        item[1] += 1
                        update = True
                        break
                if update == False:
                    # this is the point in which the program has a slight problem,
                    # where it will not append the new trigram ending for some reason
                    # even though this should do it in the same sense and the bigram
                    # loop, which does work. 
                    wordData[d].append([wordList[word1 + 2], 1])
            else: 
                if word1 + 2 < len(wordList):
                    wordData[d] = [[wordList[word1 + 2], 1]]
        word1 += 1
        word2 += 1 

    return wordData
    
#
# The main script. This script does the following:
#
# * Processes a series of lines of text input into the console.
#      => The words of the text are put in the list `textWords`
#
# * Scans the text to compute statistics about bi-grams and tri-
# grams that occur in the text. This uses the function `train`.
#
# • Generates a random text from the bi-/tri-gram dictionary
#   using a stochastic process. This uses the procedure 'chat'.
#

if __name__ == "__main__":

    # Read the words of a text (including ".", "!", and "?") into a list.
    print("READING text from STDIN. Hit ctrl-d when done entering text.")
    textWords = readWordsFromInput()
    print("DONE.")

    # Process the words, computing a dictionary.
    biTriDict = train(textWords)
    chat(biTriDict, 30, 70)
