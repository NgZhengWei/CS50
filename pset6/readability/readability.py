from cs50 import get_string

def main():
    txt = get_string("Text: ")
    wordCount = len(txt.split())
    letterCount = sum(l.isalpha() for l in txt)
    sentenceCount = 0
    sentenceIndicator = ["!", "?", "."]
    for char in txt:
        if char in sentenceIndicator:
            sentenceCount += 1

    print(colemanFormula(wordCount, letterCount, sentenceCount))

def colemanFormula(word, letter, sentence):
    L = (letter / word) * 100
    S = (sentence / word) * 100
    result = 0.0588 * L - 0.296 * S - 15.8

    if result < 1:
        return "Before Grade 1"
    elif result >= 16:
        return "Grade 16+"
    else:
        return str(round(result))

main()