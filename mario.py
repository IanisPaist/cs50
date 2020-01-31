possible_values = [x for x in range(1,9)]

height = 0

while height not in possible_values:
    height = input("Height:")
    if height.isnumeric():
        height = int(height)

for hashes in range(1, height + 1):
    spaces = height - hashes
    print(" " * spaces, end="")
    print("#" * hashes, end = "")
    print("  ", end="")
    print("#" * hashes)
