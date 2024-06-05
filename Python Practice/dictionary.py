my_dictionay = {"one": 1, "two": 2, "three": 3, "four": 4, "five": 5}

print(my_dictionay["one"]) # 1

# Add a new key-value pair
my_dictionay["six"] = 6

# iterate over the dictionary   
for it in my_dictionay:
    print(it, my_dictionay[it]) # one 1, two 2, three 3, four 4, five 5, six 6

# iterate over the keys
for key, values in my_dictionay.items():
    print(key, values) # one 1, two 2, three 3, four 4, five 5, six 6

# iterate over the values
for values in my_dictionay.values():
    print(values) # 1, 2, 3, 4, 5, 6

# Check if a key exists in the dictionary
if "one" in my_dictionay:
    print("Yes") # Yes

# length of the dictionary
print(len(my_dictionay)) # 6

# pop a key-value pair
my_dictionay.pop("one")
print(my_dictionay) # {'two': 2, 'three': 3, 'four': 4, 'five': 5, 'six': 6}

# dictionary of dictionary
my_dictionay = {
    "one": {"name": "John", "age": 24},
    "two": {"name": "Jane", "age": 25},
    "three": {"name": "Doe", "age": 26}
}

print(my_dictionay["one"]["name"]) # John
print(my_dictionay["two"]["name"]) # Jane


square_num = {x: x*x for x in range(10)}
print(square_num) # {0: 0, 1: 1, 2: 4, 3: 9, 4: 16, 5: 25, 6: 36, 7: 49, 8: 64, 9: 81}