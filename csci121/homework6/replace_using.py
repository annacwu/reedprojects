def replace_using(s,d):
    sentence = s.split(' ')
    sen2 = sentence[:]
    
    def look_for(string, list):
        i = 0
        while i < len(list):
            if list[i] == string:
                return i
            i += 1
        return None

    for i in d:
        # iteratior = 0
        # print("LOOKING FOR: " + i)
        # while iteratior < len(sentence):
        #     print("CURRENT WORD: " + sentence[iteratior])
        #     if look_for(i, sentence):
        #         ind = look_for(i, sentence)
        #         print("MATCH FOUND: " + str(ind))
        #         sen2[ind] = d[i]
        #     iteratior += 1

        while look_for(i, sentence) != None:
            ind = look_for(i, sentence)
            sen2[ind] = d[i]
            sentence[ind] = "0"
            

    replacement = ' '.join(sen2)
    return replacement
