# Tuple is a collection which is ordered and unchangeable. Allows duplicate members.
tea_types = ("Black", "Green", "White", "Oolong", "Herbal")
print(tea_types) # ('Black', 'Green', 'White', 'Oolong', 'Herbal')

# Accessing tuple items
print(tea_types[1]) # Green

# Looping through tuple
for tea in tea_types:
    print(tea)

# Check if item exists
if "Black" in tea_types:
    print("Yes, 'Black' is in the tea_types tuple")

# Tuple length
print(len(tea_types)) # 5

# Add items
# Once a tuple is created, you cannot add items to it. Tuples are unchangeable.

# Remove items
# You cannot remove items in a tuple. Tuples are unchangeable.

# Join two tuples
tea_types_2 = ("Chai", "Matcha", "Pu'erh")
tea_types = tea_types + tea_types_2

print(tea_types) # ('Black', 'Green', 'White', 'Oolong', 'Herbal', 'Chai', 'Matcha', 'Pu'erh')

# Tuple constructor
# It is also possible to use the tuple() constructor to make a tuple.
tea_types = tuple(("Black", "Green", "White", "Oolong", "Herbal"))
print(tea_types) # ('Black', 'Green', 'White', 'Oolong', 'Herbal')

# Tuple methods
# count()	Returns the number of times a specified value occurs in a tuple
# index()	Searches the tuple for a specified value and returns the position of where it was found
print(tea_types.count("Black")) # 1
print(tea_types.index("Green")) # 1

# Unpacking a tuple
# When we create a tuple, we normally assign values to it. This is called "packing" a tuple:
fruits = ("apple", "banana", "cherry")
print(fruits) # ('apple', 'banana', 'cherry')

# But, in Python, we are also allowed to extract the values back into variables. This is called "unpacking":
(green, yellow, red) = fruits
print(green) # apple

# Using asterisk *
# If the number of variables is less than the number of values, you can add an * to the variable name and the values will be assigned to the variable as a list:
fruits = ("apple", "banana", "cherry", "strawberry", "raspberry")
(green, yellow, *red) = fruits
print(green) # apple
print(yellow) # banana
print(red) # ['cherry', 'strawberry', 'raspberry']

# If the asterisk is added to another variable name than the last, Python will assign values to the variable until the number of values left matches the number of variables left.
fruits = ("apple", "mango", "banana", "cherry", "strawberry", "raspberry")
(green, *yellow, red) = fruits
print(green) # apple
print(yellow) # ['mango', 'banana', 'cherry', 'strawberry']
print(red) # raspberry