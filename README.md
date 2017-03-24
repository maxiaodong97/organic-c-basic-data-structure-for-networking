# organic-c-basic-data-structure-for-networking

An organic C data structure and defines for network related operations.

I wrote it a few years ago mainly for my hobby projects, but I would like to contribute to public domain if this can be beneficial to anyone. After coding java for a few years, I kind of missing the speed adn beauty of C!

To try it: 
```
$ make
$./ut./exe
+------------------------------------------+
| testing pt_slist_t....                   |
+------------------------------------------+
the list is: 1 2 3 4 5 4 3 2 1 0 
sort the list: 0 1 1 2 2 3 3 4 4 5 
reverse it: 5 4 4 3 3 2 2 1 1 0 
+------------------------------------------+
| testing pt_dlist_t....                   |
+------------------------------------------+
count = 5
3 1 0 2 4 
reverse traverse: 4 2 0 1 3 
append a new list: 4 2 0 1 3 4 2 0 1 3 
After remove 0 : 3 1 2 4 3 1 2 4 
After remove 1 : 3 2 4 3 2 4 
After remove 2 : 3 4 3 4 
After remove 3 : 4 4 
After remove 4 : 
+------------------------------------------+
| testing pt_queue_t....                   |
+------------------------------------------+
count = 5: 3 1 0 2 4 
peek head = 3
peek tail = 4
After remove head 3 : 1 0 2 4 
After remove tail = 4 : 1 0 2 
After remove head 1 : 0 2 
After remove tail = 2 : 0 
After remove head 0 : 
+------------------------------------------+
| testing pt_vector_t....                  |
+------------------------------------------+
[0] = 0
[1] = 1
[2] = 2
[3] = 3
[4] = 4
[0] = 0
[1] = 1
[2] = 2
[3] = 3
[4] = 4
[5] = 0
[6] = 1
[7] = 2
[8] = 3
[9] = 4
+------------------------------------------+
| testing pt_hash_t....                    |
+------------------------------------------+
		 elp 0x1c0e070: key=00000000, val=ff000000
		 elp 0x1c0e0f0: key=01000000, val=00010000
		 elp 0x1c0e010: key=02000000, val=01010000
		 elp 0x1c0e030: key=03000000, val=02010000
		 elp 0x1c0e130: key=04000000, val=03010000
		 elp 0x1c0e150: key=05000000, val=04010000
		 elp 0x1c0e170: key=06000000, val=05010000
		 elp 0x1c0e5f0: key=07000000, val=06010000
		 elp 0x1c0e610: key=08000000, val=07010000
		 elp 0x1c0e630: key=09000000, val=08010000
iter: key = 0, value = 255
iter: key = 1, value = 256
iter: key = 2, value = 257
iter: key = 3, value = 258
iter: key = 4, value = 259
iter: key = 5, value = 260
iter: key = 6, value = 261
iter: key = 7, value = 262
iter: key = 8, value = 263
iter: key = 9, value = 264
find value 255 for key = 0
find value 256 for key = 1
find value 257 for key = 2
find value 258 for key = 3
find value 259 for key = 4
find value 260 for key = 5
find value 261 for key = 6
find value 262 for key = 7
find value 263 for key = 8
find value 264 for key = 9
key = 10 not found
create_cqueue 8 buckets 1 delta time 
+------------------------------------------+
| testing pt_cqueue_t....                  |
+------------------------------------------+
--------------------------------
-> bucket 0: 
   bucket 1: 
    object_1 timeout value 1
   bucket 2: 
    object_2 timeout value 2
   bucket 3: 
    object_3 timeout value 3
   bucket 4: 
    object_4 timeout value 4
   bucket 5: 
   bucket 6: 
   bucket 7: 
--------------------------------
--------------------------------
   bucket 0: 
-> bucket 1: 
    object_1 timeout value 1
   bucket 2: 
    object_2 timeout value 2
   bucket 3: 
    object_3 timeout value 3
   bucket 4: 
    object_4 timeout value 4
   bucket 5: 
   bucket 6: 
   bucket 7: 
--------------------------------
at time 3, [object_1] triggered!
at time 3, [object_2] triggered!
at time 3, [object_3] triggered!
--------------------------------
   bucket 0: 
   bucket 1: 
   bucket 2: 
   bucket 3: 
-> bucket 4: 
    object_4 timeout value 4
    object_1 timeout value 1
   bucket 5: 
    object_2 timeout value 2
   bucket 6: 
    object_3 timeout value 3
   bucket 7: 
--------------------------------
at time 6, [object_4] triggered!
at time 6, [object_1] triggered!
at time 6, [object_2] triggered!
at time 6, [object_3] triggered!
--------------------------------
   bucket 0: 
    object_2 timeout value 2
   bucket 1: 
    object_3 timeout value 3
   bucket 2: 
    object_4 timeout value 4
   bucket 3: 
   bucket 4: 
   bucket 5: 
   bucket 6: 
-> bucket 7: 
    object_1 timeout value 1
--------------------------------
at time 9, [object_1] triggered!
at time 9, [object_2] triggered!
at time 9, [object_3] triggered!
--------------------------------
   bucket 0: 
   bucket 1: 
-> bucket 2: 
    object_4 timeout value 4
    object_1 timeout value 1
   bucket 3: 
    object_2 timeout value 2
   bucket 4: 
    object_3 timeout value 3
   bucket 5: 
   bucket 6: 
   bucket 7: 
--------------------------------
at time 12, [object_4] triggered!
at time 12, [object_1] triggered!
at time 12, [object_2] triggered!
at time 12, [object_3] triggered!
--------------------------------
   bucket 0: 
    object_4 timeout value 4
   bucket 1: 
   bucket 2: 
   bucket 3: 
   bucket 4: 
-> bucket 5: 
    object_1 timeout value 1
   bucket 6: 
    object_2 timeout value 2
   bucket 7: 
    object_3 timeout value 3
--------------------------------
at time 15, [object_1] triggered!
at time 15, [object_2] triggered!
at time 15, [object_3] triggered!
--------------------------------
-> bucket 0: 
    object_4 timeout value 4
    object_1 timeout value 1
   bucket 1: 
    object_2 timeout value 2
   bucket 2: 
    object_3 timeout value 3
   bucket 3: 
   bucket 4: 
   bucket 5: 
   bucket 6: 
   bucket 7: 
--------------------------------
at time 18, [object_4] triggered!
at time 18, [object_1] triggered!
at time 18, [object_2] triggered!
at time 18, [object_3] triggered!
--------------------------------
   bucket 0: 
   bucket 1: 
   bucket 2: 
-> bucket 3: 
    object_1 timeout value 1
   bucket 4: 
    object_2 timeout value 2
   bucket 5: 
    object_3 timeout value 3
   bucket 6: 
    object_4 timeout value 4
   bucket 7: 
--------------------------------
at time 21, [object_1] triggered!
at time 21, [object_2] triggered!
at time 21, [object_3] triggered!
--------------------------------
   bucket 0: 
    object_3 timeout value 3
   bucket 1: 
   bucket 2: 
   bucket 3: 
   bucket 4: 
   bucket 5: 
-> bucket 6: 
    object_4 timeout value 4
    object_1 timeout value 1
   bucket 7: 
    object_2 timeout value 2
--------------------------------
at time 24, [object_4] triggered!
at time 24, [object_1] triggered!
at time 24, [object_2] triggered!
at time 24, [object_3] triggered!
--------------------------------
   bucket 0: 
-> bucket 1: 
    object_1 timeout value 1
   bucket 2: 
    object_2 timeout value 2
   bucket 3: 
    object_3 timeout value 3
   bucket 4: 
    object_4 timeout value 4
   bucket 5: 
   bucket 6: 
   bucket 7: 
--------------------------------
at time 27, [object_1] triggered!
at time 27, [object_2] triggered!
at time 27, [object_3] triggered!
--------------------------------
   bucket 0: 
   bucket 1: 
   bucket 2: 
   bucket 3: 
-> bucket 4: 
    object_4 timeout value 4
    object_1 timeout value 1
   bucket 5: 
    object_2 timeout value 2
   bucket 6: 
    object_3 timeout value 3
   bucket 7: 
--------------------------------


```
