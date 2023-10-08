while True:
    try:
        # Prompt to enter number 1-8
        n = int(input("Height: "))
        if n > 0 and n <= 8:
            break
    except ValueError:
        print("Invalid input. Please enter a positive integer less than or equal to 8.")

for i in range(n):
    # Print spaces
    for j in range(n - i - 1):
        print(" ", end="")
    # Print grid of bricks
    for k in range(i + 1):
        print("#", end="")
    print("\n", end="")