num_list = "123456789"

print(num_list[0:3]) # 123
print(num_list[3:6]) # 456
print(num_list[6:]) # 789
print(num_list[:3]) # 123
print(num_list[::2]) # 13579



str1 = "         Hello World!         "
print(str1.strip()) # "Hello World!" Removes white spaces from both ends


str2 = "Lemon, Orange, Apple, Banana"
print(str2.split(",")) # ['Lemon', ' Orange', ' Apple', ' Banana']


str3 = "Hello World!. I am Garuav Chamoli, Hello!"
print(str3.find("Hello")) # 0
print(str3.count("Hello")) # 2
print(str3.replace("Hello", "Hi")) # Hi World!. I am Garuav Chamoli, Hi!  # Replaces all occurences of Hello with Hi

fruit = ["Apple", "Banana", "Orange"]
print("".join(fruit)) # AppleBananaOrange
print(" ".join(fruit)) # Apple Banana Orange
print("<-->".join(fruit)) # Apple<-->Banana<-->Orange