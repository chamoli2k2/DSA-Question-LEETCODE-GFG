list1 = [1, 2, 3, 4, 5]
print(list1[0:3]) # [1, 2, 3]
print(list1[3:5]) # [4, 5]
print(list1[5:]) # []
print(list1[:3]) # [1, 2, 3]
print(list1[::2]) # [1, 3, 5]
print(list1[::-1]) # [5, 4, 3, 2, 1]
print(list1[-1]) # 5
print(list1[-2]) # 4

list2 = ["Red", "Blue", "Green", "Yellow", "Orange", "Purple"]
list2[1] = "Black"
print(list2) # ['Red', 'Black', 'Green', 'Yellow', 'Orange', 'Purple']

list2[1:2] = ["Hello"]
print(list2) # ['Red', 'Hello', 'Green', 'Yellow', 'Orange', 'Purple']

list2[1:3] = ["Hello", "World"]
print(list2) # ['Red', 'Hello', 'World', 'Yellow', 'Orange', 'Purple']

list2[1:1] = ["Value_is_inserted_here"]
print(list2) # ['Red', 'Value_is_inserted_here', 'Hello', 'World', 'Yellow', 'Orange', 'Purple']

for it in list2:
    print(it) # Red Value_is_inserted_here Hello World Yellow Orange Purple

for it in list2:
    print(it, end=" <-> ") # Red <-> Value_is_inserted_here <-> Hello <-> World <-> Yellow <-> Orange <-> Purple <->

if "Orange" in list2:
    print("This list have orange")

list1.pop() # Removes last element from the list
print(list1) # [1, 2, 3, 4]

list1.insert(1, "Insearting") # Inserts element at index 1
print(list1) # [1, 'Insearting', 2, 3, 4]

list1.remove(1) 
print(list1) # ['Insearting', 2, 3, 4]


list1_copy = list1.copy() # Copy the list to another list both list have different memory location
list1.append("Appended") # Appends element to the list
list1_copy.append("Appended_to_copy")
print(list1) # ['Insearting', 2, 3, 4, 'Appended']
print(list1_copy) # ['Insearting', 2, 3, 4, 'Appended_to_copy']


squared_nums = [x*2 for x in range(10)]
print(squared_nums) # [0, 2, 4, 6, 8, 10, 12, 14, 16, 18] 