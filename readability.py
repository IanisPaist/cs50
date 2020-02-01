import cs50
import re

#определяем количество букв
def count_chars(string):
    count = 0
    for char in string:
        if char.isalpha():
            count += 1
    return count

#определяем количество слов
def count_words(text):
    return len(text.split())

#определяем количество предложений
def count_sentences(text):
    return len(re.split("[.!?]", text)) - 1


#Main function
text = cs50.get_string("Text: ")

letters = count_chars(text)
words = count_words(text)
sentences = count_sentences(text)

#считаем количество букв/предложений на 100 слов
L = letters / words * 100
S = sentences / words * 100

#считаем индекс
index = int(round(0.0588 * L - 0.296 * S - 15.8, 0))

#выводим результат
if index > 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade {}".format(index))