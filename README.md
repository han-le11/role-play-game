# Role-playging game

## Overall 
My small role playing game, in which the game characters fight against each other. 

A character is composed of the following data.

- `name` is the name of the character, which is a string that can have arbitrary length.

- `hit points` (HP) is the health level of the character, which is a positive integer. HP is reduced when the character gets damage in the fight. The character will die when HP reduces to zero.

- `experience` is the experience level of the character, which is a non-negative integer. It increases when the character hits its opponents in the fight. The experience points increase by the amount of inflicted damage. A new character starts with zero experience.

- `weapon` that the character carries. A weapon is defined with the following data.

    - `name` is the name of the weapon.

    - `damage` is the damage the weapon may inflict.

## Game commands 

### Add character
- Add character command, which adds a new character to the game.The command has the following format.
```
A <name> <hit-points> <weapon-name> <weapon-damage>
```
where

<name>
is the character name, which is an arbitrary length string.

<hit-points>
is the character hit points, which is a positive integer.

<weapon-name>
is the character’s weapon name, which is an arbitrary length string.

<weapon-damage>
is the character’s weapon damage, which is a positive integer.

For example, a valid add character command is as follows.

```
A Bilbo 25 Dagger 8
```

Initially the character has 0 experience points, and full hit-points.

If the character with the specified name is already in the game, the program will print an error message.

### Attack 
- Attack command, which makes the given character to attack a target character.
```
H <attacker-name> <target-name>
```
where

<attacker-name>
is the character name that attacks.

<target-name>
is the character name that is being attacked.

For example, a valid attack command is as follows.
```
H Bilbo Smeagol
```
The target will receive damage by the amount the attacker’s weapon inflicts. The attacker will gain experience by the amount of damage its weapon inflicts.

The outcome of the attack must be printed on the screen. For the example above, after the command, the following should be printed on the screen.

```
Bilbo attacked Smeagol with Dagger by 6 damage.
Smeagol has 8 hit points remaining.
Bilbo gained 6 experience points.
SUCCESS
```
If the given attacker or target do not exist in the database, an error message will be printed.

### Print game
- Print game command, which displays the game content. The command has the following format.
```
L
```
It prints the stored characters on a separate line as follows.
```
<name> <hit-points> <experience> <weapon-name> <weapon-damage>
```
where the entries are the names of the character data fields.

For example, print game command output is as follows.
```
Bilbo 12 6 Dagger 6
Smeagol 8 0 Axe 12
SUCCESS
```

The database entries should be printed in the order of character experience such that the characters with higher experience points will be printed first.

The worth of this command is two points. The worth of printing all the database entries is 1 point. If their order is also correct, you will get another point.

### Save to file
- Save to file command, which saves the database to a text file. The command has the following format.
```
W <filename>
```
where

<filename>
is the name of the text file.

It writes the stored characters on a separate line as follows.
```
<name> <hit-points> <experience> <weapon-name> <weapon-damage>
```
where the entries are the names of the character data fields.

### Load from file
- Load from file command, which loads characters from a text file saved using Save to file command. The command has the following format.
```
O <filename>
```
where

<filename>
is the name of the text file.

It assumes the stored characters are on a separate line in the format specified above.

If an error occurs, it should print an error message.

If the file is valid and loaded correctly, the current game must be replaced with the entries loaded from the file.

### Quit game
- Quit game command, which releases all allocated memory and exits the program.

The command has the following format.
```
Q
```
This operation is implemented so that memory leaks are prevented.