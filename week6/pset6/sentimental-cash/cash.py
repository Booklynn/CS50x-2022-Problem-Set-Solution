from cs50 import get_float


def main():
    # Ask how many cents the customer is owed
    cents = get_cents()

    # Calculate the number of quarters to give the customer
    quarters = calculate_quarters(cents)
    cents = cents - quarters * 25

    # Calculate the number of dimes to give the customer
    dimes = calculate_dimes(cents)
    cents = cents - dimes * 10

    # Calculate the number of nickels to give the customer
    nickels = calculate_nickels(cents)
    cents = cents - nickels * 5

    # Calculate the number of pennies to give the customer
    pennies = calculate_pennies(cents)
    cents = cents - pennies * 1

    # Sum coins
    coins = quarters + dimes + nickels + pennies

    # Print total number of coins to give the customer
    print(coins)


def get_cents():
    # If the user inputs a negative int, prompt the user again
    while True:
        cents = get_float("Change owed: ")
        if cents > 0:
            return round(cents * 100)


def calculate_quarters(cents):
    total_quarters = 0
    quarter = 25
    while cents >= quarter:
        cents = cents - quarter
        total_quarters += 1
    return total_quarters


def calculate_dimes(cents):
    total_dimes = 0
    dime = 10
    while cents >= dime:
        cents = cents - dime
        total_dimes += 1
    return total_dimes


def calculate_nickels(cents):
    total_nickels = 0
    nickel = 5
    while cents >= nickel:
        cents = cents - nickel
        total_nickels += 1
    return total_nickels


def calculate_pennies(cents):
    total_pennies = 0
    penny = 1
    while cents >= 1:
        cents = cents - penny
        total_pennies += 1
    return total_pennies


if __name__ == "__main__":
    main()