/*
 * Robert Au 
 * ECS36A
 * Bishop
 * 
 * This is for HW3 problem 5
 */

/*
 * EXPLANATION:
 * 
 * &1 takes the first element of our output 
 * (NOT to be confused with the zeroth element at the beginning)
 * Since the &1 is directed at the string ["\021%six\012\0"],
 * it only takes the first element, which is %six
 * 
 * /021 (Device control 1) counts as the 0th element - so it is skipped 
 * 
 * %six actually means placeholder %s, 
 * which will be filled in with a string, 
 * then followed by the characters "ix"
 * 
 * /012 (line feed "\n") is ignored
 * (would skip to new line and continue string from there)
 * 
 * /0 (null byte) is also skipped
 * (would terminate string)
 * 
 * Then the comma (,) means the next part is an argument, 
 * (1)["have"]+"fun"-0x60, which tells us what %s will print.
 * 
 * (1)["have"] takes the first element from the string "have", 
 * which would be "a"
 * 
 * + "fun" will also print 'fun' (without quotes) in the output
 * 
 * -0x60 is the subtaction of an ascii character, 
 * and 0x60 converts to "`"  in hex (accent mark symbol without quotes),
 * which happens to be 1 away from "a" (which is 0x61 in hex)
 * 
 * So subtraction of these two characters equals 1
 * 
 * Now we have 1 + "fun"
 * which means start at the first element (not zeroth) of the string,
 * and include the rest of the following characters, 
 * so instead of "fun" we now have "un"
 * 
 * Now we know that %s is pointing at "un" and that it is in front of 
 * the characters "ix"
 * 
 * Meaning this printf statement should print "unix"
 */
main() { printf(&1["\021%six\012\0"],(1)["have"]+"fun"-0x60);}
