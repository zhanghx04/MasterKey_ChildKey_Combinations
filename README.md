# MasterKey ChildKey Combinations
Author: Haoxiang Zhang

## Introduction
The repository solves the problem about the combination of driver pins and key pins.
Every lock 

## Explain

Mid and Bottom parts combination for the master key and child keys.

The child key can open only the specific door. The master key can open all the doors.

In the normal lock, there top part and bottom part. Each part has different length.
When the key is inserted into the lock, both top and bottom part will be pushed up.
If the key is right, the top of all the bottom parts will on the same level, which the key can be rotated.

If the manager want to use Master key. Then, the bottom part need to be separated to the mid-part and bottom-part.
Therefore, when the child key is inserted, the top of the mid-parts can be on the same level. When the master
key is inserted, the top of the bottom parts can be on the same level.

    e.g.
               top ||   bot               key
        @@@@@@@@[   4   ][5     ]       [  2]
        @@@@@@@@@@@[   4   ][2  ]    [     5]
        @@@@@@@[   4   ][6      ]        [ 1]
        @@@@@@@@[   4   ][5     ]       [  2]
        @@@@@@@[   4   ][6      ]        [ 1]
        @@@@@@@[   4   ][6      ]        [ 1]

        ---------- After Insert Key ------------

               top ||   bot   key
        @@@[   4   ][5     ][  2]
        @@@[   4   ][2  ][     5]
        @@@[   4   ][6      ][ 1]
        @@@[   4   ][5     ][  2]
        @@@[   4   ][6      ][ 1]
        @@@[   4   ][6      ][ 1]


    rule:
        bot is min(master[i], child[i])
        mid is abs(master[i] - child[i])