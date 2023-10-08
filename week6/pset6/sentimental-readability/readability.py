from cs50 import get_string


def main():
    # Ask for input
    text = get_string("Text: ")

    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # The average number of letters per 100 words in the text
    L = (float(letters) / float(words)) * float(100)
    # The average number of sentences per 100 words in the text
    S = (float(sentences) / float(words)) * float(100)

    # Coleman-Liau formula
    index = 0.0588 * L - 0.296 * S - 15.8
    index = int(round(index))

    # If the index number is less than 1
    if index <= 0:
        print("Before Grade 1")
    # If the resulting index number is 16 or higher
    elif index >= 16:
        print("Grade 16+")
    else:
        print("Grade", int(index))


def count_letters(text):
    total_letters = 0

    # Count only letters
    for i in range(len(text)):
        if text[i].isalpha():
            total_letters += 1
    return total_letters


def count_words(text):
    total_words = 1

    # Find words by couting only space and then + 1
    for i in range(len(text)):
        if text[i].isspace():
            total_words += 1
    return total_words


def count_sentences(text):
    total_sentences = 0

    # Find sentences by finding . or a ! or a ?
    for i in range(len(text)):
        if text[i] == '.' or text[i] == '!' or text[i] == '?':
            total_sentences += 1
    return total_sentences


if __name__ == "__main__":
    main()